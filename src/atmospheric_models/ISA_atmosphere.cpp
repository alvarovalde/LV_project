#include "ISA_atmosphere.h"
#include <cmath>
#include "constants.h"

static constexpr double Tropo_grad =-0.0065;


#include <algorithm>

// Physical constants (could also be put in a separate header)
static constexpr double P0   = 101325.0;  // Sea-level pressure (Pa)
static constexpr double T0   = 288.15;    // Sea-level temperature (K)
static constexpr double g    = 9.80665;   // m/s^2
static constexpr double R    = 287.053;   // J/(kg·K)
static constexpr double Lapse_Troposphere = 0.0065;  // K/m (for 0–11 km)

ISA_atmosphere::ISA_atmosphere() {
    // Constructor (could perform initialization if needed)
}

double ISA_atmosphere::directTemperature(double altitude) const {
    // For demonstration, assume the troposphere formula for altitudes <= 11 km.
    // You would add piecewise logic for higher altitudes.
    if (altitude < 0.0) altitude = 0.0;
    if (altitude <= 11000.0) {
        return T0 - Lapse_Troposphere * altitude;
    }
    // Above 11 km, we simply clamp (or add further layers)
    return 216.65;
}

double ISA_atmosphere::directPressure(double altitude) const {
    if (altitude < 0.0) altitude = 0.0;
    if (altitude <= 11000.0) {
        double T = T0 - Lapse_Troposphere * altitude;
        double exponent = g / (Lapse_Troposphere * R);
        return P0 * std::pow(T / T0, exponent);
    }
    // For altitudes > 11 km, use a simple isothermal exponential model for demonstration.
    double P_tropo = P0 * std::pow((T0 - Lapse_Troposphere * 11000.0) / T0,
                                   g / (Lapse_Troposphere * R));
    return P_tropo * std::exp(-g * (altitude - 11000.0) / (R * 216.65));
}

double ISA_atmosphere::getTemperature(double altitude) const {
    return directTemperature(altitude);
}

double ISA_atmosphere::getPressure(double altitude) const {
    return directPressure(altitude);
}

double ISA_atmosphere::getDensity(double altitude) const {
    double p = getPressure(altitude);
    double t = getTemperature(altitude);
    return p / (R * t);
}
