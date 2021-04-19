#version 460 

uniform layout(binding=1, r8) image3D volume_texture;
uniform layout(binding=2, r8) image3D volume_texture_smoothed;


uniform uvec3 volume_dimensions;


uvec3 unflatten_vertex_id(uint one_d_index, uvec3 three_d_resolution) {


  uvec3 result = uvec3(0,0,0);
  result.z = one_d_index / (three_d_resolution.x * three_d_resolution.y);
  one_d_index -= result.z * three_d_resolution.x * three_d_resolution.y;
  result.y = one_d_index / three_d_resolution.x;
  result.x = one_d_index % three_d_resolution.x;

  return result;
}

const int K = 1;

void main()
{


	ivec3 voxel_3d_idx = ivec3(unflatten_vertex_id( uint(gl_VertexID), volume_dimensions));

  float sum = 0.f;
  float samples = 0.f;

  for (int z = -K; z <= K; ++z){
    for (int y = -K; y <= K; ++y){
      for (int x = -K; x <= K; ++x){
          
          ivec3 sample_idx = voxel_3d_idx + ivec3(x,y,z);

          if (sample_idx.x >= 0 && sample_idx.y >= 0 && sample_idx.z >= 0 && sample_idx.x < volume_dimensions.x && sample_idx.y < volume_dimensions.y && sample_idx.z < volume_dimensions.z) {

            sum += imageLoad(volume_texture, sample_idx).r;

            samples += 1.f;
          }

      }
    }
  }

  float result = sum / samples;

  imageStore(volume_texture_smoothed, voxel_3d_idx, vec4(result, 0.f,0.f,0.f) );

  // vec4 r = imageLoad(volume_texture, voxel_3d_idx );
  // imageStore(volume_texture_smoothed, voxel_3d_idx, r);
	


	return;
}

