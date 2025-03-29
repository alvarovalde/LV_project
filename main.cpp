//Header files and preprocessor declarations
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Object.h"



constexpr double GRAV_CONST = 6.67430e-11;

float screenHeight = 1000.0f;
float screenWidth = 1400.0f;

//function declarations
GLFWwindow* StartGLFW(); // a function StartGLFW that returns a pointer to a window
GLFWwindow*  setUpSimulation();
void checkScreenCollision(Object &obj);





//initiate main
int main() {

    // run all pre things
    GLFWwindow* window = setUpSimulation();






    std::vector<Object> objs = {
            Object(std::vector<float>{screenWidth/2,screenHeight/2},std::vector<float>{0,0},5.972* pow(10,24),20),
            Object(std::vector<float>{2*screenWidth/3,screenHeight/2},std::vector<float>{0,-10},7.35* pow(10,22),5),
    };


    while(!glfwWindowShouldClose(window))
    {
        glfwMakeContextCurrent( window );
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);

        for (auto& obj : objs){

            for (auto& obj2 :objs){
                if (&obj2 == &obj){continue;};
                float dx = obj2.position[0] - obj.position[0];
                float dy =  obj2.position[1] - obj.position[1];
                float distance = sqrt(dx*dx+dy*dy);
                std::vector<float> direction = {dx/distance,dy/distance};
                distance *=100000;

                float Gforce = (GRAV_CONST * obj.mass *obj2.mass / (distance*distance));
                float acc1 = Gforce / obj.mass;

                std::vector<float> acc = {acc1*direction[0],acc1*direction[1]};
                obj.accelerate(acc[0],acc[1]);
            }
            obj.updatePos();
            obj.DrawCircle(100);
            //obj.checkCollisionWithScreen(screenWidth,screenHeight);






        }








        glfwSwapBuffers(window);
        glfwPollEvents();
    }




    glfwTerminate();
    return 0;
}



//_______________________________________ FUNCTION DEFINITIONS____________________________________________-





GLFWwindow*  setUpSimulation()
{
    GLFWwindow* window = StartGLFW(); // Call function and create variable window, which is an instance of a pointer to a GLFWwindow object
    glfwMakeContextCurrent( window );

    // Set up the viewport and projection once, after window creation
    glViewport(0, 0, static_cast<GLsizei>(screenWidth), static_cast<GLsizei>(screenHeight)); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION);  // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0.0, screenWidth, 0.0, screenHeight, -1.0, 1.0); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW);  // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity();
    return  window;
};


//function definition
GLFWwindow* StartGLFW()  // a function StartGLFW that returns a pointer to a window
{
    if (!glfwInit()){ // if not flag glfwInit
        std::cerr << "failed to iitialize glfw, panic!" << std::endl;
        return nullptr; // if process fails return null pointer
    }

    // Else, create a window by calling a functio from the library and return a pointer to the window
    GLFWwindow* window = glfwCreateWindow(screenWidth,screenHeight,"gravity simulator", NULL,NULL);

            return window;

};

