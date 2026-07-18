# Engine/CMakeLists.txt

add_library(Engine STATIC
    Core/Application.cpp
    Renderer/Renderer.cpp
    Renderer/vulkan/VulkanContext.cpp
    Window/Window.cpp
    Renderer/Mesh/Vertex.cpp
    Renderer/Mesh/Mesh.cpp
    Renderer/opengl/OpenglRenderer.cpp
    Renderer/vulkan/VulkanDevice.cpp
    Renderer/vulkan/VulkanRenderPass.cpp
    Renderer/vulkan/VulkanShader.cpp
    Renderer/vulkan/VulkanSwapchain.cpp
    Renderer/vulkan/VulkanFrameBuffer.cpp
    Renderer/vulkan/VulkanPipeline.cpp
    Renderer/vulkan/VulkanCommandBuffer.cpp
    Renderer/vulkan/VulkanCommandPool.cpp
    Renderer/vulkan/VulkanSync.cpp
    Renderer/vulkan/VulkanBuffer.cpp
 "Window/Window.h" "Window/Window.cpp" "Renderer/opengl/OpenglRenderer.cpp" "Renderer/opengl/OpenglRenderer.h" "Renderer/vulkan/VulkanDevice.cpp" "Renderer/vulkan/VulkanDevice.h" "Renderer/vulkan/VulkanSwapchain.cpp" "Renderer/vulkan/VulkanSwapchain.h" "Renderer/vulkan/VulkanRenderPass.cpp" "Renderer/vulkan/VulkanRenderPass.h" "Renderer/vulkan/VulkanPipeline.h" "Renderer/vulkan/VulkanCommandPool.cpp" "Renderer/vulkan/VulkanCommandPool.h" "Renderer/vulkan/VulkanCommandBuffer.cpp" "Renderer/vulkan/VulkanCommandBuffer.h" "Renderer/vulkan/VulkanSync.cpp" "Renderer/vulkan/VulkanSync.h" "Renderer/vulkan/utils/VulkanUtils.h" "Renderer/vulkan/VulkanShader.h"  "Renderer/vulkan/VulkanFrameBuffer.h" "Renderer/vulkan/VulkanFrameBuffer.cpp" "Renderer/Camera/Camera.cpp" "Renderer/Camera/Camera.h" "Renderer/Mesh/Vertex.h" "Renderer/vulkan/VulkanBuffer.h" "Renderer/vulkan/VulkanBuffer.cpp" "Renderer/Mesh/Vertex.cpp")

# 对外公开头文件目录
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接第三方库
target_link_libraries(Engine PUBLIC
    glfw
    Vulkan::Vulkan
    glad::glad
)