#include <memory>
#include "atmosphere.h"
#include "constants.h"

#ifndef ATMOSPHEREFACTORY_H
#define ATMOSPHEREFACTORY_H


class AtmosphereFactory {
public:
    static std::unique_ptr<Atmosphere> createAtmosphere(AtmosphereType type);

};


#endif //GRAVITY_SIMULATOR_ATMOSPHEREFACTORY_H
