#pragma once

#include <worldapi/worldapidef.h>

#include <memory>
#include <vector>
#include <string>
#include <set>
#include <functional>

#include "../WorldFolder.h"
#include "ChunkSystem.h"
#include "WorldObject.h"
#include "LODData.h"
#include "IWorldDecorator.h"

class World;

typedef IWorldDecorator<World> IWorldChunkDecorator;

class PrivateWorld;

class WORLDAPI_EXPORT World {
public:
	/** Create a complete and rich world that can be used
	 * as a demonstration of the API power ! */
	static World * createDemoWorld();

	World();
	World(const World & world) = delete;
	virtual ~World();

    void addChunkDecorator(IWorldChunkDecorator * decorator);

	// NAVIGATION
	ChunkNode exploreNeighbour(const ChunkNode &chunk, const maths::vec3d &direction);
	ChunkNode exploreLocation(const maths::vec3d &location);
protected:
	virtual void onFirstExploration(ChunkNode &chunk);

private:
	PrivateWorld * _internal;

	ChunkSystem _chunkSystem;
	WorldFolder _directory; // TODO remplacer �a par un ICache, qui peut �tre un dossier, une interface r�seau, rien...
};
