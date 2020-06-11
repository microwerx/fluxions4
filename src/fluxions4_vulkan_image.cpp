#include <fluxions4_vulkan_image.hpp>
#include <hatchetfish.hpp>


namespace Fluxions {
	bool VulkanImage::init(VulkanImageFormat format, VulkanImageType imageType, uint32_t width, uint32_t height, uint32_t depth) {
		VulkanContext* context = VulkanContext::GetContext();
		if (!context) return false;
		VkDevice device = VulkanContext::GetDevice();
		if (!device) return false;

		VkFormatProperties properties = context->getFormatProperties((VkFormat)format_);
		format_ = (VkFormat)format;
		imageType_ = imageType;

		// 1. Create the VkImage

		VkImageCreateInfo ici{};
		ici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ici.imageType = VK_IMAGE_TYPE_2D;
		ici.extent.width = width;
		ici.extent.height = height;
		ici.extent.depth = depth;
		ici.mipLevels = 1;
		ici.arrayLayers = 1;
		ici.samples = VK_SAMPLE_COUNT_1_BIT;
		ici.format = format_;
		ici.tiling = VK_IMAGE_TILING_OPTIMAL;
		ici.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		ici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		ici.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		switch (imageType) {
		case VulkanImageType::IMAGE_TYPE_2D:
			ici.imageType = VK_IMAGE_TYPE_2D;
			break;
		case VulkanImageType::IMAGE_TYPE_2D_ARRAY:
			ici.imageType = VK_IMAGE_TYPE_2D;
			ici.extent.depth = 1;
			ici.arrayLayers = depth;
			break;
		case VulkanImageType::IMAGE_TYPE_CUBE_MAP:
			ici.imageType = VK_IMAGE_TYPE_3D;
			ici.extent.depth = 1;
			ici.arrayLayers = 6;
			ici.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			break;
		case VulkanImageType::IMAGE_TYPE_COLOR_BUFFER:
			ici.imageType = VK_IMAGE_TYPE_2D;
			ici.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			break;
		case VulkanImageType::IMAGE_TYPE_DEPTH_BUFFER:
			ici.imageType = VK_IMAGE_TYPE_2D;
			ici.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			break;
		}

		if (vkCreateImage(device, &ici, nullptr, &image_) != VK_SUCCESS) {
			return false;
		}

		// 2. Create the VkDeviceMemory and bind it to the image

		VkMemoryRequirements mr{};
		vkGetImageMemoryRequirements(device, image_, &mr);

		VkMemoryAllocateInfo mai{};
		mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		mai.allocationSize = mr.size;
		mai.memoryTypeIndex = context->findMemoryTypeIndex(mr.memoryTypeBits);

		if (vkAllocateMemory(device, &mai, nullptr, &deviceMemory_) != VK_SUCCESS) {
			HFLOGERROR("Could not allocate image memory");
			return false;
		}

		vkBindImageMemory(device, image_, deviceMemory_, 0);

		// 3. Create the VkImageView

		VkImageViewCreateInfo ivci{};
		//VkStructureType            sType;
		//const void*                pNext;
		//VkImageViewCreateFlags     flags;
		//VkImage                    image;
		//VkImageViewType            viewType;
		//VkFormat                   format;
		//VkComponentMapping         components;
		//VkImageSubresourceRange    subresourceRange;
		ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ivci.image = image_;
		ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ivci.format = (VkFormat)format_;
		//VkImageAspectFlags    aspectMask;
		//uint32_t              baseMipLevel;
		//uint32_t              levelCount;
		//uint32_t              baseArrayLayer;
		//uint32_t              layerCount;
		ivci.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		if (vkCreateImageView(device, &ivci, nullptr, &imageView_) != VK_SUCCESS) {
			HFLOGERROR("Could not create image view");
			return false;
		}

		return true;
	}

	void VulkanImage::kill() {

	}


	//////////////////////////////////////////////////////////////////
	// VulkanSampler /////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////


	bool VulkanSampler::init(VulkanSamplerMode mode) {
		if (sampler_) kill();
		VkDevice device = VulkanContext::GetDevice();
		if (!device) return false;

		VkSamplerCreateInfo sci{};
		//VkStructureType         sType;
		//const void*             pNext;
		//VkSamplerCreateFlags    flags;
		//VkFilter                magFilter;
		//VkFilter                minFilter;
		//VkSamplerMipmapMode     mipmapMode;
		//VkSamplerAddressMode    addressModeU;
		//VkSamplerAddressMode    addressModeV;
		//VkSamplerAddressMode    addressModeW;
		//float                   mipLodBias;
		//VkBool32                anisotropyEnable;
		//float                   maxAnisotropy;
		//VkBool32                compareEnable;
		//VkCompareOp             compareOp;
		//float                   minLod;
		//float                   maxLod;
		//VkBorderColor           borderColor;
		//VkBool32                unnormalizedCoordinates;
		sci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sci.magFilter = VK_FILTER_NEAREST;
		sci.minFilter = VK_FILTER_NEAREST;
		sci.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sci.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sci.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sci.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sci.mipLodBias = 0.0f;
		sci.anisotropyEnable = VK_FALSE;
		sci.maxAnisotropy = 16.0f;
		sci.compareEnable = VK_FALSE;
		sci.compareOp = VK_COMPARE_OP_ALWAYS;
		sci.minLod = 0.0f;
		sci.maxLod = 0.0f;
		sci.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		sci.unnormalizedCoordinates = VK_FALSE;

		switch (mode) {
		case VulkanSamplerMode::VULKAN_NO_FILTER_REPEAT:
			break;
		}

		if (vkCreateSampler(device, &sci, nullptr, &sampler_) != VK_SUCCESS) {
			HFLOGERROR("Could not create sampler");
			return false;
		}

		return true;
	}


	void VulkanSampler::kill() {
		VkDevice device = VulkanContext::GetDevice();
		if (!device) return;

		if (sampler_) {
			vkDestroySampler(device, sampler_, nullptr);
			sampler_ = nullptr;
		}
	}


	//////////////////////////////////////////////////////////////////
	// FACTORY ROUTINES //////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////


	VulkanImage MakeColorAttachmentBuffer(uint32_t w, uint32_t h, VulkanImageFormat format) {
		VulkanImage image;
		image.init(format, VulkanImageType::IMAGE_TYPE_COLOR_BUFFER, w, h, 1);
		return image;
	}


	VulkanImage MakeDepthAttachmentBuffer(uint32_t w, uint32_t h, VulkanImageFormat format) {
		VulkanImage image;
		image.init(format, VulkanImageType::IMAGE_TYPE_DEPTH_BUFFER, w, h, 1);
		return image;
	}
}
