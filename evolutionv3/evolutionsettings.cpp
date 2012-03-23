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
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>

#include <QFile>
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
  QString errorMsg;
  int errorRow;
  int errorCol;
  if ( !doc.setContent( &file, &errorMsg, &errorRow, &errorCol ) ) {
    kDebug() << "Unable to load document.Parse error in line " << errorRow
             << ", col " << errorCol << ": " << errorMsg;
    return;
  }
  return;
  QDomElement config = doc.documentElement();

  if ( config.isNull() ) {
    kDebug() << "No config found";
    return;
  }
  for ( QDomElement e = config.firstChildElement(); !e.isNull(); e = e.nextSiblingElement() ) {
    const QString tag = e.tagName();
    if ( tag == QLatin1String( "entry" ) ) {
      if(e.hasAttribute( "name" ) && e.attribute("name") == QLatin1String("accounts") )
      {
        readAccount(e);
      }
    }
  }
}

EvolutionSettings::~EvolutionSettings()
{
}
   
void EvolutionSettings::readAccount(const QDomElement &account)
{
  for ( QDomElement accountConfig = account.firstChildElement(); !accountConfig.isNull(); accountConfig = accountConfig.nextSiblingElement() ) {
    if(accountConfig.tagName() == QLatin1String("li")) {
      //TODO
    }
  }
}
