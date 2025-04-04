
#include "AtmosphereFactory.h"
#include "ISA_atmosphere.h"
#include <stdexcept>

std::unique_ptr<Atmosphere> AtmosphereFactory::createAtmosphere(AtmosphereType type)
{
    switch (type) {
        case AtmosphereType::ISA:
            return std::make_unique<ISA_atmosphere>();
        default:
            throw std::runtime_error("Unknown AtmosphereType!");

    }
}