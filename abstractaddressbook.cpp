/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

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
#include "abstractaddressbook.h"
#include "importwizard.h"
#include "importaddressbookpage.h"

#include <KABC/Addressee>
#include <KLocale>
#include <Akonadi/ItemCreateJob>
#include <Akonadi/Item>
#include <Akonadi/CollectionDialog>

#include <QPointer>

AbstractAddressBook::AbstractAddressBook(ImportWizard *parent)
  : mCollection( -1 ), mImportWizard(parent)
{
}

AbstractAddressBook::~AbstractAddressBook()
{

}

void AbstractAddressBook::createGroup()
{
  //TODO
}

void AbstractAddressBook::createContact( const KABC::Addressee& address )
{
  addAddressBookImportInfo( i18n( "Creating new contact..." ) );

  if ( !mCollection.isValid() )
  {
    const QStringList mimeTypes( KABC::Addressee::mimeType() );
    QPointer<Akonadi::CollectionDialog> dlg = new Akonadi::CollectionDialog( mImportWizard );
    dlg->setMimeTypeFilter( mimeTypes );
    dlg->setAccessRightsFilter( Akonadi::Collection::CanCreateItem );
    dlg->setCaption( i18n( "Select Address Book" ) );
    dlg->setDescription( i18n( "Select the address book the new contact shall be saved in:" ) );
    
    if ( dlg->exec() == QDialog::Accepted && dlg ) {
      mCollection = dlg->selectedCollection();
    } else {
      addAddressBookImportError( i18n( "Address Book was not selected." ) );
      delete dlg;
      return;
    }

    delete dlg;
  }
  
  Akonadi::Item item;
  item.setPayload<KABC::Addressee>( address );
  item.setMimeType( KABC::Addressee::mimeType() );
  Akonadi::ItemCreateJob *job = new Akonadi::ItemCreateJob( item, mCollection );
  connect( job, SIGNAL(result(KJob*)), SLOT(slotStoreDone(KJob*)) );

}

void AbstractAddressBook::slotStoreDone(KJob*job)
{
  if ( job->error() ) {
    qDebug()<<" job->errorString() : "<<job->errorString();
    addAddressBookImportError( i18n( "Error during create contact : %1", job->errorString() ) );
    return;
  }
  addAddressBookImportInfo( i18n( "Contact created done" ) );
}


void AbstractAddressBook::addAddressBookImportInfo( const QString& log )
{
  mImportWizard->importAddressBookPage()->addFilterImportInfo( log );
}

void AbstractAddressBook::addAddressBookImportError( const QString& log )
{
  mImportWizard->importAddressBookPage()->addFilterImportError( log );
}

#include "abstractaddressbook.moc"

  
