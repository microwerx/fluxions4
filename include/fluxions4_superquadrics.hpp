#ifndef FLUXIONS_SUPERQUADRICS_HPP
#define FLUXIONS_SUPERQUADRICS_HPP


#include <fluxions4_vulkan_mesh.hpp>


namespace Fluxions {
	void CreateSuperquadric(VulkanMesh& mesh, float eta1, float eta2, uint32_t slices, uint32_t stacks);
}

#endif
