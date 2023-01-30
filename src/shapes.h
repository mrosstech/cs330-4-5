#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cylinder
{
public:
    // the program ID
    unsigned int ID;
    int numSlices;
    float height;
    float width;

    // constructor gets height and width of the cylinder along with number of slices
    Cylinder(float height, float width, int numSlices);

    // draw the cylinder
    void drawCylinder();

    // set the model, view, and transform matrices for the cylinder
    void setMatrix4fv(const std::string &name, glm::mat4 value) const;

    // Create vertexes for the cylinder
    void creatVertices()

};

void createVertices() {
    
}