#include <iostream>

int main() {
    int N, R;
    std::cin >> N >> R;
    int W, H, D;
    uint64_t mass = 0;
    for (size_t i = 0; i < N; ++i) {
        std::cin >> W >> H >> D;
        mass += (uint64_t)W * (uint64_t)H * (uint64_t)D * (uint64_t)R;
    }

    std::cout << mass;

    return 0;
}
