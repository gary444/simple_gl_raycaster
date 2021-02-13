
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
#include "gl/Textures.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ScreenQuad.hpp"

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




double last_mouse_pos_x = 0.0;
double last_mouse_pos_y = 0.0;

float delta_x_mouse = 0.0;
float delta_y_mouse = 0.0;

float rotation_sensitivity = 0.002f;

float rendering_field_of_view = 60.0f;
float rendering_clip_near = 0.1f;
float rendering_clip_far  = 1000.0f;


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

    // loading volume file data
    g_volume_data = g_volume_loader.load_volume(volume_string);
    g_channel_size = g_volume_loader.get_bit_per_channel(volume_string) / 8;
    g_channel_count = g_volume_loader.get_channel_count(volume_string);

    // setting up proxy geometry
    cube.freeVAO();
    cube = grt::gl::Cube(glm::vec3(0.0, 0.0, 0.0), g_max_volume_bounds);

    glActiveTexture(GL_TEXTURE0 + volume_texture_unit);
    volume_texture = createTexture3D(g_vol_dimensions.x, g_vol_dimensions.y, g_vol_dimensions.z, g_channel_size, g_channel_count, (char*)&g_volume_data[0]);

    return 0 != volume_texture;

}

int main(int argc,  char * argv[]) {

    using namespace grt;

    
    if( cmd_option_exists(argv, argv+argc, "-h") 
        || argc < 5) 
    { 
        std::cout << "simple volume raycasting app\n";
        std::cout << "usage: <app_name> <path_to_volume> <output image path> <img resolution x> <img resolution y>\n";
        return -1;
    }


    const std::string volume_path = argv[1];
    const std::string img_out_path = argv[2];
    const glm::vec2 img_res ( atoi(argv[3]), atoi(argv[4]) );

    std::cout << "initialising GL..." << std::endl;
    GLFWwindow *window = nullptr;
    if (!setupGL(window, img_res.x, img_res.y)) {
        std::cout << "Error initialising GL" << std::endl;
    }


    grt::gl::Cube cube;

    // load a volume
    std::cout << "Reading Volume..." << std::endl;
    if (!read_volume(volume_path, cube)){
        std::cout << "Error reading volume / creating texture" << std::endl;
        return -1;
    }

    std::cout << "Image will be output to path: " << img_out_path << std::endl;
    std::cout << "Image resolution: " << glm::to_string(img_res) << std::endl;


    grt::gl::Shader rayshader("resources/shaders/texture_passthrough.vert", "resources/shaders/raycast_orthogonal.frag");
	rayshader.Use();  

    glActiveTexture(GL_TEXTURE0 + volume_texture_unit);

    glUniform1i(glGetUniformLocation(rayshader.Program, "volume_texture"), volume_texture_unit);
    glUniform3fv(glGetUniformLocation(rayshader.Program, "max_bounds"), 1, glm::value_ptr(g_max_volume_bounds));


    // render full screen quad
    grt::gl::ScreenQuad::render();

    // download rendered image
	grt::gl::download_default_framebuffer_to_image(img_res.x, img_res.y, img_out_path);


    
    glfwTerminate( );
   
    return 0;
    
}
