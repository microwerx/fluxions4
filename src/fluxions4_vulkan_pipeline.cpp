#include <hatchetfish.hpp>
#include <fluxions4_vulkan_pipeline.hpp>
#include <fluxions4_vulkan_ubo.hpp>
#include <fluxions4_vulkan_vertex.hpp>

namespace Fluxions {
	// Temporary structs we will replace with our own later...

	static uint32_t vs_spirv_source[] = {
		#include "vkcube.vert.spv.h.in"
	};

	static uint32_t fs_spirv_source[] = {
		#include "vkcube.frag.spv.h.in"
#include <hatchetfish.hpp>
	};


	VulkanPipeline::VulkanPipeline(VulkanContext& vc) :
		context_(&vc) {}


	VulkanPipeline::~VulkanPipeline() {

	}


	bool VulkanPipeline::init() {
		_createShaders();
		_createPipelineLayout();
		_createGraphicsPipeline();
		_createDescriptorPool();
		_createDescriptorSet();

		uint32_t allocationSize = sizeof(struct StandardUbo);
		ubo_buffer_.init(context_, allocationSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

		_createDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		_createDescriptor(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		resize(0, 0, context_->width(), context_->height(), 0.0f, 1.0f);
		return true;
	}


	void VulkanPipeline::kill() {
		ubo_buffer_.kill();
		vkDestroyPipeline(device(), pipeline_, nullptr);
		vkDestroyDescriptorSetLayout(device(), descriptorSetLayout_, nullptr);
		vkDestroyPipelineLayout(device(), pipelineLayout_, nullptr);
		vkDestroyDescriptorPool(device(), descriptorPool_, nullptr);
		vkDestroyShaderModule(device(), vsShaderModule_, nullptr);
		vkDestroyShaderModule(device(), fsShaderModule_, nullptr);
	}


	void VulkanPipeline::use(float t) {
		vkCmdSetViewport(commandBuffer(), 0, 1, &viewport_);
		vkCmdSetScissor(commandBuffer(), 0, 1, &scissor_);

		struct StandardUbo ubo;
		ubo.vert.modelview.translate({ 0.0f, 0.0f, -8.0f });
		ubo.vert.modelview.rotate(45.0f + (0.25f * t), { 1.0f, 0.0f, 0.0f });
		ubo.vert.modelview.rotate(45.0f - (0.50f * t), { 0.0f, 1.0f, 0.0f });
		ubo.vert.modelview.rotate(10.0f + (0.15f * t), { 0.0f, 0.0f, 1.0f });
		float aspect = (float)context_->width() / (float)context_->height();
		FxMatrix4f projection;
		projection.perspective(45.0f, aspect, 0.1f, 100.0f);
		ubo.vert.modelviewprojection = projection * ubo.vert.modelview;
		ubo.vert.color.g = 0.5f * sin(t) + 0.5f;
		memcpy(ubo.vert.normal, &ubo.vert.modelview, sizeof(ubo.vert.normal));
		ubo_buffer_.copyToMap(0, (void*)&ubo, sizeof(ubo));

		//VkBuffer buffers[] = { vbuffer.buffer(), vbuffer.buffer(), vbuffer.buffer() };
		//VkDeviceSize offsets[] = { vertex_offset_, colors_offset_, normals_offset_ };
		//vkCmdBindVertexBuffers(commandBuffer(), 0, 3, buffers, offsets);
		vkCmdBindPipeline(commandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
		vkCmdBindDescriptorSets(commandBuffer(),
								VK_PIPELINE_BIND_POINT_GRAPHICS,
								pipelineLayout_,
								0, 1,
								&descriptorSet_, 0, nullptr);

		//vkCmdDraw(commandBuffer(), 4, 1, 0, 0);
		//vkCmdDraw(commandBuffer(), 4, 1, 4, 0);
		//vkCmdDraw(commandBuffer(), 4, 1, 8, 0);
		//vkCmdDraw(commandBuffer(), 4, 1, 12, 0);
		//vkCmdDraw(commandBuffer(), 4, 1, 16, 0);
		//vkCmdDraw(commandBuffer(), 4, 1, 20, 0);
	}


	void VulkanPipeline::restore() {

	}


	bool VulkanPipeline::_createShaders() {
		VkShaderModuleCreateInfo vsShaderModuleCreateInfo = {
			//VkStructureType              sType;
			//const void*                  pNext;
			//VkShaderModuleCreateFlags    flags;
			//size_t                       codeSize;
			//const uint32_t*              pCode;
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr, 0,
			sizeof(vs_spirv_source),
			(uint32_t*)vs_spirv_source
		};
		if (vkCreateShaderModule(device(), &vsShaderModuleCreateInfo, nullptr, &vsShaderModule_) != VK_SUCCESS) {
			HFLOGERROR("Could not create vertex shader module");
			return false;
		}

		VkShaderModuleCreateInfo fsShaderModuleCreateInfo = {
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr, 0,
			sizeof(fs_spirv_source),
			(uint32_t*)fs_spirv_source
		};
		if (vkCreateShaderModule(device(), &fsShaderModuleCreateInfo, nullptr, &fsShaderModule_) != VK_SUCCESS) {
			HFLOGERROR("Could not create fragment shader module");
			return false;
		}

		return true;
	}


	bool VulkanPipeline::_createPipelineLayout() {
		std::array<VkDescriptorSetLayoutBinding, MaxBindings> dslbs;
		//uint32_t              binding;
		//VkDescriptorType      descriptorType;
		//uint32_t              descriptorCount;
		//VkShaderStageFlags    stageFlags;
		//const VkSampler*      pImmutableSamplers;
		dslbs[0] = { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1, VK_SHADER_STAGE_VERTEX_BIT, nullptr };
		dslbs[1] = { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr };

		VkDescriptorSetLayoutCreateInfo dslci{};
		//VkStructureType                        sType;
		//const void*                            pNext;
		//VkDescriptorSetLayoutCreateFlags       flags;
		//uint32_t                               bindingCount;
		//const VkDescriptorSetLayoutBinding*    pBindings;
		dslci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		dslci.bindingCount = MaxBindings;
		dslci.pBindings = dslbs.data();
		if (vkCreateDescriptorSetLayout(device(), &dslci, nullptr, &descriptorSetLayout_) != VK_SUCCESS) {
			HFLOGERROR("Could not create descriptor set layout");
			return false;
		}

		VkPipelineLayoutCreateInfo plci{};
		//VkStructureType                 sType;
		//const void*                     pNext;
		//VkPipelineLayoutCreateFlags     flags;
		//uint32_t                        setLayoutCount;
		//const VkDescriptorSetLayout*    pSetLayouts;
		//uint32_t                        pushConstantRangeCount;
		//const VkPushConstantRange*      pPushConstantRanges;
		plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		plci.setLayoutCount = 1;
		plci.pSetLayouts = &descriptorSetLayout_;

		if (vkCreatePipelineLayout(device(), &plci, nullptr, &pipelineLayout_) != VK_SUCCESS) {
			HFLOGERROR("Could not create pipeline layout");
			return false;
		}

		return (descriptorSet_ != nullptr) && (pipelineLayout_ != nullptr);
	}


	bool VulkanPipeline::_createGraphicsPipeline() {
		VulkanPipelineCreateInfo vpci = {
			//VkPrimitiveTopology topology;
			//PipelineCullMode cullMode;
			//PipelineDepthMode depthMode;
			//PipelineBlendMode blendMode;
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			PipelineCullMode::CULL_MODE_BACK,
			PipelineDepthMode::DEPTH_LESS,
			PipelineBlendMode::ALPHA_BLENDING
		};


		VkPipelineCache pipelineCache = VK_NULL_HANDLE;
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] = {
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
				VK_SHADER_STAGE_VERTEX_BIT, vsShaderModule_, "main", nullptr
			},
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
				VK_SHADER_STAGE_FRAGMENT_BIT, fsShaderModule_, "main", nullptr
			},
		};

		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
			//VkStructureType                             sType;
			//const void*                                 pNext;
			//VkPipelineVertexInputStateCreateFlags       flags;
			//uint32_t                                    vertexBindingDescriptionCount;
			//const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
			//uint32_t                                    vertexAttributeDescriptionCount;
			//const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			nullptr, 0,
			1, VulkanVertex::GetBindingDescriptions().data(),
			4, VulkanVertex::GetAttributeDescriptions().data()
		};

		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
			//VkStructureType                            sType;
			//const void*                                pNext;
			//VkPipelineInputAssemblyStateCreateFlags    flags;
			//VkPrimitiveTopology                        topology;
			//VkBool32                                   primitiveRestartEnable;
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr, 0,
			vpci.topology,
			//VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			//VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
			VK_FALSE
		};

		VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo = {
			//VkStructureType                           sType;
			//const void*                               pNext;
			//VkPipelineTessellationStateCreateFlags    flags;
			//uint32_t                                  patchControlPoints;
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			nullptr, 0,
			0
		};

		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
			//VkStructureType                       sType;
			//const void*                           pNext;
			//VkPipelineViewportStateCreateFlags    flags;
			//uint32_t                              viewportCount;
			//const VkViewport*                     pViewports;
			//uint32_t                              scissorCount;
			//const VkRect2D*                       pScissors;
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr, 0,
			1, nullptr,
			1, nullptr
		};

		VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {
			//VkStructureType                            sType;
			//const void*                                pNext;
			//VkPipelineRasterizationStateCreateFlags    flags;
			//VkBool32                                   depthClampEnable;
			//VkBool32                                   rasterizerDiscardEnable;
			//VkPolygonMode                              polygonMode;
			//VkCullModeFlags                            cullMode;
			//VkFrontFace                                frontFace;
			//VkBool32                                   depthBiasEnable;
			//float                                      depthBiasConstantFactor;
			//float                                      depthBiasClamp;
			//float                                      depthBiasSlopeFactor;
			//float                                      lineWidth;
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr, 0,
			VK_FALSE,
			VK_FALSE,
			VK_POLYGON_MODE_FILL,
			//VK_POLYGON_MODE_LINE,
			VK_CULL_MODE_NONE,
			VK_FRONT_FACE_CLOCKWISE,
			VK_FALSE,
			0.0f,
			0.0f,
			0.0f,
			1.0f
		};
		switch (vpci.cullMode) {
		case PipelineCullMode::CULL_NONE:
			pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
			break;
		case PipelineCullMode::CULL_MODE_BACK:
			pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
			break;
		case PipelineCullMode::CULL_MODE_FRONT:
			pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
			break;
		}

		VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {
			//VkStructureType                          sType;
			//const void*                              pNext;
			//VkPipelineMultisampleStateCreateFlags    flags;
			//VkSampleCountFlagBits                    rasterizationSamples;
			//VkBool32                                 sampleShadingEnable;
			//float                                    minSampleShading;
			//const VkSampleMask*                      pSampleMask;
			//VkBool32                                 alphaToCoverageEnable;
			//VkBool32                                 alphaToOneEnable;
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			nullptr, 0,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FALSE,
			0.0f,
			nullptr,
			VK_FALSE,
			VK_FALSE
		};

		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo = {
			//VkStructureType                           sType;
			//const void*                               pNext;
			//VkPipelineDepthStencilStateCreateFlags    flags;
			//VkBool32                                  depthTestEnable;
			//VkBool32                                  depthWriteEnable;
			//VkCompareOp                               depthCompareOp;
			//VkBool32                                  depthBoundsTestEnable;
			//VkBool32                                  stencilTestEnable;
			//VkStencilOpState                          front;
			//VkStencilOpState                          back;
			//float                                     minDepthBounds;
			//float                                     maxDepthBounds;
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			nullptr, 0,
			VK_TRUE,
			VK_TRUE,
			VK_COMPARE_OP_LESS,
			//VK_FALSE,
			//VK_FALSE,
			//VK_COMPARE_OP_NEVER,
			VK_FALSE,
			VK_FALSE,
			{ VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_NEVER, 0, 0, 0 },
			{ VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_NEVER, 0, 0, 0 },
			0.0f,
			0.0f
		};

		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentStatus = {
			//VkBool32                 blendEnable;
			//VkBlendFactor            srcColorBlendFactor;
			//VkBlendFactor            dstColorBlendFactor;
			//VkBlendOp                colorBlendOp;
			//VkBlendFactor            srcAlphaBlendFactor;
			//VkBlendFactor            dstAlphaBlendFactor;
			//VkBlendOp                alphaBlendOp;
			//VkColorComponentFlags    colorWriteMask;
			VK_FALSE,
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_OP_ADD,
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_OP_ADD,
			VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
		};

		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {
			//VkStructureType                               sType;
			//const void*                                   pNext;
			//VkPipelineColorBlendStateCreateFlags          flags;
			//VkBool32                                      logicOpEnable;
			//VkLogicOp                                     logicOp;
			//uint32_t                                      attachmentCount;
			//const VkPipelineColorBlendAttachmentState*    pAttachments;
			//float                                         blendConstants[4];
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			nullptr, 0,
			VK_FALSE,
			VK_LOGIC_OP_CLEAR,
			1,
			&pipelineColorBlendAttachmentStatus,
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		};

		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {
			//VkStructureType                      sType;
			//const void*                          pNext;
			//VkPipelineDynamicStateCreateFlags    flags;
			//uint32_t                             dynamicStateCount;
			//const VkDynamicState*                pDynamicStates;
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			nullptr, 0,
			2,
			dynamicStates
		};

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
			//VkStructureType                                  sType;
			//const void*                                      pNext;
			//VkPipelineCreateFlags                            flags;
			//uint32_t                                         stageCount;
			//const VkPipelineShaderStageCreateInfo*           pStages;
			//const VkPipelineVertexInputStateCreateInfo*      pVertexInputState;
			//const VkPipelineInputAssemblyStateCreateInfo*    pInputAssemblyState;
			//const VkPipelineTessellationStateCreateInfo*     pTessellationState;
			//const VkPipelineViewportStateCreateInfo*         pViewportState;
			//const VkPipelineRasterizationStateCreateInfo*    pRasterizationState;
			//const VkPipelineMultisampleStateCreateInfo*      pMultisampleState;
			//const VkPipelineDepthStencilStateCreateInfo*     pDepthStencilState;
			//const VkPipelineColorBlendStateCreateInfo*       pColorBlendState;
			//const VkPipelineDynamicStateCreateInfo*          pDynamicState;
			//VkPipelineLayout                                 layout;
			//VkRenderPass                                     renderPass;
			//uint32_t                                         subpass;
			//VkPipeline                                       basePipelineHandle;
			//int32_t                                          basePipelineIndex;
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			nullptr, 0,
			2, pipelineShaderStageCreateInfos,
			&pipelineVertexInputStateCreateInfo,
			&pipelineInputAssemblyStateCreateInfo,
			&pipelineTessellationStateCreateInfo,
			&pipelineViewportStateCreateInfo,
			&pipelineRasterizationStateCreateInfo,
			&pipelineMultisampleStateCreateInfo,
			&pipelineDepthStencilStateCreateInfo,
			&pipelineColorBlendStateCreateInfo,
			&pipelineDynamicStateCreateInfo,
			pipelineLayout_,
			context_->renderPass(),
			0,
			VK_NULL_HANDLE,
			-1
		};
		vkCreateGraphicsPipelines(device(), pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &pipeline_);

		return pipeline_ != nullptr;
	}


	bool VulkanPipeline::_createDescriptorPool() {
		std::array<VkDescriptorPoolSize, 2> dps;
		//VkDescriptorType    type;
		//uint32_t            descriptorCount;
		dps[0] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, context_->maxSets() };
		dps[1] = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, context_->maxSets() };

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
			//VkStructureType                sType;
			//const void*                    pNext;
			//VkDescriptorPoolCreateFlags    flags;
			//uint32_t                       maxSets;
			//uint32_t                       poolSizeCount;
			//const VkDescriptorPoolSize*    pPoolSizes;
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			nullptr, 0,
			context_->maxSets(),
			(uint32_t)dps.size(), dps.data()
		};
		if (vkCreateDescriptorPool(device(), &descriptorPoolCreateInfo, NULL, &descriptorPool_) != VK_SUCCESS) {
			HFLOGERROR("Could not create descriptor pool");
			return false;
		}

		return descriptorPool_ != nullptr;
	}


	bool VulkanPipeline::_createDescriptorSet() {
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
			//VkStructureType                 sType;
			//const void*                     pNext;
			//VkDescriptorPool                descriptorPool;
			//uint32_t                        descriptorSetCount;
			//const VkDescriptorSetLayout*    pSetLayouts;
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			descriptorPool_,
			1, &descriptorSetLayout_
		};
		if (vkAllocateDescriptorSets(device(), &descriptorSetAllocateInfo, &descriptorSet_) != VK_SUCCESS) {
			HFLOGERROR("Could not allocate descriptor sets");
			return false;
		}
		return true;
	}


	bool VulkanPipeline::_createDescriptor(uint32_t binding, VkDescriptorType type) {
		//VkBuffer        buffer;
		//VkDeviceSize    offset;
		//VkDeviceSize    range;
		VkDescriptorBufferInfo dbiUbo = { ubo_buffer_.buffer(), 0, sizeof(struct StandardUbo) };

		//VkSampler        sampler;
		//VkImageView      imageView;
		//VkImageLayout    imageLayout;
		VkDescriptorImageInfo dbiSampler = { nullptr, nullptr, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

		VkWriteDescriptorSet wds{};
		//VkStructureType                  sType;
		//const void*                      pNext;
		//VkDescriptorSet                  dstSet;
		//uint32_t                         dstBinding;
		//uint32_t                         dstArrayElement;
		//uint32_t                         descriptorCount;
		//VkDescriptorType                 descriptorType;
		//const VkDescriptorImageInfo*     pImageInfo;
		//const VkDescriptorBufferInfo*    pBufferInfo;
		//const VkBufferView*              pTexelBufferView;
		wds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		wds.dstSet = descriptorSet_;
		wds.dstBinding = binding;
		wds.descriptorCount = 1;
		wds.descriptorType = type;
		switch (type) {
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			wds.pBufferInfo = &dbiUbo;
			break;
		case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			wds.pImageInfo = &dbiSampler;
			return false;
			break;
		}
		vkUpdateDescriptorSets(device(), 1, &wds, 0, nullptr);
		return true;
	}
}
