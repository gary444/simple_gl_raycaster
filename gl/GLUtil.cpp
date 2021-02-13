
#include <iostream>
#include <vector>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stbi/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include <stbi/stb_image_write.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "GLUtil.hpp"

namespace grt {
namespace gl {

void download_default_framebuffer_to_image(const uint32_t WIDTH, const uint32_t HEIGHT, const std::string& outpath){
	std::cout << "Downloading framebuffer to " << outpath << std::endl;

	// download pixel data
	std::vector<uint8_t> img_data_flipped (WIDTH * HEIGHT * 3);
	glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, img_data_flipped.data() );

	// flip image
	std::vector<uint8_t> image_data (img_data_flipped.size());
	const uint32_t num_pixels = WIDTH * HEIGHT;

	for (uint32_t px = 0; px < num_pixels; ++px){
		const uint32_t x = px % WIDTH;	
		const uint32_t y = px / WIDTH;

		const uint32_t y_flip = HEIGHT - 1 - y;


		const uint32_t px_unflipped = x + y_flip*WIDTH;

		image_data[px*3+0] = img_data_flipped[px_unflipped*3+0];
		image_data[px*3+1] = img_data_flipped[px_unflipped*3+1];
		image_data[px*3+2] = img_data_flipped[px_unflipped*3+2];
	}

	// save to image
	stbi_write_png(outpath.c_str(), WIDTH, HEIGHT, 3, image_data.data(), WIDTH*3);

	return;
}




}
}