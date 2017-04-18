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

#ifndef OperaImportData_H
#define OperaImportData_H

#include "abstract/abstractimporter.h"
class ImportWizard;

class OperaImportData : public LibImportWizard::AbstractImporter
{
public:
    explicit OperaImportData(QObject *parent, const QList<QVariant> & = QList<QVariant>());
    ~OperaImportData();

    TypeSupportedOptions supportedOption() Q_DECL_OVERRIDE;
    bool foundMailer() const Q_DECL_OVERRIDE;

    bool importMails() Q_DECL_OVERRIDE;
    bool importAddressBook() Q_DECL_OVERRIDE;
    bool importSettings() Q_DECL_OVERRIDE;

    QString name() const Q_DECL_OVERRIDE;

};

#endif /* OperaImportData_H */
