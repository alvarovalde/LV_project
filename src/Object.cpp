#include "Object.h"
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>

Object::Object(){
    std::cout << "Object initiated at 0,0" << std::endl;
    this->position = std::vector<float> {0,0};
    this->velocity = std::vector<float> {0,0};
    this->radius = 10;

}

Object::Object(std::vector<float> position, std::vector<float> velocity,float mass, float radius){
this->position = position;
this->velocity = velocity;
this->radius = radius;
this->mass = mass;
}

void Object::accelerate(float ax, float ay){
    this->velocity[0] += ax;
    this->velocity[1] += ay;
};


void Object::updatePos(){
    this->position[0] += this->velocity[0];
    this->position[1] += this->velocity[1];
}

void Object::DrawCircle(int res =100)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(this->position[0],this->position[1]);

    for(int i =0; i<=res; i++)
    {
        float angle = 2.0f * PI * (static_cast<float>(i) / res);
        float x = this->position[0] + cos(angle)*this->radius;
        float y =this->position[1] + sin(angle)*this->radius;
        glVertex2d(x,y);

    }
    glEnd();
};


void Object::checkCollisionWithScreen(float screenWidth, float screenHeight)
{
    // Check vertical bounds
    if (this->position[1] - this->radius < 0)
    {
        // If the bottom of the circle is below 0, reposition it at the bottom boundary.
        this->position[1] = this->radius;
        this->velocity[1] *= -0.95;
    }

    else if (this->position[1] + this->radius > screenHeight)
    {
        // If the top of the circle is above the screen height, reposition it at the top boundary.
        this->position[1] = screenHeight - this->radius;
        this->velocity[1] *= -0.95;
    }


    // Check horizontal bounds
    if (this->position[0] - this->radius < 0)
    {
        // If the left of the circle is off the screen, reposition it.
        this->position[0] = this->radius;
        this->velocity[0] *= -0.95;
    }
    else if (this->position[0] + this->radius > screenWidth)
    {
        // If the right of the circle is off the screen, reposition it.
        this->position[0] = screenWidth - this->radius;
        this->velocity[0] *= -0.95;
    }
};