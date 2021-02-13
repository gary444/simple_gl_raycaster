#version 430 
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_shading_language_include : require


layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 pass_TexCoord;

void main(void) {
   gl_Position = vec4(position, 0.0f, 1.0f);
   pass_TexCoord = texcoord;
}
