#ifndef TEXTURES_HPP
#define TEXTURES_HPP


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


GLuint createTexture2D(unsigned const& width, unsigned const& height,
    const char* data);

GLuint createTexture2D(unsigned const& width, 
                      unsigned const& height,
                      GLint const& internal_format,
                      GLenum const& format,
                      GLenum const& type,
                      const char* data
                      );

GLuint createTexture3D(unsigned const& width, unsigned const& height,
    unsigned const& depth, unsigned const channel_size,
    unsigned const channel_count, const char* data);
GLuint createTexture3D(unsigned const& width, unsigned const& height,
    unsigned const& depth, unsigned const channel_size,
    unsigned const channel_count, const GLenum type, const char* data);



    

#endif