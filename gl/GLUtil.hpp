#ifndef GLUTIL_HPP
#define GLUTIL_HPP

#include <string>


namespace grt {
namespace gl {


void download_default_framebuffer_to_image(const uint32_t WIDTH, const uint32_t HEIGHT, const std::string& outpath);

}
}

#endif