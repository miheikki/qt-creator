/***************************************************************************
**
** Copyright (C) 2015 Jochen Becher
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef QMT_TEXTSCANNER_H
#define QMT_TEXTSCANNER_H

#include <QObject>

#include "sourcepos.h"

#include "qmt/infrastructure/exceptions.h"

namespace qmt {

class ITextSource;
class SourceChar;
class Token;

class QMT_EXPORT TextScannerError :
        public Exception
{
public:

    TextScannerError(const QString &errorMsg, const SourcePos &sourcePos);

    ~TextScannerError();

public:

    SourcePos sourcePos() const { return m_sourcePos; }

private:
    SourcePos m_sourcePos;
};


class QMT_EXPORT TextScanner : public QObject
{
    Q_OBJECT

    class TextScannerPrivate;

public:
    explicit TextScanner(QObject *parent = 0);

    ~TextScanner();

signals:

public:

    void setKeywords(const QList<QPair<QString, int> > &keywords);

    void setOperators(const QList<QPair<QString, int> > &operators);

    void setSource(ITextSource *textSource);

    SourcePos sourcePos() const;

public:

    Token read();

    void unread(const Token &token);

private:

    SourceChar readChar();

    void unreadChar(const SourceChar &sourceChar);

    void skipWhitespaces();

    Token scanString(const SourceChar &delimiterChar);

    Token scanNumber(const SourceChar &firstDigit);

    Token scanIdentifier(const SourceChar &firstChar);

    Token scanColorIdentifier(const SourceChar &firstChar);

    Token scanOperator(const SourceChar &firstChar);

private:

    TextScannerPrivate *d;

};

} // namespace qmt

#endif // QMT_TEXTSCANNER_H
