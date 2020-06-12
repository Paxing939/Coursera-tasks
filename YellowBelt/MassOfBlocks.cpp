#include <iostream>
#include <vector>

int main() {
    int N;
    std::cin >> N;
    std::vector<int> temperatures;
    temperatures.reserve(N);
    int temperature;
    int64_t average = 0;
    for (size_t i = 0; i < N; ++i) {
        std::cin >> temperature;
        temperatures.emplace_back(temperature);
        average += temperature;
    }
    average /= N;

    std::vector<int> indexes_temperatures_higher_average;
    for (size_t i = 0; i < N; ++i) {
        if (temperatures[i] > average) {
            indexes_temperatures_higher_average.push_back(i);
        }
    }

    std::cout << indexes_temperatures_higher_average.size() << '\n';
    for (auto index : indexes_temperatures_higher_average) {
        std::cout << index << " ";
    }

    return 0;
}

