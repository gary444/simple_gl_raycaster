#version 150
//#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require


// mode 0 : average IP
// mode 1 : max IP
// mode 2 : FTB compositing


#define MODE 2


in vec3 ray_entry_position;

layout(location = 0) out vec4 FragColor;

uniform sampler3D volume_texture;

uniform vec3    camera_location;

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


const float scale_factor = 1.f;


float sample_volume (vec3 sampling_pos) {

    // correct for non-unit cubes
    vec3 sample_coords = sampling_pos / max_bounds;

    float sample_val = texture(volume_texture, sample_coords).r;

    return sample_val;
}

bool
inside_volume_bounds(const in vec3 sampling_position)
{
    return (   all(greaterThanEqual(sampling_position, vec3(0.0)))
            && all(lessThanEqual(sampling_position, max_bounds)));
}



void main()
{
    /// One step trough the volume
    vec3 ray_increment      = normalize(ray_entry_position - camera_location) * sampling_distance;
    /// Position in Volume
    vec3 sampling_pos       = ray_entry_position + ray_increment;

    /// Init color of fragment
    vec4 out_col = vec4(0.0, 0.0, 0.0, 1.0);



    /// check if we are inside volume
    bool inside_volume = inside_volume_bounds(sampling_pos);
    
    if (!inside_volume)
        discard;

    // FragColor = vec4(1,0,0,1);
    // return;

    // float accum_opacity = 0.f;



#if MODE == 0

    float max_value = 0.f;

    // the traversal loop,
    // termination when the sampling position is outside volume boundary
    while (inside_volume)
    {      

        // assume cube is in 0 - 1 space anyway : todo - correct this!
        float sample_val = sample_volume(sampling_pos); 
        sample_val *= scale_factor;

        max_value = max (max_value, sample_val);

        // increment the ray sampling position
        sampling_pos  += ray_increment;

        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }

    out_col = vec4(max_value, max_value, max_value, 1.f);
#endif

#if MODE == 1

    float sum = 0.f;
    float samples = 0.f;

    // the traversal loop,
    // termination when the sampling position is outside volume boundary
    while (inside_volume)
    {      


        // assume cube is in 0 - 1 space anyway
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

#endif



#if MODE == 2

    float accum_opacity = 0.f;

    // the traversal loop,
    // termination when the sampling position is outside volume boundary
    while (inside_volume)
    {      


        // assume cube is in 0 - 1 space anyway
        float sample_val = sample_volume(sampling_pos); 

        vec3 intensity = vec3(sample_val, sample_val, sample_val);
        float opacity = 0.01f;

        intensity *= opacity;

        out_col.xyz += (intensity * (1.f - accum_opacity));

        accum_opacity = 1.f - (1.f - opacity) * (1.f - accum_opacity);

        // increment the ray sampling position
        sampling_pos  += ray_increment;

        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }

    out_col.w = accum_opacity;

#endif


    // return the calculated color value
    FragColor = out_col;
}

