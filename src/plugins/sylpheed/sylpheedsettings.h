/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

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

#ifndef SYLPHEEDSETTINGS_H
#define SYLPHEEDSETTINGS_H

#include <ImportWizard/AbstractSettings>
#include <QString>

class KConfigGroup;
class QFile;

class SylpheedSettings : public AbstractSettings
{
public:
    SylpheedSettings();
    ~SylpheedSettings();
    virtual void importSettings(const QString &filename, const QString &path);

protected:
    void readCustomHeader(QFile *customHeaderFile);
    virtual void readGlobalSettings(const KConfigGroup &group);
    void readAccount(const KConfigGroup &accountConfig, bool checkMailOnStartup, int intervalCheckMail);
    void readIdentity(const KConfigGroup &accountConfig);
    QString readTransport(const KConfigGroup &accountConfig);
    void readPop3Account(const KConfigGroup &accountConfig, bool checkMailOnStartup, int intervalCheckMail);
    void readImapAccount(const KConfigGroup &accountConfig, bool checkMailOnStartup, int intervalCheckMail);
    void readSignature(const KConfigGroup &accountConfig, KIdentityManagement::Identity *identity);

    virtual void readSettingsColor(const KConfigGroup &group);
    virtual void readTemplateFormat(const KConfigGroup &group);

    virtual void readTagColor(const KConfigGroup &group);

    virtual void readDateFormat(const KConfigGroup &group);
    QString convertToKmailTemplate(const QString &templateStr);
};

#endif /* SYLPHEEDSETTINGS_H */
