#ifndef FLUXIONS_VULKAN_CONFIG_HPP
#define FLUXIONS_VULKAN_CONFIG_HPP

#include "fluxions_vulkan_context.hpp"
#include <fluxions_vulkan_buffer.hpp>

namespace Fluxions {
	// VulkanConfig is used to for a whole render pass
	class VulkanConfig {
	public:
		VulkanConfig(VulkanContext& vc);
		~VulkanConfig();

		bool init();
		void kill();

		void use(float t);
		void restore();

		void setContext(VulkanContext& vc) { context_ = vc; }
		void setClearColor(FxColor4f clearColor) { clearColor_ = clearColor; }

	private:
		VulkanContext& context_;
		FxColor4f clearColor_;

		VkPipelineLayout pipelineLayout_{ nullptr };
		VkPipeline pipeline_{ nullptr };

		uint32_t vertex_offset_{ 0 };
		uint32_t colors_offset_{ 0 };
		uint32_t normals_offset_{ 0 };
		VkDescriptorSet descriptorSet_{ nullptr };

		VkBuffer buffer_{ nullptr };
		VkDeviceMemory deviceMemory_{ nullptr };
		uint8_t* map_{ nullptr };

		VulkanBuffer vbuffer;
	};
}

#endif
