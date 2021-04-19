#version 430 

#define MODE 4

uniform layout(binding=3, r32f) image2D target_image;
uniform layout(binding=0, r32f) image2D debug_image;

uniform float sampling_distance;

// rays travel from z=1 to z=0
uniform float z_slice_start = 0.51f;
uniform float z_slice_end   = 0.5f;

uniform uvec2 target_image_size;

// uint debug_image_size = min(target_image_size.x, target_image_size.y);
// uvec2 debug_image_dims    = uvec2(debug_image_size, debug_image_size);    
uvec2 debug_image_dims    = uvec2(1600, 1600);    

uniform sampler3D volume_texture;
uniform vec3    max_bounds;

uniform vec2 refractive_index_bounds;

const vec2 pixel_size = vec2(1.f, 1.f) / vec2(target_image_size); 


// Brownlee: The value k typically maps to the largest expected
// displacement as to yield normalized intensities without clamping 
const float k = 100.f;


// rays travel from z=1 to z=0
// const float z_slice_start = 0.51f;
// const float z_slice_end   = 0.5f;

float sample_volume (vec3 sampling_pos) {
    vec3 sample_coords = sampling_pos / max_bounds;
    float sample_val = texture(volume_texture, sample_coords).r;
    return sample_val;
}


bool inside_volume_bounds(const in vec3 sampling_position)
{
    return (   all(greaterThanEqual(sampling_position, vec3(0.0)))
            && all(lessThanEqual(sampling_position, max_bounds)));
}


float scale_refractive_index_from_normalised_value(const float val) {
	return refractive_index_bounds.x + val*(refractive_index_bounds.y - refractive_index_bounds.x);
}


// central differences
vec3 get_gradient_at_point (vec3 pnt){
	vec3 grad;
	vec3 pos_shift = pnt + sampling_distance;
	vec3 neg_shift = pnt - sampling_distance;

	grad.x = (sample_volume(vec3(pos_shift.x, pnt.y, pnt.z)) - sample_volume(vec3(neg_shift.x, pnt.y, pnt.z))) / (2*sampling_distance);
	grad.y = (sample_volume(vec3(pnt.x, pos_shift.y, pnt.z)) - sample_volume(vec3(pnt.x, neg_shift.y, pnt.z))) / (2*sampling_distance);
	grad.z = (sample_volume(vec3(pnt.x, pnt.y, pos_shift.z)) - sample_volume(vec3(pnt.x, pnt.y, neg_shift.z))) / (2*sampling_distance);

	return grad;
}




void main()
{

    // get 2D pixel index
    uint idx = uint (gl_VertexID);
    uvec2 idx_2d = uvec2( idx % target_image_size.x, idx / target_image_size.x);



#if MODE == 0
// test writing to texture ------------
    imageStore(target_image, ivec2(idx_2d), vec4(float(idx_2d.x) / target_image_size.x, 0.f, 0.f, 0.f));
#endif

// test ray casting in a straight line
#if MODE == 1
    // get ray start point in image space (without jitter)
    vec2 pos_in_image_space = vec2(idx_2d) * pixel_size;
    vec3 ray_entry_position = vec3(pos_in_image_space, 1.f) * max_bounds;
    // // rays will be cast along Z-axis from Z=[max z]
    vec3 ray_direction = vec3(0,0,-1);
    vec3 ray_increment      = ray_direction * sampling_distance;
    vec3 sampling_pos       = ray_entry_position + (ray_increment * 0.000001);

    /// check if we are inside volume
    bool inside_volume = inside_volume_bounds(sampling_pos);

    if (!inside_volume)
        return;
    
    while (inside_volume) {
        // increment the ray sampling position
        sampling_pos  += ray_increment;
        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }
    // after we have left the volume, write to image
    imageStore(target_image, ivec2(idx_2d), vec4(float(idx_2d.x) / target_image_size.x, 0.f, 0.f, 0.f));
#endif


// test maximum intensity projection in straight line
#if MODE == 2
    // get ray start point in image space (without jitter)
    vec2 pos_in_image_space = vec2(idx_2d) * pixel_size;
    vec3 ray_entry_position = vec3(pos_in_image_space, 1.f) * max_bounds;
    // // rays will be cast along Z-axis from Z=[max z]
    vec3 ray_direction = vec3(0,0,-1);
    vec3 ray_increment      = ray_direction * sampling_distance;
    vec3 sampling_pos       = ray_entry_position + (ray_increment * 0.000001);

    /// check if we are inside volume
    bool inside_volume = inside_volume_bounds(sampling_pos);

    if (!inside_volume)
        return;
    
    float max_val = 0.f;

    while (inside_volume) {

        float sample_val = sample_volume(sampling_pos); 

        max_val = max(max_val, sample_val); 

        // increment the ray sampling position
        sampling_pos  += ray_increment;
        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }
    // after we have left the volume, write to image
    imageStore(target_image, ivec2(idx_2d), vec4(max_val, 0.f, 0.f, 0.f));
#endif

// test average intensity projection in straight line
#if MODE == 3
    // get ray start point in image space (without jitter)
    vec2 pos_in_image_space = vec2(idx_2d) * pixel_size;
    vec3 ray_entry_position = vec3(pos_in_image_space, 1.f) * max_bounds;
    // // rays will be cast along Z-axis from Z=[max z]
    vec3 ray_direction = vec3(0,0,-1);
    vec3 ray_increment      = ray_direction * sampling_distance;
    vec3 sampling_pos       = ray_entry_position + (ray_increment * 0.000001);

    /// check if we are inside volume
    bool inside_volume = inside_volume_bounds(sampling_pos);

    if (!inside_volume)
        return;
    
    float sum = 0.f;
    float samples = 0.f;

    while (inside_volume) {

        float sample_val = sample_volume(sampling_pos); 

        sum += sample_val;
        samples += 1.f;

        // increment the ray sampling position
        sampling_pos  += ray_increment;
        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }

    if (samples == 0.f) return; 

    // after we have left the volume, write to image
    imageStore(target_image, ivec2(idx_2d), vec4(sum / samples, 0.f, 0.f, 0.f));
#endif


// schlieren raycasting
#if MODE == 4
    // get ray start point in image space (without jitter)
    vec2 pos_in_image_space = (vec2(idx_2d)+vec2(0.5f)) * pixel_size;
    vec3 ray_entry_position = vec3(pos_in_image_space, 1.f) * max_bounds;
    // // rays will be cast along Z-axis from Z=[max z]
    const vec3 initial_ray_direction = vec3(0,0,-1); 
    vec3 ray_direction = initial_ray_direction;

    vec3 sampling_pos       = ray_entry_position + (ray_direction * 0.000001);

    bool inside_volume = true;


    bool DEBUG = (idx_2d.y == (target_image_size.y * 0.5f)) && (idx_2d.x % 20 == 0);

    
    while (inside_volume)
    {      

        const float normalized_z = sampling_pos.z / max_bounds.z;

        if (normalized_z < z_slice_start && normalized_z > z_slice_end ){

            const float sample_val = sample_volume(sampling_pos); 
            const float refractive_index = scale_refractive_index_from_normalised_value(sample_val);
            const vec3  refractive_index_gradient = get_gradient_at_point(sampling_pos);

            // increment the ray sampling position (according to Brownlee eq. 7)
            sampling_pos += (ray_direction * sampling_distance / refractive_index);

            // adjust the ray direction (Brownlee eq 8)
            ray_direction += (sampling_distance * refractive_index_gradient);
            ray_direction = normalize(ray_direction);

        } else {
            sampling_pos += (ray_direction * sampling_distance);
        }

        if (DEBUG) {
            const uvec2 debug_tex_coords = uvec2((sampling_pos.xz / max_bounds.xz) * vec2(debug_image_dims) );
            imageStore(debug_image, ivec2(debug_tex_coords), vec4(1.f, 0.f, 0.f, 0.f));
        }

        // update the loop termination condition
        inside_volume  = inside_volume_bounds(sampling_pos);
    }


    // TODO find exact intersection with image plane (Z = 0)
    // for now just use the first sample that has Z <= 0 and is in image plane bounds
    if (sampling_pos.z <= 0.f
        && (sampling_pos.x >= 0.f && sampling_pos.x < max_bounds.x && sampling_pos.y >= 0.f && sampling_pos.y < max_bounds.y) )
    {

        // cutoff simulation
        // eq9 
        vec3 e = ray_direction - initial_ray_direction;
        // eq 10, 11
        // float e_x = dot(e, vec3(1,0,0)); 
        const float e_y = dot(e, vec3(0,1,0)); 
        // eq 13 
        const float I = clamp(0.5f - e_y * k, 0.f, 1.f); 
        // const float I = 0.25f;


        vec2 end_tex_coords = sampling_pos.xy / max_bounds.xy;
        uvec2 end_px_coords = uvec2(end_tex_coords * vec2(target_image_size));

        // load existing value
        // TODO use atomic operations?
        const float existing_I = imageLoad(target_image, ivec2(end_px_coords)).r;
        
        // TODO contribute to multiple pixels
        imageStore(target_image, ivec2(end_px_coords), vec4(existing_I + I, 0.f, 0.f, 0.f));
    }
#endif


}

