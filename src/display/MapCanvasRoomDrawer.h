#pragma once
/************************************************************************
**
** Authors:   Nils Schimmelmann <nschimme@gmail.com>
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

#ifndef MMAPPER_MAP_CANVAS_ROOM_DRAWER_H
#define MMAPPER_MAP_CANVAS_ROOM_DRAWER_H

#include <vector>
#include <QColor>
#include <QtCore>

#include "../expandoracommon/room.h"
#include "../global/roomid.h"
#include "../mapdata/ExitDirection.h"
#include "../mapdata/infomark.h"
#include "FontFormatFlags.h"
#include "MapCanvasData.h"
#include "RoadIndex.h"

class InfoMark;
class OpenGL;
class QOpenGLTexture;
class QPaintDevice;
class Room;
class XDisplayList;
struct Vec3f;

/* TODO: move these elsewhere */
static constexpr const float ROOM_Z_DISTANCE = 7.0f;
static constexpr const float ROOM_Z_LAYER_BUMP = 0.00001f;
static constexpr const float ROOM_BOOST_BUMP = 0.01f;
static constexpr const float ROOM_WALLS_BUMP = 0.009f; // was 0.005f but should be below boost

using RoomVector = std::vector<const Room *>;
using LayerToRooms = std::map<int, RoomVector>;

class MapCanvasRoomDrawer final
{
private:
    const MapCanvasData &m_mapCanvasData;
    OpenGL &m_opengl;
    const Coordinate2f &m_visible1;
    const Coordinate2f &m_visible2;
    const qint16 &m_currentLayer;
    const float &m_scaleFactor;
    const MapCanvasData::DrawLists &m_gllist;
    const MapCanvasData::Textures &m_textures;

public:
    explicit MapCanvasRoomDrawer(const MapCanvasData &data, OpenGL &opengl)
        : m_mapCanvasData(data)
        , m_opengl(opengl)
        , m_visible1{m_mapCanvasData.m_visible1}
        , m_visible2{m_mapCanvasData.m_visible2}
        , m_currentLayer{m_mapCanvasData.m_currentLayer}
        , m_scaleFactor{m_mapCanvasData.m_scaleFactor}
        , m_gllist{m_mapCanvasData.m_gllist}
        , m_textures{m_mapCanvasData.m_textures}
    {}

    void drawRooms(const LayerToRooms &layerToRooms,
                   const RoomIndex &roomIndex,
                   const RoomLocks &locks);
    void drawWallsAndExits(const Room *room, const RoomIndex &rooms);

    void drawInfoMark(InfoMark *);

    void drawRoomDoorName(const Room *sourceRoom,
                          ExitDirection sourceDir,
                          const Room *targetRoom,
                          ExitDirection targetDir);
    void drawConnection(const Room *leftRoom,
                        const Room *rightRoom,
                        ExitDirection startDir,
                        ExitDirection endDir,
                        bool oneway,
                        bool inExitFlags = true);
    void drawFlow(const Room *room, const RoomIndex &rooms, ExitDirection exitDirection);
    void drawVertical(const Room *room,
                      const RoomIndex &rooms,
                      qint32 layer,
                      ExitDirection direction,
                      const MapCanvasData::DrawLists::ExitUpDown::OpaqueTransparent &exlists,
                      XDisplayList doorlist);

    void drawExit(const Room *const room, const RoomIndex &rooms, qint32 layer, ExitDirection dir);
    void drawRoomConnectionsAndDoors(const Room *room, const RoomIndex &rooms);
    void drawInfoMarks();

    void renderText(float x,
                    float y,
                    const QString &text,
                    const QColor &color = Qt::white,
                    FontFormatFlags fontFormatFlag = FontFormatFlags::NONE,
                    float rotationAngle = 0.0f);

    void alphaOverlayTexture(QOpenGLTexture *texture);
    void drawLineStrip(const std::vector<Vec3f> &points);
    void drawListWithLineStipple(XDisplayList list, const QColor &color);

    void drawTextBox(const QString &name, float x, float y, float width, float height);
    void drawRoom(const Room *room, bool wantExtraDetail);
    void drawBoost(const Room *room, const RoomLocks &locks);

private:
    void drawConnEndTriUpDownUnknown(qint32 dX, qint32 dY, float dstZ);
    void drawConnEndTriNone(qint32 dX, qint32 dY, float dstZ);

private:
    void drawConnStartTri(ExitDirection startDir, float srcZ);
    void drawConnEndTri(ExitDirection endDir, qint32 dX, qint32 dY, float dstZ);
    void drawConnEndTri1Way(ExitDirection endDir, qint32 dX, qint32 dY, float dstZ);

private:
    void drawConnectionLine(ExitDirection startDir,
                            ExitDirection endDir,
                            bool oneway,
                            bool neighbours,
                            qint32 dX,
                            qint32 dY,
                            float srcZ,
                            float dstZ);

    void drawConnectionTriangles(ExitDirection startDir,
                                 ExitDirection endDir,
                                 bool oneway,
                                 qint32 dX,
                                 qint32 dY,
                                 float srcZ,
                                 float dstZ);

public:
    template<typename T>
    float getScaledFontWidth(T x, FontFormatFlags flags = FontFormatFlags::NONE) const
    {
        return m_opengl.getFontWidth(x, flags) * 0.022f / m_mapCanvasData.m_scaleFactor
               * m_mapCanvasData.m_currentStepScaleFactor;
    }

    float getScaledFontHeight() const
    {
        return m_opengl.getFontHeight() * 0.007f / m_mapCanvasData.m_scaleFactor
               * m_mapCanvasData.m_currentStepScaleFactor;
    }
};

#endif // MMAPPER_MAP_CANVAS_ROOM_DRAWER_H
