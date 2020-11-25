#ifndef FLUXIONS4_VULKAN_DESCRIPTOR_SET_HPP
#define FLUXIONS4_VULKAN_DESCRIPTOR_SET_HPP

#include <fluxions4_gte_base.hpp>
#include <fluxions4_vulkan_context.hpp>
#include <fluxions4_vulkan_buffer.hpp>

namespace Fluxions {
	struct VertShaderUbo {
		FxMatrix4f model;
		FxMatrix4f view;
		FxMatrix4f modelview;
		FxMatrix4f modelviewprojection;
		float normal[12];
		FxColor4f color{ 0.1f, 0.2f, 0.3f, 1.0f };
	};

	struct FragShaderUbo {
		FxColor4f Kd{ 1.0f, 1.0f, 1.0f, 1.0f };
		FxColor4f Ks{ 1.0f, 1.0f, 1.0f, 1.0f };
		float spec_roughness;
	};

	struct StandardUbo {
		VertShaderUbo vert;
		FragShaderUbo frag;
	};

	class VulkanDescriptorSet {
	public:
		VulkanDescriptorSet() {}
		~VulkanDescriptorSet() { kill(); }

		bool init(VkDescriptorPool pool, VkDescriptorSetLayout layout);
		void kill();
		
		void useDELETEME(float t, float x);
		void copyToBuffer();

		VkDescriptorSet& descriptorSet() { return descriptorSet_; }

		StandardUbo ubo;

	private:
		static constexpr uint32_t INIT_FLAG = 1;
		static constexpr uint32_t UBO_DIRTY_FLAG = 2;
		std::bitset<32> flags;
		VkDescriptorSet descriptorSet_{};
		VulkanBuffer ubo_buffer_;

		VkDescriptorPool pool_{};

		bool _createDescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout layout);
		bool _createDescriptor(uint32_t binding, VkDescriptorType type);
	};
}

#endif
