#pragma once

#include <worldapi/worldapidef.h>

#include "../maths/Vector.h"

class WORLDAPI_EXPORT LODData {
public:
	LODData();
	LODData(const maths::vec3d & chunkSize);
	LODData(const LODData & other);
	virtual ~LODData();

	const maths::vec3d & getChunkSize() const { return _chunkSize; }
	double getMinDetailSize() const { return _minDetailSize; }
private:
	maths::vec3d _chunkSize;
	double _minDetailSize;
};

