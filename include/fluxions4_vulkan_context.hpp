#ifndef FLUXIONS_VULKAN_HPP
#define FLUXIONS_VULKAN_HPP

#include <fluxions4_stdcxx.hpp>
#include <fluxions4_gte_base.hpp>
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

		void setClearColor(FxColor4f color4) { memcpy(&clearValues[0].color, &color4, sizeof(FxColor4f)); }
		void setClearDepthStencil(float depth, uint32_t stencil) { clearValues[1].depthStencil.depth = depth, clearValues[1].depthStencil.stencil = stencil; }
		bool beginFrame();
		void presentFrame();

		VkSurfaceKHR surface() { return surface_; }
		VkDevice device() { return device_; }
		VkQueue queue() { return queue_; }
		VkRenderPass renderPass() { return renderPass_; }
		VkSemaphore semaphore() { return semaphore_; }
		uint32_t width() const { return width_; }
		uint32_t height() const { return height_; }
		float aspectRatio() const { return (float)width_ / (float)height_; }
		uint32_t maxSets() const { return static_cast<uint32_t>(swapchainFramebuffers_.size()); }

		VkFence& fence() { return swapchainFramebuffers_[frameImageIndex_].fence_; }
		VkCommandBuffer& commandBuffer() { return swapchainFramebuffers_[frameImageIndex_].commandBuffer_; }
		VkFramebuffer& framebuffer() { return swapchainFramebuffers_[frameImageIndex_].framebuffer_; }

		uint32_t findMemoryTypeIndex(uint32_t allowedMemoryTypeBits, uint32_t desiredBits) const;
		VkFormatProperties getFormatProperties(VkFormat format);

		uint32_t majorVersion() const { return VK_VERSION_MAJOR(version_); }
		uint32_t minorVersion() const { return VK_VERSION_MINOR(version_); }
		uint32_t patchVersion() const { return VK_VERSION_PATCH(version_); }

		// Static member to get recently created context
		// - returns nullptr if not initialized
		static VulkanContext* GetContext() { return recentlyUsedContext_; }
		
		// Static member to get device of recently created context
		// - returns nullptr if device not available
		static VkDevice GetDevice() {
			if (recentlyUsedContext_)
				return recentlyUsedContext_->device();
			return nullptr;
		}

		// Static member to get device of recently created context
		// - returns nullptr if device not available
		static VkCommandBuffer GetCommandBuffer() {
			if (recentlyUsedContext_)
				return recentlyUsedContext_->commandBuffer();
			return nullptr;
		}

	private:
		static VulkanContext* recentlyUsedContext_;
		std::string name_;
		std::string title_;
		uint32_t width_{ 1280 };
		uint32_t height_{ 720 };
		SDL_Window* window_{ nullptr };
		uint32_t version_{ 0 };
		VkClearValue clearValues[2] = { { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } };

		std::vector<const char*> extensions_{
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME, // This extension allows us to receive debug messages
			VK_KHR_SURFACE_EXTENSION_NAME,		// This extension allows us to use KHR Surfaces
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
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

		VkFormat depthBufferFormat_{ VK_FORMAT_D32_SFLOAT };
		VkImage depthBufferImage_;
		VkImageView depthBufferImageView_;
		VkDeviceMemory depthBufferDeviceMemory_;

		//VulkanImage depthBufferImage;

		struct SwapChainImageBuffer {
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
		std::vector<SwapChainImageBuffer> swapchainFramebuffers_;

		// Validation Layer Support
#ifdef NDEBUG
		static constexpr bool useValidationLayers = false;
		static constexpr bool useDebugCallback = false;
#else
		static constexpr bool useValidationLayers = true;
		static constexpr bool useDebugCallback = true;
#endif
		std::vector<VkLayerProperties> availableLayerProperties;
		VkLayerProperties* validationLayerProperties{ nullptr };
		VkDebugUtilsMessengerEXT debugUtilsMessenger{ nullptr };

		static constexpr uint32_t MIN_FRAMES_IN_QUEUE = 2;
		static constexpr uint32_t MAX_FRAMES_IN_QUEUE = 4;
		uint32_t frameImageIndex_{ 0 };

		bool _handleError(VkResult result);

		bool _createSDLWindow();
		// report on SDL extensions if applicable
		bool _getSDLExtensions();
		// signal to use validation layer
		bool _useValidationLayer();
		// returns true if Vulkan instance created
		bool _createInstance();
		// returns number of devices
		bool _enumerateDevices();
		// returns number of devices with a graphics queue
		bool _checkQueueFamilyProperties();
		// returns true if device is created
		bool _createDevice();
		// returns true if Surface created
		bool _createSDLVulkanSurface();
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
