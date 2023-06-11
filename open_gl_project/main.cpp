#include "data_ecg.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

float ratio;
float r=1,g=0,b=0;
float offsetY = -0.5f;
float sp=0;
typedef struct
{
    GLfloat x, y, z;
    GLfloat r, g, b, a;
} Vertex;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << key << std::endl;
    if (key == 82) {
        r = 1;
        g = 0;
        b = 0;
    }
    if (key == 71) {
        r = 0;
        g = 1;
        b = 0;
    }
    if (key == 66) {
        r = 0;
        g = 0;
        b = 1;
    }
    if (key == 45) {
        offsetY = offsetY - 0.1;
    }

    if (key == 61) {
        offsetY = offsetY + 0.1;
    }
    if (key == 85) {
        sp = sp + 0.001;
    }

    if (key == 76) {
        sp = sp - 0.001;
    }

}

void DrawLineSegment(Vertex v1, Vertex v2)
{
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor4f(v1.r, v1.g, v1.b, v1.a);
    glVertex3f(v1.x, v1.y, v1.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}
void DrawGrid(GLfloat width, GLfloat height, GLfloat gridWidth)
{
    for (float i = -height; i < height; i += gridWidth)
    {
        Vertex v1 = { -width, i, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
        Vertex v2 = { width, i, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
        DrawLineSegment(v1, v2);
    }
    for (float i = -width; i < width; i += gridWidth)
    {
        Vertex v1 = { i, -height, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
        Vertex v2 = { i, height, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
        DrawLineSegment(v1, v2);
    }
}
void PlotECGData(int offset, int size)
{
    const float space = 0.01042-sp;
    float pos = -2.65f;

    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor4f(r,g,b, 1.0f);
    for (size_t i = offset; i < size + offset; ++i)
    {
        const float data = data_ecg[i] + offsetY;
        glVertex3f(pos, data, 0.0f);
        pos += space;
    }
    glEnd();
}
void ECG(int counter)
{
    const int dataSize = 1024;
    PlotECGData(counter, dataSize * 0.5);
}
int main(int argc, char const* argv[])
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    GLFWwindow* window = glfwCreateWindow(1280, 480, "Electrocardiogram", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(2);
    }
    glfwMakeContextCurrent(window);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float counter = 0;
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = (float)width / (float)height;
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glfwSetKeyCallback(window, key_callback);

        DrawGrid(5.0f, 1.0f, 0.1f);
        if (counter > 5000)
        {
            counter = 0;
        }
        counter += 5;
        ECG(counter);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
}

