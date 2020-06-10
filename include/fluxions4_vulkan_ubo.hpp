#ifndef FLUXIONS4_VULKAN_UBO_HPP
#define FLUXIONS4_VULKAN_UBO_HPP

#include <fluxions4_gte_base.hpp>

namespace Fluxions {
	struct VertShaderUbo {
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
}

#endif
