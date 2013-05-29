/**
* @file
*
* @section LICENSE
*
* OsmAnd - Android navigation software based on OSM maps.
* Copyright (C) 2010-2013  OsmAnd Authors listed in AUTHORS file
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MODEL_MAP_OBJECT_H_
#define __MODEL_MAP_OBJECT_H_

#include <stdint.h>
#include <tuple>

#include <QList>
#include <QMap>
#include <QVector>
#include <QString>

#include <OsmAndCore.h>
#include <Area.h>

namespace OsmAnd {

    class ObfMapSection;
    class Rasterizer;

    namespace Model {

        class OSMAND_CORE_API MapObject
        {
        public:
            enum FoundationType
            {
                Unknown,
                FullLand,
                FullWater
            };
        private:
        protected:
            MapObject(ObfMapSection* section);

            uint64_t _id;
            FoundationType _foundation;
            bool _isArea;
            QVector< PointI > _coordinates;
            QList< QVector< PointI > > _polygonInnerCoordinates;
            QVector< std::tuple< QString, QString > > _types;
            QVector< std::tuple< QString, QString > > _extraTypes;
            QMap< uint32_t, QString > _names;
        public:
            virtual ~MapObject();

            ObfMapSection* const section;
            const uint64_t& id;
            const FoundationType& foundation;
            const QMap<uint32_t, QString>& names;

            int getSimpleLayerValue() const;
            bool isClosedFigure(bool checkInner = false) const;

            bool containsType(const QString& tag, const QString& value, bool checkAdditional = false) const;

            friend class OsmAnd::ObfMapSection;
            friend class OsmAnd::Rasterizer;
        };

    } // namespace Model

} // namespace OsmAnd

#endif // __MODEL_MAP_OBJECT_H_
