#include <memory>
#include "atmosphere.h"
#include "constants.h"

#ifndef ATMOSPHEREFACTORY_H
#define ATMOSPHEREFACTORY_H


class AtmosphereFactory {
public:
    static std::unique_ptr<Atmosphere> createAtmosphere(AtmosphereType type,InterpolationType Type, double step);

};


#endif //GRAVITY_SIMULATOR_ATMOSPHEREFACTORY_H
