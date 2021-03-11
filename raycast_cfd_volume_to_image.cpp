
#include <cmath>
#include <iostream>
#include <chrono>
#include <string>
#include <limits>
#include <memory>
#include <numeric>
#include <thread>
#include <atomic> 
#include <cstring>
#include <dirent.h>




#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>



#include "helpers/mouse_and_keyboard_callbacks.hpp"
#include "helpers/util.hpp"

#include "gl/Shader.hpp"
#include "gl/ViewControls.hpp"

#include "gl/Cube.hpp"
#include "gl/GLSetup.hpp"
#include "gl/ScreenQuad.hpp"
#include "gl/Textures.hpp"
#include "gl/GLUtil.hpp"

#include "volume/volume_loader_raw.hpp"

bool keep_playing  = true;
float time_between_frames = 1.f;


auto last_cam_mat = glm::mat4(1.0);
auto cam_mat = glm::mat4(1.0f);

bool move_forward = false;
bool move_backward = false;
bool move_right = false;
bool move_left  = false;
bool move_up = false;
bool move_down  = false;
bool rotate_cam = false;

float elapsed_seconds_last_frame = 0.0;
float cam_speed = 0.5f;

Volume_loader_raw g_volume_loader;
volume_data_type g_volume_data;
glm::ivec3 g_vol_dimensions;
glm::vec3 g_max_volume_bounds;
unsigned g_channel_size = 0;
unsigned g_channel_count = 0;
GLuint g_volume_texture = 0;

float g_sampling_distance = 0.001f;







void update_camera() {
    glm::vec4 cam_pos = cam_mat[3];

    glm::vec4 right_vec = cam_mat[0];
    glm::vec4 up_vec = cam_mat[1];
    glm::vec4 forward_vec = cam_mat[2];

    bool changed_cam_pos = false;

    if (move_forward) {
        cam_pos -= cam_speed * forward_vec * elapsed_seconds_last_frame;
        changed_cam_pos = true;
    }

    if(move_backward) {
        cam_pos += cam_speed * forward_vec * elapsed_seconds_last_frame;
        changed_cam_pos = true;        
    }

    //
    if (move_right) {
        cam_pos += cam_speed * right_vec * elapsed_seconds_last_frame;
        changed_cam_pos = true;
    }

    if(move_left) {
        cam_pos -= cam_speed * right_vec * elapsed_seconds_last_frame;
        changed_cam_pos = true;        
    }

    if(move_up) {
        cam_pos += cam_speed * up_vec * elapsed_seconds_last_frame;
        changed_cam_pos = true;        
    }

    if(move_down) {
        cam_pos -= cam_speed * up_vec * elapsed_seconds_last_frame;
        changed_cam_pos = true;        
    }

    if(changed_cam_pos) {
        cam_mat[3] = cam_pos;
    }   
    

}


double last_mouse_pos_x = 0.0;
double last_mouse_pos_y = 0.0;

float delta_x_mouse = 0.0;
float delta_y_mouse = 0.0;

float rotation_sensitivity = 0.002f;


const GLuint WIDTH = 2560, HEIGHT = 1440;


float rendering_field_of_view = 60.0f;
float rendering_clip_near = 0.1f;
float rendering_clip_far  = 1000.0f;


auto perspective_mat = 
    glm::perspective(glm::radians(rendering_field_of_view), (float)WIDTH / (float)HEIGHT, rendering_clip_near, rendering_clip_far);


const GLuint volume_texture_unit = 0;
GLuint volume_texture = 0;


bool read_volume(const std::string& volume_string, grt::gl::Cube& cube){

    //init volume g_volume_loader
    //Volume_loader_raw g_volume_loader;
    //read volume dimensions
    g_vol_dimensions = g_volume_loader.get_dimensions(volume_string);

    g_sampling_distance = 1.0f / glm::max(glm::max(g_vol_dimensions.x, g_vol_dimensions.y), g_vol_dimensions.z);

    unsigned max_dim = std::max(std::max(g_vol_dimensions.x,
        g_vol_dimensions.y),
        g_vol_dimensions.z);

    std::cout << "Texture size: " << g_vol_dimensions.x << " x " << g_vol_dimensions.y << " x " << g_vol_dimensions.z << std::endl;

    // calculating max volume bounds of volume (0.0 .. 1.0)
    g_max_volume_bounds = glm::vec3(g_vol_dimensions) / glm::vec3((float)max_dim);

    std::cout << "Volume bounds: " << g_max_volume_bounds.x << " x " << g_max_volume_bounds.y << " x " << g_max_volume_bounds.z << std::endl;
    
    // loading volume file data
    g_volume_data = g_volume_loader.load_volume(volume_string);
    g_channel_size = g_volume_loader.get_bit_per_channel(volume_string) / 8;
    g_channel_count = g_volume_loader.get_channel_count(volume_string);

    // setting up proxy geometry
    cube.freeVAO();
    cube = grt::gl::Cube(glm::vec3(0.0, 0.0, 0.0), g_max_volume_bounds);

    glActiveTexture(GL_TEXTURE0 + volume_texture_unit);
    volume_texture = createTexture3D(g_vol_dimensions.x, g_vol_dimensions.y, g_vol_dimensions.z, g_channel_size, g_channel_count, GL_UNSIGNED_BYTE, (char*)&g_volume_data[0]);

    return 0 != volume_texture;

}



int main(int argc,  char * argv[]) {

    using namespace grt;

    
    if( cmd_option_exists(argv, argv+argc, "-h") 
        || argc < 4) 
    { 
        std::cout << "simple volume raycasting app\n";
        std::cout << "usage: <app_name> <path_to_volume> <output image width> <output image height> [options...] \nOptions:\n";
        // std::cout << "\t-f:  \n";

        return -1;
    }


    // float frame_rate = 1.f;
    // if (cmd_option_exists(argv, argv+argc, "-r") ){
    //     frame_rate = atof(get_cmd_option(argv, argv+argc, "-r"));
    //     time_between_frames = 1.f / frame_rate;
    //     std::cout << "Frame Rate: " << frame_rate << std::endl;
    // }


    // double util = 0;
    // if( cmd_option_exists(argv, argv+argc, "-u") ) {
    //     util = atof(get_cmd_option(argv, argv+argc, "-u"));
    // }



    // const std::string volume_path = argv[1];
    const std::string img_out_path = argv[2];
    const glm::vec2 img_res ( atoi(argv[3]), atoi(argv[4]) );

    std::cout << "Render resolution: " << glm::to_string(img_res) << std::endl;


    std::cout << "initialising GL..." << std::endl;
    GLFWwindow *window = nullptr;
    if (!setupGL(window, img_res.x, img_res.y)) {
        std::cout << "Error initialising GL" << std::endl;
    }


    grt::gl::Cube cube;


    // load a volume
    std::cout << "Reading Volume..." << std::endl;
    const std::string volume_path = argv[1];
    if (!read_volume(volume_path, cube)){
        std::cout << "Error reading volume / creating texture" << std::endl;
        return -1;
    }

    std::cout << "Created volume" << std::endl;

    // grt::gl::Shader rayshader("resources/shaders/raycast.vert", "resources/shaders/raycast_cfd.frag");
    grt::gl::Shader rayshader("resources/shaders/texture_passthrough.vert", "resources/shaders/raycast_orthogonal_cfd.frag");
	rayshader.Use();  

    glActiveTexture(GL_TEXTURE0 + volume_texture_unit);

    glUniform1i(glGetUniformLocation(rayshader.Program, "volume_texture"), volume_texture_unit);
    glUniform3fv(glGetUniformLocation(rayshader.Program, "max_bounds"), 1, glm::value_ptr(g_max_volume_bounds));


    // render full screen quad
    grt::gl::ScreenQuad::render();


    // download rendered image
	grt::gl::download_default_framebuffer_to_image(img_res.x, img_res.y, img_out_path);

    // //main loop
    // while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
    //        && !glfwWindowShouldClose( window ) )
    // {

        //         //update timestep according to time passed since program start
        // //could lead to skipped frames if playback rate is too high
        // auto start_timestamp = std::chrono::steady_clock::now();
        // current_timestep_to_render = (uint32_t)(elapsed_seconds_since_program_start / time_between_frames);
        // frame_is_update_frame = (current_timestep_to_render != last_timestep_rendered);

        // // setup model matrices
        // auto view_mat = glm::inverse(cam_mat);
        // //standard kinect view
        // view_mat = view_mat 
        //             * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -2.f) ) //normal avatar view
        //             * glm::rotate(glm::mat4(1.0f), float(M_PI), glm::vec3(0.0f, 1.0f, 0.0f) )// normal avatar view
        //             ;
        // auto mvp_mat = perspective_mat * view_mat;

        // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClearColor( 0.85f, 0.862, 0.878f,1.0f );

        // if (frame_is_update_frame){

        //     std::cout << "CURRENT FRAME : " << current_timestep_to_render << std::endl;

        //     last_timestep_rendered = current_timestep_to_render;

        // }


        // rayshader.Use();
        
        // const glm::vec4 camera_translate = glm::column(glm::inverse(view_mat), 3);
        // const glm::vec3 camera_location = glm::vec3(camera_translate.x, camera_translate.y, camera_translate.z);
        

        // glUniform1i(glGetUniformLocation(rayshader.Program, "volume_texture"), volume_texture_unit);
        // glUniformMatrix4fv(glGetUniformLocation(rayshader.Program, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp_mat));
        // glUniform3fv(glGetUniformLocation(rayshader.Program, "camera_location"), 1, glm::value_ptr(camera_location));
        // glUniform3fv(glGetUniformLocation(rayshader.Program, "max_bounds"), 1, glm::value_ptr(g_max_volume_bounds));
        // cube.draw();



        // glfwPollEvents( );
        // glfwSwapBuffers( window );
        
        // auto end_timestamp = std::chrono::steady_clock::now();
        // float const elapsed_micro_seconds = std::chrono::duration_cast<std::chrono::microseconds>(end_timestamp - start_timestamp).count();
        // if (keep_playing) elapsed_seconds_since_program_start += elapsed_micro_seconds / 1000000.0;
        // elapsed_seconds_last_frame =  elapsed_micro_seconds / 1000000.0;
        // // std::cout << "Frame time: " << elapsed_micro_seconds / 1000.0  << " ms" << std::endl;


        // update_camera();

    // }



    
    glfwTerminate( );
    



    return 0;
    
}
