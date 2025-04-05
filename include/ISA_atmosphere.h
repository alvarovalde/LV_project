#ifndef GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H
#define GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H


#include "Atmosphere.h"

static constexpr double L0   = 0.0065;     //K/m, tropospheric lapse
static constexpr double Tropo_grad =-0.0065;

class ISA_atmosphere: public Atmosphere {
public:
    ISA_atmosphere();
    virtual ~ISA_atmosphere() = default;

    double getTemperature(double Geometric_altitude) const override;
    double getPressure(double Geometric_altitude) const override;
    double getDensity(double Geometric_altitude) const override;

    void getProperties(double altitudeMeters,
                       double &temperature,
                       double &pressure,
                       double &density) const override;
private:
    void computeISAProperties(double altitudeMeters,
                              double &temperature,
                              double &pressure,
                              double &density) const;

};


#endif //GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H
