
#include "AtmosphereFactory.h"
#include "ISA_atmosphere.h"
#include <stdexcept>

std::unique_ptr<Atmosphere> AtmosphereFactory::createAtmosphere(AtmosphereType type, InterpolationType interpType,double step)
{
    switch (type) {
        case AtmosphereType::ISA:
            return std::make_unique<ISA_atmosphere>();

        case AtmosphereType::ISA_TABLE:
            return std::make_unique<ISA_Tabulated>(interpType,step);
        default:
            throw std::runtime_error("Unknown AtmosphereType!");

    }
}