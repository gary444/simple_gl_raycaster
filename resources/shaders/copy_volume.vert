#version 460 

uniform layout(binding=1, r8) image3D source_volume;
uniform layout(binding=2, r8) image3D dest_volume;

uniform uvec3 volume_dimensions;


uvec3 unflatten_vertex_id(uint one_d_index, uvec3 three_d_resolution) {


  uvec3 result = uvec3(0,0,0);
  result.z = one_d_index / (three_d_resolution.x * three_d_resolution.y);
  one_d_index -= result.z * three_d_resolution.x * three_d_resolution.y;
  result.y = one_d_index / three_d_resolution.x;
  result.x = one_d_index % three_d_resolution.x;

  return result;
}

void main()
{


	ivec3 voxel_3d_idx = ivec3(unflatten_vertex_id(gl_VertexID, volume_dimensions));

	vec4 src_val = imageLoad(source_volume, voxel_3d_idx);
	imageStore(dest_volume, voxel_3d_idx, src_val );

	// imageStore(source_volume, voxel_3d_idx, vec4(0) );

	return;
}

