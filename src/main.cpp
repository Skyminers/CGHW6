#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shaders.h"
#include "Camera.h"
#include "bezier.h"
#include "division.h"
#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *windows, double xOffset, double yOffset);

void readObj(const char *fileName);

vector<GLfloat> objVertices, objColor;
vector<GLuint> objElements;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//#define DEBUG
#define RIGHT_WHEEL_KEY glm::vec3(2,0.1,0)

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

GLuint vboCarVertices, vboCarColors, iboCarElements,
        vboCPVertices, vboCPColors, iboCPElements,
        vboObjVertices, vboObjColors, iboObjElements;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dream Car", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shaders shader("../src/shaders/vertexShader.glsl", "../src/shaders/fragmentShader.glsl");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    Bezier();

    glGenBuffers(1, &vboCarVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboCarVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(realVertices), realVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vboCarColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboCarColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(realColors), realColors, GL_STATIC_DRAW);

    glGenBuffers(1, &iboCarElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCarElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(realElements), realElements, GL_STATIC_DRAW);

#ifdef DEBUG
    glGenBuffers(1, &vboCPVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboCPVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(carVertices), carVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vboCPColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboCPColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CPColors), CPColors, GL_STATIC_DRAW);

    glGenBuffers(1, &iboCPElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCPElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CPElements), CPElements, GL_STATIC_DRAW);
#endif

    doSubdivision("wheel.obj","subdivisionOut.obj",5);

    readObj("./subdivisionOut.obj");

    glGenBuffers(1, &vboObjVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboObjVertices);
    glBufferData(GL_ARRAY_BUFFER, objVertices.size()*sizeof(GLfloat), &objVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vboObjColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboObjColors);
    glBufferData(GL_ARRAY_BUFFER, objColor.size()*sizeof(GLfloat), &objColor[0], GL_STATIC_DRAW);

    glGenBuffers(1, &iboObjElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboObjElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objElements.size()*sizeof(GLint), &objElements[0], GL_STATIC_DRAW);

    // render loop
    // -----------
    shader.useProgram();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw

        glBindVertexArray(VAO);

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f,
                                                100.0f);
        glm::mat4 view = camera.getViewMat();
        glm::mat4 model;

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vboCarVertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, vboCarColors);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCarElements);
        glDrawElements(GL_TRIANGLES, sizeof(realElements) / sizeof(realElements[0]), GL_UNSIGNED_INT, 0);

#ifdef DEBUG
        glBindBuffer(GL_ARRAY_BUFFER, vboCPVertices);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
        glBindBuffer(GL_ARRAY_BUFFER, vboCPColors);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCPElements);
        size_t offset = 0;
        for (int p = 0; p < LEN_PATCHES; p++)
            for (int i = 0; i < DEG+1; i++, offset += (DEG+1)*sizeof(GLuint))
                glDrawElements(GL_LINE_LOOP, DEG+1, GL_UNSIGNED_INT, (GLvoid*)offset);
#endif

        glBindBuffer(GL_ARRAY_BUFFER, vboObjVertices);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

        glBindBuffer(GL_ARRAY_BUFFER, vboObjColors);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboObjElements);

        model = glm::translate(model, RIGHT_WHEEL_KEY);
        shader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, objElements.size(), GL_UNSIGNED_INT,0);

        model = glm::translate(model, glm::vec3(3.3,0,0));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, objElements.size(), GL_UNSIGNED_INT,0);

        model = glm::translate(model, glm::vec3(0,0,3));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, objElements.size(), GL_UNSIGNED_INT,0);

        model = glm::translate(model, glm::vec3(-3.3,0,0));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, objElements.size(), GL_UNSIGNED_INT,0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void readObj(const char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("Can not open Obj file");
        return;
    }
    char buffer[1024];
    char blank[] = " ,\t\n";
    objVertices.clear();
    objColor.clear();
    objElements.clear();

    while (true) {
        if (fgets(buffer, 1024, f) == NULL) break;

        char *s = strtok(buffer, blank);
        if (s == NULL) continue;
        if (strcmp(s, "v") == 0) {
            GLfloat p[3];
            for (int i = 0; i < 3; ++i) {
                s = strtok(NULL, blank);
                objVertices.push_back(p[i] = atof(s));
            }
            GLfloat c = abs(p[2])*1.2;
            if(c > 0.7) c = 0.6;
            objColor.push_back(c);
            objColor.push_back(c);
            objColor.push_back(c);
        }
        if (strcmp(s, "f") == 0) {
            for (int i = 0; i < 3; ++i) {
                s = strtok(NULL, blank);
                objElements.push_back(atoi(s)-1);
            }
        }
    }
    fclose(f);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKey(cameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKey(cameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKey(cameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKey(cameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKey(cameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKey(cameraMovement::DOWN, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    static bool firstFlag = true;
    static float lastX, lastY;
    if (firstFlag) {
        lastX = xpos;
        lastY = ypos;
        firstFlag = false;
    }
    float xOffset = (xpos - lastX);
    float yOffset = (lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *windows, double xOffset, double yOffset) {
    camera.processScroll(yOffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
