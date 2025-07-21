#include "MapRasterLayerProvider_Software_P.h"
#include "MapRasterLayerProvider_Software.h"

#define OSMAND_PERFORMANCE_METRICS 1
#if !defined(OSMAND_PERFORMANCE_METRICS)
#   define OSMAND_PERFORMANCE_METRICS 0
#endif // !defined(OSMAND_PERFORMANCE_METRICS)

#include "ignore_warnings_on_external_includes.h"
#include <SkStream.h>
#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkImageEncoder.h>
#include "restore_internal_warnings.h"

#include "MapPrimitivesProvider.h"
#include "ObfsCollection.h"
#include "ObfDataInterface.h"
#include "MapRasterizer.h"
#include "MapPresentationEnvironment.h"
#include "Stopwatch.h"
#include "Utilities.h"
#include "Logging.h"

OsmAnd::MapRasterLayerProvider_Software_P::MapRasterLayerProvider_Software_P(MapRasterLayerProvider_Software* owner_)
    : MapRasterLayerProvider_P(owner_)
    , owner(owner_)
{
}

OsmAnd::MapRasterLayerProvider_Software_P::~MapRasterLayerProvider_Software_P()
{
}

sk_sp<SkImage> OsmAnd::MapRasterLayerProvider_Software_P::rasterize(
    const MapRasterLayerProvider::Request& request,
    const std::shared_ptr<const MapPrimitivesProvider::Data>& primitivesTile,
    MapRasterLayerProvider_Metrics::Metric_obtainData* const metric_)
{
#if OSMAND_PERFORMANCE_METRICS
    MapRasterLayerProvider_Metrics::Metric_obtainData localMetric;
    const auto metric = metric_ ? metric_ : &localMetric;
#else
    const auto metric = metric_;
#endif

    const Stopwatch totalStopwatch(
#if OSMAND_PERFORMANCE_METRICS
        true
#else
        metric != nullptr
#endif // OSMAND_PERFORMANCE_METRICS
        );

    // Allocate rasterization target
    const auto tileSize = owner->getTileSize();
    SkBitmap bitmap;
    if (!bitmap.tryAllocPixels(SkImageInfo::MakeN32Premul(tileSize, tileSize)))
    {
        LogPrintf(LogSeverityLevel::Error,
            "Failed to allocate buffer for rasterization surface %dx%d",
            tileSize,
            tileSize);
        return nullptr;
    }

    // Create rasterization canvas
    SkCanvas canvas(bitmap);

    // Perform actual rasterization
    if (!owner->fillBackground)
        canvas.clear(SK_ColorTRANSPARENT);
    _mapRasterizer->rasterize(
        Utilities::tileBoundingBox31(request.tileId, request.zoom),
        primitivesTile->primitivisedObjects,
        canvas,
        owner->fillBackground,
        nullptr,
        metric ? metric->findOrAddSubmetricOfType<MapRasterizer_Metrics::Metric_rasterize>().get() : nullptr,
        request.queryController);

#if OSMAND_PERFORMANCE_METRICS
#if OSMAND_PERFORMANCE_METRICS <= 1
    auto time_since_epoch = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count();
    LogPrintf(LogSeverityLevel::Info, ">>>> %ld RASTER %f: %dx%d@%d rasterized on CPU",
        millis, totalStopwatch.elapsed(),
        request.tileId.x,
        request.tileId.y,
        request.zoom);
#else
    LogPrintf(LogSeverityLevel::Info,
        "%dx%d@%d rasterized on CPU in %fs:\n%s",
        tileId.x,
        tileId.y,
        zoom,
        totalStopwatch.elapsed(),
        qPrintable(metric ? metric->toString(QLatin1String("\t - ")) : QLatin1String("(null)")));
#endif // OSMAND_PERFORMANCE_METRICS <= 1
#endif // OSMAND_PERFORMANCE_METRICS

    return bitmap.asImage();
}
