message(STATUS "Enter src cmake file")

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

add_library(compiler STATIC ${sources})
