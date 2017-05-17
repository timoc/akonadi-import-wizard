/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

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
#ifndef SeaMonkeyImportData_H
#define SeaMonkeyImportData_H

#include <ImportWizard/AbstractImporter>

class SeaMonkeyImportData : public LibImportWizard::AbstractImporter
{
public:
    explicit SeaMonkeyImportData(QObject *parent, const QList<QVariant> & = QList<QVariant>());
    ~SeaMonkeyImportData();

    TypeSupportedOptions supportedOption() Q_DECL_OVERRIDE;
    bool foundMailer() const Q_DECL_OVERRIDE;

    bool importSettings() Q_DECL_OVERRIDE;
    bool importMails() Q_DECL_OVERRIDE;
    bool importFilters() Q_DECL_OVERRIDE;
    bool importAddressBook() Q_DECL_OVERRIDE;
    QString name() const Q_DECL_OVERRIDE;
private:
    QString defaultProfile();

    QString mDefaultProfile;
};

#endif /* SeaMonkeyImportData_H */