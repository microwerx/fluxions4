#include <fluxions4_vulkan_render_pass.hpp>


namespace Fluxions {
	bool VulkanRenderPass::init(VulkanRenderPassMode mode, VkExtent2D extent) {
		if (flags.test(INIT_FLAG)) { kill(); }
		
		mode_ = mode;

		return true;
	}


	void VulkanRenderPass::kill() {
		flags.reset(INIT_FLAG);
	}


	bool VulkanRenderPass::beginFrame() {
		return true;
	}


	void VulkanRenderPass::presentFrame() {

	}
}
