

// Std. Includes
#include <string>
#include <iostream>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include <shader.h>
#include <arcball.h>
#include <Model.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function prototypes
GLFWwindow* glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void render();
void processInput(GLFWwindow* window);
// Global variables

// your source directory (folder) name (use '/' not '\' or '\\' in Windows)
string sourceDirStr = "C:/Users/k5911/Desktop/new_graphics/projects/PROJECT_V1/PROJECT_V1";
string modelDirStr = "C:/Users/k5911/Desktop/new_graphics/data";

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWwindow* mainWindow = NULL;
glm::mat4 projection;
Shader* shader = NULL;
Shader* stshader = NULL;
// For model
Model* ourModel = NULL;
Model* stModel = NULL;
// for arcball
float arcballSpeed = 0.2f;
static Arcball camArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
static Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;

// for camera
glm::vec3 cameraOrigPos(0.0f, 0.0f, 9.0f);
glm::vec3 cameraPos; // current position of camera
glm::vec3 modelPan(0.0f, 0.0f, 0.0f);  // model panning vector


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Shader* chrshader = NULL;
Shader* gramashader = NULL;
Shader* boyshader = NULL;
Shader* doushader = NULL;
int main()
{
    mainWindow = glAllInit();

    // Create shader program object
    std::string svs = sourceDirStr + "/skel_anim.vs";
    std::string sfs = sourceDirStr + "/skel_anim.fs";
    string vs = sourceDirStr + "/modelLoading.vs";
    string fs = sourceDirStr + "/modelLoading.fs";


    shader = new Shader(vs.c_str(), fs.c_str());
    stshader = new Shader(vs.c_str(), fs.c_str());
    chrshader = new Shader(svs.c_str(), sfs.c_str());
    gramashader = new Shader(svs.c_str(), sfs.c_str());
    boyshader = new Shader(svs.c_str(), sfs.c_str());
    doushader = new Shader(svs.c_str(), sfs.c_str());
    // Load a model
    std::string modelPath = modelDirStr + "/last/dae2/statics/park.obj";
    std::string stPath = modelDirStr + "/last/dae2/statics/streetlight.obj";
    std::string chrPath = modelDirStr + "/last/dae2/Adding2.dae";
    std::string graPath = modelDirStr + "/last/dae2/grandma.dae";
    std::string boyPath = modelDirStr + "/last/dae2/boy.dae";
    std::string douPath = modelDirStr + "/last/dae2/doughnut.dae";
    ourModel = new Model(modelPath);
    stModel = new Model(stPath);
    Model chrModel(chrPath);
    Model graModel(graPath);
    Model boyModel(boyPath);
    Model douModel(douPath);


    // Initializing projection transformation
    projection = glm::perspective(glm::radians(45.0f),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
    shader->use();
    shader->setMat4("projection", projection);
    stshader->use();
    stshader->setMat4("projection", projection);
    cameraPos = cameraOrigPos;

    Animation anim(chrPath, &chrModel);
    Animator animator(&anim);
    Animation anim2(graPath, &graModel);
    Animator animator2(&anim2);
    Animation anim3(boyPath, &boyModel);
    Animator animator3(&anim3);
    Animation anim4(douPath, &douModel);
    Animator animator4(&anim4);

    // Game loop
    while (!glfwWindowShouldClose(mainWindow))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(mainWindow);
        animator.UpdateAnimation(deltaTime);
        animator2.UpdateAnimation(deltaTime);
        animator3.UpdateAnimation(deltaTime);
        animator4.UpdateAnimation(deltaTime);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view = view * camArcBall.createRotationMatrix();
        shader->setMat4("view", view);

        // Draw the loaded model
        glm::mat4 model(1.0);
            
        // Rotate model by arcball and panning
        model = glm::translate(model, modelPan);
        model = model * modelArcBall.createRotationMatrix();

        // It's a bit too big for our scene, so scale it down
        model = glm::scale( model, glm::vec3( 50.0f, 50.0f, 50.0f) );

        shader->setMat4("model", model);

        ourModel->Draw(*shader);
        //street
        stshader->use();

        glm::mat4 stview = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        stview = stview * camArcBall.createRotationMatrix();
        stshader->setMat4("view", stview);

        // Draw the loaded model
        glm::mat4 stmodel(1.0);

        // Rotate model by arcball and panning
        stmodel = glm::translate(stmodel, modelPan);
        stmodel = stmodel * modelArcBall.createRotationMatrix();

        // It's a bit too big for our scene, so scale it down
        stmodel = glm::scale(stmodel, glm::vec3(0.3f, 0.3f, 0.3f));

        stshader->setMat4("model", stmodel);

        stModel->Draw(*stshader);





        glm::mat4 projection1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view1 = camera.GetViewMatrix();


        chrshader->use();
        chrshader->setMat4("projection", projection1);
        chrshader->setMat4("view", view1);

        auto transforms = animator.GetFinalBoneMatrices();

        for (size_t i = 0; i < transforms.size(); ++i) {
            chrshader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        }

        glm::mat4 cmodel = glm::mat4(1.0f);
        cmodel = glm::translate(cmodel, glm::vec3(0.0f, 0.0f, 0.0f));
        cmodel = glm::rotate(cmodel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        cmodel = glm::scale(cmodel, glm::vec3(0.003f, 0.003f, 0.003f));
        chrshader->setMat4("model", cmodel);

        chrModel.Draw(*chrshader);
        //grama
        gramashader->use();
        gramashader->setMat4("projection", projection1);
        gramashader->setMat4("view", view1);

        auto transforms2 = animator2.GetFinalBoneMatrices();

        for (size_t i = 0; i < transforms2.size(); ++i) {
            gramashader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms2[i]);
        }

        glm::mat4 gmodel = glm::mat4(1.0f);
        gmodel = glm::translate(gmodel, glm::vec3(-1.0f, 0.0f, 0.0f));
        gmodel = glm::scale(gmodel, glm::vec3(0.003f, 0.003f, 0.003f));
        gramashader->setMat4("model", gmodel);

        graModel.Draw(*gramashader);

        //boy
        boyshader->use();
        boyshader->setMat4("projection", projection1);
        boyshader->setMat4("view", view1);

        auto transforms3 = animator3.GetFinalBoneMatrices();

        for (size_t i = 0; i < transforms2.size(); ++i) {
            boyshader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms3[i]);
        }

        glm::mat4 bmodel = glm::mat4(1.0f);
        bmodel = glm::translate(bmodel, glm::vec3(-1.0f, 0.0f, -1.0f));
        bmodel = glm::scale(bmodel, glm::vec3(0.003f, 0.003f, 0.003f));
        boyshader->setMat4("model", bmodel);

        boyModel.Draw(*boyshader);
        
        //doughnout
        doushader->use();
        doushader->setMat4("projection", projection1);
        doushader->setMat4("view", view1);

        auto transforms4 = animator4.GetFinalBoneMatrices();

        for (size_t i = 0; i < transforms2.size(); ++i) {
            doushader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms4[i]);
        }

        glm::mat4 dmodel = glm::mat4(1.0f);
        dmodel = glm::translate(dmodel, glm::vec3(1.0f, 0.0f, -1.0f));
        dmodel = glm::scale(dmodel, glm::vec3(3.0f, 3.0f, 3.0f));
        doushader->setMat4("model", dmodel);

        douModel.Draw(*doushader);


        

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void render()
{

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Model Loading", NULL, NULL);

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //이거 애매함
    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // OpenGL initialization stuffs
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
    stshader->use();
    shader->setMat4("projection", projection);
    stshader->setMat4("projection", projection);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        cameraPos = cameraOrigPos;
        modelPan[0] = modelPan[1] = modelPan[2] = 0.0f;
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        arcballCamRot = !arcballCamRot;
        if (arcballCamRot) {
            cout << "ARCBALL: Camera rotation mode" << endl;
        }
        else {
            cout << "ARCBALL: Model  rotation mode" << endl;
        }
    }
    else if (key == GLFW_KEY_LEFT) {
        modelPan[0] -= 0.1;
    }
    else if (key == GLFW_KEY_RIGHT) {
        modelPan[0] += 0.1;
    }
    else if (key == GLFW_KEY_DOWN) {
        modelPan[1] -= 0.1;
    }
    else if (key == GLFW_KEY_UP) {
        modelPan[1] += 0.1;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (arcballCamRot)
        camArcBall.mouseButtonCallback(window, button, action, mods);
    else
        modelArcBall.mouseButtonCallback(window, button, action, mods);
}

void cursor_position_callback(GLFWwindow* window, double x, double y) {
    if (arcballCamRot)
        camArcBall.cursorCallback(window, x, y);
    else
        modelArcBall.cursorCallback(window, x, y);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraPos[2] -= (yoffset * 0.5);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
     
}
