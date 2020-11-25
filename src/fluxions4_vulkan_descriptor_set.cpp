#include <fluxions4_vulkan_descriptor_set.hpp>
#include <hatchetfish.hpp>


namespace Fluxions {
	bool VulkanDescriptorSet::init(VkDescriptorPool pool, VkDescriptorSetLayout layout) {
		pool_ = pool;

		_createDescriptorSet(pool, layout);

		uint32_t allocationSize = sizeof(struct StandardUbo);
		ubo_buffer_.init(allocationSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

		_createDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		_createDescriptor(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		return true;
	}


	void VulkanDescriptorSet::kill() {
		flags.reset(INIT_FLAG);
		ubo_buffer_.kill();
	}


	void VulkanDescriptorSet::useDELETEME(float t, float x) {
		VulkanContext* context = VulkanContext::GetContext();

		ubo.vert.view.loadIdentity();
		ubo.vert.view.translate({ x, 0.0f, -8.0f });
		ubo.vert.model.loadIdentity();
		ubo.vert.model.rotate(0.25f * t, { 0.0f, 1.0f, 0.0f });
		ubo.vert.model.rotate(75.0f, { 1.0f, 0.0f, 0.0f });
		ubo.vert.modelview = ubo.vert.view * ubo.vert.model;
		//ubo.vert.modelview.rotate(45.0f + (0.25f * t), { 1.0f, 0.0f, 0.0f });
		//ubo.vert.modelview.rotate(45.0f - (0.50f * t), { 0.0f, 1.0f, 0.0f });
		//ubo.vert.modelview.rotate(10.0f + (0.15f * t), { 0.0f, 0.0f, 1.0f });
		FxMatrix4f projection;
		projection.perspective(45.0f, context->aspectRatio(), 0.1f, 100.0f);
		ubo.vert.modelviewprojection = projection * ubo.vert.modelview;
		ubo.vert.color.g = 0.5f * sin(t) + 0.5f;
		memcpy(ubo.vert.normal, &ubo.vert.model, sizeof(ubo.vert.normal));
		flags.set(UBO_DIRTY_FLAG);
	}


	void VulkanDescriptorSet::copyToBuffer() {
		if (flags.test(UBO_DIRTY_FLAG)) {
			ubo_buffer_.copyToMap(0, (void*)&ubo, sizeof(ubo));
			flags.reset(UBO_DIRTY_FLAG);
		}
	}


	bool VulkanDescriptorSet::_createDescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout layout) {
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
			//VkStructureType                 sType;
			//const void*                     pNext;
			//VkDescriptorPool                descriptorPool;
			//uint32_t                        descriptorSetCount;
			//const VkDescriptorSetLayout*    pSetLayouts;
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			pool,
			1, &layout
		};
		if (vkAllocateDescriptorSets(VulkanContext::GetDevice(), &descriptorSetAllocateInfo, &descriptorSet_) != VK_SUCCESS) {
			HFLOGERROR("Could not allocate descriptor sets");
			return false;
		}
		return true;
	}


	bool VulkanDescriptorSet::_createDescriptor(uint32_t binding, VkDescriptorType type) {
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
        default:
            break;
		}
		vkUpdateDescriptorSets(VulkanContext::GetDevice(), 1, &wds, 0, nullptr);
		return true;
	}

}
