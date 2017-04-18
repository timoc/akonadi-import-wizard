/*
   Copyright (C) 2012-2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ABSTRACTBASE_H
#define ABSTRACTBASE_H

#include <QObject>
#include "libimportwizard_export.h"
#include <QString>
#include <QMap>
#include <QVariant>

class QWidget;
namespace PimCommon
{
class CreateResource;
}
namespace LibImportWizard
{
class LIBIMPORTWIZARD_EXPORT AbstractBase : public QObject
{
    Q_OBJECT
public:
    explicit AbstractBase();
    virtual ~AbstractBase();

    QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings);

    void setParentWidget(QWidget *parent);
    QWidget *parentWidget() const;

protected:
    virtual void addImportInfo(const QString &log) = 0;
    virtual void addImportError(const QString &log) = 0;

private:
    void slotCreateResourceError(const QString &);
    void slotCreateResourceInfo(const QString &);
    PimCommon::CreateResource *mCreateResource;
    QWidget *mParentWidget;
};
}
#endif // ABSTRACTBASE_H
