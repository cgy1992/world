#ifndef VKWORLD_DIAMONDSQUARETERRAIN_H
#define VKWORLD_DIAMONDSQUARETERRAIN_H

#include "world/core/WorldConfig.h"

#include <random>

#include "ITerrainWorker.h"

namespace world {

class WORLDAPI_EXPORT DiamondSquareTerrain : public ITerrainWorker {
public:
    /** Create a DiamondSquareTerrain worker.
     * \param jitter how much we change the height at each stage of the diamond
     * square algorithm, in percentage of the height difference. */
    DiamondSquareTerrain(double jitter = 0.5);

    void processTerrain(Terrain &terrain) override;

    void processTile(ITileContext &context) override;

    TerrainGrid *getStorage() override { return &_storage; }

private:
    std::mt19937_64 _rng;

    std::uniform_real_distribution<double> _jitter;

    TerrainGrid _storage;


    double value(double h1, double h2);

    void init(Terrain &terrain);

    void compute(Terrain &terrain, int level, bool left = false,
                 bool right = false, bool top = false, bool bottom = false);

    void copyParent(const Terrain &parent, Terrain &terrain,
                    const vec2i &offset);
};

} // namespace world

#endif // VKWORLD_DIAMONDSQUARETERRAIN_H
