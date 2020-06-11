#include <fluxions4_superquadrics.hpp>


namespace Fluxions {
	float spcos(float thetaInRadians, float eta) {
		float cosine = cos(thetaInRadians);
		float sign = cosine < 0.0 ? -1.0f : 1.0f;
		return sign * pow(abs(cosine), eta);
	}

	float spsin(float thetaInRadians, float eta) {
		float sine = sin(thetaInRadians);
		float sign = sine < 0.0 ? -1.0f : 1.0f;
		return sign * pow(abs(sine), eta);
	}

	FxVector3f sqE(float n, float w, float theta, float phi) {
		FxVector3f v;
		v.x = spcos(phi, n) * spcos(theta, w);
		v.y = spcos(phi, n) * spsin(theta, w);
		v.z = spsin(phi, n);
		return v;
	}

	FxVector3f sqT(float n, float w, float theta, float phi) {
		FxVector3f v;
		v.x = (2.0f + spcos(phi, n)) * spcos(theta, w);
		v.y = (2.0f + spcos(phi, n)) * spsin(theta, w);
		v.z = spsin(phi, n);
		return v;
	}

	FxVector3f sqEN(float n, float w, float theta, float phi) {
		float f = n * w *
			pow(sin(phi), n - 1.0f) * pow(cos(phi), 2.0f * n - 1.0f) *
			pow(sin(theta), w - 1.0f) * pow(cos(theta), w - 1.0f);
		float cn = cos(phi);
		float sn = sin(phi);
		float cw = cos(theta);
		float sw = sin(theta);
		float two_minus_n = 2.0f - n;
		float two_minus_w = 2.0f - w;
		FxVector3f v;
		v.x = spcos(phi, two_minus_n) * spcos(theta, two_minus_w);
		v.y = spcos(phi, two_minus_n) * spsin(theta, two_minus_w);
		v.z = spsin(phi, two_minus_n);
		return normalize(v);
	}

	VulkanMesh CreateSuperquadric(float eta1, float eta2, uint32_t slices, uint32_t stacks) {
		VulkanMesh mesh;

		uint32_t curVertex = 0;
		mesh.resizeVertices(2 * (1 + slices) * (1 + stacks));

		constexpr float TWOPI = 6.28318531f;
		constexpr float PI = 3.141592653f;
		constexpr float PI2 = PI * 0.5f;
		constexpr float PI4 = PI * 0.25f;

		VulkanVertex v[2];

		uint32_t vcount = 0;
		float phi = -PI2;
		float dphi = PI / stacks;
		float dtheta = TWOPI / slices;
		for (uint32_t stack = 0; stack <= stacks; stack++) {
			float theta = -PI;
			for (uint32_t slice = 0; slice <= slices; slice++) {

				// top and bottom
				v[0].position = sqE(eta1, eta2, theta, phi + dphi);
				v[0].normal = sqEN(eta1, eta2, theta, phi + dphi);
				v[0].color = { 0.0f, 1.0f, 1.0f };// v[0].normal;
				v[1].position = sqE(eta1, eta2, theta, phi);
				v[1].normal = sqEN(eta1, eta2, theta, phi);
				v[1].color = { 1.0f, 0.0f, 1.0f };// v[1].normal;

				mesh.updateVertexData(v, curVertex, 2);
				curVertex += 2;
				theta += dtheta;
			}
			//mesh.drawSurface({ slices * 2 + 2, 1, vcount, 0 });
			vcount += slices * 2 + 2;
			phi += dphi;
		}
		mesh.drawSurface({ curVertex, 1, 0, 0 });

		//mesh.resizeIndices()
		//for (uint32_t slice = 0; slice < slices; slice++) {
		//	for (uint32_t stack = 0; stack < stacks; stack++) {
		//		//uint32_t vertexCount;
		//		//uint32_t instanceCount;
		//		//uint32_t firstVertex;
		//		//uint32_t firstInstance;
		//		mesh.drawSurface({ 4, 1, 0, 0 });
		//	}
		//}

		return mesh;
	}
}
