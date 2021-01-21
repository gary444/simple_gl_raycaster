#version 460 core
layout (location = 0) in vec3 vol_space_position;

uniform int layer;

uniform sampler3D[4] cv_xyz;
uniform sampler3D[4] cv_xyz_inv;
uniform sampler2DArray key_frame_depth;
uniform sampler2DArray optical_flow;

uniform mat4 mat_inv_vol_to_world;

uniform uvec2 of_field_dims;

out vec3 pass_color;

const float min_depth = 0.5f;
const float max_depth = 3.f;

// const float MAX_VOL_SPACE_CORR_DISTANCE = 0.01;


const float MAX_NORMED_DEPTH_DISPARITY = 0.02;



float normalize_depth(float depth) {
  return (depth - min_depth)/(max_depth - min_depth);
}

bool is_valid(float depth) {
  return depth > min_depth && depth < max_depth;
}


#define SHOW_ED_NODE_POSITION_AFTER_OF

void main()

{
    pass_color = vec3(1,1,1);

#ifndef SHOW_ED_NODE_POSITION_AFTER_OF


	// convert 3d vol space to (depth & OF) UV space
	vec3 ed_depth_space = texture(cv_xyz_inv[layer], vol_space_position).xyz;

	// get actual position of surface at this pixel
	// get depth at this UV in the key frame depth image
	float key_depth = texture(key_frame_depth, vec3(ed_depth_space.xy, layer) ).r;
	float normed_key_depth = normalize_depth(key_depth);
	
	vec3 key_pos_world = texture(cv_xyz[layer], ed_depth_space).xyz;
	vec3 key_pos_vol_space = (mat_inv_vol_to_world * vec4(key_pos_world, 1.0)).xyz;



	if (!is_valid(key_depth) || abs(normed_key_depth - ed_depth_space.z) > MAX_NORMED_DEPTH_DISPARITY){

    	pass_color = vec3(0,0,1);
		
	}


	gl_Position = vec4(ed_depth_space.xy * 2.f - 1.f , 1.0, 1.0);

#else 


	vec3 accum_translate = vec3 (0,0,0);
	float samples = 0.f;


	// get UV coordinate for ed node (ed node already in volume space)
	vec3 ed_depth_space = texture(cv_xyz_inv[layer], vol_space_position).xyz;

	// get depth at this UV in the key frame depth image
	const float key_depth = texture(key_frame_depth, vec3(ed_depth_space.xy, layer) ).r;
	const float normed_key_depth = normalize_depth(key_depth);
	
	// check depth is valid and close to ED node's depth
	if (!is_valid(key_depth) || abs(normed_key_depth - ed_depth_space.z) > MAX_NORMED_DEPTH_DISPARITY){
		pass_color = vec3(0,0,1);
	}

	// check that depth sample is not too close to a gradient
	// if (texture(depth_gradient_local_region_unit, vec3(ed_depth_space.xy, layer)).r > MAX_LOCAL_DEPTH_GRADIENT){
	// 	continue;
	// }

	// get optical flow - same texture coord as ED node
	const vec2 opt_flow = texture(optical_flow, vec3(ed_depth_space.xy, layer) ).rg;

	// filter small OF values
	// if (length(opt_flow) < MIN_OPTICAL_FLOW_THRESHOLD){
	// 	continue;
	// }

	// get OF end point as a UV
	// convert pixel difference to UV difference
	const vec2 uv_opt_flow = opt_flow / vec2(of_field_dims);
	const vec2 target_frame_lookup_uv = ed_depth_space.xy + uv_opt_flow;

	gl_Position = vec4(target_frame_lookup_uv.xy * 2.f - 1.f , 1.0, 1.0);


	// const float tgt_depth = texture(tgt_frame_depth, vec3(target_frame_lookup_uv, layer)).r;

	// if (!is_valid(tgt_depth)){
	// 	continue;
	// }

	// // at the point we can find the volume space position of the start and end of the translation
	// const vec3 tgt_pos_world = texture(cv_xyz[layer], vec3(target_frame_lookup_uv, normalize_depth(tgt_depth))).xyz;
	// const vec3 tgt_pos_vol_space = (mat_inv_vol_to_world * vec4(tgt_pos_world, 1.0)).xyz;

	// const vec3 key_pos_world = texture(cv_xyz[layer], ed_depth_space).xyz;
	// const vec3 key_pos_vol_space = (mat_inv_vol_to_world * vec4(key_pos_world, 1.0)).xyz;



	// if (distance(ed_node_pos_vol_space, tgt_pos_vol_space) > MAX_VOL_SPACE_CORR_DISTANCE){
	// 	continue;
	// }

	// // accumulate translate 
	// // translate is the vector from key vol position of depth sample ED's depth sample, to target position after optical flow 
	// accum_translate += (tgt_pos_vol_space - key_pos_vol_space);
	// samples += 1.f;

	// // calculate average translation and write to buffer
	// if (samples < 0.5f){
	// 	 // signifies no translate
	// 	// of_corr_ssbo[ed_id*3+0] = -2.f;
	// 	of_corr_ssbo[ed_id*3+0] = 0.f;
	// 	of_corr_ssbo[ed_id*3+1] = 0.f;
	// 	of_corr_ssbo[ed_id*3+2] = 0.f;
	// } else {
	// 	accum_translate /= samples;
	// 	of_corr_ssbo[ed_id*3+0] = accum_translate.x;
	// 	of_corr_ssbo[ed_id*3+1] = accum_translate.y;
	// 	of_corr_ssbo[ed_id*3+2] = accum_translate.z;	
	// }



#endif

}
