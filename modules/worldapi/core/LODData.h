#pragma once

#include "core/WorldConfig.h"

#include "math/Vector.h"

namespace world {

    class WORLDAPI_EXPORT LODData {
    public:
        LODData();

        LODData(const vec3d &chunkSize, double maxResolution = 0.2);

        LODData(const LODData &other);

        virtual ~LODData();

        const vec3d &getChunkSize() const { return _chunkSize; }

        double getMaxResolution() const { return _maxResolution; }

    private:
        vec3d _chunkSize;
        double _maxResolution;
    };
}