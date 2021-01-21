#ifndef APP_GPU_TSDF_SVO_EXTRACTION_MOUSE_AND_KEYBOARD_CALLBACKS_H
#define APP_GPU_TSDF_SVO_EXTRACTION_MOUSE_AND_KEYBOARD_CALLBACKS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void cursor_position_callback(GLFWwindow* window, double current_mouse_pos_x, double current_mouse_pos_y);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif //APP_GPU_TSDF_SVO_EXTRACTION_MOUSE_AND_KEYBOARD_CALLBACKS_H