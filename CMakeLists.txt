############################################################
# CMake Build Script

############################################################
# Project settings
cmake_minimum_required(VERSION 3.9)
project(BasicGLRaycast LANGUAGES CXX)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinsizeRel."
        FORCE)
endif()

get_filename_component(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} ABSOLUTE)

# message(STATUS "Build type:     ${CMAKE_BUILD_TYPE}")
# message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

############################################################


set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)


############################################################
# Directories


list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/install)
set(THIRD_PARTY_PROJECTS_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(LAMURE_CONFIG_DIR ${CMAKE_BINARY_DIR})


# if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE STRING "Install path prefix, prepended onto install directories." FORCE)
# endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#matrix multiplication libs
# set(CUDA_MM_LIBRARIES "-lcublas -lcurand")



file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})

############################################################
# compiler flags

if(MSVC)
    set(PROJECT_COMPILE_FLAGS "-D_VARIADIC_MAX=10 -D _SCL_SECURE=0 -D _USE_MATH_DEFINES=1 /MP -D NOMINMAX")
    set(PROJECT_LIBS "")
elseif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(PROJECT_COMPILE_FLAGS "-std=c++14 -Wall -pthread -fPIC -fopenmp")
    set(PROJECT_LIBS "pthread")
endif()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROJECT_COMPILE_FLAGS}")
# set(CMAKE_CUDA_FLAGS "-std=c++14")
# SET(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++14")


set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")


################################
# FreeImage
################################
include (find_freeimage)

# IGL 

include_directories("~/Documents/libs/libigl/include/")


find_package(GLFW3 REQUIRED)
include(findGLFW3)
include(FindOpenGL)
include(FindGLEW)

################################
# GLUT
################################
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${GLOBAL_EXT_DIR}/freeglut/include)
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${GLOBAL_EXT_DIR}/freeglut/lib)
find_package(GLUT REQUIRED)


################################
# glfw
################################
find_package(GLFW3 REQUIRED)
include(findGLFW3)
include(FindOpenGL)
include(FindGLEW)

################################
# OpenGL
################################
if (UNIX)
  find_package(OpenGL REQUIRED)
  include_directories(${OpenGL_INCLUDE_DIRS})
  link_directories(${OpenGL_LIBRARY_DIRS})
  add_definitions(${OpenGL_DEFINITIONS})
endif ()



################################
# GLM
################################
include(find_glm)

#########
# Eigen
#########

find_package(Eigen3 3.2.0 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


#Flann
find_package (FLANN)
include_directories(SYSTEM ${FLANN_INCLUDE_DIRS} )




######
#Armadillo
######

include(FindArmadillo)


######
# Open CV
######



find_package(OpenCV REQUIRED opencv_optflow opencv_xfeatures2d)
# find_package(OpenCV REQUIRED PATHS "~/Documents/repos/OpenCV/opencv/build" )
# message("CV version: " ${OpenCV_VERSION})
# MESSAGE("Include dirs ${OpenCV_INCLUDE_DIRS}")
# MESSAGE("LINK LIBRARIES ${OpenCV_LIBS}")



set (CUDA_BIN_PATH "/opt/cuda/latest")

find_package(CUDA REQUIRED)
set(CUDA_HOST_COMPILER ${CMAKE_CXX_COMPILER})



file(GLOB_RECURSE HELPER_FILES ${PROJECT_SOURCE_DIR}/helpers/*)
file(GLOB_RECURSE GL_FILES ${PROJECT_SOURCE_DIR}/gl/*)
file(GLOB_RECURSE VOLUME_FILES ${PROJECT_SOURCE_DIR}/volume/*)

set(THIRD_PARTY_PROJECTS_DIR ${CMAKE_SOURCE_DIR}/third_party)

# message(${GL_FILES})

include_directories(${GLFW_INCLUDE_DIRS} ${THIRD_PARTY_PROJECTS_DIR} )


set(RAYCAST_APP_NAME "basic_gl_raycast")
add_executable(${RAYCAST_APP_NAME} ${RAYCAST_APP_NAME}.cpp ${HELPER_FILES} ${GL_FILES} ${VOLUME_FILES} )
target_link_libraries(${RAYCAST_APP_NAME} ${OpenCV_LIBS} )


target_link_libraries(${RAYCAST_APP_NAME}
        ${OPENGL_LIBRARY}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        glfw
        )

set(RAYCAST_APP_NAME "raycast_to_image")
add_executable(${RAYCAST_APP_NAME} ${RAYCAST_APP_NAME}.cpp ${HELPER_FILES} ${GL_FILES} ${VOLUME_FILES} )
target_link_libraries(${RAYCAST_APP_NAME} ${OpenCV_LIBS} )

# include_directories(${GLFW_INCLUDE_DIRS} )

target_link_libraries(${RAYCAST_APP_NAME}
        ${OPENGL_LIBRARY}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        glfw
        )


set(RAYCAST_APP_NAME "raycast_cfd_volume")
add_executable(${RAYCAST_APP_NAME} ${RAYCAST_APP_NAME}.cpp ${HELPER_FILES} ${GL_FILES} ${VOLUME_FILES} )
target_link_libraries(${RAYCAST_APP_NAME} ${OpenCV_LIBS} )
# include_directories(${GLFW_INCLUDE_DIRS} )
target_link_libraries(${RAYCAST_APP_NAME}
        ${OPENGL_LIBRARY}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        glfw
        )

set(RAYCAST_APP_NAME "raycast_cfd_volume_to_image")
add_executable(${RAYCAST_APP_NAME} ${RAYCAST_APP_NAME}.cpp ${HELPER_FILES} ${GL_FILES} ${VOLUME_FILES} )
target_link_libraries(${RAYCAST_APP_NAME} ${OpenCV_LIBS} )
# include_directories(${GLFW_INCLUDE_DIRS} )
target_link_libraries(${RAYCAST_APP_NAME}
        ${OPENGL_LIBRARY}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        glfw
        )

set(RAYCAST_APP_NAME "schlieren_raycast")
add_executable(${RAYCAST_APP_NAME} ${RAYCAST_APP_NAME}.cpp ${HELPER_FILES} ${GL_FILES} ${VOLUME_FILES} )
target_link_libraries(${RAYCAST_APP_NAME} ${OpenCV_LIBS} )
# include_directories(${GLFW_INCLUDE_DIRS} )
target_link_libraries(${RAYCAST_APP_NAME}
        ${OPENGL_LIBRARY}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        glfw
        )



# copy over shaders and models
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources 
        DESTINATION bin/
        FILES_MATCHING
        PATTERN "*.frag"
        PATTERN "*.vert"
        PATTERN "*.comp"
        PATTERN "*.geom"
        PATTERN "*.tess"
        PATTERN "*.incl"
        PATTERN "*.obj"
)

