#include "CollectorContextWrap.h"

#include "../assets/Object3D.h"

namespace world {

    CollectorContextWrap::CollectorContextWrap(ICollector &wrapped)
            : _collector(wrapped),
              _currentChunk(false, ChunkKeys::none()),
              _currentObject(false, ObjectKeys::defaultKey()),
              _offset() {

    }

    void CollectorContextWrap::setCurrentChunk(ChunkKey key) {
        _currentChunk = std::make_pair(true, key);
    }

    void CollectorContextWrap::setCurrentObject(ObjectKey key) {
        _currentObject = std::make_pair(true, key);
    }

    void CollectorContextWrap::setOffset(const vec3d &offset) {
        _offset = offset;
    }

    void CollectorContextWrap::addItem(const ItemKey &key, const Object3D &object) {
        Object3D clone = object;
        addItemUnsafe(key, clone);
    }

    void CollectorContextWrap::addItemUnsafe(const ItemKey &key, Object3D &object) {
        object.setPosition(object.getPosition() + _offset);
        passItemTo(_collector, mutateKey(key), object);
    }

	void CollectorContextWrap::removeItem(const ItemKey &key) {
		_collector.removeItem(mutateKey(key));
	}

	bool CollectorContextWrap::hasItem(const ItemKey &key) const {
		return _collector.hasItem(mutateKey(key));
	}

	ICollector::ItemKey CollectorContextWrap::mutateKey(const ItemKey &key) const {
		return ItemKeys::from(
			_currentChunk.first ? _currentChunk.second : std::get<0>(key),
			_currentObject.first ? _currentObject.second : std::get<1>(key),
			std::get<2>(key)
		);
	}
}