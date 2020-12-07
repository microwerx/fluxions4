#include <hatchetfish.hpp>
#include <fluxions4.hpp>
#include <fluxions4_vulkan_descriptor_set.hpp>


namespace Fluxions {
	bool VulkanDescriptorSet::init(VkDescriptorPool pool, VkDescriptorSetLayout layout) {
		pool_ = pool;

		if (!_createDescriptorSet(pool, layout))
			return false;

		uint32_t allocationSize = sizeof(struct StandardUbo);
		ubo_buffer_.init(allocationSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

		_createDescriptor(FX4_VERT_UBLOCK_BINDING, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		_createDescriptor(FX4_FRAG_UBLOCK_BINDING, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		_createDescriptor(FX4_FRAG_MAP_KD_BINDING, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		return true;
	}


	void VulkanDescriptorSet::kill() {
		flags.reset(INIT_FLAG);
		ubo_buffer_.kill();
	}


	void VulkanDescriptorSet::update() {
		flags.set(UBO_DIRTY_FLAG);
	}


	void VulkanDescriptorSet::copyToBuffer() {
		if (flags.test(UBO_DIRTY_FLAG)) {
			ubo_buffer_.copyToMap(0, (void*)&ubo, sizeof(ubo));
			flags.reset(UBO_DIRTY_FLAG);
		}
	}


	bool VulkanDescriptorSet::_createDescriptorSet(VkDescriptorPool pool, const VkDescriptorSetLayout layout) {
		VkDescriptorSetAllocateInfo dsai{};
		//VkStructureType                 sType;
		//const void*                     pNext;
		//VkDescriptorPool                descriptorPool;
		//uint32_t                        descriptorSetCount;
		//const VkDescriptorSetLayout*    pSetLayouts;
		dsai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		dsai.descriptorPool = pool;
		dsai.descriptorSetCount = 1;
		dsai.pSetLayouts = &layout;
		if (vkAllocateDescriptorSets(VulkanContext::GetDevice(), &dsai, &descriptorSet_) != VK_SUCCESS) {
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

		if (!descriptorSet_) return false;

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
