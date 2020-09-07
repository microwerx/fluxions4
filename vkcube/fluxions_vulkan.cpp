#include <hatchetfish.hpp>
#include "fluxions_vulkan.hpp"


////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace Fluxions {

	VulkanContext::VulkanContext() {}


	VulkanContext::~VulkanContext() {}


	bool VulkanContext::init() {
		try {
			if (!_createSDLWindow()) return false;
			if (!_getSDLExtensions()) return false;
			if (!_createInstance()) return false;
			if (!_enumerateDevices()) return false;
			if (!_checkQueueFamilyProperties()) return false;
			if (!_createDevice()) return false;
			if (!_chooseSurfaceFormat()) return false;
			if (!_createRenderPass()) return false;
			if (!_createCommandPool()) return false;
			if (!_createSemaphore()) return false;
			if (!_createSwapChain()) return false;
		}
		catch (const std::exception& e) {
			HFLOGERROR("failed to initialize Vulkan context: '%s'", e.what());
			return false;
		}
		return true;
	}


	void VulkanContext::kill() {}


	bool VulkanContext::beginFrame() {
		VkResult result =
			vkAcquireNextImageKHR(device_, swapchain_, UINT64_MAX, semaphore_, VK_NULL_HANDLE, &frameImageIndex_);
		return (result == VK_SUCCESS) && (frameImageIndex_ < MAX_FRAMES_IN_QUEUE);
	}


	void VulkanContext::swapBuffers() {
		VkResult result;
		vkQueuePresentKHR(
			queue_,
			&(VkPresentInfoKHR){
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.swapchainCount = 1,
				.pSwapchains =
					(VkSwapchainKHR[]){
						swapchain_,
					},
				.pImageIndices =
					(uint32_t[]){
						frameImageIndex_,
					},
				.pResults = &result,
			});
		if (result != VK_SUCCESS) return;

		vkQueueWaitIdle(queue_);
	}


	////////////////////////////////////////////////////////////////////////////
	// PRIVATE IMPLEMENTATION //////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////


	bool VulkanContext::_createSDLWindow() {
		window_ = SDL_CreateWindow(
			title_.c_str(),
			::SDL_WINDOWPOS_CENTERED, // center horizontally
			::SDL_WINDOWPOS_CENTERED, // center vertically
			width_,
			height_,
			SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
	}

	bool VulkanContext::_getSDLExtensions() {
		uint32_t count;
		if (!SDL_Vulkan_GetInstanceExtensions(window_, &count, NULL)) { throw std::runtime_error(SDL_GetError()); }

		size_t extension_count = extensions_.size();
		extensions_.resize(extension_count + count);
		if (!SDL_Vulkan_GetInstanceExtensions(window_, &count, extensions_.data() + count)) {
			throw std::runtime_error(SDL_GetError());
		}
	}


	bool VulkanContext::_createInstance() {
		const char* extension = nullptr;
		vkCreateInstance(
			&(VkInstanceCreateInfo){
				.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				.pApplicationInfo =
					&(VkApplicationInfo){
						.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
						.pApplicationName = name_.c_str(),
						.apiVersion = VK_MAKE_VERSION(1, 0, 2),
					},
				.enabledExtensionCount = static_cast<uint32_t>(extensions_.size()),
				.ppEnabledExtensionNames = extensions_.data(),
			},
			NULL,
			&instance_);
		return instance_ != nullptr;
	}


	bool VulkanContext::_enumerateDevices() {
		uint32_t count;
		vkEnumeratePhysicalDevices(instance_, &count, NULL);
		physicalDevices_.resize(count);
		HFLOGINFO("found %d physical devices", count);
		if (!count) { throw std::runtime_error("no physical devices found"); }

		vkEnumeratePhysicalDevices(instance_, &count, &physicalDevices_[0]);
		pd_ = physicalDevices_[0];
		vkGetPhysicalDeviceProperties(pd_, &pdProperties_);
		vkGetPhysicalDeviceMemoryProperties(pd_, &pdMemoryProperties_);
		HFLOGINFO("vendor id:    %04x", pdProperties_.vendorID);
		HFLOGINFO("vendor name:  %s", pdProperties_.deviceName);
		HFLOGINFO("memory heaps: %d", pdMemoryProperties_.memoryHeapCount);
		HFLOGINFO("memory types: %d", pdMemoryProperties_.memoryTypeCount);
		return true;
	}


	bool VulkanContext::_checkQueueFamilyProperties() {
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(pd_, &count, NULL);
		if (count) {
			pdQueueFamilyProperties_.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(pd_, &count, &pdQueueFamilyProperties_[0]);
			if (!(pdQueueFamilyProperties_[0].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				throw std::runtime_error("VK_QUEUE_GRAPHICS_BIT not set");
			}
		}
		return count != 0;
	}


	bool VulkanContext::_createDevice() {
		vkCreateDevice(
			pd_,
			&(VkDeviceCreateInfo){
				.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				.queueCreateInfoCount = 1,
				.pQueueCreateInfos =
					&(VkDeviceQueueCreateInfo){
						.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
						.queueFamilyIndex = 0,
						.queueCount = 1,
						.pQueuePriorities = (float[]){ 1.0f },
					},
				.enabledExtensionCount = 1,
				.ppEnabledExtensionNames =
					(const char* const[]){
						VK_KHR_SWAPCHAIN_EXTENSION_NAME,
					},
			},
			NULL,
			&device_);

		if (!device_) return false;

		vkGetDeviceQueue(device_, 0, 0, &queue_);

		return queue_ != nullptr;
	}


	bool VulkanContext::_createSurface() {
		if (!SDL_Vulkan_CreateSurface(window_, instance_, &surface_)) {
			// surface not creted
			throw std::runtime_error(SDL_GetError());
		}
		return true;
	}


	bool VulkanContext::_chooseSurfaceFormat() {
		uint32_t numSurfaceFormats = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(pd_, surface_, &numSurfaceFormats, NULL);
		if (!numSurfaceFormats) { throw std::runtime_error("physical device surface formats not found"); }

		surfaceFormats_.resize(numSurfaceFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(pd_, surface_, &numSurfaceFormats, &surfaceFormats_[0]);

		VkFormat format = VK_FORMAT_UNDEFINED;
		for (int i = 0; i < numSurfaceFormats; i++) {
			switch (surfaceFormats_[i].format) {
			case VK_FORMAT_R8G8B8A8_SRGB:
			case VK_FORMAT_B8G8R8A8_SRGB:
				// RGBA 32-bit color OK
				// BGRA 32-bit color OK
				format = surfaceFormats_[i].format;
				break;
			case VK_FORMAT_R8G8B8_SRGB:
			case VK_FORMAT_B8G8R8_SRGB:
			case VK_FORMAT_R5G6B5_UNORM_PACK16:
			case VK_FORMAT_B5G6R5_UNORM_PACK16:
				// Non 32-bit formats are questionable, so let's avoid for now
			default:
				continue;
			}
		}

		if (format == VK_FORMAT_UNDEFINED) { throw std::runtime_error("surface image format undefined"); }
		renderPassImageFormat_ = format;
		return true;
	}


	bool VulkanContext::_createRenderPass() {
		vkCreateRenderPass(
			device_,
			&(VkRenderPassCreateInfo){
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				.attachmentCount = 1,
				.pAttachments = (VkAttachmentDescription[]){ {
					.format = renderPassImageFormat_,
					.samples = VK_SAMPLE_COUNT_1_BIT,
					.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
					.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
					.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
					.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				} },
				.subpassCount = 1,
				.pSubpasses = (VkSubpassDescription[]){ {
					.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
					.inputAttachmentCount = 0,
					.colorAttachmentCount = 1,
					.pColorAttachments =
						(VkAttachmentReference[]){
							{ .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
					.pResolveAttachments =
						(VkAttachmentReference[]){ { .attachment = VK_ATTACHMENT_UNUSED,
													 .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
					.pDepthStencilAttachment = NULL,
					.preserveAttachmentCount = 0,
					.pPreserveAttachments = NULL,
				} },
				.dependencyCount = 0 },
			NULL,
			&renderPass_);
		return renderPass_ != nullptr;
	}


	bool VulkanContext::_createCommandPool() {
		vkCreateCommandPool(
			device_,
			&(const VkCommandPoolCreateInfo){ .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
											  .queueFamilyIndex = 0,
											  .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT },
			NULL,
			&commandPool_);
		return commandPool_ != nullptr;
	}


	bool VulkanContext::_createSemaphore() {
		vkCreateSemaphore(
			device_,
			&(VkSemaphoreCreateInfo){
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			},
			NULL,
			&semaphore_);
		return semaphore_ != nullptr;
	}


	bool VulkanContext::_createSwapChain() {
		VkSurfaceCapabilitiesKHR surface_caps;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pd_, surface_, &surface_caps);
		if (!(surface_caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR))
			throw std::runtime_error("Surface capabilities does not support VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR");

		VkBool32 supported;
		vkGetPhysicalDeviceSurfaceSupportKHR(pd_, 0, surface_, &supported);
		if (!supported) throw std::runtime_error("physical device surface is not supported");

		uint32_t count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(pd_, surface_, &count, NULL);
		presentModes_.resize(count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(pd_, surface_, &count, &presentModes_[0]);

		presentMode_ = VK_PRESENT_MODE_MAILBOX_KHR;
		for (auto& pm : presentModes_) {
			if (pm == VK_PRESENT_MODE_FIFO_KHR) {
				presentMode_ = pm;
				break;
			}
		}

		uint32_t minImageCount = MIN_FRAMES_IN_QUEUE;
		if (MIN_FRAMES_IN_QUEUE < surface_caps.minImageCount) {
			if (surface_caps.minImageCount > MAX_FRAMES_IN_QUEUE) {
				throw std::runtime_error("surface minimum image count exceeds maximum allowed");
			}
			minImageCount = surface_caps.minImageCount;
		}

		if (surface_caps.maxImageCount > 0 && minImageCount > surface_caps.maxImageCount) {
			minImageCount = surface_caps.maxImageCount;
		}

		vkCreateSwapchainKHR(
			device_,
			&(VkSwapchainCreateInfoKHR){
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.surface = surface_,
				.minImageCount = minImageCount,
				.imageFormat = renderPassImageFormat_,
				.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
				.imageExtent = { width_, height_ },
				.imageArrayLayers = 1,
				.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 1,
				.pQueueFamilyIndices = (uint32_t[]){ 0 },
				.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = presentMode_,
			},
			NULL,
			&swapchain_);

		vkGetSwapchainImagesKHR(device_, swapchain_, &swapchainImageCount_, NULL);
		if (!(swapchainImageCount_ > 0)) { throw std::runtime_error("swapchain image count is 0"); }
		swapchainImages_.resize(swapchainImageCount_);
		vkGetSwapchainImagesKHR(device_, swapchain_, &swapchainImageCount_, &swapchainImages_[0]);
		if (swapchainImageCount_ > MAX_FRAMES_IN_QUEUE) {
			throw std::runtime_error("swapchain image count exceeds max frames allowed");
		}
		swapchainFramebuffers_.resize(swapchainImageCount_);
		for (uint32_t i = 0; i < swapchainImageCount_; i++) { _initImageBuffer(i, swapchainImages_[i]); }
	}


	void VulkanContext::_initImageBuffer(uint32_t i, VkImage image) {
		swapchainFramebuffers_[i].image_ = image;
		vkCreateImageView(device_,
                     &(VkImageViewCreateInfo) {
                        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                        .image = image,
                        .viewType = VK_IMAGE_VIEW_TYPE_2D,
                        .format = renderPassImageFormat_,
                        .components = {
                           .r = VK_COMPONENT_SWIZZLE_R,
                           .g = VK_COMPONENT_SWIZZLE_G,
                           .b = VK_COMPONENT_SWIZZLE_B,
                           .a = VK_COMPONENT_SWIZZLE_A,
                        },
                        .subresourceRange = {
                           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                           .baseMipLevel = 0,
                           .levelCount = 1,
                           .baseArrayLayer = 0,
                           .layerCount = 1,
                        },
                     },
                     NULL,
                     &swapchainFramebuffers_[i].view_);

		vkCreateFramebuffer(
			device_,
			&(VkFramebufferCreateInfo){ .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
										.renderPass = renderPass_,
										.attachmentCount = 1,
										.pAttachments = &swapchainFramebuffers_[i].view_,
										.width = width_,
										.height = height_,
										.layers = 1 },
			NULL,
			&swapchainFramebuffers_[i].framebuffer_);

		vkCreateFence(
			device_,
			&(VkFenceCreateInfo){ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT },
			NULL,
			&swapchainFramebuffers_[i].fence_);

		vkAllocateCommandBuffers(
			device_,
			&(VkCommandBufferAllocateInfo){
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.commandPool = commandPool_,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = 1,
			},
			&swapchainFramebuffers_[i].commandBuffer_);
	}
} // namespace Fluxions
