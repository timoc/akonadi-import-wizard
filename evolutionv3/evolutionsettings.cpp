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

#include "evolutionsettings.h"

#include <kpimidentities/identity.h>

#include <mailtransport/transportmanager.h>

#include <KConfig>
#include <KConfigGroup>
#include <KDebug>

#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>

EvolutionSettings::EvolutionSettings( const QString& filename, ImportWizard *parent )
    :AbstractSettings( parent )
{
  //Read gconf file
  QFile file(filename);
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug()<<" We can't open file"<<filename;
    return;
  }
  QDomDocument doc;
  if ( !loadInDomDocument( &file, doc ) )
    return;
  QDomElement config = doc.documentElement();

  if ( config.isNull() ) {
    kDebug() << "No config found";
    return;
  }
  for ( QDomElement e = config.firstChildElement(); !e.isNull(); e = e.nextSiblingElement() ) {
    const QString tag = e.tagName();
    if ( tag == QLatin1String( "entry" ) ) {
      if ( e.hasAttribute( "name" ) ) {
        const QString attr = e.attribute("name");
        if ( attr == QLatin1String( "accounts" ) ) {
          readAccount(e);
        } else if ( attr == QLatin1String( "signatures" ) ) {
          readSignatures( e );
        }
      }  
    }
  }
}

EvolutionSettings::~EvolutionSettings()
{
}

bool EvolutionSettings::loadInDomDocument( QFile *file, QDomDocument & doc )
{
  QString errorMsg;
  int errorRow;
  int errorCol;
  if ( !doc.setContent( file, &errorMsg, &errorRow, &errorCol ) ) {
    kDebug() << "Unable to load document.Parse error in line " << errorRow
             << ", col " << errorCol << ": " << errorMsg;
    return false;
  }
  return true;
}

bool EvolutionSettings::loadInDomDocument( const QString &file, QDomDocument & doc )
{
  QString errorMsg;
  int errorRow;
  int errorCol;
  if ( !doc.setContent( file, &errorMsg, &errorRow, &errorCol ) ) {
    kDebug() << "Unable to load document.Parse error in line " << errorRow
             << ", col " << errorCol << ": " << errorMsg;
    return false;
  }
  return true;
}


void EvolutionSettings::readSignatures(const QDomElement &account)
{
  for ( QDomElement signatureConfig = account.firstChildElement(); !signatureConfig.isNull(); signatureConfig = signatureConfig.nextSiblingElement() ) {
    if(signatureConfig.tagName() == QLatin1String("li")) {
      QDomElement stringValue = signatureConfig.firstChildElement();
      extractSignatureInfo(stringValue.text());
    }
  }
}

void EvolutionSettings::extractSignatureInfo( const QString&info )
{
  qDebug()<<" signature info "<<info;
  QDomDocument signature;
  if ( !loadInDomDocument( info, signature ) )
    return;

  QDomElement domElement = signature.documentElement();

  if ( domElement.isNull() ) {
    kDebug() << "Signature not found";
    return;
  }
  for ( QDomElement e = domElement.firstChildElement(); !e.isNull(); e = e.nextSiblingElement() ) {
    KPIMIdentities::Signature signature;
    
    const QString tag = e.tagName();
    const QString uid = e.attribute( QLatin1String( "uid" ) );
    const QString signatureName = e.attribute( QLatin1String( "name" ) );
    const QString format = e.attribute( QLatin1String( "text" ) );
    const bool automatic = ( e.attribute( QLatin1String( "auto" ) ) == QLatin1String( "true" ) );

    if ( format == QLatin1String( "text/html" ) ) {
      signature.setInlinedHtml( true );
    } else if ( format == QLatin1String( "text/plain" ) ) {
      signature.setInlinedHtml( false );
    }
    
    
    if ( tag == QLatin1String( "filename" ) ) {
      if ( e.hasAttribute( QLatin1String( "script" ) ) && e.attribute( QLatin1String( "script" ) ) == QLatin1String( "true" ) ){
        signature.setUrl( e.text(), true );
        signature.setType( KPIMIdentities::Signature::FromCommand );
      }
      else {
        signature.setUrl( QDir::homePath() + QLatin1String( ".local/share/evolution/signatures/" ) + e.text(), false );
        signature.setType( KPIMIdentities::Signature::FromFile );

      }
    }
    
    if ( automatic ) {
      //TODO
    }
    
    mMapSignature.insert( uid, signature );
        
    qDebug()<<" signature tag :"<<tag;
  }
}

void EvolutionSettings::readAccount(const QDomElement &account)
{
  for ( QDomElement accountConfig = account.firstChildElement(); !accountConfig.isNull(); accountConfig = accountConfig.nextSiblingElement() ) {
    if(accountConfig.tagName() == QLatin1String("li")) {
      QDomElement stringValue = accountConfig.firstChildElement();
      extractAccountInfo(stringValue.text());
    }
  }
}

void EvolutionSettings::extractAccountInfo(const QString& info)
{
  qDebug()<<" info "<<info;
  //Read QDomElement
  QDomDocument account;
  if ( !loadInDomDocument( info, account ) )
    return;

  QDomElement domElement = account.documentElement();

  if ( domElement.isNull() ) {
    kDebug() << "Account not found";
    return;
  }
  KPIMIdentities::Identity* newIdentity = createIdentity();
  QString name;
  if(domElement.hasAttribute(QLatin1String("name"))) {
    name = domElement.attribute(QLatin1String("name"));
  }
  for ( QDomElement e = domElement.firstChildElement(); !e.isNull(); e = e.nextSiblingElement() ) {
    const QString tag = e.tagName();
    if ( tag == QLatin1String( "identity" ) )
    {
      for ( QDomElement identity = e.firstChildElement(); !identity.isNull(); identity = identity.nextSiblingElement() ) {
        const QString identityTag = identity.tagName();
        if ( identityTag == QLatin1String( "name" ) )
        {
          newIdentity->setIdentityName( identity.text() );
        }
        else if ( identityTag == QLatin1String( "addr-spec" ) )
        {
          newIdentity->setPrimaryEmailAddress(identity.text());
        }
        else if ( identityTag == QLatin1String( "organization" ) )
        {
          newIdentity->setOrganization(identity.text());
        }
        else if ( identityTag == QLatin1String( "signature" ) )
        {
          if ( identity.hasAttribute( "uid" ) ) {
            newIdentity->setSignature( mMapSignature.value( identity.attribute( "uid" ) ) );
          }
        }
        else if ( identityTag == QLatin1String( "reply-to" ) )
        {
          newIdentity->setReplyToAddr( identity.text() );
        }
        else
        {
          qDebug()<<" tag identity not found :"<<identityTag;
        }
      }
    }
    else if ( tag == QLatin1String( "source" ) )
    {
      if(e.hasAttribute(QLatin1String("save-passwd"))&& e.attribute( "save-passwd" ) == QLatin1String( "true" ) ) {
        //TODO
      }
      int interval = -1;
      bool intervalCheck = false;
      if(e.hasAttribute(QLatin1String("auto-check"))) {
        intervalCheck = ( e.attribute(QLatin1String("auto-check")) == QLatin1String( "true" ) );
      }
      if(e.hasAttribute(QLatin1String("auto-check-timeout"))) {
        interval = e.attribute(QLatin1String("auto-check-timeout")).toInt();
      }
      for ( QDomElement server = e.firstChildElement(); !server.isNull(); server = server.nextSiblingElement() ) {
        const QString serverTag = server.tagName();
        if ( serverTag == QLatin1String( "url" ) ) {
          qDebug()<<" server.text() :"<<server.text();
          QUrl serverUrl( server.text() );
          const QString scheme = serverUrl.scheme();
          QMap<QString, QVariant> settings;
          const int port = serverUrl.port();

          const QString path = serverUrl.path();
          qDebug()<<" path !"<<path;
          const QString userName = serverUrl.userInfo();
          //imapx://name@pop3.xx.org:993/;security-method=ssl-on-alternate-port;namespace;shell-command=ssh%20-C%20-l%20%25u%20%25h%20exec%20/usr/sbin/imapd%20;use-shell-command=true
          if(scheme == QLatin1String("imap") || scheme == QLatin1String("imapx")) {
            if( port > 0 )
              settings.insert(QLatin1String("ImapPort"),port);
            //Perhaps imapx is specific don't know
            if ( intervalCheck ) {
              settings.insert( QLatin1String( "IntervalCheckEnabled" ), true );
            }
            if ( interval > -1 ) {
              settings.insert(QLatin1String("IntervalCheckTime" ), interval );
            }

            bool found = false;
            const QString securityMethod = getSecurityMethod( path, found );
#if 0 //FIXME
            if( found ) {
              if( securityMethod == QLatin1String("none")) {
                //Nothing
              } else if(securityMethod == QLatin1String("ssl-on-alternate-port")){
                settings.insert( QLatin1String( "UseSSL" ), true );
              } else {
                qDebug()<<" security method unknown : "<<path;
              }
            } else {
              settings.insert( QLatin1String( "UseTLS" ), true );
            }
#endif

            addAuth(settings, QLatin1String( "Authentication" ), userName);
            createResource( "akonadi_imap_resource", name,settings );
          } else if(scheme == QLatin1String("pop")) {
            if( port > 0 )
              settings.insert(QLatin1String("Port"),port);
            bool found = false;
            const QString securityMethod = getSecurityMethod( path, found );
            if( found ) {
              if( securityMethod == QLatin1String("none")) {
                //Nothing
              } else if(securityMethod == QLatin1String("ssl-on-alternate-port")){
                settings.insert( QLatin1String( "UseSSL" ), true );
              } else {
                qDebug()<<" security method unknown : "<<path;
              }
            } else {
              settings.insert( QLatin1String( "UseTLS" ), true );
            }

            if ( intervalCheck ) {
              settings.insert( QLatin1String( "IntervalCheckEnabled" ), true );
            }
            if ( interval > -1 ) {
              settings.insert(QLatin1String("IntervalCheckInterval" ), interval );
            }
            if(e.hasAttribute(QLatin1String("keep-on-server"))&& e.attribute(QLatin1String("keep-on-server") ) == QLatin1String( "true" ) ) {
              settings.insert(QLatin1String("LeaveOnServer"),true);
            }
            addAuth(settings, QLatin1String( "AuthenticationMethod" ), userName);
            createResource( "akonadi_pop3_resource", name, settings );

          } else if( scheme == QLatin1String("spool") || scheme == QLatin1String("mbox") ) {
            //mbox file
            settings.insert(QLatin1String("Path"),path);
            settings.insert(QLatin1String("DisplayName"),name);
            createResource( "akonadi_mbox_resource", name, settings );
          } else if( scheme == QLatin1String("maildir")) {
            settings.insert(QLatin1String("Path"),path);
            createResource( "akonadi_maildir_resource", name, settings );
          } else {
            qDebug()<<" unknown scheme "<<scheme;
          }
        } else {
          qDebug()<<" server tag unknow :"<<serverTag;
        }
      }
    }
    else if ( tag == QLatin1String( "transport" ) )
    {
      if ( e.hasAttribute( "save-passwd" ) && e.attribute( "save-passwd" ) == QLatin1String( "true" ) )
      {
        //TODO save to kwallet ?
      }
      
      MailTransport::Transport *transport = createTransport();
      for ( QDomElement smtp = e.firstChildElement(); !smtp.isNull(); smtp = smtp.nextSiblingElement() ) {
        const QString smtpTag = smtp.tagName();
        if ( smtpTag == QLatin1String( "url" ) ) {
          qDebug()<<" smtp.text() :"<<smtp.text();
          QUrl smtpUrl( smtp.text() );
          const QString scheme = smtpUrl.scheme();
          if(scheme == QLatin1String("sendmail")) {
            transport->setType(MailTransport::Transport::EnumType::Sendmail);
          } else {
            transport->setHost( smtpUrl.host() );
            transport->setName( smtpUrl.host() );

            const int port = smtpUrl.port();
            if ( port > 0 )
              transport->setPort( port );

            const QString userName = smtpUrl.userInfo();
            bool found = false;
            const QString authMethod = getAuthMethod(userName, found);
            if( found ) {
              if(authMethod==QLatin1String("PLAIN")) {
                transport->setAuthenticationType(MailTransport::Transport::EnumAuthenticationType::PLAIN);
              } else if(authMethod==QLatin1String("NTLM")) {
                transport->setAuthenticationType(MailTransport::Transport::EnumAuthenticationType::NTLM);
              } else if(authMethod==QLatin1String("DIGEST-MD5")) {
                transport->setAuthenticationType(MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5);
              } else if(authMethod==QLatin1String("CRAM-MD5")) {
                transport->setAuthenticationType(MailTransport::Transport::EnumAuthenticationType::CRAM_MD5);
              } else if(authMethod==QLatin1String("LOGIN")) {
                transport->setAuthenticationType(MailTransport::Transport::EnumAuthenticationType::LOGIN);
              } else if(authMethod==QLatin1String("POPB4SMTP")) {
                transport->setAuthenticationType(MailTransport::Transport::EnumAuthenticationType::APOP); //????
              } else {
                qDebug()<<" smtp auth method unknown "<<authMethod;
              }
            }

            const QString path = smtpUrl.path();
            found = false;
            const QString securityMethod = getSecurityMethod( path, found );
            if( found ) {
              if( securityMethod == QLatin1String("none")) {
                transport->setEncryption( MailTransport::Transport::EnumEncryption::None );

              } else if(securityMethod == QLatin1String("ssl-on-alternate-port")){
                transport->setEncryption( MailTransport::Transport::EnumEncryption::SSL );
              } else {
                qDebug()<<" security method unknown : "<<path;
              }
            } else {
              transport->setEncryption( MailTransport::Transport::EnumEncryption::TLS );
            }
          }
        } else {
          qDebug()<<" smtp tag unknow :"<<smtpTag;
        }
      }
      storeTransport(transport, true );
    }
    else if ( tag == QLatin1String( "drafts-folder" ) )
    {
      const QString selectedFolder = adaptFolder( e.text().remove( QLatin1String( "folder://" ) ) );
      newIdentity->setDrafts(selectedFolder); 
    }
    else if ( tag == QLatin1String( "sent-folder" ) )
    {
      const QString selectedFolder = adaptFolder( e.text().remove( QLatin1String( "folder://" ) ) );
      newIdentity->setFcc(selectedFolder);
    }
    else if ( tag == QLatin1String( "auto-cc" ) )
    {
      if ( e.hasAttribute( "always" ) && ( e.attribute( "always" ) == QLatin1String( "true" ) ) )
      {
        QDomElement recipient = e.firstChildElement();
        const QString text = recipient.text();
        newIdentity->setCc(text);
      }
    }
    else if( tag == QLatin1String("reply-to"))
    {
      newIdentity->setReplyToAddr(e.text());
    }
    else if ( tag == QLatin1String( "auto-bcc" ) )
    {
      if ( e.hasAttribute( "always" ) && ( e.attribute( "always" ) == QLatin1String( "true" ) ) )
      {
        QDomElement recipient = e.firstChildElement();
        const QString text = recipient.text();
        newIdentity->setBcc(text);
      }
    }
    else if ( tag == QLatin1String( "receipt-policy" ) )
    {
      if ( e.hasAttribute( QLatin1String( "policy" ) ) ) {
        const QString policy = e.attribute( QLatin1String( "policy" ) );
        //TODO
      }
    }
    else if ( tag == QLatin1String( "pgp" ) )
    {
      if ( e.hasAttribute( QLatin1String( "encrypt-to-self" ) ) &&
           ( e.attribute( QLatin1String( "encrypt-to-self" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
      if ( e.hasAttribute( QLatin1String( "always-trust" ) ) &&
           ( e.attribute( QLatin1String( "always-trust" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
      if ( e.hasAttribute( QLatin1String( "always-sign" ) ) &&
           ( e.attribute( QLatin1String( "always-sign" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
      if ( e.hasAttribute( QLatin1String( "no-imip-sign" ) ) &&
           ( e.attribute( QLatin1String( "no-imip-sign" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
    }
    else if ( tag == QLatin1String( "smime" ) )
    {
      if ( e.hasAttribute( QLatin1String( "sign-default" ) ) &&
           ( e.attribute( QLatin1String( "sign-default" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
      if ( e.hasAttribute( QLatin1String( "encrypt-default" ) ) &&
           ( e.attribute( QLatin1String( "encrypt-default" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
      if ( e.hasAttribute( QLatin1String( "encrypt-to-self" ) ) &&
           ( e.attribute( QLatin1String( "encrypt-to-self" ) ) == QLatin1String( "true" ) ) ) {
        //TODO
      }
      //TODO
    }
    else
      qDebug()<<" tag not know :"<<tag;

  }
  storeIdentity(newIdentity);
}

QString EvolutionSettings::getSecurityMethod( const QString& path, bool & found )
{
  const int index = path.indexOf(QLatin1String("security-method="));
  if(index != -1) {
    const QString securityMethod = path.right(path.length() - index - 16 /*security-method=*/);
    found = true;
    return securityMethod;
  }
  found = false;
  return QString();
}

QString EvolutionSettings::getAuthMethod( const QString& path, bool & found)
{
  const int index = path.indexOf(QLatin1String("auth="));
  if(index != -1) {
    const QString securityMethod = path.right(path.length() - index - 5 /*auth=*/);
    found = true;
    return securityMethod;
  }
  found = false;
  return QString();
}

void EvolutionSettings::addAuth(QMap<QString, QVariant>& settings, const QString & argument, const QString& userName)
{
  bool found = false;
  const QString authMethod = getAuthMethod(userName, found);
  if( found ) {
    if(authMethod==QLatin1String("PLAIN")) {
      settings.insert( argument, MailTransport::Transport::EnumAuthenticationType::PLAIN );
    } else if(authMethod==QLatin1String("NTLM")) {
      settings.insert( argument, MailTransport::Transport::EnumAuthenticationType::NTLM );
    } else if(authMethod==QLatin1String("DIGEST-MD5")) {
      settings.insert( argument, MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5 );
    } else if(authMethod==QLatin1String("CRAM-MD5")) {
      settings.insert( argument, MailTransport::Transport::EnumAuthenticationType::CRAM_MD5 );
    } else if(authMethod==QLatin1String("LOGIN")) {
      settings.insert( argument, MailTransport::Transport::EnumAuthenticationType::LOGIN );
    } else if(authMethod==QLatin1String("POPB4SMTP")) {
      settings.insert( argument, MailTransport::Transport::EnumAuthenticationType::APOP ); //????
    } else {
      qDebug()<<" smtp auth method unknown "<<authMethod;
    }
  }
}
