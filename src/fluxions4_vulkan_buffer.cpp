#include <fluxions4_vulkan_buffer.hpp>


namespace Fluxions {
	VulkanBuffer::VulkanBuffer() {

	}


	VulkanBuffer::~VulkanBuffer() {

	}


	bool VulkanBuffer::init(VulkanContext* context, VkDeviceSize allocationSize, VkBufferUsageFlags usage) {
		context_ = context;
		if (!context_) throw std::runtime_error("VulkanBuffer::init() -> context is NULL");

		if (allocationSize_) {
			kill();
		}

		allocationSize_ = allocationSize;

		VkBufferCreateInfo bufferCreateInfo = {
			//VkStructureType        sType;
			//const void*            pNext;
			//VkBufferCreateFlags    flags;
			//VkDeviceSize           size;
			//VkBufferUsageFlags     usage;
			//VkSharingMode          sharingMode;
			//uint32_t               queueFamilyIndexCount;
			//const uint32_t*        pQueueFamilyIndices;
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr, 0,
			allocationSize,
			usage,
			VK_SHARING_MODE_EXCLUSIVE,
			0, nullptr
		};
		vkCreateBuffer(context->device(), &bufferCreateInfo, nullptr, &buffer_);

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(context->device(), buffer_, &memoryRequirements);
		uint32_t memoryTypeIndex = context->findMemoryTypeIndex(memoryRequirements.memoryTypeBits);
		if (memoryTypeIndex == VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM) {
			throw std::runtime_error("memory type bits failed");
		}

		allocationSize_ = memoryRequirements.size;

		VkMemoryAllocateInfo memoryAllocationInfo = {
			//VkStructureType    sType;
			//const void*        pNext;
			//VkDeviceSize       allocationSize;
			//uint32_t           memoryTypeIndex;
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			allocationSize_,
			memoryTypeIndex
		};
		switch (vkAllocateMemory(context->device(), &memoryAllocationInfo, nullptr, &memory_)) {
		case VK_ERROR_OUT_OF_HOST_MEMORY:                 throw std::runtime_error("vkAllocateMemory() -> VK_ERROR_OUT_OF_HOST_MEMORY");
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:               throw std::runtime_error("vkAllocateMemory() -> VK_ERROR_OUT_OF_DEVICE_MEMORY");
		case VK_ERROR_TOO_MANY_OBJECTS:                   throw std::runtime_error("vkAllocateMemory() -> VK_ERROR_TOO_MANY_OBJECTS");
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:            throw std::runtime_error("vkAllocateMemory() -> VK_ERROR_INVALID_EXTERNAL_HANDLE");
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: throw std::runtime_error("vkAllocateMemory() -> VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR");
		}

		switch (vkMapMemory(context->device(), memory_, 0, allocationSize_, 0, (void**)&map_)) {
		case VK_ERROR_OUT_OF_HOST_MEMORY:   throw std::runtime_error("vkMapMemory() -> VK_ERROR_OUT_OF_HOST_MEMORY");
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: throw std::runtime_error("vkMapMemory() -> VK_ERROR_OUT_OF_DEVICE_MEMORY");
		case VK_ERROR_MEMORY_MAP_FAILED:    throw std::runtime_error("vkMapMemory() -> VK_ERROR_MEMORY_MAP_FAILED");
		}

		switch (vkBindBufferMemory(context->device(), buffer_, memory_, 0)) {
		case VK_ERROR_OUT_OF_HOST_MEMORY:                 throw std::runtime_error("vkBindBufferMemory() -> VK_ERROR_OUT_OF_HOST_MEMORY");
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:               throw std::runtime_error("vkBindBufferMemory() -> VK_ERROR_OUT_OF_DEVICE_MEMORY");
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: throw std::runtime_error("vkBindBufferMemory() -> VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR");
		};

		return true;
	}


	void VulkanBuffer::kill() {
		if (!memory_ || !buffer_) return;
		vkUnmapMemory(context_->device(), memory_);
		vkFreeMemory(context_->device(), memory_, nullptr);
		vkDestroyBuffer(context_->device(), buffer_, nullptr);
		memory_ = nullptr;
		buffer_ = nullptr;
		allocationSize_ = 0;
	}


	bool VulkanBuffer::copyToMap(size_t offset, void* src, size_t size) {
		if (size == 0) return false;
		if (offset + size > allocationSize_) return false;
		memcpy(map_ + offset, src, size);
		return true;
	}
}
