// Std. Includes
#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <Model.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function prototypes
GLFWwindow* glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

// Global variables
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWwindow* mainWindow = NULL;
glm::mat4 projection;
Shader* shader = NULL;
std::vector<Model> simulationFrames;
unsigned int currentFrame = 0;
float frameInterval = 0.1f; // Time per frame in seconds
float lastFrameTime = 0.0f;

int main()
{
    mainWindow = glAllInit();

    // Create shader program object
    std::string vs = "C:/CG/projects/FinalProject/FinalProject/fountain.vs";
    std::string fs = "C:/CG/projects/FinalProject/FinalProject/fountain.fs";
    shader = new Shader(vs.c_str(), fs.c_str());

    // Load simulation frames
    for (int i = 200; i <= 240; ++i) {
        std::string modelPath = "C:/CG/data/last/fountain0" + std::to_string(i) + ".obj";
        simulationFrames.push_back(Model(modelPath));
    }

    // Initialize projection transformation
    projection = glm::perspective(glm::radians(45.0f),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->use();
    shader->setMat4("projection", projection);

    // Game loop
    while (!glfwWindowShouldClose(mainWindow))
    {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;

        processInput(mainWindow);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        shader->setMat4("view", view);

        glm::mat4 model(1.0);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        shader->setMat4("model", model);

        simulationFrames[currentFrame].Draw(*shader);

        if (deltaTime >= frameInterval) {
            currentFrame = (currentFrame + 1) % simulationFrames.size();
            lastFrameTime = currentFrameTime;
        }

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* glAllInit()
{
    // Initialize GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fluid Simulation", NULL, NULL);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    // OpenGL initialization stuffs
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    projection = glm::perspective(glm::radians(45.0f),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->use();
    shader->setMat4("projection", projection);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
