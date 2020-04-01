#include <fluxions_vulkan_config.hpp>

namespace Fluxions {
	// Temporary structs we will replace with our own later...
	struct ubo {
		FxMatrix4f modelview;
		FxMatrix4f modelviewprojection;
		float normal[12];
	};

	static uint32_t vs_spirv_source[] = {
		#include "vkcube.vert.spv.h.in"
	};

	static uint32_t fs_spirv_source[] = {
		#include "vkcube.frag.spv.h.in"
	};

	static const float vVertices[] = {
		// front
		-1.0f, -1.0f, +1.0f, // point blue
		+1.0f, -1.0f, +1.0f, // point magenta
		-1.0f, +1.0f, +1.0f, // point cyan
		+1.0f, +1.0f, +1.0f, // point white
		// back
		+1.0f, -1.0f, -1.0f, // point red
		-1.0f, -1.0f, -1.0f, // point black
		+1.0f, +1.0f, -1.0f, // point yellow
		-1.0f, +1.0f, -1.0f, // point green
		// right
		+1.0f, -1.0f, +1.0f, // point magenta
		+1.0f, -1.0f, -1.0f, // point red
		+1.0f, +1.0f, +1.0f, // point white
		+1.0f, +1.0f, -1.0f, // point yellow
		// left
		-1.0f, -1.0f, -1.0f, // point black
		-1.0f, -1.0f, +1.0f, // point blue
		-1.0f, +1.0f, -1.0f, // point green
		-1.0f, +1.0f, +1.0f, // point cyan
		// top
		-1.0f, +1.0f, +1.0f, // point cyan
		+1.0f, +1.0f, +1.0f, // point white
		-1.0f, +1.0f, -1.0f, // point green
		+1.0f, +1.0f, -1.0f, // point yellow
		// bottom
		-1.0f, -1.0f, -1.0f, // point black
		+1.0f, -1.0f, -1.0f, // point red
		-1.0f, -1.0f, +1.0f, // point blue
		+1.0f, -1.0f, +1.0f  // point magenta
	};

	static const float vColors[] = {
		// front
		0.0f,  0.0f,  1.0f, // blue
		1.0f,  0.0f,  1.0f, // magenta
		0.0f,  1.0f,  1.0f, // cyan
		1.0f,  1.0f,  1.0f, // white
		// back
		1.0f,  0.0f,  0.0f, // red
		0.0f,  0.0f,  0.0f, // black
		1.0f,  1.0f,  0.0f, // yellow
		0.0f,  1.0f,  0.0f, // green
		// right
		1.0f,  0.0f,  1.0f, // magenta
		1.0f,  0.0f,  0.0f, // red
		1.0f,  1.0f,  1.0f, // white
		1.0f,  1.0f,  0.0f, // yellow
		// left
		0.0f,  0.0f,  0.0f, // black
		0.0f,  0.0f,  1.0f, // blue
		0.0f,  1.0f,  0.0f, // green
		0.0f,  1.0f,  1.0f, // cyan
		// top
		0.0f,  1.0f,  1.0f, // cyan
		1.0f,  1.0f,  1.0f, // white
		0.0f,  1.0f,  0.0f, // green
		1.0f,  1.0f,  0.0f, // yellow
		// bottom
		0.0f,  0.0f,  0.0f, // black
		1.0f,  0.0f,  0.0f, // red
		0.0f,  0.0f,  1.0f, // blue
		1.0f,  0.0f,  1.0f  // magenta
	};

	static const float vNormals[] = {
		// front
		+0.0f, +0.0f, +1.0f, // forward
		+0.0f, +0.0f, +1.0f, // forward
		+0.0f, +0.0f, +1.0f, // forward
		+0.0f, +0.0f, +1.0f, // forward
		// back
		+0.0f, +0.0f, -1.0f, // backbard
		+0.0f, +0.0f, -1.0f, // backbard
		+0.0f, +0.0f, -1.0f, // backbard
		+0.0f, +0.0f, -1.0f, // backbard
		// right
		+1.0f, +0.0f, +0.0f, // right
		+1.0f, +0.0f, +0.0f, // right
		+1.0f, +0.0f, +0.0f, // right
		+1.0f, +0.0f, +0.0f, // right
		// left
		-1.0f, +0.0f, +0.0f, // left
		-1.0f, +0.0f, +0.0f, // left
		-1.0f, +0.0f, +0.0f, // left
		-1.0f, +0.0f, +0.0f, // left
		// top
		+0.0f, +1.0f, +0.0f, // up
		+0.0f, +1.0f, +0.0f, // up
		+0.0f, +1.0f, +0.0f, // up
		+0.0f, +1.0f, +0.0f, // up
		// bottom
		+0.0f, -1.0f, +0.0f, // down
		+0.0f, -1.0f, +0.0f, // down
		+0.0f, -1.0f, +0.0f, // down
		+0.0f, -1.0f, +0.0f  // down
	};


	VulkanConfig::VulkanConfig(VulkanContext& vc) :
		context_(vc) {

	}


	VulkanConfig::~VulkanConfig() {

	}


	bool VulkanConfig::init() {
		VkDescriptorSetLayoutBinding dslb;
		memset(&dslb, 0, sizeof(dslb));
		dslb.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		dslb.descriptorCount = 1;
		dslb.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		dslb.pImmutableSamplers = NULL;

		VkDescriptorSetLayoutCreateInfo dslci;
		memset(&dslci, 0, sizeof(dslci));
		dslci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		dslci.bindingCount = 1;
		dslci.pBindings = &dslb;

		VkDescriptorSetLayout set_layout;
		vkCreateDescriptorSetLayout(context_.device(), &dslci, nullptr, &set_layout);

		VkPipelineLayoutCreateInfo plci;
		memset(&plci, 0, sizeof(plci));
		plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		plci.setLayoutCount = 1;
		plci.pSetLayouts = &set_layout;

		vkCreatePipelineLayout(context_.device(), &plci, nullptr, &pipelineLayout_);

		VkVertexInputBindingDescription vibd[3];
		vibd[0].binding = 0;
		vibd[0].stride = 3 * sizeof(float);
		vibd[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vibd[1].binding = 1;
		vibd[1].stride = 3 * sizeof(float);
		vibd[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vibd[2].binding = 2;
		vibd[2].stride = 3 * sizeof(float);
		vibd[2].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		VkVertexInputAttributeDescription viad[3];
		viad[0].location = 0;
		viad[0].binding = 0;
		viad[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		viad[0].offset = 0;
		viad[1].location = 1;
		viad[1].binding = 1;
		viad[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		viad[1].offset = 0;
		viad[2].location = 2;
		viad[2].binding = 2;
		viad[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		viad[2].offset = 0;

		VkPipelineVertexInputStateCreateInfo pvisci;
		memset(&pvisci, 0, sizeof(pvisci));
		pvisci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		pvisci.vertexBindingDescriptionCount = 3;
		pvisci.pVertexBindingDescriptions = vibd;
		pvisci.vertexAttributeDescriptionCount = 3;
		pvisci.pVertexAttributeDescriptions = viad;

		VkShaderModuleCreateInfo vs_smci;
		memset(&vs_smci, 0, sizeof(vs_smci));
		vs_smci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vs_smci.codeSize = sizeof(vs_spirv_source);
		vs_smci.pCode = (uint32_t*)vs_spirv_source;
		VkShaderModule vs_module;
		vkCreateShaderModule(context_.device(), &vs_smci, nullptr, &vs_module);

		VkShaderModuleCreateInfo fs_smci;
		memset(&fs_smci, 0, sizeof(fs_smci));
		fs_smci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		fs_smci.codeSize = sizeof(fs_spirv_source);
		fs_smci.pCode = (uint32_t*)fs_spirv_source;
		VkShaderModule fs_module;
		vkCreateShaderModule(context_.device(), &fs_smci, nullptr, &fs_module);

		VkPipelineCache pc = VK_NULL_HANDLE;
		VkPipelineShaderStageCreateInfo pssci[2];
		memset(&pssci[0], 0, sizeof(VkPipelineShaderStageCreateInfo) * 2);
		pssci[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pssci[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		pssci[0].module = vs_module;
		pssci[0].pName = "main";
		pssci[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pssci[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		pssci[1].module = fs_module;
		pssci[1].pName = "main";

		VkPipelineInputAssemblyStateCreateInfo piasci;
		memset(&piasci, 0, sizeof(piasci));
		piasci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		piasci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		piasci.primitiveRestartEnable = false;

		VkPipelineViewportStateCreateInfo pvsci;
		memset(&pvsci, 0, sizeof(pvsci));
		pvsci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pvsci.viewportCount = 1;
		pvsci.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo prsci;
		memset(&prsci, 0, sizeof(prsci));
		prsci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		prsci.rasterizerDiscardEnable = false;
		prsci.polygonMode = VK_POLYGON_MODE_FILL;
		prsci.cullMode = VK_CULL_MODE_BACK_BIT;
		prsci.frontFace = VK_FRONT_FACE_CLOCKWISE;
		prsci.lineWidth = 1.0f;

		VkPipelineMultisampleStateCreateInfo pmsci;
		memset(&pmsci, 0, sizeof(pmsci));
		pmsci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pmsci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo pdssci;
		memset(&pdssci, 0, sizeof(pdssci));
		pdssci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

		VkPipelineColorBlendAttachmentState pcbas;
		memset(&pcbas, 0, sizeof(pcbas));
		pcbas.colorWriteMask = VK_COLOR_COMPONENT_A_BIT |
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT;

		VkPipelineColorBlendStateCreateInfo pcbsci;
		memset(&pcbsci, 0, sizeof(pcbsci));
		pcbsci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pcbsci.attachmentCount = 1;
		pcbsci.pAttachments = &pcbas;

		VkDynamicState ds[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo pdsci;
		memset(&pdsci, 0, sizeof(pdsci));
		pdsci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pdsci.dynamicStateCount = 2;
		pdsci.pDynamicStates = ds;

		VkPipeline pipeline{ 0 };
		VkGraphicsPipelineCreateInfo gpci;
		memset(&gpci, 0, sizeof(gpci));
		gpci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		gpci.stageCount = 2;
		gpci.pStages = pssci;
		gpci.pVertexInputState = &pvisci;
		gpci.pInputAssemblyState = &piasci;
		gpci.pViewportState = &pvsci;
		gpci.pRasterizationState = &prsci;
		gpci.pMultisampleState = &pmsci;
		gpci.pDepthStencilState = &pdssci;
		gpci.pColorBlendState = &pcbsci;
		gpci.pDynamicState = &pdsci;
		gpci.flags = 0;
		gpci.layout = pipelineLayout_;
		gpci.renderPass = context_.renderPass();
		gpci.subpass = 0;
		gpci.basePipelineHandle = VK_NULL_HANDLE;
		gpci.basePipelineIndex = -1;
		vkCreateGraphicsPipelines(context_.device(), pc, 1, &gpci, nullptr, &pipeline_);

		vertex_offset_ = sizeof(struct ubo);
		colors_offset_ = vertex_offset_ + sizeof(vVertices);
		normals_offset_ = colors_offset_ + sizeof(vColors);
		uint32_t mem_size = normals_offset_ + sizeof(vNormals);

		VkBufferCreateInfo bci;
		memset(&bci, 0, sizeof(bci));
		bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bci.size = mem_size;
		bci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bci.flags = 0;

		vkCreateBuffer(context_.device(), &bci, nullptr, &buffer_);

		VkMemoryRequirements reqs;
		vkGetBufferMemoryRequirements(context_.device(), buffer_, &reqs);
		int memory_type = context_.findMemoryType(reqs.memoryTypeBits);
		if (memory_type < 0) {
			throw std::runtime_error("memory type bits failed");
		}

		VkMemoryAllocateInfo mai;
		memset(&mai, 0, sizeof(mai));
		mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		mai.allocationSize = mem_size;
		mai.memoryTypeIndex = memory_type;
		vkAllocateMemory(context_.device(), &mai, nullptr, &deviceMemory_);

		if (VK_SUCCESS != vkMapMemory(context_.device(), deviceMemory_, 0, mem_size, 0, (void**)&map_)) {
			throw std::runtime_error("vkMemoryMap failed");
		}
		memcpy(map_ + vertex_offset_, vVertices, sizeof(vVertices));
		memcpy(map_ + colors_offset_, vColors, sizeof(vColors));
		memcpy(map_ + normals_offset_, vNormals, sizeof(vNormals));

		vkBindBufferMemory(context_.device(), buffer_, deviceMemory_, 0);

		VkDescriptorPoolSize dps;
		memset(&dps, 0, sizeof(dps));
		dps.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		dps.descriptorCount = 1;
		VkDescriptorPool desc_pool;
		VkDescriptorPoolCreateInfo dpci;
		memset(&dpci, 0, sizeof(dpci));
		dpci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		dpci.pNext = NULL;
		dpci.flags = 0;
		dpci.maxSets = 1;
		dpci.poolSizeCount = 1;
		dpci.pPoolSizes = &dps;
		vkCreateDescriptorPool(context_.device(), &dpci, NULL, &desc_pool);

		VkDescriptorSetAllocateInfo dsai;
		memset(&dsai, 0, sizeof(dsai));
		dsai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		dsai.descriptorPool = desc_pool;
		dsai.descriptorSetCount = 1;
		dsai.pSetLayouts = &set_layout;
		vkAllocateDescriptorSets(context_.device(), &dsai, &descriptorSet_);

		VkDescriptorBufferInfo dbi;
		memset(&dbi, 0, sizeof(dbi));
		dbi.buffer = buffer_;
		dbi.offset = 0;
		dbi.range = sizeof(struct ubo);

		VkWriteDescriptorSet wds;
		memset(&wds, 0, sizeof(wds));
		wds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		wds.dstSet = descriptorSet_;
		wds.dstBinding = 0;
		wds.dstArrayElement = 0;
		wds.descriptorCount = 1;
		wds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		wds.pBufferInfo = &dbi;

		vkUpdateDescriptorSets(context_.device(), 1, &wds, 0, nullptr);

		return true;
	}


	void VulkanConfig::kill() {

	}


	void VulkanConfig::use(float t) {
		struct ubo ubo;
		ubo.modelview.translate({ 0.0f, 0.0f, -8.0f });
		ubo.modelview.rotate(45.0f + (0.25f * t), { 1.0f, 0.0f, 0.0f });
		ubo.modelview.rotate(45.0f - (0.5f * t), { 0.0f, 1.0f, 0.0f });
		ubo.modelview.rotate(10.0f + (0.15f * t), { 0.0f, 0.0f, 1.0f });
		float aspect = (float)context_.height() / (float)context_.width();
		FxMatrix4f projection;
        projection.perspective(45.0f, aspect, 1.0f, 100.0f);
		// projection.frustum(-2.8f, +2.8f, -2.8f * aspect, +2.8f * aspect, 6.0f, 10.0f);
		ubo.modelviewprojection = ubo.modelview * projection;

		// The mat3 normalMatrix is laid out as 3 vec4s. 
		memcpy(ubo.normal, &ubo.modelview, sizeof(ubo.normal));
		memcpy(map_, &ubo, sizeof(ubo));

		vkWaitForFences(context_.device(), 1, &context_.fence(), VK_TRUE, UINT64_MAX);
		vkResetFences(context_.device(), 1, &context_.fence());

		VkCommandBufferBeginInfo commandBufferBeginInfo;
		memset(&commandBufferBeginInfo, 0, sizeof(commandBufferBeginInfo));
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = 0;
		vkBeginCommandBuffer(context_.commandBuffer(), &commandBufferBeginInfo);

		VkClearValue clearValue;
		clearValue.color.float32[0] = clearColor_.r;
		clearValue.color.float32[1] = clearColor_.g;
		clearValue.color.float32[2] = clearColor_.b;
		clearValue.color.float32[3] = clearColor_.a;

		VkRenderPassBeginInfo renderPassBeginInfo;
		memset(&renderPassBeginInfo, 0, sizeof(renderPassBeginInfo));
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = context_.renderPass();
		renderPassBeginInfo.framebuffer = context_.framebuffer();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = context_.width();
		renderPassBeginInfo.renderArea.extent.height = context_.height();
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;
		vkCmdBeginRenderPass(context_.commandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkBuffer buffers[] = { buffer_, buffer_, buffer_ };
		VkDeviceSize deviceSizes[] = { vertex_offset_, colors_offset_, normals_offset_ };
		vkCmdBindVertexBuffers(context_.commandBuffer(), 0, 3, buffers, deviceSizes);
		vkCmdBindPipeline(context_.commandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
		vkCmdBindDescriptorSets(context_.commandBuffer(),
								VK_PIPELINE_BIND_POINT_GRAPHICS,
								pipelineLayout_,
								0, 1,
								&descriptorSet_, 0, nullptr);

		VkViewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)context_.width();
		viewport.height = (float)context_.height();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(context_.commandBuffer(), 0, 1, &viewport);

		VkRect2D scissor;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = context_.width();
		scissor.extent.height = context_.height();
		vkCmdSetScissor(context_.commandBuffer(), 0, 1, &scissor);

		vkCmdDraw(context_.commandBuffer(), 4, 1, 0, 0);
		vkCmdDraw(context_.commandBuffer(), 4, 1, 4, 0);
		vkCmdDraw(context_.commandBuffer(), 4, 1, 8, 0);
		vkCmdDraw(context_.commandBuffer(), 4, 1, 12, 0);
		vkCmdDraw(context_.commandBuffer(), 4, 1, 16, 0);
		vkCmdDraw(context_.commandBuffer(), 4, 1, 20, 0);

		vkCmdEndRenderPass(context_.commandBuffer());

		vkEndCommandBuffer(context_.commandBuffer());
	}


	void VulkanConfig::restore() {

	}
}
