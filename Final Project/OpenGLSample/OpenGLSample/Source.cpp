#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "shader.hpp"
#include "../../../Assignment 4-3/OpenGLSample/OpenGLSample/camera.h"

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;

GLfloat rotationAngle = 0.0f;
GLfloat rotationSpeed = 1.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void moveLightSource();

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW window options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFW window
    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set key callback function
    glfwSetKeyCallback(window, keyCallback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Set viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Load and compile shaders
    GLuint shaderProgram = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    // Define positions and indices for the corndog
    GLfloat cornDogVertices[] = {
        // Positions
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    GLuint cornDogIndices[] = {
        0, 1, 2, // Front face
        2, 3, 0,
        1, 5, 6, // Right face
        6, 2, 1,
        7, 6, 5, // Back face
        5, 4, 7,
        4, 0, 3, // Left face
        3, 7, 4,
        4, 5, 1, // Bottom face
        1, 0, 4,
        3, 2, 6, // Top face
        6, 7, 3
    };

    GLuint cornDogVAO, cornDogVBO, cornDogEBO;
    glGenVertexArrays(1, &cornDogVAO);
    glGenBuffers(1, &cornDogVBO);
    glGenBuffers(1, &cornDogEBO);

    glBindVertexArray(cornDogVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cornDogVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cornDogVertices), cornDogVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cornDogEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cornDogIndices), cornDogIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Define positions and indices for the chapstick
    GLfloat chapstickVertices[] = {
        // Positions
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    GLuint chapstickIndices[] = {
        0, 1, 2, // Front face
        2, 3, 0,
        1, 5, 6, // Right face
        6, 2, 1,
        7, 6, 5, // Back face
        5, 4, 7,
        4, 0, 3, // Left face
        3, 7, 4,
        4, 5, 1, // Bottom face
        1, 0, 4,
        3, 2, 6, // Top face
        6, 7, 3
    };

    GLuint chapstickVAO, chapstickVBO, chapstickEBO;
    glGenVertexArrays(1, &chapstickVAO);
    glGenBuffers(1, &chapstickVBO);
    glGenBuffers(1, &chapstickEBO);

    glBindVertexArray(chapstickVAO);
    glBindBuffer(GL_ARRAY_BUFFER, chapstickVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chapstickVertices), chapstickVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chapstickEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(chapstickIndices), chapstickIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Define positions and indices for the plane
    GLfloat planeVertices[] = {
        // Positions
        -0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, 0.5f
    };

    GLuint planeIndices[] = {
        0, 1, 2, // Triangle 1
        0, 2, 3  // Triangle 2
    };

    GLuint planeVAO, planeVBO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Clear the color and depth buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Set the view matrix based on the camera position and orientation
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Render the corndog
        glBindVertexArray(cornDogVAO);
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render the chapstick
        glBindVertexArray(chapstickVAO);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render the plane
        glBindVertexArray(planeVAO);
        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

        // Update the rotation angle
        rotationAngle += rotationSpeed;
    }

    // Cleanup
    glDeleteVertexArrays(1, &cornDogVAO);
    glDeleteBuffers(1, &cornDogVBO);
    glDeleteBuffers(1, &cornDogEBO);

    glDeleteVertexArrays(1, &chapstickVAO);
    glDeleteBuffers(1, &chapstickVBO);
    glDeleteBuffers(1, &chapstickEBO);

    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &planeEBO);

    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
