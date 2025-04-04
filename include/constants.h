#ifndef SIMULATOR_CONSTANTS_H
#define SIMULATOR_CONSTANTS_H

namespace constants {


    inline constexpr double G0      = 9.80665;   //m/s^2
    inline constexpr double R_AIR  = 287.053;   //J/(kg·K)
    inline constexpr double P0     = 101325.0;  //Pa
    inline constexpr double T0     = 288.15;    //K


    inline constexpr double GRAV_CONST = 6.67430e-11;

    inline float screenHeight = 1000.0f;
    inline float screenWidth = 1400.0f;
}


enum class AtmosphereType {
    ISA,
    ISA_TABLE,
};

enum class InterpolationType{
    LINEAR,
};


#endif //GRAVITY_SIMULATOR_CONSTANTS_H
