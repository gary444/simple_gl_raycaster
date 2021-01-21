#include <iostream>




#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

// #include <grt/types/CameraDescription.h>
// #include <grt/CutSelection/CutUpdateCharacteristics.hpp>

#include "mouse_and_keyboard_callbacks.hpp"

extern bool rotate_cam;
extern bool keep_rotating;
extern bool keep_playing;

extern bool projectively_texture_mesh;
extern bool show_cut_depth;



extern bool move_forward;
extern bool move_backward;
extern bool move_right;
extern bool move_left;
extern bool move_down;
extern bool move_up;


extern float delta_x_mouse;
extern float delta_y_mouse;

extern double last_mouse_pos_x;
extern double last_mouse_pos_y;
extern float rotation_sensitivity;

extern glm::mat4 cam_mat;



// extern grt::types::general_camera_description camera_description;

void cursor_position_callback(GLFWwindow* window, double current_mouse_pos_x, double current_mouse_pos_y){

    if(rotate_cam) {
        delta_x_mouse = (current_mouse_pos_x-last_mouse_pos_x) * rotation_sensitivity;
        delta_y_mouse = (current_mouse_pos_y-last_mouse_pos_y) * rotation_sensitivity;
        

        last_mouse_pos_x = current_mouse_pos_x;
        last_mouse_pos_y = current_mouse_pos_y;


        {
            glm::vec4 current_right_vec   = cam_mat[0];
            glm::vec4 current_up_vec      = cam_mat[1];
            glm::vec4 current_forward_vec = cam_mat[2];
            //cam_mat = glm::rotate(cam_mat, -delta_x_mouse, glm::fvec3{0.0, 1.f, 0.0});
            cam_mat[0] = current_right_vec * glm::rotate(glm::mat4(1.0), delta_x_mouse, glm::fvec3{0.f, 1.0, 0.0});
            cam_mat[1] = current_up_vec * glm::rotate(glm::mat4(1.0), delta_x_mouse, glm::fvec3{0.f, 1.0, 0.0});
            cam_mat[2] = current_forward_vec * glm::rotate(glm::mat4(1.0), delta_x_mouse, glm::fvec3{0.f, 1.0, 0.0});
        }


        {
            glm::vec4 current_right_vec   = cam_mat[0];
            glm::vec4 current_up_vec      = cam_mat[1];
            glm::vec4 current_forward_vec = cam_mat[2];
            //cam_mat = glm::rotate(cam_mat, -delta_x_mouse, glm::fvec3{0.0, 1.f, 0.0});
            cam_mat[0] = current_right_vec * glm::rotate(glm::mat4(1.0), delta_y_mouse, glm::fvec3{1.f, 0.0, 0.0});
            cam_mat[1] = current_up_vec * glm::rotate(glm::mat4(1.0), delta_y_mouse, glm::fvec3{1.f, 0.0, 0.0});
            cam_mat[2] = current_forward_vec * glm::rotate(glm::mat4(1.0), delta_y_mouse, glm::fvec3{1.f, 0.0, 0.0});
        }
    
    }

}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if(action == GLFW_PRESS) {
        rotate_cam = true;
        glfwGetCursorPos(window, &last_mouse_pos_x, &last_mouse_pos_y);
      } else {
        rotate_cam = false;        
      }
    }


    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{





    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        keep_playing = !keep_playing;
    }

    //zoom
    if (key == GLFW_KEY_W ) {
        if(action == GLFW_PRESS) {
            move_forward = true;
        } else if(action == GLFW_RELEASE){
            move_forward = false;  
        }
    }

    if (key == GLFW_KEY_S ) {
        if(action == GLFW_PRESS) {
            move_backward = true;
        } else if(action == GLFW_RELEASE){
            move_backward = false;  
        }
    }

    if (key == GLFW_KEY_A ) {
        if(action == GLFW_PRESS) {
            move_left = true;
        } else if(action == GLFW_RELEASE){
            move_left = false;  
        }
    }

    if (key == GLFW_KEY_D ) {
        if(action == GLFW_PRESS) {
            move_right = true;
        } else if(action == GLFW_RELEASE){
            move_right = false;  
        }
    }

    if (key == GLFW_KEY_Q ) {
        if(action == GLFW_PRESS) {
            move_up = true;
        } else if(action == GLFW_RELEASE){
            move_up = false;  
        }
    }

    if (key == GLFW_KEY_E ) {
        if(action == GLFW_PRESS) {
            move_down = true;
        } else if(action == GLFW_RELEASE){
            move_down = false;  
        }
    }



}
