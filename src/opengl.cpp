
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "shapes.h"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Initialize Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f); // Camera is 3 units 'above' the scene
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


// Start in perspective mode
bool usePerspective = true;

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
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        usePerspective = !usePerspective;
}

void loadTexture(std::string texturePath) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
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


    // Set texture repetition parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   

    // Set mipmap modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST); 

    // Create the model matrix:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Create the view matrix:
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f)); 

    // Create the projection matrix
    glm::mat4 perspective = glm::mat4(1.0f);
    perspective = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 ortho = glm::mat4(1.0f);
    ortho = glm::ortho(-5.0f, 5.0f, 5.0f, -5.0f, -30.0f, 10.0f);

    // Initialize light colors
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
    glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);

    // Load Textures
    unsigned int planks, iron, wax, woodgrain, bricks;
    glGenTextures(1, &planks);
    glGenTextures(1, &iron);
    glGenTextures(1, &wax);
    glGenTextures(1, &woodgrain);
    glGenTextures(1, &bricks);
   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, planks);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    loadTexture(".\\resources\\textures\\wood.jpg");
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, iron);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    loadTexture(".\\resources\\textures\\iron.jpg");


    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, wax);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    loadTexture(".\\resources\\textures\\wax.jpg");

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, woodgrain);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    loadTexture(".\\resources\\textures\\woodgrain.jpg");

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, bricks);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    loadTexture(".\\resources\\textures\\wall.jpg");

    // Build and compile shaders
    // uses custom include to make creating new shaders easier.
    Shader myShader("../shaders/color.vs", "../shaders/color.fs");
    Shader lightSourceShader("../shaders/lightSource.vs", "../shaders/lightSource.fs");
    Shader lightingShader("../shaders/multiLight.vs", "../shaders/multiLight.fs");
    
    glm::vec3 lightPos = glm::vec3(3.0f, -3.0f, 1.0f);
    Cylinder helmet_bottom(1.0f, 1.0f, 0.0f, 1.0f, 0.4f, 112/255.f, 124/255.f, 130/255.f, 20);
    Cone helmet_top(1.0f, 1.0f, 1.0f, 0.2f, 0.4f, 112/255.f, 124/255.f, 130/255.f, 20);
    Plane table(0.0f, 0.0f, 0.0f, 6.0f, 4.0f, 139/255.f, 69/255.f, 19/255.f);
    Cylinder candle_bottom(1.6f, 1.0f, 0.0f, 0.5f, 0.2f, 112/255.f, 124/255.f, 130/255.f, 20);
    Cylinder candle_top(1.6f, 1.0f, 0.5f, 0.05f, 0.15f, 112/255.f, 124/255.f, 130/255.f, 20);
    Cone practiceCone(0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 112/255.f, 124/255.f, 130/255.f, 4);
    Cube lightSourceCube(lightPos.x, lightPos.y, lightPos.z, 0.5f, 0.5f, 0.5f,112/255.f, 124/255.f, 130/255.f);
    Cube subjectCube(0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f, 112/255.f, 124/255.f, 130/255.f);

    while(!glfwWindowShouldClose(window))
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 10.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // Enable lighting shader
        lightingShader.use();
        
        // Set the view position
        lightingShader.setVec3("viewPos", cameraPos);

        // Set directional light properties
        lightingShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.3f, 0.0f));
        lightingShader.setVec3("dirLight.direction", glm::vec3(3.0f, 0.0f, -3.0f));

        // Set point light properties
        lightingShader.setVec3("pointLights[0].position", glm::vec3(1.0f, 0.0f, 0.5f));
        lightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setVec3("pointLights[0].diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
        lightingShader.setVec3("pointLights[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setFloat("pointLights[0].constant", 1.0f );
        lightingShader.setFloat("pointLights[0].linear", 0.7f);
        lightingShader.setFloat("pointLights[0].quadratic", 1.8f);

        // Set material properties
        lightingShader.setInt("material.specular", 4);
        lightingShader.setInt("material.diffuse", 4);
        lightingShader.setFloat("material.shininess", 64.0f);
        
        // Set model, view, and projection for lighting shader
        lightingShader.setMatrix4fv("model", model);
        lightingShader.setMatrix4fv("view", view);
        if (usePerspective) {
            lightingShader.setMatrix4fv("projection", perspective);
        } else {
            lightingShader.setMatrix4fv("projection", ortho);
        }
        practiceCone.draw();
        glfwPollEvents();    
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

}
