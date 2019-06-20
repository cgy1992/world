#ifndef WORLD_TERRAINNODE_H
#define WORLD_TERRAINNODE_H

#include "world/core/WorldConfig.h"

#include <utility>
#include <functional>
#include <set>

#include "world/core/TileSystem.h"
#include "world/flat/IGround.h"
#include "Terrain.h"
#include "ITerrainWorker.h"

namespace world {

class PGround;

/** This class manages an infinite ground with as much details
 * as we want. */
class WORLDAPI_EXPORT HeightmapGround : public GroundNode {
public:
    struct Tile;

    HeightmapGround(double unitSize = 6000, double minAltitude = -2000,
                    double maxAltitude = 4000);

    ~HeightmapGround() override;

    // PARAMETERS
    // TODO constraints
    void setMaxAltitude(double max) { _maxAltitude = max; }

    void setMinAltitude(double min) { _minAltitude = min; }

    double getMaxAltitude() const { return _maxAltitude; }

    double getMinAltitude() const { return _minAltitude; }

    double getAltitudeRange() const { return _maxAltitude - _minAltitude; }

    void setTerrainResolution(int terrainRes) { _terrainRes = terrainRes; }

    void setTextureRes(int textureRes) {
        _textureRes = textureRes;
        _tileSystem._bufferRes.x = _tileSystem._bufferRes.y = _textureRes;
    }

    void setMaxLOD(int lod) { _tileSystem._maxLod = lod; }

    // TERRAIN WORKERS
    /** Adds a default worker set to generate heightmaps in the
     * ground. This method is for quick-setup purpose. */
    void setDefaultWorkerSet();

    /** Creates and adds an ITerrainWorker to the Ground. The
     * ITerrainWorkers belong to the core of the generation process.
     * Each part of the ground is generated by a chain of worker,
     * each of which adds one feature after the other to the terrain.
     * */
    template <typename T, typename... Args> T &addWorker(Args &&... args);

    void setLodRange(const ITerrainWorker &worker, int minLod, int maxLod);

    // EXPLORATION
    double observeAltitudeAt(double x, double y, double resolution) override;

    void collect(ICollector &collector, const IResolutionModel &resolutionModel,
                 const ExplorationContext &ctx =
                     ExplorationContext::getDefault()) override;

    void paintTexture(const vec2d &origin, const vec2d &size,
                      const vec2d &resolutionRange, const Image &img) override;

private:
    PGround *_internal;

    // Param�tres
    /** L'altitude minimum du monde. Le niveau de la mer est fix� � 0. */
    double _minAltitude;
    /** L'altitude maximum du monde. Le niveau de la mer est fix� � 0. */
    double _maxAltitude;

    int _terrainRes = 33;
    int _textureRes = 1024;

    TileSystem _tileSystem;

    bool _manageCache = false;
    u32 _maxCacheSize = 20000;

    // WORKER
    void addWorkerInternal(ITerrainWorker *worker);

    double observeAltitudeAt(double x, double y, int lvl);

    void addTerrain(const TileCoordinates &key, ICollector &collector);


    // CACHE
    /** Updates the cache, free old used memory if needed (by saving
     * the terrains to files or discard them) */
    void updateCache();

    void registerAccess(const TileCoordinates &key, Tile &tile);


    // ACCESS
    HeightmapGround::Tile &provide(const TileCoordinates &key);

    Terrain &provideTerrain(const TileCoordinates &key);

    Mesh &provideMesh(const TileCoordinates &key);

    /** Get tile at given key if it exists. If it does not exist, returns
     * nullopt. */
    optional<HeightmapGround::Tile &> lookUpTile(const TileCoordinates &key);

    bool isGenerated(const TileCoordinates &key);


    // DATA
    /** Gets a unique string id for the given tile in the Ground. */
    std::string getTerrainDataId(const TileCoordinates &key) const;


    // GENERATION
    void addNotGeneratedParents(std::set<TileCoordinates> &keys);

    /** Generate the terrains located at all the keys given in parameters. If
     * the terrains already exist they are not generated again. */
    void generateTerrains(const std::set<TileCoordinates> &keys);

    void generateMesh(const TileCoordinates &key);

    friend class PGround;
    friend class GroundContext;
};

template <typename T, typename... Args>
T &HeightmapGround::addWorker(Args &&... args) {
    T *worker = new T(args...);
    addWorkerInternal(worker);
    return *worker;
};
} // namespace world

#endif // WORLD_TERRAINNODE_H
