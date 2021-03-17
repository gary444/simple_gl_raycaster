#version 430 


// 0: average intensity projection
// 1: check for any non-zero value
// 2: schlieren raycast
#define MODE 1


uniform layout(binding=0, r32f) image2D target_image;

uniform uvec2 target_image_size;

uniform sampler3D volume_texture;
uniform vec3    max_bounds;

uniform vec2 refractive_index_bounds;



// const float sampling_distance = 0.001f;


// float sample_volume (vec3 sampling_pos) {
//     float sample_val = texture(volume_texture, sampling_pos).r;
//     return sample_val;
// }


// bool inside_volume_bounds(const in vec3 sampling_position)
// {
//     return (   all(greaterThanEqual(sampling_position, vec3(0.0)))
//             && all(lessThanEqual(sampling_position, max_bounds )));
// }


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

    uint idx = uint (gl_VertexID);


    // // get 2D pixel index
    uvec2 idx_2d = uvec2( idx % target_image_size.x, idx / target_image_size.x);

    imageStore(target_image, ivec2(idx_2d), vec4(float(idx_2d.x) / target_image_size.x, 0.f, 0.f, 0.f));


    // // get ray start point (without jitter)
    //             vec3 ray_entry_position; // TODO continue


    // // rays will be cast along X-axis from X=0
    // vec3 ray_direction = vec3(1,0,0);
    // // so starting point of ray is (0, tex.y, tex.x)
    // // vec3 ray_entry_position = vec3(0,pass_TexCoord.y, pass_TexCoord.x);

    // /// One step trough the volume
    // vec3 ray_increment      = ray_direction * sampling_distance;
    // // Position in Volume
    // vec3 sampling_pos       = ray_entry_position + ray_increment;

    // /// Init color of fragment
    // vec4 out_col = vec4(0.0, 0.0, 0.0, 1.0);

    // /// check if we are inside volume
    // bool inside_volume = inside_volume_bounds(sampling_pos);
    
    // // if (!inside_volume)
    //     discard;



// #if MODE == 0

//     float sum = 0.f;
//     float samples = 0.f;
//     // the traversal loop,
//     // termination when the sampling position is outside volume boundary
//     while (inside_volume)
//     {      
//         float sample_val = sample_volume(sampling_pos); 

//         sum += sample_val;
//         samples += 1.f;
//         // increment the ray sampling position
//         sampling_pos  += ray_increment;
//         // update the loop termination condition
//         inside_volume  = inside_volume_bounds(sampling_pos);
//     }

//     sum /= samples;
// #endif 

// #if MODE == 1


//     while (inside_volume)
//     {      
//         float sample_val = sample_volume(sampling_pos); 

//         if (sample_val != 0.f) {
//         	// FragColor = vec4(vec3(sample_val),1.f);
//         	return;
//         }

//         // increment the ray sampling position
//         sampling_pos  += ray_increment;
//         // update the loop termination condition
//         inside_volume  = inside_volume_bounds(sampling_pos);
//     }

// #endif


// #if MODE == 2


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

// #endif

}

