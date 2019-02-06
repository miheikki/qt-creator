/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "kitconfigwidget.h"

#include "kit.h"
#include "kitinformation.h"

namespace ProjectExplorer {

KitAspectWidget::KitAspectWidget(Kit *kit, const KitAspect *ki) : m_kit(kit),
    m_kitInformation(ki), m_isSticky(kit->isSticky(ki->id()))
{ }

Core::Id KitAspectWidget::kitInformationId() const
{
    return m_kitInformation->id();
}

bool KitAspectWidget::isMutable() const
{
    return m_kit->isMutable(m_kitInformation->id());
}

void KitAspectWidget::setMutable(bool b)
{
    m_kit->setMutable(m_kitInformation->id(), b);
}

QString KitAspectWidget::msgManage()
{
    return tr("Manage...");
}

void KitAspectWidget::setPalette(const QPalette &p)
{
    if (mainWidget())
        mainWidget()->setPalette(p);
    if (buttonWidget())
        buttonWidget()->setPalette(p);
}

void KitAspectWidget::setStyle(QStyle *s)
{
    if (mainWidget())
        mainWidget()->setStyle(s);
    if (buttonWidget())
        buttonWidget()->setStyle(s);
}

} // namespace ProjectExplorer
