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

#include "balsaaddressbook.h"
#include "importwizardutil.h"
#include "helper_p.h"

#include <KContacts/Addressee>
#include <KContacts/LDIFConverter>

#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <QUrl>

#include "balsaplugin_debug.h"
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

BalsaAddressBook::BalsaAddressBook(const QString &filename)
    : mFileName(filename)
{
}

BalsaAddressBook::~BalsaAddressBook()
{

}

void BalsaAddressBook::importAddressBook()
{
    KConfig config(mFileName);
    const QStringList addressBookList = config.groupList().filter(QRegularExpression("address-book-\\d+"));
    if (addressBookList.isEmpty()) {
        addAddressBookImportInfo(i18n("No addressbook found"));
    } else {
        for (const QString &addressbook : addressBookList) {
            KConfigGroup grp = config.group(addressbook);
            readAddressBook(grp);
        }
    }
}

void BalsaAddressBook::readAddressBook(const KConfigGroup &grp)
{
    const QString type = grp.readEntry(QStringLiteral("Type"));
    if (type.isEmpty()) {
        addAddressBookImportInfo(i18n("No addressbook found"));
        return;
    }
    const QString name = grp.readEntry(QStringLiteral("Name"));

    if (type == QLatin1String("LibBalsaAddressBookLdap")) {
        ldapStruct ldap;
        ldap.dn = grp.readEntry(QStringLiteral("BaseDN"));
        ldap.useTLS = (grp.readEntry(QStringLiteral("EnableTLS")) == QLatin1String("true"));
        ldap.ldapUrl = QUrl(grp.readEntry(QStringLiteral("Host")));
        ldap.port = ldap.ldapUrl.port();
        //TODO: verify
        const QString bookDN  = grp.readEntry(QStringLiteral("BookDN")); //TODO ?
        ImportWizardUtil::mergeLdap(ldap);
        addAddressBookImportInfo(i18n("Ldap created"));
    } else if (type == QLatin1String("LibBalsaAddressBookGpe")) {
        qCDebug(BALSAPLUGIN_LOG) << " Import it !";
    } else if (type == QLatin1String("LibBalsaAddressBookLdif")) {
        const QString path = grp.readEntry(QStringLiteral("Path"));
        if (!path.isEmpty()) {
            KContacts::Addressee::List contacts;
            KContacts::ContactGroup::List contactsGroup;
            QFile file(path);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                stream.setCodec("ISO 8859-1");

                const QString wholeFile = stream.readAll();
                const QDateTime dtDefault = QFileInfo(file).lastModified();
                file.close();

                KContacts::LDIFConverter::LDIFToAddressee(wholeFile, contacts, contactsGroup, dtDefault);
                for (KContacts::Addressee contact : qAsConst(contacts)) {
                    addImportContactNote(contact, QStringLiteral("Balsa"));
                    createContact(contact);
                }
            }
        }
    } else if (type == QLatin1String("LibBalsaAddressBookVcard")) {
        const QString path = grp.readEntry(QStringLiteral("Path"));
        if (!path.isEmpty()) {
            QMap<QString, QVariant> settings;
            settings.insert(QStringLiteral("Path"), path);
            settings.insert(QStringLiteral("DisplayName"), name);
            addAddressBookImportInfo(i18n("New addressbook created: %1", createResource(QStringLiteral("akonadi_vcard_resource"), name, settings)));
        }
    } else {
        qCDebug(BALSAPLUGIN_LOG) << " unknown addressbook type :" << type;
    }
}
