#version 430 


uniform layout(binding=0, r32f) image2D target_image;

uniform uvec2 target_image_size;

uniform sampler3D volume_texture;
uniform vec3    max_bounds;

uniform vec2 refractive_index_bounds;

const vec2 pixel_size = vec2(1.f, 1.f) / vec2(target_image_size); 


const float sampling_distance = 0.01f;


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


// float scale_refractive_index_from_normalised_value(const float val) {
// 	return refractive_index_bounds.x + val*(refractive_index_bounds.y - refractive_index_bounds.x);
// }


// // central differences
// vec3 get_gradient_at_point (vec3 pnt){
// 	vec3 grad;
// 	vec3 pos_shift = pnt + sampling_distance;
// 	vec3 neg_shift = pnt - sampling_distance;

// 	grad.x = (sample_volume(vec3(pos_shift.x, pnt.y, pnt.z)) - sample_volume(vec3(neg_shift.x, pnt.y, pnt.z))) / (2*sampling_distance);
// 	grad.y = (sample_volume(vec3(pnt.x, pos_shift.y, pnt.z)) - sample_volume(vec3(pnt.x, neg_shift.y, pnt.z))) / (2*sampling_distance);
// 	grad.z = (sample_volume(vec3(pnt.x, pnt.y, pos_shift.z)) - sample_volume(vec3(pnt.x, pnt.y, neg_shift.z))) / (2*sampling_distance);

// 	return grad;
// }




void main()
{

    // get 2D pixel index
    uint idx = uint (gl_VertexID);
    uvec2 idx_2d = uvec2( idx % target_image_size.x, idx / target_image_size.x);



// test writing to texture ------------
    // imageStore(target_image, ivec2(idx_2d), vec4(float(idx_2d.x) / target_image_size.x, 0.f, 0.f, 0.f));


// test ray casting in a straight line
    // // get ray start point in image space (without jitter)
    // vec2 pos_in_image_space = vec2(idx_2d) * pixel_size;
    // vec3 ray_entry_position = vec3(pos_in_image_space, 1.f) * max_bounds;
    // // // rays will be cast along Z-axis from Z=[max z]
    // vec3 ray_direction = vec3(0,0,-1);
    // vec3 ray_increment      = ray_direction * sampling_distance;
    // vec3 sampling_pos       = ray_entry_position + (ray_increment * 0.000001);

    // /// check if we are inside volume
    // bool inside_volume = inside_volume_bounds(sampling_pos);

    // if (!inside_volume)
    //     return;
    
    // while (inside_volume) {
    //     // increment the ray sampling position
    //     sampling_pos  += ray_increment;
    //     // update the loop termination condition
    //     inside_volume  = inside_volume_bounds(sampling_pos);
    // }
    // // after we have left the volume, write to image
    // imageStore(target_image, ivec2(idx_2d), vec4(float(idx_2d.x) / target_image_size.x, 0.f, 0.f, 0.f));





// test maximum intensity projection in straight line
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





    /// Init color of fragment
    vec4 out_col = vec4(0.0, 0.0, 0.0, 1.0);





//     while (inside_volume)
//     {      
//         const float sample_val = sample_volume(sampling_pos); 
//         const float refractive_index = scale_refractive_index_from_normalised_value(sample_val);
//         const vec3  refractive_index_gradient = get_gradient_at_point(sampling_pos);

//         // increment the ray sampling position (according to Brownlee eq. 7)
//         sampling_pos += (ray_direction * sampling_distance / refractive_index);

//         // adjust the ray direction (Brownlee eq 8)
//         ray_direction += sampling_distance * refractive_index_gradient;

//         // update the loop termination condition
//         inside_volume  = inside_volume_bounds(sampling_pos);
//     }



}

