#ifndef FLUXIONS_VULKAN_HPP
#define FLUXIONS_VULKAN_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <fluxions_gte_base.hpp>
extern "C" {
#include <vulkan/vulkan.h>	
#include <SDL2/SDL_vulkan.h>
}

////////////////////////////////////////////////////////////////////////////////
// INTERFACE ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


namespace Fluxions {
	class VulkanContext {
	public:
		VulkanContext();
		~VulkanContext();

		void setName(std::string&& name) { name_ = name; }
		void setWindowTitle(std::string&& windowTitle) { title_ = windowTitle; }

		bool init() noexcept(false);
		void kill();

		bool beginFrame();
		void clearScreen(_Color4f color4);
		void swapBuffers();

		static constexpr uint32_t MIN_FRAMES_IN_QUEUE = 2;
		static constexpr uint32_t MAX_FRAMES_IN_QUEUE = 4;

		VkSurfaceKHR surface() { return surface_; }

	private:
		std::string name_;
		std::string title_;
		uint32_t width_{ 1280 };
		uint32_t height_{ 720 };
		SDL_Window* window_{ nullptr };

		std::vector<const char*> extensions_{
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME, // This extension allows us to receive debug messages
			VK_KHR_SURFACE_EXTENSION_NAME		// This extension allows us to use KHR Surfaces
		};
		VkInstance instance_{ nullptr };
		std::vector<VkPhysicalDevice> physicalDevices_;
		VkPhysicalDevice pd_{ nullptr };
		VkPhysicalDeviceProperties pdProperties_;
		VkPhysicalDeviceMemoryProperties pdMemoryProperties_;
		std::vector<VkQueueFamilyProperties> pdQueueFamilyProperties_;
		VkDevice device_{ nullptr };
		VkQueue queue_{ nullptr };

		VkFormat renderPassImageFormat_{ VK_FORMAT_B8G8R8A8_SRGB };
		VkRenderPass renderPass_{ nullptr };
		VkCommandPool commandPool_{ nullptr };
		VkSemaphore semaphore_{ nullptr };

		struct ImageBuffer {
			VkImage image_{ nullptr };
			VkImageView view_{ nullptr };
			VkFramebuffer framebuffer_{ nullptr };
			VkFence fence_{ nullptr };
			VkCommandBuffer commandBuffer_{ nullptr };
		};
		std::vector<VkSurfaceFormatKHR> surfaceFormats_;
		VkSurfaceKHR surface_{ nullptr };
		VkSwapchainKHR swapchain_{ nullptr };
		std::vector<VkPresentModeKHR> presentModes_;
		VkPresentModeKHR presentMode_{ VK_PRESENT_MODE_MAILBOX_KHR };
		uint32_t swapchainImageCount_{ 0 };
		std::vector<VkImage> swapchainImages_;
		std::vector<ImageBuffer> swapchainFramebuffers_;


		uint32_t frameImageIndex_{ 0 };

		bool _createSDLWindow();
		// report on SDL extensions if applicable
		bool _getSDLExtensions();
		// returns true if Vulkan instance created
		bool _createInstance();
		// returns number of devices
		bool _enumerateDevices();
		// returns number of devices with a graphics queue
		bool _checkQueueFamilyProperties();
		// returns true if device is created
		bool _createDevice();
		// returns true if Surface created
		bool _createSurface();
		// returns true if good image format found
		bool _chooseSurfaceFormat();
		// returns true if render pass created
		bool _createRenderPass();
		// returns true if command pool created
		bool _createCommandPool();
		// returns true if semaphore created
		bool _createSemaphore();
		// returns true if swap chain created
		bool _createSwapChain();
		// create a framebuffer image for the swap chain
		void _initImageBuffer(uint32_t i, VkImage image);
	};
} // namespace Fluxions


#endif
