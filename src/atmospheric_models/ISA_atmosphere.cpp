#include "ISA_atmosphere.h"
#include <cmath>
#include <iostream>  // For warning message

namespace
{
    // Physical constants
    constexpr double G0 = 9.80665;       // m/s^2
    constexpr double R  = 287.053;       // J/(kg·K) for dry air

    // Sea-level reference
    constexpr double SEA_LEVEL_TEMP   = 288.15;   // K
    constexpr double SEA_LEVEL_PRESS  = 101325.0; // Pa
    constexpr double SEA_LEVEL_DENS   = 1.225;    // kg/m^3

    // For the 1976 Standard Atmosphere up to ~86 km, we define discrete layers:
    //   hBase (m), tBase (K), pBase (Pa), lapseRate (K/m)
    struct AtmosphereLayer
    {
        double hBase;     // layer base altitude, in [m]
        double tBase;     // temperature at base, in [K]
        double pBase;     // pressure at base, in [Pa]
        double lapseRate; // [K/m]
    };

    // Typically the standard atmosphere is segmented as:
    //   0–11 km, 11–20 km, 20–32 km, 32–47 km, 47–51 km, 51–71 km, 71–86 km
    // The data below is approximate and can be refined as needed.
    constexpr AtmosphereLayer LAYERS[] = {
            // hBase,   tBase,  pBase,    lapseRate
            {   0.0,   288.15, 101325.0, -0.0065   },  //  0–11 km
            {11000.0,  216.65, 22632.10,  0.0      },  // 11–20 km
            {20000.0,  216.65, 5474.89,   0.0010   },  // 20–32 km
            {32000.0,  228.65, 868.019,   0.0028   },  // 32–47 km
            {47000.0,  270.65, 110.906,   0.0      },  // 47–51 km
            {51000.0,  270.65, 66.9389,  -0.0028   },  // 51–71 km
            {71000.0,  214.65, 3.95642,  -0.0020   }   // 71–86 km (approx)
    };
    constexpr int NUM_LAYERS = sizeof(LAYERS) / sizeof(LAYERS[0]);

    // The top of our known piecewise layers is about 86 km (the last layer’s hBase is ~71 km,
    // so effectively we handle up to ~86 km). Above that, we can do a simple isothermal or
    // exponential extension. For example:
    constexpr double EXTEND_TOP_ALTITUDE = 86000.0; // 86 km
    // Temperature at 86 km (extrapolate or approximate):
    // We can sample the last layer if you prefer.
    // We'll pick T ~ 186.87K or so. Let's do an approximate from the last layer.
    constexpr double EXTEND_TOP_TEMP    = 186.95;   // K  (example near 86+ km)
    // Pressure at 86 km from the last layer formula (approx):
    constexpr double EXTEND_TOP_PRESS   = 0.227;    // Pa  (example, fairly small)
    // We'll do an isothermal extension from 86 km to 1000 km.
    // That means:  P(h) = P(86km)*exp[-g0*(h-86km)/(R*T_ext)]
    // If you want more layers up to 1000 km, insert them similarly.

    // Above some altitude, the density & pressure become extremely small.
    // We'll define a threshold ratio below which we warn once:
    constexpr double NEGLIGIBLE_RATIO = 1.0e-6; // 1e-6 of sea-level conditions
    // (That is, if density < (1.225 * 1.0e-6) => we are effectively in "space".)

    // We'll keep a static boolean to avoid spamming multiple warnings:
    static bool g_warnedSpace = false;
}

//------------------------------------------------------------------------------
ISA_atmosphere::ISA_atmosphere()
{
    // If you want to precompute anything, do it here.
    // In this version, we do not need precomputation; everything is in computeISAProperties().
}

//------------------------------------------------------------------------------
void ISA_atmosphere::computeISAProperties(double altitudeMeters,
                                          double &temperature,
                                          double &pressure,
                                          double &density) const
{
    // 1) Optionally clamp altitude below 0 if you want:
    // altitudeMeters = std::max(0.0, altitudeMeters);

    // 2) If altitude < top of known piecewise layers (~86 km), we do the standard steps:
    if (altitudeMeters < EXTEND_TOP_ALTITUDE)
    {
        // find the layer in which altitude falls
        int layerIndex = NUM_LAYERS - 1;
        for (int i = 0; i < NUM_LAYERS - 1; ++i)
        {
            double nextBase = LAYERS[i+1].hBase;
            // if the altitude is less than the next layer’s base, we’re in layer i
            if (altitudeMeters < nextBase)
            {
                layerIndex = i;
                break;
            }
        }

        const auto &layer = LAYERS[layerIndex];
        double hBase   = layer.hBase;
        double tBase   = layer.tBase;
        double pBase   = layer.pBase;
        double lapse   = layer.lapseRate; // K/m
        double dh      = altitudeMeters - hBase; // above this layer's base

        if (std::abs(lapse) > 1.0e-15)
        {
            // T = T_base + lapse*(h-h_base)
            // P = P_base * (T / T_base)^(-g0/(R*lapse))
            temperature = tBase + lapse * dh;
            double exponent = -G0 / (R * lapse);
            pressure = pBase * std::pow(temperature / tBase, exponent);
        }
        else
        {
            // isothermal layer
            temperature = tBase;
            double exponent = -G0 * dh / (R * tBase);
            pressure = pBase * std::exp(exponent);
        }
    }
        // 3) If altitude >= ~86 km, we do a simplified extension up to 1000 km
    else if (altitudeMeters <= 1.0e6)  // 1000 km in meters = 1.0e6
    {
        // Simple isothermal extension from 86 km to 1000 km
        // We'll define a constant temperature (EXTEND_TOP_TEMP).
        // Then: P(h) = P(86km) * exp[-g0*(h - 86km)/(R*T)]
        // This is quite rough, but fast & simple.

        double dh = altitudeMeters - EXTEND_TOP_ALTITUDE;
        temperature = EXTEND_TOP_TEMP;
        double exponent = - (G0 * dh) / (R * EXTEND_TOP_TEMP);
        pressure = EXTEND_TOP_PRESS * std::exp(exponent);
    }
    else
    {
        // For altitude > 1000 km, treat it as vacuum.
        // You could keep extending if you truly want 2000 km, 3000 km, etc.
        // We'll just clamp to near-vacuum:
        temperature = EXTEND_TOP_TEMP;   // or some fixed outer-space approx
        pressure    = 0.0;
    }

    // 4) Density from ideal gas law
    if (pressure > 0.0 && temperature > 0.0)
        density = pressure / (R * temperature);
    else
        density = 0.0;

    // 5) Check if we are effectively in "space" => ratio < 1e-6 of sea-level?
    //    Print a single warning if so and not already done.
    double rhoRatio = density / SEA_LEVEL_DENS;
    if (rhoRatio < NEGLIGIBLE_RATIO && !g_warnedSpace)
    {
        g_warnedSpace = true;
        std::cerr << "[ISA_atmosphere WARNING] Altitude ~"
                  << altitudeMeters << " m => density < "
                  << NEGLIGIBLE_RATIO << " * sea-level density. "
                  << "Atmosphere considered negligible (space).\n";
    }
}

//------------------------------------------------------------------------------
double ISA_atmosphere::getTemperature(double Geometric_altitude) const
{
    double T, P, rho;
    computeISAProperties(Geometric_altitude, T, P, rho);
    return T;
}

//------------------------------------------------------------------------------
double ISA_atmosphere::getPressure(double Geometric_altitude) const
{
    double T, P, rho;
    computeISAProperties(Geometric_altitude, T, P, rho);
    return P;
}

//------------------------------------------------------------------------------
double ISA_atmosphere::getDensity(double Geometric_altitude) const
{
    double T, P, rho;
    computeISAProperties(Geometric_altitude, T, P, rho);
    return rho;
}

//------------------------------------------------------------------------------
void ISA_atmosphere::getProperties(double altitudeMeters,
                                   double &temperature,
                                   double &pressure,
                                   double &density) const
{
    computeISAProperties(altitudeMeters, temperature, pressure, density);
}




