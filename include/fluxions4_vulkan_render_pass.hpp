#ifndef FLUXIONS4_VULKAN_RENDERER_HPP
#define FLUXIONS4_VULKAN_RENDERER_HPP

#include <fluxions4_vulkan_context.hpp>

namespace Fluxions {
	enum class VulkanRenderPassMode {
		SWAPCHAIN_MAILBOX = 0,
		SWAPCHAIN_FIFO = 1,
		RENDER_TO_TEXTURE2D = 2,
		RENDER_TO_CUBEMAP = 3,
	};

	class VulkanRenderPass {
	public:
		VulkanRenderPass() {}
		~VulkanRenderPass() { kill(); }

		bool init(VulkanRenderPassMode mode, VkExtent2D extent);
		void kill();

		void setClearColor(FxColor4f color4) { memcpy(&clearValues[0].color, &color4, sizeof(FxColor4f)); }
		void setClearDepthStencil(float depth, uint32_t stencil) { clearValues[1].depthStencil.depth = depth, clearValues[1].depthStencil.stencil = stencil; }
		bool beginFrame();
		void presentFrame();

	private:
		VkClearValue clearValues[2] = { { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } };

		VulkanRenderPassMode mode_;

		static constexpr uint32_t INIT_FLAG = 1;
		std::bitset<32> flags;
	};
}

#endif
