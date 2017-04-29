#pragma once

#include <worldapi/worldapidef.h>

#include <vector>
#include <random>
#include <memory>

#include "Map.h"
#include "WorldParameters.h"

// MODULES (� d�placer ?)

class MapGenerator;

/** Classe de base des modules du g�n�rateur de cartes. */
class WORLDAPI_EXPORT MapGeneratorModule {
public:
	MapGeneratorModule(MapGenerator * parent);
	virtual ~MapGeneratorModule() = default;

	virtual void generate(Map & map) const = 0;
protected:
	MapGenerator * _parent;

	std::mt19937 & rng() const;
	arma::cube & reliefMap(Map & map) const;
};

/** Classe de base des modules du g�n�rateur de cartes servant
� g�n�rer la carte de relief. */
class WORLDAPI_EXPORT ReliefMapGenerator : public MapGeneratorModule {
public:
	ReliefMapGenerator(MapGenerator * parent);
};

class WORLDAPI_EXPORT CustomWorldRMGenerator : public ReliefMapGenerator {
public:
	CustomWorldRMGenerator(MapGenerator * parent, float biomeDensity = 1, uint32_t limitBrightness = 4);

	void setBiomeDensity(float biomeDensity);
	void setLimitBrightness(uint32_t);

	void setDifferentialLaw(const relief::diff_law & law);

	virtual void generate(Map & map) const;
private:
	static const float PIXEL_UNIT;
	/** Le nombre moyen de biomes par bloc de 100 pixels de WorldMap.*/
	float _biomeDensity;
	/** La nettet� des limites entre les biomes. En pratique c'est
	le "p" dans l'algo de l'interpolation. */
	uint32_t _limitBrightness;

	/** Loi de probabilit� du diff�rentiel d'altitude en fonction
	de l'altitude. */
	std::unique_ptr<relief::diff_law> _diffLaw;
};


// MapGenerator

/** */
class WORLDAPI_EXPORT MapGenerator {
public:
	MapGenerator(uint32_t sizeX, uint32_t sizeY);
	~MapGenerator();

	Map * generate();

	template <class T, typename... Args>
	void emplaceReliefMapGenerator(Args&&... args) {
		_reliefMap = std::make_unique<T>(this, args...);
	}
private:
	mutable std::mt19937 _rng;

	uint32_t _sizeX, _sizeY;

	std::unique_ptr<ReliefMapGenerator> _reliefMap;


	friend class MapGeneratorModule;
};