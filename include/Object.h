#include <vector>
#include <cmath>


constexpr double PI = 3.141592653589793238462643383279;

#ifndef GRAVITY_SIMULATOR_OBJECT_H
#define GRAVITY_SIMULATOR_OBJECT_H


class Object {

public:
    // values
    std::vector<float> position;
    std::vector<float> velocity;
    float radius;
    double mass;

    // constructors
    Object();
    Object(std::vector<float> position, std::vector<float> velocity,float mass, float radius);

    //methods
    void accelerate(float ax, float ay);
    void updatePos();
    void DrawCircle(int res);
    void checkCollisionWithScreen(float screenWidth, float screenHeight);

};



#endif //GRAVITY_SIMULATOR_OBJECT_H
