#ifndef FLUXIONS4_HPP
#define FLUXIONS4_HPP

#include <fluxions_gte_matrix4.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions {
	struct VertPushConstants {
		Matrix4f modelMatrix;
	};
	
	struct FragPushConstants {
		Color4f Kd;
		Color4f Ks;
		Vector4f Mixes;
	};

	constexpr unsigned FX4_NUM_DESCRIPTOR_BINDINGS = 3;

	constexpr unsigned FX4_VERT_UBLOCK_BINDING = 0;
	constexpr unsigned FX4_FRAG_UBLOCK_BINDING = 1;
	constexpr unsigned FX4_FRAG_MAP_KD_BINDING = 2;

	constexpr unsigned FX4_NUM_PUSH_CONSTANTS = 2;
	
	constexpr unsigned FX4_VERT_PUSH_CONSTANTS_BINDING = 0;
	constexpr unsigned FX4_VERT_PUSH_CONSTANTS_OFFSET = 0;
	constexpr unsigned FX4_VERT_PUSH_CONSTANTS_SIZE = sizeof(VertPushConstants);

	constexpr unsigned FX4_FRAG_PUSH_CONSTANTS_BINDING = 1;
	constexpr unsigned FX4_FRAG_PUSH_CONSTANTS_OFFSET = sizeof(VertPushConstants);
	constexpr unsigned FX4_FRAG_PUSH_CONSTANTS_SIZE = sizeof(FragPushConstants);
}

#endif
