#pragma once
/************************************************************************
**
** Authors:   Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve),
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

#ifndef PRESPAMMEDPATH_H_
#define PRESPAMMEDPATH_H_

#include <QObject>
#include <QString>
#include <QtCore>

#include "../parser/CommandQueue.h"

class MapCanvas;
class MapData;

class PrespammedPath : public QObject
{
    Q_OBJECT
public:
    explicit PrespammedPath(QObject *parent = nullptr);
    ~PrespammedPath();

public:
    auto &getQueue() const { return m_queue; }

signals:
    void update();

public slots:
    void setPath(CommandQueue, bool);

private:
    CommandQueue m_queue{};
};

#endif
