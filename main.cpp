#include <iostream>
#include <sstream>
#include <string>

#include "Camera.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const char* APP_TITLE = "Jeremy's 3D Rendering Demo";
int gWindowWidth = 1280;
int gWindowHeight = gWindowWidth * 9 / 16;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;

FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0;
const float MOUSE_SENSITIVITY = 0.1f;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main() {
    if (!initOpenGL()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");

    const int numModels = 1;
    Mesh mesh[numModels];
    Texture2D texture[numModels];

    mesh[0].loadOBJ("models/deer.obj");
    // mesh[1].loadOBJ("models/woodcrate.obj");
    // mesh[2].loadOBJ("models/deer.obj");
    // mesh[3].loadOBJ("models/floor.obj");

    texture[0].loadTexture("textures/deer.jpg", true);
    // texture[1].loadTexture("textures/woodcrate_diffuse.jpg", true);
    // texture[2].loadTexture("textures/deer.jpg", true);
    // texture[3].loadTexture("textures/tile_floor.jpg", true);

    glm::vec3 modelPos[] = {glm::vec3(-2.5f, 1.0f, 0.0f), glm::vec3(2.5f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f)};

    float scale = 0.075f;
    glm::vec3 modelScale[] = {
        glm::vec3(scale, scale, scale),  //
        glm::vec3(1.00f, 1.00f, 1.00f),  //
        glm::vec3(1.00f, 1.00f, 1.00f),  //
        glm::vec3(10.0f, 1.00f, 10.0f)   //
    };

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(gWindow)) {
        showFPS(gWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        glfwPollEvents();
        update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);

        view = fpsCamera.getViewMatrix();

        projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

        shaderProgram.use();

        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);

        for (int i = 0; i < numModels; i++) {
            model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
            model *= glm::rotate(glm::mat4(1.0), glm::radians(270.0f), glm::vec3(1, 0, 0));
            shaderProgram.setUniform("model", model);
            shaderProgram.setUniform("time", (float) glfwGetTime());

            texture[i].bind(0);
            mesh[i].draw();
            texture[i].unbind(0);
        }

        glfwSwapBuffers(gWindow);

        lastTime = currentTime;
    }

    glfwTerminate();

    return 0;
}

bool initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
    if (gWindow == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
    glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    glClearColor(0.0f / 255, 20.0f / 255, 60.0f / 255, 1.0f);

    glViewport(0, 0, gWindowWidth, gWindowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        gWireframe = !gWireframe;
        if (gWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height) {
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY) {
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    fpsCamera.setFOV((float)fov);
}
void update(double elapsedTime) {
    double mouseX, mouseY;

    glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
    } else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
    }

    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
    } else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
    }

    if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
    } else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
    }

    if (glfwGetKey(gWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0, 1, 0));
    } else if (glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0, -1, 0));
    }
}

void showFPS(GLFWwindow* window) {
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime();

    elapsedSeconds = currentSeconds - previousSeconds;

    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << APP_TITLE << "    "
             << "FPS: " << fps << "    "
             << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());
        frameCount = 0;
    }

    frameCount++;
}