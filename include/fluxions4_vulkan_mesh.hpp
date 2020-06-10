#ifndef FLUXIONS_VULKAN_MESH_HPP
#define FLUXIONS_VULKAN_MESH_HPP

#include <fluxions4_gte_base.hpp>

namespace Fluxions {
	struct VulkanVertex {
		FxVector3f position;
		FxVector3f normal;
		FxVector3f color;
		FxVector3f texCoord;
	};

	class VulkanMesh {
	public:
		VulkanMesh();
		~VulkanMesh();
	};
}

#endif
