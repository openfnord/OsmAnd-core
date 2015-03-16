#ifndef _OSMAND_CORE_MAP_OBJECTS_SYMBOLS_PROVIDER_P_H_
#define _OSMAND_CORE_MAP_OBJECTS_SYMBOLS_PROVIDER_P_H_

#include "stdlib_common.h"
#include <array>
#include <functional>

#include "QtExtensions.h"
#include "ignore_warnings_on_external_includes.h"
#include <QReadWriteLock>
#include <QList>
#include <QVector>
#include "restore_internal_warnings.h"

#include "OsmAndCore.h"
#include "CommonTypes.h"
#include "PrivateImplementation.h"
#include "IMapTiledSymbolsProvider.h"
#include "MapObject.h"
#include "MapObjectsSymbolsProvider.h"

namespace OsmAnd
{
    class MapSymbolsGroup;

    class MapObjectsSymbolsProvider_P Q_DECL_FINAL
    {
    public:
        typedef MapObjectsSymbolsProvider::FilterCallback FilterCallback;
        typedef MapObjectsSymbolsProvider::MapObjectSymbolsGroup MapObjectSymbolsGroup;

    private:
        struct ComputedPinPoint
        {
            PointI point31;

            unsigned int basePathPointIndex;
            float normalizedOffsetFromBasePathPoint;
        };

        QList<ComputedPinPoint> computePinPoints(
            const QVector<PointI>& path31,
            const float globalPaddingInPixels,
            const float blockSpacingInPixels,
            const float symbolSpacingInPixels,
            const QVector<float>& symbolsWidthsInPixels,
            const ZoomLevel minZoom,
            const ZoomLevel maxZoom,
            const ZoomLevel neededZoom) const;

        void computeSymbolsPinPoints(
            const QVector<float>& symbolsWidthsN,
            const int symbolsCount,
            float nextSymbolStartN,
            const QVector<float>& pathSegmentsLengthN,
            const QVector<PointI>& path31,
            const float symbolSpacingN,
            QList<ComputedPinPoint>& outComputedPinPoints) const;

    protected:
        MapObjectsSymbolsProvider_P(MapObjectsSymbolsProvider* owner);

        ImplementationInterface<MapObjectsSymbolsProvider> owner;

        struct RetainableCacheMetadata : public IMapDataProvider::RetainableCacheMetadata
        {
            RetainableCacheMetadata(
                const std::shared_ptr<const IMapDataProvider::RetainableCacheMetadata>& binaryMapPrimitivesRetainableCacheMetadata);
            virtual ~RetainableCacheMetadata();

            std::shared_ptr<const IMapDataProvider::RetainableCacheMetadata> binaryMapPrimitivesRetainableCacheMetadata;
        };
    public:
        virtual ~MapObjectsSymbolsProvider_P();

        bool obtainData(
            const TileId tileId,
            const ZoomLevel zoom,
            std::shared_ptr<MapObjectsSymbolsProvider::Data>& outTiledData,
            const IQueryController* const queryController,
            const FilterCallback filterCallback);

    friend class OsmAnd::MapObjectsSymbolsProvider;
    };
}

#endif // !defined(_OSMAND_CORE_MAP_OBJECTS_SYMBOLS_PROVIDER_P_H_)
