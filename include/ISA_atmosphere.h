#ifndef GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H
#define GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H

#include "Atmosphere.h"



class ISA_atmosphere: public Atmosphere {
public:
    ISA_atmosphere();
    virtual ~ISA_atmosphere() = default;

    double getTemperature(double Geometric_caltitude) const override;
    double getPressure(double Geometric_altitude) const override;
    double getDensity(double Geometric_altitude) const override;
private:
    double directTemperature(double Geometric_altitude) const;
    double directPressure(double Geometric_altitude) const;

};


#endif //GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H
