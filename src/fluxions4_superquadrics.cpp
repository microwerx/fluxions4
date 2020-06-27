#include <fluxions4_superquadrics.hpp>


namespace Fluxions {
	float signum(float x) {
		if (x < -0.00001f) return -1.0f;
		if (x > 0.00001f) return 1.0f;
		return 0.0f;
	}

	float spcos(float thetaInRadians, float eta) {
		float cosine = cos(thetaInRadians);
		return signum(cosine) * pow(abs(cosine), eta);
	}

	float spsin(float thetaInRadians, float eta) {
		float sine = sin(thetaInRadians);
		return signum(sine) * pow(abs(sine), eta);
	}

	FxVector3f sqE(float n, float w, float theta, float phi) {
		FxVector3f v;
		v.x = spcos(phi, n) * spcos(theta, w);
		v.y = spcos(phi, n) * spsin(theta, w);
		v.z = spsin(phi, n);
		return v;
	}

	FxVector3f sqT(float n, float w, float alpha, float theta, float phi) {
		FxVector3f v;
		v.x = 1.0 / alpha * (alpha + spcos(phi, n)) * spcos(theta, w);
		v.y = 1.0 / alpha * (alpha + spcos(phi, n)) * spsin(theta, w);
		v.z = 1.0 / alpha * spsin(phi, n);
		return v;
	}

	FxVector3f sqEN(float n, float w, float theta, float phi) {
		float two_minus_n = 2.0f - n;
		float two_minus_w = 2.0f - w;
		FxVector3f v;
		v.x = spcos(phi, two_minus_n) * spcos(theta, two_minus_w);
		v.y = spcos(phi, two_minus_n) * spsin(theta, two_minus_w);
		v.z = spsin(phi, two_minus_n);
		return normalize(v);
	}

	FxVector3f sqENormal(float n, float w, float theta, float phi) {
		constexpr float delta = 0.001f;
		FxVector3f a = sqE(n, w, theta - delta, phi);
		FxVector3f b = sqE(n, w, theta + delta, phi);
		FxVector3f c = sqE(n, w, theta, phi - delta);
		FxVector3f d = sqE(n, w, theta, phi + delta);
		FxVector3f edge1 = b - a;
		FxVector3f edge2 = d - c;
		FxVector3f N = cross(edge1, edge2);
		return normalize(N);
	}

	FxVector3f sqTNormal(float n, float w, float alpha, float theta, float phi) {
		constexpr float delta = 0.001f;
		FxVector3f a = sqT(n, w, alpha, theta - delta, phi);
		FxVector3f b = sqT(n, w, alpha, theta + delta, phi);
		FxVector3f c = sqT(n, w, alpha, theta, phi - delta);
		FxVector3f d = sqT(n, w, alpha, theta, phi + delta);
		FxVector3f edge1 = b - a;
		FxVector3f edge2 = d - c;
		FxVector3f N = cross(edge1, edge2);
		return normalize(N);
	}

	void CreateSuperquadricToroid(VulkanMesh& mesh, float eta1, float eta2, float alpha, uint32_t slices, uint32_t stacks) {
		mesh.init();

		const uint32_t NumQuadrants = 8;
		const uint32_t NumVerticesPerQuadrant = (1 + slices) * (1 + stacks);
		uint32_t curVertex = 0;
		mesh.resizeVertices(NumQuadrants * NumVerticesPerQuadrant);

		constexpr float TWOPI = 6.28318531f;
		constexpr float PI = 3.141592653f;
		constexpr float PI2 = PI * 0.5f;
		constexpr float PI4 = PI * 0.25f;
		constexpr float PI8 = PI * 0.125f;

		FxVector3f quadrants[8] = {
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f},
		};

		VulkanVertex v;

		//eta1 = 0.3f;// 0.1 + 0.5 * eta1;
		//eta2 = 0.0f;// 0.1 + 0.5 * eta2;

		uint32_t vcount = 0;
		for (uint32_t q = 0; q < NumQuadrants; q++) {
			if (q == 0) {
				float ds = 1.0f / slices;
				float dt = 1.0f / stacks;

				const float dphi = PI / stacks;
				const float dtheta = PI2 / slices;

				float phi = -PI;
				for (uint32_t stack = 0; stack <= stacks; stack++) {
					float theta = 0;
					//if (stack == stacks) phi = 0;
					for (uint32_t slice = 0; slice <= slices; slice++) {
						//if (slice == slices) theta = -PI2;
						v.position = sqT(eta1, eta2, alpha, theta, phi);
						v.normal = sqTNormal(eta1, eta2, alpha, theta, phi);
						if (slice & 1) {
							v.color = (stack & 1) ? FxVector3f{ 0.0f, 1.0f, 1.0f } : FxVector3f{ 1.0f, 0.0f, 1.0f };
						}
						else {
							v.color = (stack & 1) ? FxVector3f{ 0.0f, 0.5f, 0.5f } : FxVector3f{ 0.5f, 0.0f, 0.5f };
						}
						v.texcoord = { slice * ds, stack * dt, 0.0f };
						mesh.vertices[vcount] = v;
						vcount++;

						theta += dtheta;
					}
					phi += dphi;
				}
			}
			else {
				const uint32_t start = q * NumVerticesPerQuadrant;
				// flip the signs on the coordinates
				for (uint32_t i = 0; i < vcount; i++) {
					VulkanVertex v_j = mesh.vertices[i];
					v_j.position *= quadrants[q];
					v_j.normal *= quadrants[q];
					mesh.vertices[start + i] = v_j;
				}
			}
		}

		const uint32_t w = 1 + slices;
		const uint32_t h = stacks;
		const uint32_t numDegenerate = stacks - 1 + (NumQuadrants - 1);

		uint32_t count = 0;
		mesh.resizeIndices(NumQuadrants * (numDegenerate + h * w) * 2);
		for (uint32_t q = 0; q < NumQuadrants; q++) {
			uint32_t start = q * vcount;
			for (uint32_t j = 0; j < h; j++) {
				for (uint32_t i = 0; i < w; i++) {
					mesh.indices[count++] = start + j * w + i;
					mesh.indices[count++] = start + (j + 1) * w + i;
				}

				if (j < h - 1) {
					// form degenerate triangle
					mesh.indices[count] = mesh.indices[count - 1];
					count++;
					mesh.indices[count++] = start + (j + 1) * w;
				}
			}
			if (q != NumQuadrants - 1) {
				mesh.indices[count] = mesh.indices[count - 1];
				count++;
				mesh.indices[count++] = (q + 1) * vcount;
			}
		}

		//uint32_t vertexCount;
		//uint32_t instanceCount;
		//uint32_t firstVertex;
		//uint32_t firstInstance;
		mesh.drawSurface({ count, 1, 0, 0 });
		mesh.copyToBuffer();
	}



	void CreateSuperquadric(VulkanMesh& mesh, float eta1, float eta2, uint32_t slices, uint32_t stacks) {
		mesh.init();

		const uint32_t NumQuadrants = 8;
		const uint32_t NumVerticesPerQuadrant = (1 + slices) * (1 + stacks);
		uint32_t curVertex = 0;
		mesh.resizeVertices(NumQuadrants * NumVerticesPerQuadrant);

		constexpr float TWOPI = 6.28318531f;
		constexpr float PI = 3.141592653f;
		constexpr float PI2 = PI * 0.5f;
		constexpr float PI4 = PI * 0.25f;
		constexpr float PI8 = PI * 0.125f;

		FxVector3f quadrants[8] = {
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f},
		};

		VulkanVertex v;

		uint32_t vcount = 0;
		for (uint32_t q = 0; q < NumQuadrants; q++) {
			if (q == 0) {
				float ds = 1.0f / slices;
				float dt = 1.0f / stacks;

				const float dphi = PI2 / stacks;
				const float dtheta = PI2 / slices;
				const float alpha = 2.0f;

				float phi = -PI2;
				for (uint32_t stack = 0; stack <= stacks; stack++) {
					float theta = 0;
					//if (stack == stacks) phi = 0;
					for (uint32_t slice = 0; slice <= slices; slice++) {
						//if (slice == slices) theta = -PI2;
						v.position = sqE(eta1, eta2, theta, phi);
						v.normal = sqENormal(eta1, eta2, theta, phi);
						if (slice & 1) {
							v.color = (stack & 1) ? FxVector3f{ 0.0f, 1.0f, 1.0f } : FxVector3f{ 1.0f, 0.0f, 1.0f };
						}
						else {
							v.color = (stack & 1) ? FxVector3f{ 0.0f, 0.5f, 0.5f } : FxVector3f{ 0.5f, 0.0f, 0.5f };
						}
						v.texcoord = { slice * ds, stack * dt, 0.0f };
						mesh.vertices[vcount] = v;
						vcount++;

						theta += dtheta;
					}
					phi += dphi;
				}
			}
			else {
				const uint32_t start = q * NumVerticesPerQuadrant;
				// flip the signs on the coordinates
				for (uint32_t i = 0; i < vcount; i++) {
					VulkanVertex v_j = mesh.vertices[i];
					v_j.position *= quadrants[q];
					v_j.normal *= quadrants[q];
					mesh.vertices[start + i] = v_j;
				}
			}
		}

		const uint32_t w = 1 + slices;
		const uint32_t h = stacks;
		const uint32_t numDegenerate = stacks - 1 + (NumQuadrants - 1);

		uint32_t count = 0;
		mesh.resizeIndices(NumQuadrants * (numDegenerate + h * w) * 2);
		for (uint32_t q = 0; q < NumQuadrants; q++) {
			uint32_t start = q * vcount;
			for (uint32_t j = 0; j < h; j++) {
				for (uint32_t i = 0; i < w; i++) {
					mesh.indices[count++] = start + j * w + i;
					mesh.indices[count++] = start + (j + 1) * w + i;
				}

				if (j < h - 1) {
					// form degenerate triangle
					mesh.indices[count] = mesh.indices[count - 1];
					count++;
					mesh.indices[count++] = start + (j + 1) * w;
				}
			}
			if (q != NumQuadrants - 1) {
				mesh.indices[count] = mesh.indices[count - 1];
				count++;
				mesh.indices[count++] = (q + 1) * vcount;
			}
		}

		//uint32_t vertexCount;
		//uint32_t instanceCount;
		//uint32_t firstVertex;
		//uint32_t firstInstance;
		mesh.drawSurface({ count, 1, 0, 0 });
		mesh.copyToBuffer();
	}
}
