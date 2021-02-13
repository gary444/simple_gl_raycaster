

#include <GL/glew.h>
// #include "Shader.hpp"


namespace grt{
namespace gl{


class ScreenQuad
{
        // screen quad setup
    float vertices[8] = {
         1.0f,  1.0f,  // top right
         1.0f, -1.0f,  // bottom right
        -1.0f, -1.0f,  // bottom left
        -1.0f,  1.0f   // top left 
    };

    float texCoords[8] = {
         1.0f,  1.0f,  // top right
         1.0f,  0.0f,  // bottom right
         0.0f,  0.0f,  // bottom left
         0.0f,  1.0f   // top left 
    };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    GLuint quadVAO = 0; 
    GLuint quadVBO = 0; 
    GLuint quadTBO = 0;
    GLuint quadEBO = 0;

    // grt::gl::Shader quadShader = grt::gl::Shader ("resources/shaders/quad.vert", "resources/shaders/quad.frag");

    // ScreenQuad();

    // singleton
    // prevent construction by user
    ScreenQuad();
    // ~ScreenQuad();
    ScreenQuad(ScreenQuad const&) = delete;
    ScreenQuad& operator=(ScreenQuad const&) = delete;


public:
    static void render();

    // static void setTextureUnit(const GLuint unit);
    // static void setLayer(const GLuint layer);
    // static void setAlpha(const GLuint alpha);

};

}
}