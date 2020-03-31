#ifndef FLUXIONS_VULKAN_CONFIG_HPP
#define FLUXIONS_VULKAN_CONFIG_HPP

#include "fluxions_vulkan_context.hpp"

namespace Fluxions {
	// VulkanConfig is used to for a whole render pass
	class VulkanConfig {
	public:
		VulkanConfig(VulkanContext& vc);
		~VulkanConfig();

		bool init();
		void kill();
	private:

		VulkanContext& vc_;
	};
}

#endif
