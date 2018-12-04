#pragma once
/************************************************************************
**
** Authors:   Jan 'Kovis' Struhar <kovis@sourceforge.net> (Kovis)
**            Marek Krejza <krejza@gmail.com> (Caligor)
**
** This file is part of the MMapper project.
** Maintained by Nils Schimmelmann <nschimme@gmail.com>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the:
** Free Software Foundation, Inc.
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
************************************************************************/

#ifndef ANSI_COMBO_H
#define ANSI_COMBO_H

#include <QComboBox>
#include <QIcon>
#include <QString>
#include <QVector>
#include <QtCore>

class QColor;
class QObject;
class QWidget;

enum class AnsiMode { ANSI_FG, ANSI_BG };

class AnsiCombo : public QComboBox
{
    using super = QComboBox;
    Q_OBJECT

public:
    static void makeWidgetColoured(QWidget *, const QString &ansiColor);

    explicit AnsiCombo(AnsiMode mode, QWidget *parent = nullptr);
    explicit AnsiCombo(QWidget *parent = nullptr);

    void initColours(AnsiMode mode);

    /// get currently selected ANSI code like [32m for green colour
    QString text() const;

    void setText(const QString &);

    static constexpr const int DEFAULT_FG = 254;
    static constexpr const int DEFAULT_BG = 255;

    struct AnsiColor
    {
        QColor colFg{Qt::white};
        QColor colBg{Qt::black};
        int ansiCodeFg{DEFAULT_FG};
        int ansiCodeBg{DEFAULT_BG};
        QString intelligibleNameFg{"none"};
        QString intelligibleNameBg{"none"};
        bool bold{false};
        bool underline{false};
    };

    ///\return true if string is valid ANSI color code
    static AnsiColor colorFromString(const QString &ansiString);

    ///\return true, if index is valid color code
    static bool colorFromNumber(int numColor, QColor &col, QString &intelligibleName);

protected slots:
    void afterEdit(const QString &);

protected:
    /// populate the list with ANSI codes and coloured boxes
    void fillAnsiList();

    class AnsiItem
    {
    public:
        QString ansiCode{};
        QString description{};
        QIcon picture{};
    };
    using AnsiItemVector = QVector<AnsiItem>;

    static AnsiItem initAnsiItem(int index);

    AnsiItemVector colours{};
};

#endif
