/*
   Copyright (C) 2012-2019 Montel Laurent <montel@kde.org>

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

#include "sylpheedsettingsutils.h"

bool SylpheedSettingsUtils::readConfig(const QString &key, const KConfigGroup &accountConfig, int &value, bool remove_underscore)
{
    QString cleanedKey(key);
    if (remove_underscore) {
        cleanedKey.remove(QLatin1Char('_'));
    }
    const QString useKey = QLatin1String("set_") + cleanedKey;
    if (accountConfig.hasKey(useKey) && (accountConfig.readEntry(useKey, 0) == 1)) {
        value = accountConfig.readEntry(key, 0);
        return true;
    }
    return false;
}

bool SylpheedSettingsUtils::readConfig(const QString &key, const KConfigGroup &accountConfig, QString &value, bool remove_underscore)
{
    QString cleanedKey(key);
    if (remove_underscore) {
        cleanedKey.remove(QLatin1Char('_'));
    }
    const QString useKey = QLatin1String("set_") + cleanedKey;
    if (accountConfig.hasKey(useKey) && (accountConfig.readEntry(useKey, 0) == 1)) {
        value = accountConfig.readEntry(key);
        return true;
    }
    return false;
}
