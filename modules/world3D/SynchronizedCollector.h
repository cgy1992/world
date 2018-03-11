#ifndef WORLD_SYNCHRONIZEDWORLD_H
#define WORLD_SYNCHRONIZEDWORLD_H

#include <memory>
#include <mutex>
#include <atomic>

#include <worldapi/world/FlatWorldCollector.h>

// TODO supprimer cette classe une fois que le collector sera synchronisť
class SynchronizedCollector {
public:
    SynchronizedCollector();

    FlatWorldCollector & get();
    void lock();
    void unlock();

private:
    std::mutex _mutex;
    std::atomic_bool _locked;
    std::unique_ptr<FlatWorldCollector> _collector;
};

#endif //WORLD_SYNCHRONIZEDWORLD_H
