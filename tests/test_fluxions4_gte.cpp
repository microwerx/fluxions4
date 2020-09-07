#include "test_common.hpp"
#include <fluxions4_gte.h>

std::ostream& operator<<(std::ostream& os, const Vector3 v) {
    os << v.x << " " << v.y << " " << v.z;
    return os;
}

int main(int argc, char** argv) {
    Vector3 V3_POSITIVE_X{ 1.0f, 0.0f, 0.0f};
    Vector3 V3_NEGATIVE_X{-1.0f, 0.0f, 0.0f};
    Vector3 V3_POSITIVE_Y{ 0.0f, 1.0f, 0.0f};
    Vector3 V3_NEGATIVE_Y{ 0.0f,-1.0f, 0.0f};
    Vector3 V3_POSITIVE_Z{ 0.0f, 0.0f, 1.0f};
    Vector3 V3_NEGATIVE_Z{ 0.0f, 0.0f,-1.0f};
    std::cout << std::fixed << std::setprecision(5);
    std::cout << V3_POSITIVE_X << "\n";
    std::cout << dot(V3_POSITIVE_X, V3_POSITIVE_X) << "\n";
    std::cout << dot(V3_POSITIVE_X, V3_POSITIVE_Y) << "\n";
    std::cout << dot(V3_POSITIVE_X, V3_NEGATIVE_Z) << "\n";
    std::cout << cross(V3_POSITIVE_X, V3_POSITIVE_Y) << "\n";
    return 0;
}
