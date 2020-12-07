#ifndef FLUXIONS_VULKAN_CONFIG_HPP
#define FLUXIONS_VULKAN_CONFIG_HPP

#include <array>
#include <fluxions4.hpp>
#include <fluxions4_vulkan_context.hpp>
#include <fluxions4_vulkan_buffer.hpp>
#include <fluxions4_vulkan_descriptor_set.hpp>

namespace Fluxions {
	enum class PipelineTopology {
		POINTS = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
		LINE_LIST = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
		LINE_STRIP = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
		TRIANGLES = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		TRIANGLE_STRIP = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
		TRIANGLE_FAN = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN
	};

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
		CULL_NONE = VK_CULL_MODE_NONE,
		CULL_MODE_BACK = VK_CULL_MODE_BACK_BIT,
		CULL_MODE_FRONT = VK_CULL_MODE_FRONT_BIT,
		CULL_MODE_FRONT_AND_BACK = VK_CULL_MODE_FRONT_AND_BACK
	};

	struct VulkanPipelineCreateInfo {
		PipelineTopology topology;
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

		//void use(float t);
		void use(VulkanDescriptorSet& ds);
		void restore();

		VkDescriptorPool pool() { return descriptorPool_; }
		VkDescriptorSetLayout layout() { return descriptorSetLayout_; }

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

		static constexpr uint32_t INIT_FLAG = 1;
		std::bitset<32> flags;

		FxColor4f clearColor_;
		VkViewport viewport_{ 0,0,0,0,0,0 };
		VkRect2D scissor_{ 0,0,0,0 };

		VkShaderModule vsShaderModule_{ nullptr };
		VkShaderModule fsShaderModule_{ nullptr };
		VkDescriptorPool descriptorPool_{ nullptr };
		VkPipelineLayout pipelineLayout_{ nullptr };
		VkPipeline pipeline_{ nullptr };

		VkDescriptorSetLayout descriptorSetLayout_{ nullptr };

		bool _createShaders();
		bool _createPipelineLayout();
		bool _createGraphicsPipeline();
		bool _createDescriptorPool();
		bool _createDescriptorSet();
		bool _createDescriptor(uint32_t binding, VkDescriptorType type);
	};


	bool MakeDescriptorSet(VulkanPipeline& pl, VulkanDescriptorSet& ds);
}

#endif
