#ifndef GL_SETUP_HPP
#define GL_SETUP_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool setupGL(GLFWwindow *& window, const uint32_t WIDTH, const uint32_t HEIGHT);

#endif