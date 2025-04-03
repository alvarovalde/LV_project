#ifndef GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H
#define GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H


//Physical constants for ISA

//the fact that this is static means no .cpp files
//will be able to see it. It is also done at compile-time.
static constexpr double P0 = 101325.0;
static constexpr double T0 = 288.15;






class ISA_atmosphere {


private:

public:
    float altitude;


};


#endif //GRAVITY_SIMULATOR_ISA_ATMOSPHERE_H
