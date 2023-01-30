#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "shader.h"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Initialize Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Camera is 3 units 'above' the scene
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // Camera is initially looking at the origin.
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Camera up is set as the 'y' axis

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Mouse positions
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
float sensitivity = 1.0;
float sensitivityIncreaseUnit = 0.1;
float sensitivityMax = 2.0;
float sensitivityMin = 0.1;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;

float lastScrollX = 0, lastScrollY = 0;
float scrollSensitivity = 0.001;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    //const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) 
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (sensitivity + yoffset * sensitivityIncreaseUnit > sensitivityMax) {
        sensitivity = sensitivity;
        std::cout << "Sensitivity reached max" << std::endl;
    } else if (sensitivity + yoffset * sensitivityIncreaseUnit < sensitivityMin) {
        sensitivity = sensitivity;
        std::cout << "Sensitivity reached min" << std::endl;
    } else {
        sensitivity += yoffset * sensitivityIncreaseUnit;
        std::cout << "Sensitivity is: " << sensitivity << std::endl;
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(sensitivity * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}


int main () {


/*

    Initialize GLFW

*/

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    


/*

    Create GLFW Window

*/

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Michael Ross - CS330 3.3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
/*

    GLAD: Load all OpenGL Function Pointers

*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST); 

    // Build and compile shaders
    // uses custom include to make creating new shaders easier.
    Shader myShader("../shaders/default.vs", "../shaders/default.fs");

    // Setup vertex data and configure vertex attributes
    float triangleVertices1[] = {
        -0.5f, 0.0f, 0.0f, 1.0f, 0.8f, 0.9f,    // a
        0.0f, 1.0f, -0.5f, 1.0f, 1.0f, 1.0f,    // b
        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,     // c
        0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,    // d
        -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f   // e
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3,
        3, 1, 4,
        4, 1, 0,
        0, 2, 3,
        0, 4, 3
    };
    /*
    A = -0.5f, 0.0f, 0.0f, 1.0f, 0.8f, 0.9f,
    B = 0.0f, 1.0f, -0.5f, 1.0f, 1.0f, 1.0f,
    C = 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    D = 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    E = -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f,

    a, b, c
    c, b, d
    d, b, e
    e, b, a
    */

    // Create the model matrix:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Create the view matrix:
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

    // Create the projection matrix
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    // Initialize the vertex buffer objects
    unsigned int VBO, VAO, EBO;

    // Generate one vertex array
    glGenVertexArrays(1, &VAO);

    // Generate one vertex and element buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind the VBO buffer to the GL_ARRAY_BUFFER buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // **Bind the EBO buffer to ELEMENT_ARRAY_BUFFER object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Add the triangle vertices to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices1), triangleVertices1, GL_STATIC_DRAW);

    // **Add the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Describe where to find the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))); // color
    glEnableVertexAttribArray(1);



    // glUseProgram(shaderProgram);

    while(!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 10.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        myShader.setMatrix4fv("model", model);
        myShader.setMatrix4fv("view", view);
        myShader.setMatrix4fv("projection", projection);
        myShader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 12);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glfwPollEvents();    
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

}
