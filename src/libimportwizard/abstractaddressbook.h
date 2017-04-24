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

#ifndef ABSTRACTADDRESSBOOK_H
#define ABSTRACTADDRESSBOOK_H

#include <AkonadiCore/Collection>
#include "abstractbase.h"
#include "libimportwizard_export.h"

namespace KContacts {
class Addressee;
class ContactGroup;
}

class KJob;
namespace LibImportWizard {
class AbstractDisplayInfo;
class LIBIMPORTWIZARD_EXPORT AbstractAddressBook : public LibImportWizard::AbstractBase
{
    Q_OBJECT
public:
    AbstractAddressBook();
    ~AbstractAddressBook();

protected:
    void createGroup(const KContacts::ContactGroup &group);
    void createContact(const KContacts::Addressee &address);

    void addImportInfo(const QString &log) Q_DECL_OVERRIDE;
    void addImportError(const QString &log) Q_DECL_OVERRIDE;

    void addAddressBookImportInfo(const QString &log);
    void addAddressBookImportError(const QString &log);
    void cleanUp();
    void addImportContactNote(KContacts::Addressee &address, const QString &applicationName);

private:
    void slotStoreDone(KJob *job);
    bool selectAddressBook();
    Akonadi::Collection mCollection;
};
}
#endif // ABSTRACTADDRESSBOOK_H