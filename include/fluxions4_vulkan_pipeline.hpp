#ifndef FLUXIONS_VULKAN_CONFIG_HPP
#define FLUXIONS_VULKAN_CONFIG_HPP

#include <array>
#include <fluxions4_vulkan_context.hpp>
#include <fluxions4_vulkan_buffer.hpp>

namespace Fluxions {
	enum class PipelineDepthMode {
		NO_DEPTH_TEST,
		DEPTH_LESS,
		DEPTH_LEQUAL,
		DEPTH_EQUAL,
		DEPTH_LESS_NO_WRITE,
		DEPTH_LEQUAL_NO_WRITE,
		DEPTH_EQUAL_NO_WRITE
	};

	enum class PipelineBlendMode {
		NO_BLEND,
		ADDITIVE,		// GL_ONE, GL_ONE
		MULTIPLY,		// GL_SRC_COLOR, GL_ZERO
		ALPHA_BLENDING,	// GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		LIGHTEN,		// GL_DST_COLOR, GL_SRC_COLOR
		DARKEN,
	};

	enum class PipelineCullMode {
		NO_CULLING,
		CULL_BACK_FACE,
		CULL_FRONT_FACE
	};

	struct VulkanPipelineCreateInfo {
		VkPrimitiveTopology topology;
		PipelineCullMode cullMode;
		PipelineDepthMode depthMode;
		PipelineBlendMode blendMode;
	};

	// VulkanPipeline is used to for a whole render pass
	class VulkanPipeline {
	public:
		VulkanPipeline(VulkanContext& vc);
		~VulkanPipeline();

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
		VkViewport viewport_{ 0,0,0,0,0,0 };
		VkRect2D scissor_{ 0,0,0,0 };

		VkShaderModule vsShaderModule_{ nullptr };
		VkShaderModule fsShaderModule_{ nullptr };
		VkDescriptorPool descriptorPool_{ nullptr };
		VkPipelineLayout pipelineLayout_{ nullptr };
		VkPipeline pipeline_{ nullptr };

		VkDescriptorSetLayout descriptorSetLayout_{ nullptr };
		static constexpr uint32_t MaxBindings = 2;
		//std::array<VkDescriptorSetLayout, MaxBindings> descriptorSetLayouts_;

		VkDescriptorSet descriptorSet_{ nullptr };
		//static constexpr uint32_t MaxDescriptorSets = 2;
		//std::array<VkDescriptorSet, MaxDescriptorSets> descriptorSets_{};

		VulkanBuffer ubo_buffer_;

		bool _createShaders();
		bool _createPipelineLayout();
		bool _createGraphicsPipeline();
		bool _createDescriptorPool();
		bool _createDescriptorSet();
		bool _createDescriptor(uint32_t binding, VkDescriptorType type);
	};
}

#endif