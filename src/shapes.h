#ifndef SHAPES_H
#define SHAPES_H
#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"


class Cylinder
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Cylinder(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices);
        
        // Generate the vertices for the cylinder
        void generateVertices(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices);
        
        // Draw the cylinder
        void draw();

        // Initialize the OpenGL constructs for this cylinder
        void init();


    private:
        std::vector<float> vertices;
        std::vector<int> indices;
        int vertexSize;
        int indexSize;
        unsigned int VBOc, VAOc, EBOc;
};

Cylinder::Cylinder(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices) {
    generateVertices(x, y, z, height, radius, colorR, colorG, colorB, numSlices);
    init();   
}

void Cylinder::init() {
    // Generate one vertex array
    glGenVertexArrays(1, &VAOc);

    // Generate one vertex, texture and element buffer
    glGenBuffers(1, &VBOc);
    glGenBuffers(1, &EBOc);

    // Bind the texture
    glBindVertexArray(VAOc);

    // Bind the VBO buffer to the GL_ARRAY_BUFFER buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBOc);

    // **Bind the EBO buffer to ELEMENT_ARRAY_BUFFER object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOc);

    // Add the triangle vertices to the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // **Add the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Describe where to find the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6* sizeof(float))); // texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8* sizeof(float))); // normals
    glEnableVertexAttribArray(3);

}
void Cylinder::generateVertices(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices) {
    for(int i=0; i < numSlices; i++) {
        float theta = (((float)i)*2.0*M_PI) / numSlices;
        float nextTheta = (((float)i+1.0)*2.0*M_PI) / numSlices;
        float currX = x + radius*cos(theta);
        float currY = y + radius*sin(theta);
        float nextX = x + radius*cos(nextTheta);
        float nextY = y + radius*sin(nextTheta);
        glm::vec3 tempVec;
        vertices.push_back(x);                              // Circle center top - 0
        vertices.push_back(y);                              // X,Y,Z
        vertices.push_back(height + z);
        vertices.push_back(colorR);                         // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(0.5f);                           // Texture Coords
        vertices.push_back(1.0f);
        vertices.push_back(0);                              // Normals
        vertices.push_back(0);
        vertices.push_back(1);

        vertices.push_back(currX);                          // Outside current top - 1
        vertices.push_back(currY);
        vertices.push_back(height + z);
        vertices.push_back(colorR);                         // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back((1.0f / numSlices) * i);         // Texture Coords
        vertices.push_back(1.0f);
        tempVec = normalize(glm::vec3(currX, currY, 0.0));
        vertices.push_back(tempVec.x);                      // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);

        vertices.push_back(nextX);                          // Outside next top
        vertices.push_back(nextY);
        vertices.push_back(height + z);
        vertices.push_back(colorR);                         // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back((1.0f / numSlices) * (i + 1));   // Texture Coords
        vertices.push_back(1.0f);
        tempVec = normalize(glm::vec3(nextX, nextY, 0.0));
        vertices.push_back(tempVec.x);                      // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);

        vertices.push_back(nextX);                          // Outside next bottom
        vertices.push_back(nextY);
        vertices.push_back(z);
        vertices.push_back(colorR);                         // Colors
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back((1.0f / numSlices) * (i + 1));   // Texture Coords
        vertices.push_back(0.0f);
        tempVec = normalize(glm::vec3(nextX, nextY, 0.0));
        vertices.push_back(tempVec.x);                      // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);


        vertices.push_back(currX);                          // Outside current bottom
        vertices.push_back(currY);
        vertices.push_back(z);
        vertices.push_back(colorR);                         // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back((1.0f / numSlices) * (i));       /// Texture Coords
        vertices.push_back(0.0f);
        tempVec = normalize(glm::vec3(currX, currY, 0.0));
        vertices.push_back(tempVec.x);                      // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);


        vertices.push_back(x);                              // Center bottom
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(colorR);                         // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(0.5f);                           // Texture Coords
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);                           // Normals
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);


        indices.push_back(0 + i * 6);  // Top Triangle
        indices.push_back(1 + i * 6);
        indices.push_back(2 + i * 6);
        
        indices.push_back(1 + i * 6);  // Side Triangle
        indices.push_back(4 + i * 6);
        indices.push_back(2 + i * 6);
        
        indices.push_back(2 + i * 6);  // Side Triangle
        indices.push_back(4 + i * 6);
        indices.push_back(3 + i * 6);

        indices.push_back(4 + i * 6);  // Bottom Triangle
        indices.push_back(5 + i * 6);
        indices.push_back(3 + i * 6);
    }
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Cylinder::draw() {
    glBindVertexArray(VAOc);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
}






class Cube
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Cube(float x, float y, float z, float width, float length, float height, float colorR, float colorG, float colorB);
        
        // Generate the vertices for the cube
        void generateVertices(float x, float y, float z, float width, float length, float height, float colorR, float colorG, float colorB);
        
        // Draw the cube
        void draw();

        // Initialize the OpenGL constructs for this cube
        void init();
    private:
        std::vector<float> vertices;
        std::vector<int> indices;
        int vertexSize;
        int indexSize;
        unsigned int VBOc, VAOc, EBOc;
};

Cube::Cube(float x, float y, float z, float width, float length, float height, float colorR, float colorG, float colorB) {
    generateVertices(x, y, z, width, length, height, colorR, colorG, colorB);
    init();   
}

void Cube::init() {
    // Generate one vertex array
    glGenVertexArrays(1, &VAOc);

    // Generate one vertex and element buffers
    glGenBuffers(1, &VBOc);
    glGenBuffers(1, &EBOc);

    glBindVertexArray(VAOc);

    // Bind the VBO buffer to the GL_ARRAY_BUFFER buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBOc);

    // **Bind the EBO buffer to ELEMENT_ARRAY_BUFFER object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOc);

    // Add the triangle vertices to the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // **Add the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Describe where to find the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6* sizeof(float))); // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8* sizeof(float))); // normals
    glEnableVertexAttribArray(3);
}
void Cube::generateVertices(float x, float y, float z, float width, float length, float height, float colorR, float colorG, float colorB) {
    float halfWidth = width / 2.0;
    float halfLength = length / 2.0;

    // Front - Triangle 1 *********************************************
    vertices.push_back(x - halfWidth);   // Bot left front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);     
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Bot right front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Top right front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // Front - Triangle 2 ***********************************************

    vertices.push_back(x - halfWidth);   // Bot left front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - halfWidth);   // Top left front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Top right front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // Right Side - Triangle 1 ***********************************************

    vertices.push_back(x + halfWidth);   // Bot right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Top right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Top right front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Right Side - Triangle 2 *************************************************

    vertices.push_back(x + halfWidth);   // Top right front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Bot right front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Bot right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Back Side - Triangle 1 ***************************************************

    vertices.push_back(x + halfWidth);   // Bot right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Top right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - halfWidth);   // Top left top
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    // Back Side - Triangle 2 *********************************************************

    vertices.push_back(x - halfWidth);   // Top left top
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - halfWidth);   // Top left bottom
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x + halfWidth);   // Bot right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);


    // Left Side - Triangle 1 ***************************************************************
    vertices.push_back(x - halfWidth);   // Bot left front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - halfWidth);   // Top left front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - halfWidth);   // Top left top
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Left Side - Triangle 2 ***************************************************************
    vertices.push_back(x - halfWidth);   // Top left top
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    
    vertices.push_back(x - halfWidth);   // Top left bottom
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - halfWidth);   // Bot left front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Top Side - Triangle 1 ****************************************************************
    vertices.push_back(x - halfWidth);   // Top left front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back(x - halfWidth);   // Top left top
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back(x + halfWidth);   // Top right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    // Top Side - Triangle 2 ****************************************************************
    vertices.push_back(x + halfWidth);   // Top right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back(x + halfWidth);   // Top right front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back(x - halfWidth);   // Top left front
    vertices.push_back(y - halfLength);
    vertices.push_back(height + z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);


    // Bottom Side - Triangle 1 *************************************************************
    vertices.push_back(x - halfWidth);   // Bot left front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back(x - halfWidth);   // Top left bottom
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back(x + halfWidth);   // Bot right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);


    // Bottom Side - Triangle 2 *************************************************************

    vertices.push_back(x + halfWidth);   // Bot right right side
    vertices.push_back(y + halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back(x + halfWidth);   // Bot right front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back(x - halfWidth);   // Bot left front
    vertices.push_back(y - halfLength);
    vertices.push_back(z);
    vertices.push_back(colorR);
    vertices.push_back(colorG);
    vertices.push_back(colorB);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);



    indices.insert(indices.end(), {0, 1, 2, 0, 2, 3});  // Front face
    indices.insert(indices.end(), {1, 2, 4, 4, 2, 5});  // Right face
    indices.insert(indices.end(), {4, 7, 5, 5, 7, 6});  // Back face
    indices.insert(indices.end(), {0, 7, 3, 3, 6, 7});  // Left face
    indices.insert(indices.end(), {3, 2, 6, 6, 2, 5});  // Top face
    indices.insert(indices.end(), {0, 1, 7, 7, 1, 4});  // Bot face
    
    
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Cube::draw() {
    glBindVertexArray(VAOc);
    glDrawArrays(GL_TRIANGLES, 0, vertexSize);
}




class Cone
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Cone(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices);
        
        // Generate the vertices for the cylinder
        void generateVertices(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices);
        
        // Draw the cylinder
        void draw();

        // Initialize the OpenGL constructs for this cylinder
        void init();

    private:
        std::vector<float> vertices;
        std::vector<int> indices;
        int vertexSize;
        int indexSize;
        unsigned int VBOc, VAOc, EBOc;
};

Cone::Cone(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices) {
    generateVertices(x, y, z, height, radius, colorR, colorG, colorB, numSlices);
    init();   
}

void Cone::init() {
    // Generate one vertex array
    glGenVertexArrays(1, &VAOc);

    // Generate one vertex, texture and element buffer
    glGenBuffers(1, &VBOc);
    glGenBuffers(1, &EBOc);

    // Bind the texture
    glBindVertexArray(VAOc);

    // Bind the VBO buffer to the GL_ARRAY_BUFFER buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBOc);

    // **Bind the EBO buffer to ELEMENT_ARRAY_BUFFER object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOc);

    // Add the triangle vertices to the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // **Add the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Describe where to find the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); // texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float))); // normals
    glEnableVertexAttribArray(3);
}
void Cone::generateVertices(float x, float y, float z, float height, float radius, float colorR, float colorG, float colorB, int numSlices) {
    float coneAngle = atan(radius/height);
    for(int i=0; i < numSlices; i++) {
        float theta = (((float)i)*2.0*M_PI) / numSlices;
        float nextTheta = (((float)i+1)*2.0*M_PI) / numSlices;
        float currX = x + radius*cos(theta);
        float currY = y + radius*sin(theta);
        float nextX = x + radius*cos(nextTheta);
        float nextY = y + radius*sin(nextTheta);
        float normalZ = z + radius*sin(coneAngle);
        glm::vec3 tempVec;

        // Side Triangle *******************************************************************
        vertices.push_back(x);                                  // Circle center top - 0
        vertices.push_back(y);
        vertices.push_back(height + z);
        vertices.push_back(colorR);                             // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(0.5f);                               // Texture coords
        vertices.push_back(1.0f);
        tempVec = glm::normalize(glm::vec3(nextX, nextY, normalZ));
        vertices.push_back(tempVec.x);                          // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);


        vertices.push_back(nextX);                              // Outside next bottom
        vertices.push_back(nextY);
        vertices.push_back(z);
        vertices.push_back(colorR);                             // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(1.0f);                               // Texture coords
        vertices.push_back(0.0f);
        tempVec = glm::normalize(glm::vec3(nextX, nextY, normalZ));
        vertices.push_back(tempVec.x);                          // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);

    
        vertices.push_back(currX);                              // Outside current bottom
        vertices.push_back(currY);
        vertices.push_back(z);
        vertices.push_back(colorR);                             // Colors
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(0.0f);                               // Texture coords
        vertices.push_back(0.0f);
        tempVec = glm::normalize(glm::vec3(currX, currY, normalZ));
        vertices.push_back(tempVec.x);                          // Normals
        vertices.push_back(tempVec.y);
        vertices.push_back(tempVec.z);


        // Bottom Triangle **********************************************************************

        vertices.push_back(x);                                  // Center bottom
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(colorR);                             // Colors
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(0.5f);                               // Texture coords
        vertices.push_back(0.5f);
        vertices.push_back(0.0f);                               // Normals
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);

        vertices.push_back(nextX);                              // Outside next bottom
        vertices.push_back(nextY);
        vertices.push_back(z);
        vertices.push_back(colorR);                             // Color
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(1.0f);                               // Texture coords
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);                               // Normals
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);

        vertices.push_back(currX);                              // Outside current bottom
        vertices.push_back(currY);
        vertices.push_back(z);
        vertices.push_back(colorR);                             // Colors
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(0.0f);                               // Texture coords
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);                               // Normals
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);

        indices.push_back(0 + i * 4);  // Top Triangle
        indices.push_back(1 + i * 4);
        indices.push_back(2 + i * 4);
        
        indices.push_back(1 + i * 4);  // Bottom Triangle
        indices.push_back(3 + i * 4);
        indices.push_back(2 + i * 4);
    }
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Cone::draw() {
    glBindVertexArray(VAOc);
    glDrawArrays(GL_TRIANGLES, 0, vertexSize);
}


class Plane
{
    public:
        
        // constructor gets height and width of the plane
        Plane(float x, float y, float z, float width, float length, float colorR, float colorG, float colorB);
        
        // Generate the vertices for the plane
        void generateVertices(float x, float y, float z, float width, float length, float colorR, float colorG, float colorB);
        
        // Draw the plane
        void draw();

        // Initialize the OpenGL constructs for this plane
        void init();

        void loadTexture();
    private:
        std::vector<float> vertices;
        std::vector<int> indices;
        int vertexSize;
        int indexSize;
        unsigned int VBOc, VAOc, EBOc;
};

Plane::Plane(float x, float y, float z, float width, float length, float colorR, float colorG, float colorB) {
    generateVertices(x, y, z, width, length, colorR, colorG, colorB);
    init();   
}

void Plane::init() {
    // Generate one vertex array
    glGenVertexArrays(1, &VAOc);

    // Generate one vertex, texture and element buffer
    glGenBuffers(1, &VBOc);
    glGenBuffers(1, &EBOc);

    // Bind the texture
    glBindVertexArray(VAOc);

    // Bind the VBO buffer to the GL_ARRAY_BUFFER buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBOc);

    // **Bind the EBO buffer to ELEMENT_ARRAY_BUFFER object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOc);

    // Add the triangle vertices to the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // **Add the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Describe where to find the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture
    glEnableVertexAttribArray(2);

}
void Plane::generateVertices(float x, float y, float z, float width, float length, float colorR, float colorG, float colorB) {
    float halfWidth = width / 2.0;
    float halfLength = length / 2.0;
    vertices.push_back(x - halfWidth);   // Bot left
    vertices.push_back(y - halfLength);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {colorR, colorG, colorB});

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    
    vertices.push_back(x - halfWidth);   // Top left
    vertices.push_back(y + halfLength);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {colorR, colorG, colorB});

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back(x + halfWidth);   // Top right
    vertices.push_back(y + halfLength);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {colorR, colorG, colorB});

    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    vertices.push_back(x + halfWidth);   // Bot Right
    vertices.push_back(y - halfLength);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {colorR, colorG, colorB});

    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    indices.insert(indices.end(), {0, 1, 2, 2, 0, 3});  // Plane
    
    
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Plane::draw() {
    glBindVertexArray(VAOc);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
}


class Sphere
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Sphere(float x, float y, float z, float radius, float colorR, float colorG, float colorB, int numSlices, int numSectors);
        
        // Generate the vertices for the cylinder
        void generateVertices(float x, float y, float z, float radius, float colorR, float colorG, float colorB, int numSlices, int numSectors);
        
        // Draw the cylinder
        void draw();

        // Initialize the OpenGL constructs for this cylinder
        void init();
    private:
        std::vector<float> vertices;
        std::vector<int> indices;
        int vertexSize;
        int indexSize;
        unsigned int VBOc, VAOc, EBOc;
};

Sphere::Sphere(float x, float y, float z, float radius, float colorR, float colorG, float colorB, int numSlices, int numSectors) {
    generateVertices(x, y, z, radius, colorR, colorG, colorB, numSlices, numSectors);
    init();   
}

void Sphere::init() {
    // Generate one vertex array
    glGenVertexArrays(1, &VAOc);

    // Generate one vertex and element buffers
    glGenBuffers(1, &VBOc);
    glGenBuffers(1, &EBOc);

    glBindVertexArray(VAOc);

    // Bind the VBO buffer to the GL_ARRAY_BUFFER buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBOc);

    // **Bind the EBO buffer to ELEMENT_ARRAY_BUFFER object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOc);

    // Add the triangle vertices to the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // **Add the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Describe where to find the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);

}
void Sphere::generateVertices(float x, float y, float z, float radius, float colorR, float colorG, float colorB, int numSlices, int numSectors) {
    float sectorStep = 2 * M_PI / numSectors;
    float stackStep = M_PI / numSlices;
    float xy, zt, xyNext, ztNext, xt, yt;
    for(int i=0; i < numSlices; i++) {
        float phi = M_PI / 2.0f - i * stackStep;
        float nextPhi = M_PI / 2.0f - (i + 1) * stackStep;
        xy = radius * cosf(phi);             // r * cos(u)
        zt = radius * sinf(phi);
        xyNext = radius * cosf(nextPhi);
        ztNext = radius * sinf(nextPhi);
        for(int j=0; j < numSectors; j++) {
            float theta = j * sectorStep;
            float nextTheta = (j + 1) * sectorStep;

            xt = xy * cosf(theta);             // r * cos(u) * cos(v)
            yt = xy * sinf(theta);
            vertices.push_back(xy * cosf(theta));     // Bot left of this Square - 0
            vertices.push_back(xy * sinf(theta));
            vertices.push_back(zt);
            vertices.insert(vertices.end(), {colorR, colorG, colorB});

            vertices.push_back(xyNext * cosf(nextTheta)); // Bot right of this Square - 1
            vertices.push_back(xyNext * sinf(nextTheta));
            vertices.push_back(zt);
            vertices.insert(vertices.end(), {colorR, colorG, colorB});

            vertices.push_back(xyNext * cosf(theta));       // Top left of this square - 2
            vertices.push_back(xyNext * sinf(theta));
            vertices.push_back(ztNext);
            vertices.insert(vertices.end(), {colorR, colorG, colorB});

            vertices.push_back(xyNext * cosf(nextTheta));   // Top right of this square 3
            vertices.push_back(xyNext * sinf(nextTheta));
            vertices.push_back(ztNext);
            vertices.insert(vertices.end(), {colorR, colorG, colorB});

            vertices.push_back(xyNext * cosf(nextTheta)); // Bot right of this Square - 1
            vertices.push_back(xyNext * sinf(nextTheta));
            vertices.push_back(zt);
            vertices.insert(vertices.end(), {colorR, colorG, colorB});

            vertices.push_back(xyNext * cosf(theta));       // Top left of this square - 2
            vertices.push_back(xyNext * sinf(theta));
            vertices.push_back(ztNext);
            vertices.insert(vertices.end(), {colorR, colorG, colorB});
        }
    }
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Sphere::draw() {
    glBindVertexArray(VAOc);
    glDrawArrays(GL_TRIANGLES, 0, vertexSize);
}
#endif