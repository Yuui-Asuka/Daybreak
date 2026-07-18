cmake_minimum_required(VERSION 3.20)

project(Daybreak)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Vulkan_INCLUDE_DIR "D:/vs/sdk/Include")
set(Vulkan_LIBRARY "D:/vs/sdk/Lib/vulkan-1.lib")

find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glad CONFIG REQUIRED)

add_subdirectory(Engine)
add_subdirectory(Sandbox)