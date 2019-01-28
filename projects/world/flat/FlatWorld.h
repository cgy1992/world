#pragma once

#include "world/core/WorldConfig.h"

#include "world/core/World.h"
#include "IGround.h"

namespace world {
class FlatWorld;

typedef IWorldDecorator<FlatWorld> FlatWorldDecorator;

class PFlatWorld;

class WORLDAPI_EXPORT FlatWorld : public World {
public:
    /** Create a complete and rich world that can be used
     * as a demonstration of the API power ! */
    static FlatWorld *createDemoFlatWorld();

    FlatWorld();

    ~FlatWorld() override;

    template <typename T, typename... Args> T &setGround(Args... args);

    IGround &ground();

    template <typename T, typename... Args>
    T &addFlatWorldDecorator(Args... args);

    void collect(const WorldZone &zone, ICollector &collector,
                 const IResolutionModel &resolutionModel) override;

protected:
    void onFirstExploration(const WorldZone &chunk) override;

private:
    PFlatWorld *_internal;

    std::unique_ptr<IGround> _ground;

    void setGroundInternal(IGround *ground);

    void addFlatWorldDecoratorInternal(FlatWorldDecorator *decorator);
};

template <typename T, typename... Args> T &FlatWorld::setGround(Args... args) {
    T *ground = new T(args...);
    setGroundInternal(ground);
    return *ground;
}

template <typename T, typename... Args>
T &FlatWorld::addFlatWorldDecorator(Args... args) {
    T *decorator = new T(args...);
    addFlatWorldDecoratorInternal(decorator);
    return *decorator;
}
} // namespace world