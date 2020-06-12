#ifndef FLUXIONS_VULKAN_BUFFER_HPP
#define FLUXIONS_VULKAN_BUFFER_HPP

#include <fluxions4_vulkan_context.hpp>

namespace Fluxions {
	class VulkanBuffer {
	public:
		VulkanBuffer() {}
		~VulkanBuffer() { kill(); }

		bool init(VkDeviceSize allocationSize, VkBufferUsageFlags usage);
		void kill();

		VkBuffer& buffer() { return buffer_; }
		size_t size() const { return allocationSize_; }
		bool copyToMap(size_t offset, void* src, size_t size);
	private:
		size_t allocationSize_{ 0 };
		VkBuffer buffer_{ nullptr };
		VkDeviceMemory memory_{ nullptr };
		uint8_t* map_{ nullptr };
		//VulkanContext* context_{ nullptr };
	};
}

#endif
