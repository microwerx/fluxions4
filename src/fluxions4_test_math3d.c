#include <fluxions4_math3d.h>


int test_math3d_colors() {
	struct col4 red = col4_make(1.0f, 0.0f, 0.0f, 1.0f);
	struct col4 grn = col4_make(0.0f, 1.0f, 0.0f, 1.0f);
	struct col4 blu = col4_make(0.0f, 0.0f, 1.0f, 1.0f);

	return 0;
}


int test_math3d_vectors() {
	struct vec3 e1 = vec3_make(1.0f, 0.0f, 0.0f);
	struct vec3 e2 = vec3_make(0.0f, 1.0f, 0.0f);
	struct vec3 e3 = cross(e1, e2);

	printf("e3 = %s", vec3_string(e3));
	return 0;
}


int test_math3d_matrices() {
	struct mat4 I = mat4_identity();
	struct mat4 R = mat4_rotate(45.0f, vec3_make(0.0f, 1.0f, 0.0f));
	struct mat4 T = mat4_translate(vec3_make(1.0f, 2.0f, 3.0f));
	struct mat4 S = mat4_scaling(vec3_make(3.0f, 4.0f, 5.0f));
	struct mat4 RT = mat4_mult(R, T);
	struct mat4 TS = mat4_mult(T, S);
	struct mat4 RTinv = mat4_inverse(RT);

	struct mat4 Ione = mat4_add(mat4_identity(), mat4_scalar(1.0f));
}


int test_math3d() {
	if (test_math3d_vectors() < 0) return -1;
	return 0;
}
