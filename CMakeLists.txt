cmake_minimum_required(VERSION 3.0.0)
project(cuda-final-project VERSION 0.1.0)

find_package(CUDA 11.3 REQUIRED)

include(CTest)
enable_testing()

set(CMAKE_CUDA_COMPILER $ENV{CUDA_ROOT}/bin/nvcc)
enable_language(CUDA)


include_directories($ENV{CUDA_ROOT}/include $ENV{CUDA_ROOT}/include/cuda /usr/include)

link_directories($ENV{CUDA_ROOT}/lib64 /usr/lib/x86_64-linux-gnu)
link_libraries(libcudart.so libnppc.so libnppial.so libnppicc.so nppidei.so nppif.so nppig.so nppim.so nppist.so nppisu.so nppitc.so npps.so libfreeimage.so)

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# add_executable(main.exe src/image_filter.cpp)


