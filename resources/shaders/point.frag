#version 330 core



// uniform vec3 incolor = vec3(0.f,1.f,0.f);
in vec3 pass_color;

out vec4 color;


void main()
{

    color = vec4(pass_color,1);
}
