#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cylinder
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Cylinder(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices);
        
        // Generate the vertices for the cylinder
        void generateVertices(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices);
        
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

Cylinder::Cylinder(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices) {
    generateVertices(x, y, z, height, radius, colorR, colorG, colorB, numSlices);
    init();   
}

void Cylinder::init() {
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
void Cylinder::generateVertices(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices) {
    for(int i=0; i < numSlices; i++) {
        float theta = (((float)i)*2.0*M_PI) / numSlices;
        float nextTheta = (((float)i+1)*2.0*M_PI) / numSlices;
        vertices.push_back(x);                          // Circle center top - 0
        vertices.push_back(y);
        vertices.push_back(height + z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        std::cout << "Circle center top is: " << x << ", " << y << ", " << height << std::endl;
        vertices.push_back(radius*cos(theta));          // Outside current top - 1
        vertices.push_back(radius*sin(theta));
        vertices.push_back(height + z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        std::cout << "Circle current outside top is: " << radius*cos(theta) << ", " << radius*sin(theta) << ", " << height << std::endl;
        vertices.push_back(radius*cos(nextTheta));      // Outside next top
        vertices.push_back(radius*sin(nextTheta));
        vertices.push_back(height + z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        std::cout << "Circle next outside top is: " << radius*cos(nextTheta) << ", " << radius*sin(nextTheta) << ", " << height << std::endl;
        vertices.push_back(radius*cos(nextTheta));      // Outside next bottom
        vertices.push_back(radius*sin(nextTheta));
        vertices.push_back(z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(radius*cos(theta));          // Outside current bottom
        vertices.push_back(radius*sin(theta));
        vertices.push_back(z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(x);                          // Center bottom
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        //std::cout << "Added: " << vertices.size() << "vertices!" << std::endl;
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
    std::cout << "vertex size: " << vertexSize << std::endl;
}






class Cube
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Cube(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB);
        
        // Generate the vertices for the cube
        void generateVertices(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB);
        
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

Cube::Cube(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB) {
    generateVertices(x, y, z, sideLength, colorR, colorG, colorB);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);

}
void Cube::generateVertices(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB) {
    float halfSide = sideLength / 2.0;
    vertices.push_back(x - halfSide);   // Bot left front
    vertices.push_back(y - halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});
    
    vertices.push_back(x + halfSide);   // Bot right front
    vertices.push_back(y - halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x + halfSide);   // Top right front
    vertices.push_back(y - halfSide);
    vertices.push_back(sideLength + z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x - halfSide);   // Top left front
    vertices.push_back(y - halfSide);
    vertices.push_back(sideLength + z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x + halfSide);   // Bot right right side
    vertices.push_back(y + halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x + halfSide);   // Top right right side
    vertices.push_back(y + halfSide);
    vertices.push_back(sideLength + z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x - halfSide);   // Top left top
    vertices.push_back(y + halfSide);
    vertices.push_back(sideLength + z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x - halfSide);   // Top left bottom
    vertices.push_back(y + halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

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
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    std::cout << "vertex size: " << vertexSize << std::endl;
}




class Cone
{
    public:
        
        // constructor gets height and width of the cylinder along with number of slices
        Cone(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices);
        
        // Generate the vertices for the cylinder
        void generateVertices(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices);
        
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

Cone::Cone(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices) {
    generateVertices(x, y, z, height, radius, colorR, colorG, colorB, numSlices);
    init();   
}

void Cone::init() {
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
void Cone::generateVertices(float x, float y, float z, float height, float radius, int colorR, int colorG, int colorB, int numSlices) {
    for(int i=0; i < numSlices; i++) {
        float theta = (((float)i)*2.0*M_PI) / numSlices;
        float nextTheta = (((float)i+1)*2.0*M_PI) / numSlices;
        vertices.push_back(x);                          // Circle center top - 0
        vertices.push_back(y);
        vertices.push_back(height + z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);

        vertices.push_back(radius*cos(nextTheta));      // Outside next bottom
        vertices.push_back(radius*sin(nextTheta));
        vertices.push_back(z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(radius*cos(theta));          // Outside current bottom
        vertices.push_back(radius*sin(theta));
        vertices.push_back(z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);
        vertices.push_back(x);                          // Center bottom
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(colorR);
        vertices.push_back(colorG);
        vertices.push_back(colorB);

        indices.push_back(0 + i * 4);  // Top Triangle
        indices.push_back(1 + i * 4);
        indices.push_back(2 + i * 4);
        
        indices.push_back(1 + i * 4);  // Side Triangle
        indices.push_back(3 + i * 4);
        indices.push_back(2 + i * 4);
    }
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Cone::draw() {
    glBindVertexArray(VAOc);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    std::cout << "vertex size: " << vertexSize << std::endl;
}

class Plane
{
    public:
        
        // constructor gets height and width of the plane
        Plane(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB);
        
        // Generate the vertices for the plane
        void generateVertices(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB);
        
        // Draw the plane
        void draw();

        // Initialize the OpenGL constructs for this plane
        void init();
    private:
        std::vector<float> vertices;
        std::vector<int> indices;
        int vertexSize;
        int indexSize;
        unsigned int VBOc, VAOc, EBOc;
};

Plane::Plane(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB) {
    generateVertices(x, y, z, sideLength, colorR, colorG, colorB);
    init();   
}

void Plane::init() {
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
void Plane::generateVertices(float x, float y, float z, float sideLength, int colorR, int colorG, int colorB) {
    float halfSide = sideLength / 2.0;
    vertices.push_back(x - halfSide);   // Bot left
    vertices.push_back(y - halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});
    
    vertices.push_back(x - halfSide);   // Top left
    vertices.push_back(y + halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    vertices.push_back(x + halfSide);   // Top right
    vertices.push_back(y + halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});


    vertices.push_back(x + halfSide);   // Bot Right
    vertices.push_back(y - halfSide);
    vertices.push_back(z);

    vertices.insert(vertices.end(), {(float) colorR, (float) colorG, (float) colorB});

    indices.insert(indices.end(), {0, 1, 2, 2, 0, 3});  // Plane
    
    
    vertexSize = vertices.size();
    indexSize = indices.size();
}
void Plane::draw() {
    glBindVertexArray(VAOc);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    std::cout << "vertex size: " << vertexSize << std::endl;
}

#endif