#version 430 

//#extension GL_ARB_shading_language_420pack : require
// #extension GL_ARB_explicit_attrib_location : require
// in vec3 ray_entry_position;

in vec2 pass_TexCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler3D volume_texture;

// uniform vec3    camera_location;

const float sampling_distance = 0.001f;

// uniform float   iso_value;
// uniform float   iso_value_2;
uniform vec3    max_bounds;
// uniform ivec3   volume_dimensions;

// uniform vec3    light_position;
// uniform vec3    light_ambient_color;
// uniform vec3    light_diffuse_color;
// uniform vec3    light_specular_color;
// uniform float   light_ref_coef;


const float transparency_epsilon = 0.001;


float sample_volume (vec3 sampling_pos) {

    // correct for non-unit cubes
    // vec3 sample_coords = sampling_pos / max_bounds;
    vec3 sample_coords = sampling_pos;

    float sample_val = texture(volume_texture, sample_coords).r;

    return sample_val;
}

bool
inside_volume_bounds(const in vec3 sampling_position)
{
    return (   all(greaterThanEqual(sampling_position, vec3(0.0)))
            && all(lessThanEqual(sampling_position, vec3(1.0) )));
            // && all(lessThanEqual(sampling_position, max_bounds)));
}



void main()
{

    // test
    // FragColor = vec4(pass_TexCoord.x, pass_TexCoord.y, 0.0, 1.0);

    // rays will be cast along X-axis from X=0
    vec3 ray_direction = vec3(1,0,0);
    // so starting point of ray is (0, tex.y, tex.x)
    vec3 ray_entry_position = vec3(0,pass_TexCoord.y, pass_TexCoord.x);

    /// One step trough the volume
    vec3 ray_increment      = ray_direction * sampling_distance;
    // Position in Volume
    vec3 sampling_pos       = ray_entry_position + ray_increment;

    /// Init color of fragment
    vec4 out_col = vec4(0.0, 0.0, 0.0, 1.0);

    /// check if we are inside volume
    bool inside_volume = inside_volume_bounds(sampling_pos);
    
    if (!inside_volume)
        discard;

    float sum = 0.f;
    float samples = 0.f;


    // the traversal loop,
    // termination when the sampling position is outside volume boundary
    while (inside_volume)
    {      

        // assume cube is in 0 - 1 space anyway
        // float sample_val = texture(volume_texture, sampling_pos).r;
        float sample_val = sample_volume(sampling_pos); 


        sum += sample_val;
        samples += 1.f;

        // increment the ray sampling position
        sampling_pos  += ray_increment;

        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }

    sum /= samples;

    out_col = vec4(sum,sum,sum,1.f);

    // return the calculated color value
    FragColor = out_col;
}

