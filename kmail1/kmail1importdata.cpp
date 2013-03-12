/*
  Copyright (c) 2012-2013 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "mailimporter/filterinfo.h"
#include "importfilterinfogui.h"
#include "importwizard.h"

#include <KLocale>

#include <QDir>
#include <QWidget>


KMail1ImportData::KMail1ImportData(ImportWizard*parent)
    :AbstractImporter(parent)
{
}

KMail1ImportData::~KMail1ImportData()
{
}


bool KMail1ImportData::foundMailer() const
{
    QDir directory( mPath );
    if ( directory.exists() )
        return true;
    return false;
}

QString KMail1ImportData::name() const
{
    return QLatin1String("KMail1");
}

bool KMail1ImportData::importMails()
{
    return false;
}

bool KMail1ImportData::importSettings()
{
    return true;
}

bool KMail1ImportData::importAddressBook()
{
    return true;
}


AbstractImporter::TypeSupportedOptions KMail1ImportData::supportedOption()
{
    TypeSupportedOptions options;
    //options |=AbstractImporter::Mails;
    options |=AbstractImporter::Settings;
    options |=AbstractImporter::AddressBooks;
    return options;
}
