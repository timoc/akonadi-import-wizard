/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SELECTPROGRAMLISTWIDGET_H
#define SELECTPROGRAMLISTWIDGET_H

#include <QListWidget>

class SelectProgramListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit SelectProgramListWidget(QWidget *parent = nullptr);
    ~SelectProgramListWidget() override;

    void setNoProgramFound(bool noProgramFound);

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void generalPaletteChanged();
    QColor mTextColor;
    bool mNoProgramFound = false;
};

#endif // SELECTPROGRAMLISTWIDGET_H
