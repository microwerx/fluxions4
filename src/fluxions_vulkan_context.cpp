#include <algorithm>
#include <SDL2/SDL.h>
#include <hatchetfish.hpp>
#include <fluxions_vulkan_context.hpp>

#pragma warning(disable: 26812)

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace Fluxions {

	VulkanContext::VulkanContext() {
		memset(&pdMemoryProperties_, 0, sizeof(VkPhysicalDeviceMemoryProperties));
		memset(&pdProperties_, 0, sizeof(VkPhysicalDeviceProperties));
	}


	VulkanContext::~VulkanContext() {}


	bool VulkanContext::init() {
		try {
			if (!_createSDLWindow()) return false;
			if (!_getSDLExtensions()) return false;
			if (!_createInstance()) return false;
			if (!_enumerateDevices()) return false;
			if (!_checkQueueFamilyProperties()) return false;
			if (!_createDevice()) return false;
			if (!_createSDLVulkanSurface()) return false;
			if (!_chooseSurfaceFormat()) return false;
			if (!_createRenderPass()) return false;
			if (!_createCommandPool()) return false;
			if (!_createSemaphore()) return false;
			if (!_createSwapChain()) return false;
		}
		catch (const std::exception & e) {
			HFLOGERROR("failed to initialize Vulkan context: '%s'", e.what());
			return false;
		}
		return true;
	}


	void VulkanContext::kill() {
		for (auto& fb : swapchainFramebuffers_) {
			vkFreeCommandBuffers(device_, commandPool_, 1, &fb.commandBuffer_);
			vkDestroyFence(device_, fb.fence_, nullptr);
			vkDestroyFramebuffer(device_, fb.framebuffer_, nullptr);
			vkDestroyImageView(device_, fb.view_, nullptr);
			vkDestroyImage(device_, fb.image_, nullptr);
		}
		swapchainFramebuffers_.clear();
		vkDestroyCommandPool(device_, commandPool_, nullptr);
		vkDestroySemaphore(device_, semaphore_, nullptr);
		vkDestroyDevice(device_, nullptr);
		vkDestroyInstance(instance_, nullptr);
	}


	bool VulkanContext::beginFrame() {
		VkResult result =
			vkAcquireNextImageKHR(device_, swapchain_, UINT64_MAX, semaphore_, VK_NULL_HANDLE, &frameImageIndex_);
		if (result != VK_SUCCESS) return false;
		if (frameImageIndex_ >= MAX_FRAMES_IN_QUEUE) return false;

		vkWaitForFences(device_, 1, &fence(), VK_TRUE, UINT64_MAX);
		vkResetFences(device_, 1, &fence());

		VkCommandBufferBeginInfo commandBufferBeginInfo;
		memset(&commandBufferBeginInfo, 0, sizeof(commandBufferBeginInfo));
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = 0;
		vkBeginCommandBuffer(commandBuffer(), &commandBufferBeginInfo);

		VkRenderPassBeginInfo renderPassBeginInfo = {
				VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				nullptr,
				renderPass(),
				framebuffer(),
				{ { 0, 0 }, { width(), height() } },
				1,
				clearValues
		};
		vkCmdBeginRenderPass(commandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		return true;
	}


	void VulkanContext::presentFrame() {
		VkPipelineStageFlags pipelineStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submitInfo;
		memset(&submitInfo, 0, sizeof(submitInfo));
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &semaphore_;
		submitInfo.pWaitDstStageMask = pipelineStageFlags;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer();
		vkQueueSubmit(queue_, 1, &submitInfo, fence());

		VkResult result;
		VkSwapchainKHR swapchains[] = { swapchain_ };
		uint32_t imageIndices[] = { frameImageIndex_ };
		VkPresentInfoKHR presentInfo;
		memset(&presentInfo, 0, sizeof(presentInfo));
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = imageIndices;
		presentInfo.pResults = &result;

		if (vkQueuePresentKHR(queue_, &presentInfo) != VK_SUCCESS) {
			throw std::runtime_error("Unable to swap buffers");
		}

		vkQueueWaitIdle(queue_);
	}


	int VulkanContext::findMemoryTypeIndex(unsigned allowedMemoryTypeBits) const {
		for (unsigned i = 0; (1u << i) <= allowedMemoryTypeBits && i <= pdMemoryProperties_.memoryTypeCount; ++i) {
			if ((allowedMemoryTypeBits & (1u << i)) &&
				(pdMemoryProperties_.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
				(pdMemoryProperties_.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
				return (int)i;
		}
		return -1;
	}


	////////////////////////////////////////////////////////////////////////////
	// PRIVATE IMPLEMENTATION //////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////


	bool VulkanContext::_createSDLWindow() {
		window_ = SDL_CreateWindow(
			title_.c_str(),
			SDL_WINDOWPOS_CENTERED, // center horizontally
			SDL_WINDOWPOS_CENTERED, // center vertically
			width_,
			height_,
			SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
		SDL_Vulkan_GetDrawableSize(window_, (int*)&width_, (int*)&height_);
		return true;
	}

	bool VulkanContext::_getSDLExtensions() {
		uint32_t count;
		if (!SDL_Vulkan_GetInstanceExtensions(window_, &count, NULL)) { throw std::runtime_error(SDL_GetError()); }

		size_t extension_count = extensions_.size();
		extensions_.resize(extension_count + count);
		if (!SDL_Vulkan_GetInstanceExtensions(window_, &count, extensions_.data() + count)) {
			throw std::runtime_error(SDL_GetError());
		}

		auto last = std::unique(extensions_.begin(), extensions_.end(), [](const char* a, const char* b) { return (std::string(a) == std::string(b)); });
		extensions_.erase(last, extensions_.end());
		return true;
	}


	bool VulkanContext::_createInstance() {
		VkApplicationInfo applicationInfo;
		memset(&applicationInfo, 0, sizeof(VkApplicationInfo));
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = name_.c_str();
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 2);

		VkInstanceCreateInfo instanceCreateInfo;
		memset(&instanceCreateInfo, 0, sizeof(VkInstanceCreateInfo));
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions_.size());
		instanceCreateInfo.ppEnabledExtensionNames = extensions_.data();

		try {
			vkCreateInstance(&instanceCreateInfo, NULL, &instance_);
		}
		catch (const std::exception & e) {
			HFLOGERROR("'%s'", e.what());
		}
		catch (...) {
			HFLOGERROR("Unknown error calling vkCreateInstance");
		}
		return instance_ != nullptr;
	}


	bool VulkanContext::_enumerateDevices() {
		uint32_t count;
		vkEnumeratePhysicalDevices(instance_, &count, NULL);
		physicalDevices_.resize(count);
		HFLOGINFO("found %d physical devices", count);
		if (!count) { throw std::runtime_error("no physical devices found"); }

		vkEnumeratePhysicalDevices(instance_, &count, &physicalDevices_[0]);
		for (int i = count - 1; i >= 0; --i) {
			pd_ = physicalDevices_[i];
			vkGetPhysicalDeviceProperties(pd_, &pdProperties_);
			vkGetPhysicalDeviceMemoryProperties(pd_, &pdMemoryProperties_);
			HFLOGINFO("vendor id:    %04x", pdProperties_.vendorID);
			HFLOGINFO("vendor name:  %s", pdProperties_.deviceName);
			HFLOGINFO("memory heaps: %d", pdMemoryProperties_.memoryHeapCount);
			for (int j = 0; j < pdMemoryProperties_.memoryHeapCount; j++) {
				auto f = pdMemoryProperties_.memoryHeaps[j].flags;
				if (!f) continue;
				HFLOGINFO("memory heap size:       %zu", pdMemoryProperties_.memoryHeaps[j].size);
				if (f & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) HFLOGINFO("memory heap properties: Device local");
				if (f & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) HFLOGINFO("memory heap properties: Multiple instance");
			}
			HFLOGINFO("memory types: %d", pdMemoryProperties_.memoryTypeCount);
			for (int j = 0; j < pdMemoryProperties_.memoryTypeCount; j++) {
				auto f = pdMemoryProperties_.memoryTypes[j].propertyFlags;
				if (!f) continue;
				HFLOGINFO("memory type index:      %d", pdMemoryProperties_.memoryTypes[j].heapIndex);
				if (f & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) HFLOGINFO("memory type properties: Device local");
				if (f & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) HFLOGINFO("memory type properties: Host visible");
				if (f & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) HFLOGINFO("memory type properties: Host coherent");
				if (f & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) HFLOGINFO("memory type properties: Host cached");
				if (f & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) HFLOGINFO("memory type properties: Lazily allocated");
				if (f & VK_MEMORY_PROPERTY_PROTECTED_BIT) HFLOGINFO("memory type properties: Protected");
				if (f & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) HFLOGINFO("memory type properties: Device coherent (AMD)");
				if (f & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) HFLOGINFO("memory type properties: Device uncached (AMD)");
			}
			HFLOGINFO("-------");
		}
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
		float queuePriorities[] = { 1.0f };
		const char* const enabledExtensionNames[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		// TODO: should we be using the extensions_ member instead?
		VkDeviceQueueCreateInfo deviceQueueCreateInfo;
		memset(&deviceQueueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = 0;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

		VkDeviceCreateInfo deviceCreateInfo;
		memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.enabledExtensionCount = 1;
		deviceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;
		vkCreateDevice(pd_, &deviceCreateInfo, NULL, &device_);

		if (!device_) return false;

		vkGetDeviceQueue(device_, 0, 0, &queue_);

		return queue_ != nullptr;
	}


	bool VulkanContext::_createSDLVulkanSurface() {
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
		for (uint32_t i = 0; i < numSurfaceFormats; i++) {
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
		VkAttachmentDescription attachmentDescription;
		memset(&attachmentDescription, 0, sizeof(VkAttachmentDescription));
		attachmentDescription.format = renderPassImageFormat_;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachment;
		memset(&colorAttachment, 0, sizeof(VkAttachmentReference));
		colorAttachment.attachment = 0;
		colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference resolveAttachment;
		memset(&resolveAttachment, 0, sizeof(VkAttachmentReference));
		resolveAttachment.attachment = VK_ATTACHMENT_UNUSED;
		resolveAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription;
		memset(&subpassDescription, 0, sizeof(VkSubpassDescription));
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachment;
		subpassDescription.pResolveAttachments = &resolveAttachment;
		subpassDescription.pDepthStencilAttachment = NULL;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = NULL;

		VkRenderPassCreateInfo renderPassCreateInfo;
		memset(&renderPassCreateInfo, 0, sizeof(VkRenderPassCreateInfo));
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &attachmentDescription;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpassDescription;
		renderPassCreateInfo.dependencyCount = 0;

		vkCreateRenderPass(device_, &renderPassCreateInfo, NULL, &renderPass_);
		return renderPass_ != nullptr;
	}


	bool VulkanContext::_createCommandPool() {
		VkCommandPoolCreateInfo commandPoolCreateInfo;
		memset(&commandPoolCreateInfo, 0, sizeof(VkCommandPoolCreateInfo));
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = 0;
		vkCreateCommandPool(device_, &commandPoolCreateInfo, NULL, &commandPool_);
		return commandPool_ != nullptr;
	}


	bool VulkanContext::_createSemaphore() {
		VkSemaphoreCreateInfo semaphoreCreateInfo;
		memset(&semaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(device_, &semaphoreCreateInfo, NULL, &semaphore_);
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
		uint32_t queueFamilyIndices[] = { 0 };

		VkSwapchainCreateInfoKHR swapchainCreateInfo;
		memset(&swapchainCreateInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface_;
		swapchainCreateInfo.minImageCount = minImageCount;
		swapchainCreateInfo.imageFormat = renderPassImageFormat_;
		swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageExtent = { width_, height_ };
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 1;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode_;
		vkCreateSwapchainKHR(device_, &swapchainCreateInfo, NULL, &swapchain_);

		vkGetSwapchainImagesKHR(device_, swapchain_, &swapchainImageCount_, NULL);
		if (!(swapchainImageCount_ > 0)) { throw std::runtime_error("swapchain image count is 0"); }
		swapchainImages_.resize(swapchainImageCount_);
		vkGetSwapchainImagesKHR(device_, swapchain_, &swapchainImageCount_, &swapchainImages_[0]);
		if (swapchainImageCount_ > MAX_FRAMES_IN_QUEUE) {
			throw std::runtime_error("swapchain image count exceeds max frames allowed");
		}
		swapchainFramebuffers_.resize(swapchainImageCount_);
		for (uint32_t i = 0; i < swapchainImageCount_; i++) { _initImageBuffer(i, swapchainImages_[i]); }
		return true;
	}


	void VulkanContext::_initImageBuffer(uint32_t i, VkImage image) {
		swapchainFramebuffers_[i].image_ = image;
		VkImageViewCreateInfo imageViewCreateInfo;
		memset(&imageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = renderPassImageFormat_;
		imageViewCreateInfo.components = {
		   .r = VK_COMPONENT_SWIZZLE_R,
		   .g = VK_COMPONENT_SWIZZLE_G,
		   .b = VK_COMPONENT_SWIZZLE_B,
		   .a = VK_COMPONENT_SWIZZLE_A,
		};
		imageViewCreateInfo.subresourceRange = {
		   .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		   .baseMipLevel = 0,
		   .levelCount = 1,
		   .baseArrayLayer = 0,
		   .layerCount = 1,
		};
		vkCreateImageView(device_, &imageViewCreateInfo, NULL, &swapchainFramebuffers_[i].view_);

		VkFramebufferCreateInfo framebufferCreateInfo;
		memset(&framebufferCreateInfo, 0, sizeof(VkFramebufferCreateInfo));
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass_;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &swapchainFramebuffers_[i].view_;
		framebufferCreateInfo.width = width_;
		framebufferCreateInfo.height = height_;
		framebufferCreateInfo.layers = 1;
		vkCreateFramebuffer(device_, &framebufferCreateInfo, NULL, &swapchainFramebuffers_[i].framebuffer_);

		VkFenceCreateInfo fenceCreateInfo;
		memset(&fenceCreateInfo, 0, sizeof(VkFenceCreateInfo));
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		vkCreateFence(device_, &fenceCreateInfo, NULL, &swapchainFramebuffers_[i].fence_);

		VkCommandBufferAllocateInfo commandBufferAllocateInfo;
		memset(&commandBufferAllocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool_;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = 1;
		vkAllocateCommandBuffers(device_, &commandBufferAllocateInfo, &swapchainFramebuffers_[i].commandBuffer_);
	}
} // namespace Fluxions
