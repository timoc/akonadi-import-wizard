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

#include "mailapp/mailappimportdata.h"
#include "mailimporter/filtermailapp.h"
#include "mailimporter/filterinfo.h"
#include "importfilterinfogui.h"
#include "importwizard.h"

#include <KLocalizedString>

#include <QDir>

MailAppImportData::MailAppImportData(ImportWizard *parent)
    : AbstractImporter(parent)
{
    mPath = QDir::homePath();
}

MailAppImportData::~MailAppImportData()
{
}

bool MailAppImportData::foundMailer() const
{
#ifdef Q_OS_MAC
    //TODO find a method to search it. Perhaps look at binary.
    QDir directory(mPath);
    if (directory.exists()) {
        return true;
    }
#endif
    return false;
}

QString MailAppImportData::name() const
{
    return QStringLiteral("Mail App");
}

bool MailAppImportData::importMails()
{
    MailImporter::FilterMailApp mailapp;
    initializeFilter(mailapp);
    mailapp.filterInfo()->setStatusMessage(i18n("Import in progress"));
    QDir directory(mPath);
    if (directory.exists()) {
        mailapp.importMails(mPath);
    } else {
        mailapp.import();
    }
    mailapp.filterInfo()->setStatusMessage(i18n("Import finished"));
    return true;
}

AbstractImporter::TypeSupportedOptions MailAppImportData::supportedOption()
{
    TypeSupportedOptions options;
    options |= AbstractImporter::Mails;
    return options;
}