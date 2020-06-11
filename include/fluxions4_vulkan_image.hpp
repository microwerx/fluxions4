#ifndef FLUXIONS4_VULKAN_IMAGE_HPP
#define FLUXIONS4_VULKAN_IMAGE_HPP


#include <fluxions4_vulkan_context.hpp>


namespace Fluxions {
	enum class VulkanImageType {
		IMAGE_TYPE_2D,
		IMAGE_TYPE_2D_ARRAY,
		IMAGE_TYPE_CUBE_MAP,
		IMAGE_TYPE_DEPTH_BUFFER,
		IMAGE_TYPE_COLOR_BUFFER
	};

	enum class VulkanImageFormat {
		FORMAT_RGB_U8 = VK_FORMAT_R8G8B8_UINT,
		FORMAT_RGB_SRGB8 = VK_FORMAT_R8G8B8A8_SRGB,
		FORMAT_RGB_FLOAT = VK_FORMAT_R32G32B32_SFLOAT,
		FORMAT_RGBA_U8 = VK_FORMAT_R8G8B8A8_UINT,
		FORMAT_RGBA_SRGB8 = VK_FORMAT_R8G8B8A8_SRGB,
		FORMAT_RGBA_FLOAT = VK_FORMAT_R32G32B32A32_SFLOAT,
		FORMAT_DEPTH_16 = VK_FORMAT_D16_UNORM,
		FORMAT_DEPTH_16_STENCIL_8 = VK_FORMAT_D16_UNORM_S8_UINT,
		FORMAT_DEPTH_24_STENCIL_8 = VK_FORMAT_D24_UNORM_S8_UINT,
		FORMAT_DEPTH_32F = VK_FORMAT_D32_SFLOAT,
		FORMAT_DEPTH_32F_STENCIL_8 = VK_FORMAT_D32_SFLOAT_S8_UINT,
	};

	class VulkanImage {
	public:
		VulkanImage() {}
		~VulkanImage() { kill(); }

		bool init(VulkanImageFormat format, VulkanImageType imageType, uint32_t width, uint32_t height, uint32_t depth);
		void kill();

		VkImageView imageView() { return imageView_; }

	private:
		VulkanImageType imageType_{ VulkanImageType::IMAGE_TYPE_2D };
		VkFormat format_{ VK_FORMAT_R8G8B8A8_SRGB };
		VkImage image_{};
		VkDeviceMemory deviceMemory_{};
		VkImageView imageView_{};
	};

	enum class VulkanSamplerMode {
		VULKAN_NO_FILTER_REPEAT = 0
	};

	class VulkanSampler {
	public:
		VulkanSampler() {}
		~VulkanSampler() { kill(); }

		bool init(VulkanSamplerMode mode);
		void kill();

	private:
		VkSampler sampler_;
	};

	// Creates an SRGB Texture Map
	VulkanImage Make2DTextureSRGB8(uint32_t w, uint32_t h);
	
	// Creates an SRGB Texture Map
	VulkanImage MakeCubeMapTextureSRGB8(uint32_t size);

	// Creates an color image suitable for attaching to a framebuffer
	VulkanImage MakeColorAttachmentBuffer(uint32_t w, uint32_t h, VulkanImageFormat format);

	// Creates an depth image suitable for attaching to a frame buffer
	VulkanImage MakeDepthAttachmentBuffer(uint32_t w, uint32_t h, VulkanImageFormat format);
}


#endif
