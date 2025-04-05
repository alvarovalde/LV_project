//Abstract class to have the atmosphere model

#ifndef GRAVITY_SIMULATOR_ATMOSPHERE_H
#define GRAVITY_SIMULATOR_ATMOSPHERE_H


class Atmosphere {
public:
    virtual ~Atmosphere() = default;

    //Pure virtual methods for temperature, pressure, density
    virtual double getTemperature(double altitude) const = 0;
    virtual double getPressure(double altitude) const = 0;
    virtual double getDensity(double altitude) const =0;
    virtual void getProperties(double altitudeMeters,
                       double &temperature,
                       double &pressure,
                       double &density) const =0;
};


#endif //GRAVITY_SIMULATOR_ATMOSPHERE_H
