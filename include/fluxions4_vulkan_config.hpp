#ifndef FLUXIONS_VULKAN_CONFIG_HPP
#define FLUXIONS_VULKAN_CONFIG_HPP

#include "fluxions_vulkan_context.hpp"
#include <fluxions4_vulkan_buffer.hpp>

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

		void setContext(VulkanContext& vc) { context_ = &vc; }
		void setClearColor(FxColor4f clearColor) { clearColor_ = clearColor; }
		void resize(uint32_t x, uint32_t y, uint32_t w, uint32_t h, float minDepth = 0.0f, float maxDepth = 1.0f) {
			viewport_.x = (float)x;
			viewport_.y = (float)y;
			viewport_.width = (float)w;
			viewport_.height = (float)h;
			viewport_.minDepth = minDepth;
			viewport_.maxDepth = maxDepth;

			scissor_.offset = { 0, 0 };
			scissor_.extent = { w, h };
		}
	private:
		VulkanContext* context_{ nullptr };
		VkCommandBuffer commandBuffer() { return context_->commandBuffer(); }
		VkDevice device() { return context_->device(); }

		FxColor4f clearColor_;
		VkViewport viewport_;
		VkRect2D scissor_;

		VkPipelineLayout pipelineLayout_{ nullptr };
		VkPipeline pipeline_{ nullptr };

		uint32_t vertex_offset_{ 0 };
		uint32_t colors_offset_{ 0 };
		uint32_t normals_offset_{ 0 };
		VkDescriptorSet descriptorSet_{ nullptr };

		//VkBuffer buffer_{ nullptr };
		//VkDeviceMemory deviceMemory_{ nullptr };
		//uint8_t* map_{ nullptr };

		VulkanBuffer vbuffer;
	};
}

#endif
