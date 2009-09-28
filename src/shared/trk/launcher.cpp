/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#include "launcher.h"
#include "trkutils.h"
#include "trkdevice.h"

#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtCore/QQueue>
#include <QtCore/QFile>
#include <QtCore/QScopedPointer>

namespace trk {

struct LauncherPrivate {
    struct CopyState {
        QString sourceFileName;
        QString destinationFileName;
        uint copyFileHandle;
        QScopedPointer<QByteArray> data;
        int position;
    };

    LauncherPrivate();
    TrkDevice m_device;
    QString m_trkServerName;
    QByteArray m_trkReadBuffer;

    void logMessage(const QString &msg);
    // Debuggee state
    Session m_session; // global-ish data (process id, target information)

    CopyState m_copyState;
    QString m_fileName;
    QString m_installFileName;
    int m_verbose;
};

LauncherPrivate::LauncherPrivate() :
    m_verbose(0)
{
}

Launcher::Launcher() :
    d(new LauncherPrivate)
{
    connect(&d->m_device, SIGNAL(messageReceived(trk::TrkResult)), this, SLOT(handleResult(trk::TrkResult)));
}

Launcher::~Launcher()
{
    logMessage("Shutting down.\n");
    delete d;
}

void Launcher::setTrkServerName(const QString &name)
{
    d->m_trkServerName = name;
}

void Launcher::setFileName(const QString &name)
{
    d->m_fileName = name;
}

void Launcher::setCopyFileName(const QString &srcName, const QString &dstName)
{
    d->m_copyState.sourceFileName = srcName;
    d->m_copyState.destinationFileName = dstName;
}

void Launcher::setInstallFileName(const QString &name)
{
    d->m_installFileName = name;
}

void Launcher::setSerialFrame(bool b)
{
    d->m_device.setSerialFrame(b);
}

bool Launcher::serialFrame() const
{
    return d->m_device.serialFrame();
}

bool Launcher::startServer(QString *errorMessage)
{
    if (d->m_verbose) {
        const QString msg = QString::fromLatin1("Port=%1 Executable=%2 Package=%3 Remote Package=%4 Install file=%5")
                            .arg(d->m_trkServerName, d->m_fileName, d->m_copyState.sourceFileName, d->m_copyState.destinationFileName, d->m_installFileName);
        logMessage(msg);
    }
    if (!d->m_device.open(d->m_trkServerName, errorMessage))
        return false;
    d->m_device.sendTrkInitialPing();
    d->m_device.sendTrkMessage(TrkConnect); // Connect
    d->m_device.sendTrkMessage(TrkSupported, TrkCallback(this, &Launcher::handleSupportMask));
    d->m_device.sendTrkMessage(TrkCpuType, TrkCallback(this, &Launcher::handleCpuType));
    d->m_device.sendTrkMessage(TrkVersions, TrkCallback(this, &Launcher::handleTrkVersion));
    if (d->m_fileName.isEmpty())
        return true;
    if (!d->m_copyState.sourceFileName.isEmpty() && !d->m_copyState.destinationFileName.isEmpty())
        copyFileToRemote();
    else
        installAndRun();
    return true;
}

void Launcher::setVerbose(int v)
{
    d->m_verbose = v;
    d->m_device.setVerbose(v);
}

void Launcher::installAndRun()
{
    if (!d->m_installFileName.isEmpty()) {
        installRemotePackageSilently(d->m_installFileName);
    } else {
        startInferiorIfNeeded();
    }
}
void Launcher::logMessage(const QString &msg)
{
    if (d->m_verbose)
        qDebug() << "LAUNCHER: " << qPrintable(msg);
}

void Launcher::waitForTrkFinished(const TrkResult &result)
{
    Q_UNUSED(result)
    d->m_device.sendTrkMessage(TrkPing, TrkCallback(this, &Launcher::handleWaitForFinished));
}

void Launcher::terminate()
{
    //TODO handle case where application has not been started
    QByteArray ba;
    appendShort(&ba, 0x0000, TargetByteOrder);
    appendInt(&ba, d->m_session.pid, TargetByteOrder);
    d->m_device.sendTrkMessage(TrkDeleteItem, TrkCallback(this, &Launcher::waitForTrkFinished), ba);
}

void Launcher::handleResult(const TrkResult &result)
{
    QByteArray prefix = "READ BUF:                                       ";
    QByteArray str = result.toString().toUtf8();
    if (result.isDebugOutput) { // handle application output
        logMessage("APPLICATION OUTPUT: " + result.data);
        emit applicationOutputReceived(result.data);
        return;
    }
    switch (result.code) {
        case TrkNotifyAck:
            break;
        case TrkNotifyNak: { // NAK
            logMessage(prefix + "NAK: " + str);
            //logMessage(prefix << "TOKEN: " << result.token);
            logMessage(prefix + "ERROR: " + errorMessage(result.data.at(0)));
            break;
        }
        case TrkNotifyStopped: { // Notified Stopped
            logMessage(prefix + "NOTE: STOPPED  " + str);
            // 90 01   78 6a 40 40   00 00 07 23   00 00 07 24  00 00
            //const char *data = result.data.data();
//            uint addr = extractInt(data); //code address: 4 bytes; code base address for the library
//            uint pid = extractInt(data + 4); // ProcessID: 4 bytes;
//            uint tid = extractInt(data + 8); // ThreadID: 4 bytes
            //logMessage(prefix << "      ADDR: " << addr << " PID: " << pid << " TID: " << tid);
            d->m_device.sendTrkAck(result.token);
            break;
        }
        case TrkNotifyException: { // Notify Exception (obsolete)
            logMessage(prefix + "NOTE: EXCEPTION  " + str);
            d->m_device.sendTrkAck(result.token);
            break;
        }
        case TrkNotifyInternalError: { //
            logMessage(prefix + "NOTE: INTERNAL ERROR: " + str);
            d->m_device.sendTrkAck(result.token);
            break;
        }

        // target->host OS notification
        case TrkNotifyCreated: { // Notify Created
            /*
            const char *data = result.data.data();
            byte error = result.data.at(0);
            byte type = result.data.at(1); // type: 1 byte; for dll item, this value is 2.
            uint pid = extractInt(data + 2); //  ProcessID: 4 bytes;
            uint tid = extractInt(data + 6); //threadID: 4 bytes
            uint codeseg = extractInt(data + 10); //code address: 4 bytes; code base address for the library
            uint dataseg = extractInt(data + 14); //data address: 4 bytes; data base address for the library
            uint len = extractShort(data + 18); //length: 2 bytes; length of the library name string to follow
            QByteArray name = result.data.mid(20, len); // name: library name

            logMessage(prefix + "NOTE: LIBRARY LOAD: " + str);
            logMessage(prefix + "TOKEN: " + result.token);
            logMessage(prefix + "ERROR: " + int(error));
            logMessage(prefix + "TYPE:  " + int(type));
            logMessage(prefix + "PID:   " + pid);
            logMessage(prefix + "TID:   " + tid);
            logMessage(prefix + "CODE:  " + codeseg);
            logMessage(prefix + "DATA:  " + dataseg);
            logMessage(prefix + "LEN:   " + len);
            logMessage(prefix + "NAME:  " + name);
            */

            QByteArray ba;
            appendInt(&ba, d->m_session.pid);
            appendInt(&ba, d->m_session.tid);
            d->m_device.sendTrkMessage(TrkContinue, TrkCallback(), ba, "CONTINUE");
            //d->m_device.sendTrkAck(result.token)
            break;
        }
        case TrkNotifyDeleted: { // NotifyDeleted
            const ushort itemType = (unsigned char)result.data.at(1);
            const ushort len = result.data.size() > 12 ? extractShort(result.data.data() + 10) : ushort(0);
            const QString name = len ? QString::fromAscii(result.data.mid(12, len)) : QString();
            logMessage(QString::fromLatin1("%1 %2 UNLOAD: %3").
                       arg(QString::fromAscii(prefix)).arg(itemType ? QLatin1String("LIB") : QLatin1String("PROCESS")).
                       arg(name));
            d->m_device.sendTrkAck(result.token);
            if (itemType == 0) { // process
                d->m_device.sendTrkMessage(TrkDisconnect, TrkCallback(this, &Launcher::waitForTrkFinished));
            }
            break;
        }
        case TrkNotifyProcessorStarted: { // NotifyProcessorStarted
            logMessage(prefix + "NOTE: PROCESSOR STARTED: " + str);
            d->m_device.sendTrkAck(result.token);
            break;
        }
        case TrkNotifyProcessorStandBy: { // NotifyProcessorStandby
            logMessage(prefix + "NOTE: PROCESSOR STANDBY: " + str);
            d->m_device.sendTrkAck(result.token);
            break;
        }
        case TrkNotifyProcessorReset: { // NotifyProcessorReset
            logMessage(prefix + "NOTE: PROCESSOR RESET: " + str);
            d->m_device.sendTrkAck(result.token);
            break;
        }
        default: {
            logMessage(prefix + "INVALID: " + str);
            break;
        }
    }
}

void Launcher::handleTrkVersion(const TrkResult &result)
{
    if (result.data.size() < 5)
        return;
    const int trkMajor = result.data.at(1);
    const int trkMinor = result.data.at(2);
    const int protocolMajor = result.data.at(3);
    const int protocolMinor = result.data.at(4);
    // Ping mode: Log & Terminate
    if (d->m_fileName.isEmpty()) {
        QString msg;
        QTextStream(&msg) << "CPU: " << d->m_session.cpuMajor << '.' << d->m_session.cpuMinor << ' '
                << (d->m_session.bigEndian ? "big endian" : "little endian")
                << " type size: " << d->m_session.defaultTypeSize
                << " float size: " << d->m_session.fpTypeSize
                << " Trk: v" << trkMajor << '.' << trkMinor << " Protocol: " << protocolMajor << '.' << protocolMinor;
        qWarning("%s", qPrintable(msg));
        d->m_device.sendTrkMessage(TrkPing, TrkCallback(this, &Launcher::waitForTrkFinished));
    }
}

void Launcher::handleFileCreation(const TrkResult &result)
{
    // we don't do any error handling yet, which is bad
    const char *data = result.data.data();
    d->m_copyState.copyFileHandle = extractInt(data + 2);
    QFile file(d->m_copyState.sourceFileName);
    file.open(QIODevice::ReadOnly);
    d->m_copyState.data.reset(new QByteArray(file.readAll()));
    d->m_copyState.position = 0;
    file.close();
    continueCopying();
}

void Launcher::handleCopy(const TrkResult &result)
{
    Q_UNUSED(result)

    continueCopying();
}

void Launcher::continueCopying()
{
    static const int BLOCKSIZE = 1024;
    int size = d->m_copyState.data->length();
    if (size == 0)
        emit copyProgress(100);
    else {
        int percent = qMin((d->m_copyState.position*100)/size, 100);
        emit copyProgress(percent);
    }
    if (d->m_copyState.position < size) {
        QByteArray ba;
        appendInt(&ba, d->m_copyState.copyFileHandle, TargetByteOrder);
        appendString(&ba, d->m_copyState.data->mid(d->m_copyState.position, BLOCKSIZE), TargetByteOrder, false);
        d->m_copyState.position += BLOCKSIZE;
        d->m_device.sendTrkMessage(TrkWriteFile, TrkCallback(this, &Launcher::handleCopy), ba);
    } else {
        QByteArray ba;
        appendInt(&ba, d->m_copyState.copyFileHandle, TargetByteOrder);
        appendInt(&ba, QDateTime::currentDateTime().toTime_t(), TargetByteOrder);
        d->m_device.sendTrkMessage(TrkCloseFile, TrkCallback(this, &Launcher::handleFileCreated), ba);
        d->m_copyState.data.reset();
    }
}

void Launcher::handleFileCreated(const TrkResult &result)
{
    Q_UNUSED(result)
    installAndRun();
}

void Launcher::handleCpuType(const TrkResult &result)
{
    logMessage("HANDLE CPU TYPE: " + result.toString());
    //---TRK------------------------------------------------------
    //  Command: 0x80 Acknowledge
    //    Error: 0x00
    // [80 03 00  04 00 00 04 00 00 00]
    d->m_session.cpuMajor = result.data.at(1);
    d->m_session.cpuMinor = result.data.at(2);
    d->m_session.bigEndian = result.data.at(3);
    d->m_session.defaultTypeSize = result.data.at(4);
    d->m_session.fpTypeSize = result.data.at(5);
    d->m_session.extended1TypeSize = result.data.at(6);
    //d->m_session.extended2TypeSize = result.data[6];
}

void Launcher::handleCreateProcess(const TrkResult &result)
{
    if (result.errorCode()) {
        emit canNotRun(errorMessage(result.errorCode()));
        emit finished();
        return;
    }
    //  40 00 00]
    //logMessage("       RESULT: " + result.toString());
    // [80 08 00   00 00 01 B5   00 00 01 B6   78 67 40 00   00 40 00 00]
    const char *data = result.data.data();
    d->m_session.pid = extractInt(data + 1);
    d->m_session.tid = extractInt(data + 5);
    d->m_session.codeseg = extractInt(data + 9);
    d->m_session.dataseg = extractInt(data + 13);
    if (d->m_verbose) {
        const QString msg = QString::fromLatin1("Process id: %1 Thread id: %2 code: 0x%3 data: 0x%4").
                            arg(d->m_session.pid).arg(d->m_session.tid).arg(d->m_session.codeseg, 0, 16).
                            arg(d->m_session.dataseg,  0 ,16);
        logMessage(msg);
    }
    emit applicationRunning(d->m_session.pid);
    QByteArray ba;
    appendInt(&ba, d->m_session.pid);
    appendInt(&ba, d->m_session.tid);
    d->m_device.sendTrkMessage(TrkContinue, TrkCallback(), ba, "CONTINUE");
}

void Launcher::handleWaitForFinished(const TrkResult &result)
{
    logMessage("   FINISHED: " + stringFromArray(result.data));
    emit finished();
}

void Launcher::handleSupportMask(const TrkResult &result)
{
    const char *data = result.data.data() + 1;

    QByteArray str;
    for (int i = 0; i < 32; ++i) {
        //str.append("  [" + formatByte(data[i]) + "]: ");
        for (int j = 0; j < 8; ++j)
        if (data[i] & (1 << j))
            str.append(QByteArray::number(i * 8 + j, 16) + " ");
    }
    logMessage("SUPPORTED: " + str);
}


void Launcher::cleanUp()
{
    //
    //---IDE------------------------------------------------------
    //  Command: 0x41 Delete Item
    //  Sub Cmd: Delete Process
    //ProcessID: 0x0000071F (1823)
    // [41 24 00 00 00 00 07 1F]
    QByteArray ba;
    appendByte(&ba, 0x00);
    appendByte(&ba, 0x00);
    appendInt(&ba, d->m_session.pid);
    d->m_device.sendTrkMessage(TrkDeleteItem, TrkCallback(), ba, "Delete process");

    //---TRK------------------------------------------------------
    //  Command: 0x80 Acknowledge
    //    Error: 0x00
    // [80 24 00]

    //---IDE------------------------------------------------------
    //  Command: 0x1C Clear Break
    // [1C 25 00 00 00 0A 78 6A 43 40]

        //---TRK------------------------------------------------------
        //  Command: 0xA1 Notify Deleted
        // [A1 09 00 00 00 00 00 00 00 00 07 1F]
        //---IDE------------------------------------------------------
        //  Command: 0x80 Acknowledge
        //    Error: 0x00
        // [80 09 00]

    //---TRK------------------------------------------------------
    //  Command: 0x80 Acknowledge
    //    Error: 0x00
    // [80 25 00]

    //---IDE------------------------------------------------------
    //  Command: 0x1C Clear Break
    // [1C 26 00 00 00 0B 78 6A 43 70]
    //---TRK------------------------------------------------------
    //  Command: 0x80 Acknowledge
    //    Error: 0x00
    // [80 26 00]


    //---IDE------------------------------------------------------
    //  Command: 0x02 Disconnect
    // [02 27]
//    sendTrkMessage(0x02, TrkCallback(this, &Launcher::handleDisconnect));
    //---TRK------------------------------------------------------
    //  Command: 0x80 Acknowledge
    // Error: 0x00
}

void Launcher::copyFileToRemote()
{
    emit copyingStarted();
    QByteArray ba;
    appendByte(&ba, 0x10);
    appendString(&ba, d->m_copyState.destinationFileName.toLocal8Bit(), TargetByteOrder, false);
    d->m_device.sendTrkMessage(TrkOpenFile, TrkCallback(this, &Launcher::handleFileCreation), ba);
}

void Launcher::installRemotePackageSilently(const QString &fileName)
{
    emit installingStarted();
    QByteArray ba;
    appendByte(&ba, 'C');
    appendString(&ba, fileName.toLocal8Bit(), TargetByteOrder, false);
    d->m_device.sendTrkMessage(TrkInstallFile, TrkCallback(this, &Launcher::handleInstallPackageFinished), ba);
}

void Launcher::handleInstallPackageFinished(const TrkResult &)
{
    startInferiorIfNeeded();
}

void Launcher::startInferiorIfNeeded()
{
    emit startingApplication();
    if (d->m_session.pid != 0) {
        logMessage("Process already 'started'");
        return;
    }
    // It's not started yet
    QByteArray ba;
    appendByte(&ba, 0); // ?
    appendByte(&ba, 0); // ?
    appendByte(&ba, 0); // ?
    appendString(&ba, d->m_fileName.toLocal8Bit(), TargetByteOrder);
    d->m_device.sendTrkMessage(TrkCreateItem, TrkCallback(this, &Launcher::handleCreateProcess), ba); // Create Item
}

}
