#include <fluxions4_vulkan_mesh.hpp>
#include <hatchetfish.hpp>


namespace Fluxions {


	void VulkanMesh::init() {
		flags.set(INIT_FLAG);
	}

	void VulkanMesh::kill() {
		if (flags.test(INIT_FLAG)) {
			vbo.kill();
			ibo.kill();
			flags.reset(INIT_FLAG);
		}
	}


	void VulkanMesh::resizeVertices(size_t count) {
		vertices.resize(count);
	}


	void VulkanMesh::resizeIndices(size_t count) {
		indices.resize(count);
	}


	void VulkanMesh::updateVertexData(VulkanVertex* newVertices, size_t start, size_t count) {
		if (start + count > vertices.size()) {
			HFLOGERROR("mesh vertex size is smaller than input data");
			return;
		}

		memcpy(&vertices[start], newVertices, count * sizeof(VulkanVertex));
		flags.set(VBO_DIRTY_FLAG);
	}


	void VulkanMesh::updateIndexData(uint32_t* newIndices, size_t start, size_t count) {
		if (start + count > indices.size()) {
			HFLOGERROR("mesh index size is smaller than input data");
			return;
		}

		memcpy(&indices[start], newIndices, count * sizeof(uint32_t));
		flags.set(IBO_DIRTY_FLAG);
	}


	void VulkanMesh::drawSurface(VulkanSurface surface) {
		flags.set(VBO_DIRTY_FLAG);
		flags.set(IBO_DIRTY_FLAG);
		surfaces.push_back(surface);
	}


	void VulkanMesh::clearSurfaces() {
		surfaces.clear();
	}


	void VulkanMesh::copyToBuffer() {
		if (flags.test(VBO_DIRTY_FLAG)) {
			size_t vertexSize = vertices.size() * sizeof(VulkanVertex);
			if (vertexSize > vbo.size()) {
				vbo.init(vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			}
			if (!vertices.empty()) {
				vbo.copyToMap(0, &vertices[0], vertexSize);
			}
			flags.reset(VBO_DIRTY_FLAG);
		}

		if (flags.test(IBO_DIRTY_FLAG)) {
			size_t indexSize = indices.size() * sizeof(uint32_t);
			if (indexSize > ibo.size()) {
				ibo.init(indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
			}
			if (!indices.empty()) {
				ibo.copyToMap(0, &indices[0], indexSize);
			}
			flags.reset(IBO_DIRTY_FLAG);
		}
	}


	void VulkanMesh::draw() {
		VkCommandBuffer commandBuffer = VulkanContext::GetCommandBuffer();
		if (surfaces.empty()) return;

		VkBuffer buffers[] = { vbo.buffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (indices.empty()) {
			for (auto& s : surfaces) {
				vkCmdDraw(commandBuffer, s.count, s.instanceCount, s.first, s.firstInstance);
			}
		}
		else {
			vkCmdBindIndexBuffer(commandBuffer, ibo.buffer(), 0, VK_INDEX_TYPE_UINT32);
			for (auto& s : surfaces) {
				vkCmdDrawIndexed(commandBuffer, s.count, s.instanceCount, s.first, 0, s.firstInstance);
			}
		}
	}
}