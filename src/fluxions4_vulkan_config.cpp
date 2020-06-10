#include <fluxions4_vulkan_config.hpp>

namespace Fluxions {
	// Temporary structs we will replace with our own later...

	static uint32_t vs_spirv_source[] = {
		#include "vkcube.vert.spv.h.in"
	};

	static uint32_t fs_spirv_source[] = {
		#include "vkcube.frag.spv.h.in"
	};


	VulkanConfig::VulkanConfig(VulkanContext& vc) :
		context_(&vc) {}


	VulkanConfig::~VulkanConfig() {

	}


	bool VulkanConfig::init() {
		VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[] = { {
			0,                                 // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // descriptorType
			1,                                 // descriptorCount
			VK_SHADER_STAGE_VERTEX_BIT,        // stageFlags
			nullptr                            // VkSampler* pImmutableSamplers
		} };

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, // sType
			nullptr,	                       // pNext
			0,                                 // flags
			1,                                 // bindingCount
			descriptorSetLayoutBindings        // pBindings
		};

		VkDescriptorSetLayout descriptorSetLayout;
		vkCreateDescriptorSetLayout(device(), &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout);

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, // sType
			nullptr,                 // pNext
			0,                       // flags
			1,                       // setLayoutCount
			&descriptorSetLayout,    // pSetLayouts
			0,                       // pushConstantRangeCount
			nullptr                  // pPushConstantRanges
		};
		vkCreatePipelineLayout(device(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout_);

		VkVertexInputBindingDescription vertexInputBindingDescriptions[3] = {
			//uint32_t             binding;
			//uint32_t             stride;
			//VkVertexInputRate    inputRate;
			{ 0, 3 * sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX },
			{ 1, 3 * sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX },
			{ 2, 3 * sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX },
		};

		VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3] = {
			//uint32_t    location;
			//uint32_t    binding;
			//VkFormat    format;
			//uint32_t    offset;
			{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 },
			{ 1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0 },
			{ 2, 2, VK_FORMAT_R32G32B32_SFLOAT, 0 },
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
			3, vertexInputBindingDescriptions,
			3, vertexInputAttributeDescriptions
		};

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
		VkShaderModule vsShaderModule;
		vkCreateShaderModule(device(), &vsShaderModuleCreateInfo, nullptr, &vsShaderModule);

		VkShaderModuleCreateInfo fsShaderModuleCreateInfo = {
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr, 0,
			sizeof(fs_spirv_source),
			(uint32_t*)fs_spirv_source
		};
		VkShaderModule fsShaderModule;
		vkCreateShaderModule(device(), &fsShaderModuleCreateInfo, nullptr, &fsShaderModule);

		VkPipelineCache pipelineCache = VK_NULL_HANDLE;
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] = {
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
				VK_SHADER_STAGE_VERTEX_BIT, vsShaderModule, "main", nullptr
			},
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
				VK_SHADER_STAGE_FRAGMENT_BIT, fsShaderModule, "main", nullptr
			},
		};

		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
			//VkStructureType                            sType;
			//const void*                                pNext;
			//VkPipelineInputAssemblyStateCreateFlags    flags;
			//VkPrimitiveTopology                        topology;
			//VkBool32                                   primitiveRestartEnable;
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr, 0,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
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
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_CLOCKWISE,
			VK_FALSE,
			0.0f,
			0.0f,
			0.0f,
			1.0f
		};

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
			VK_FALSE,
			VK_FALSE,
			VK_COMPARE_OP_NEVER,
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

		VkPipeline pipeline{ 0 };
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

		vertex_offset_ = sizeof(struct VertShaderUbo);
		colors_offset_ = vertex_offset_ + sizeof(vVertices);
		normals_offset_ = colors_offset_ + sizeof(vColors);
		uint32_t allocationSize = normals_offset_ + sizeof(vNormals);

		vbuffer.init(context_, allocationSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		vbuffer.copyToMap(vertex_offset_, (void*)vVertices, sizeof(vVertices));
		vbuffer.copyToMap(colors_offset_, (void*)vColors, sizeof(vColors));
		vbuffer.copyToMap(normals_offset_, (void*)vNormals, sizeof(vNormals));

		VkDescriptorPoolSize descriptorPoolSizes[] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 },
		};
		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
			//VkStructureType                sType;
			//const void*                    pNext;
			//VkDescriptorPoolCreateFlags    flags;
			//uint32_t                       maxSets;
			//uint32_t                       poolSizeCount;
			//const VkDescriptorPoolSize*    pPoolSizes;
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			nullptr, 0,
			1,
			1, descriptorPoolSizes
		};
		VkDescriptorPool descriptorPool;
		vkCreateDescriptorPool(device(), &descriptorPoolCreateInfo, NULL, &descriptorPool);

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
			//VkStructureType                 sType;
			//const void*                     pNext;
			//VkDescriptorPool                descriptorPool;
			//uint32_t                        descriptorSetCount;
			//const VkDescriptorSetLayout*    pSetLayouts;
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			descriptorPool,
			1, &descriptorSetLayout
		};
		vkAllocateDescriptorSets(device(), &descriptorSetAllocateInfo, &descriptorSet_);

		VkDescriptorBufferInfo descriptorBufferInfo = {
			//VkBuffer        buffer;
			//VkDeviceSize    offset;
			//VkDeviceSize    range;
			vbuffer.buffer(), 0, sizeof(struct ubo)
		};

		VkWriteDescriptorSet writeDescriptorSet = {
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
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			nullptr,
			descriptorSet_,
			0,
			0,
			1,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			nullptr,
			&descriptorBufferInfo,
			nullptr
		};
		vkUpdateDescriptorSets(device(), 1, &writeDescriptorSet, 0, nullptr);

		resize(0, 0, context_->width(), context_->height(), 0.0f, 1.0f);
		return true;
	}


	void VulkanConfig::kill() {
		ubo_buffer_.kill();
		vkDestroyPipeline(device(), pipeline_, nullptr);
		vkDestroyPipelineLayout(device(), pipelineLayout_, nullptr);
	}


	void VulkanConfig::use(float t) {
		vkCmdSetViewport(commandBuffer(), 0, 1, &viewport_);
		vkCmdSetScissor(commandBuffer(), 0, 1, &scissor_);

		struct VertShaderUbo vs_ubo;
		vs_ubo.modelview.translate({ 0.0f, 0.0f, -8.0f });
		vs_ubo.modelview.rotate(45.0f + (0.25f * t), { 1.0f, 0.0f, 0.0f });
		vs_ubo.modelview.rotate(45.0f - (0.50f * t), { 0.0f, 1.0f, 0.0f });
		vs_ubo.modelview.rotate(10.0f + (0.15f * t), { 0.0f, 0.0f, 1.0f });
		float aspect = (float)context_->width() / (float)context_->height();
		FxMatrix4f projection;
		projection.perspective(45.0f, aspect, 0.1f, 100.0f);
		vs_ubo.modelviewprojection = projection * vs_ubo.modelview;
		vs_ubo.color.g = 0.5f * sin(t) + 0.5f;
		memcpy(vs_ubo.normal, &vs_ubo.modelview, sizeof(vs_ubo.normal));
		ubo_buffer_.copyToMap(0, (void*)&ubo, sizeof(ubo));
		//memcpy(map_, &ubo, sizeof(ubo));

		VkBuffer buffers[] = { vbuffer.buffer(), vbuffer.buffer(), vbuffer.buffer() };
		VkDeviceSize offsets[] = { vertex_offset_, colors_offset_, normals_offset_ };
		vkCmdBindVertexBuffers(commandBuffer(), 0, 3, buffers, offsets);
		vkCmdBindPipeline(commandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
		vkCmdBindDescriptorSets(commandBuffer(),
								VK_PIPELINE_BIND_POINT_GRAPHICS,
								pipelineLayout_,
								0, 1,
								&descriptorSet_, 0, nullptr);

		vkCmdDraw(commandBuffer(), 4, 1, 0, 0);
		vkCmdDraw(commandBuffer(), 4, 1, 4, 0);
		vkCmdDraw(commandBuffer(), 4, 1, 8, 0);
		vkCmdDraw(commandBuffer(), 4, 1, 12, 0);
		vkCmdDraw(commandBuffer(), 4, 1, 16, 0);
		vkCmdDraw(commandBuffer(), 4, 1, 20, 0);
	}


	void VulkanConfig::restore() {

	}
}
