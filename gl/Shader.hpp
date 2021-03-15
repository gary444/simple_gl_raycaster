#ifndef Shader_h
#define Shader_h


#include "Shadinclude.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

namespace grt {
namespace gl {



class Shader
{
public:
    GLuint Program;



    Shader(GLuint shader_type, std::string const& shader_path) {
        this->Program = glCreateProgram( );


            std::string code;

            //std::ifstream shaderFile;
            // ensures ifstream objects can throw exceptions:
            //shaderFile.exceptions ( std::ifstream::badbit );

            try
            {

                code = Shadinclude::load(shader_path, "#include");
            }
            catch ( std::ifstream::failure& e )
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            const GLchar *shaderCode = code.c_str( );
            // 2. Compile shaders
            GLuint shader;
            GLint success;
            GLchar infoLog[512];
            // Vertex Shader
            shader = glCreateShader( shader_type);
            glShaderSource( shader, 1, &shaderCode, NULL );
            glCompileShader( shader );
            // Print compile errors if any
            glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
            if ( !success )
            {
                glGetShaderInfoLog( shader, 512, NULL, infoLog );
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            /*
            // Fragment Shader
            fragment = glCreateShader( GL_FRAGMENT_SHADER );
            glShaderSource( fragment, 1, &fShaderCode, NULL );
            glCompileShader( fragment );
            // Print compile errors if any
            glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
            if ( !success )
            {
                glGetShaderInfoLog( fragment, 512, NULL, infoLog );
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }*/
            // Shader Program
            //this->Program = glCreateProgram( );
            glAttachShader( this->Program, shader );

        

        glLinkProgram( this->Program );
        // Print linking errors if any
        

        glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader( shader );
        //glDeleteShader( fragment );
    }

    // Constructor generates the shader on the fly
    //Shader( const GLchar *vertexPath, const GLchar *fragmentPath = nullptr)
    Shader( std::string const& vertexPath, std::string const& fragmentPath = "")// const GLchar *vertexPath, const GLchar *fragmentPath = nullptr)
    {
        bool fragment_shader_exists = ("" != fragmentPath);

        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        try
        {


            vertexCode = Shadinclude::load(vertexPath, "#include");

            if(fragment_shader_exists) {
                fragmentCode = Shadinclude::load(fragmentPath, "#include");
            }
        }
        catch ( std::ifstream::failure& e )
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar *vShaderCode = vertexCode.c_str( );
        const GLchar *fShaderCode = fragmentCode.c_str( );
        // 2. Compile shaders
        GLuint vertex, fragment = 0;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &vShaderCode, NULL );
        glCompileShader( vertex );
        // Print compile errors if any
        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( vertex, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        if(fragment_shader_exists) {
            // Fragment Shader
            fragment = glCreateShader( GL_FRAGMENT_SHADER );
            glShaderSource( fragment, 1, &fShaderCode, NULL );
            glCompileShader( fragment );
            // Print compile errors if any
            glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
            if ( !success )
            {
                glGetShaderInfoLog( fragment, 512, NULL, infoLog );
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

        } else {
            std::cout << "No fragment shader provided. Compiling shader program containing vertex shader only" << std::endl;
        }
        
        // Shader Program
        this->Program = glCreateProgram( );
        glAttachShader( this->Program, vertex );
        if(fragment_shader_exists) {
            glAttachShader( this->Program, fragment );
        }
        glLinkProgram( this->Program );
        // Print linking errors if any
        glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader( vertex );

        if(fragment_shader_exists) {
            glDeleteShader( fragment );
        }
        
    }

    void DispatchCompute(int group_size_x, int group_size_y, int group_size_z) {
        glDispatchCompute(group_size_x, group_size_y, group_size_z);
    }

    // Uses the current shader
    void Use( )
    {
        glUseProgram( this->Program );
    }
};

}//gl
}//grt

#endif /* Shader_h */
