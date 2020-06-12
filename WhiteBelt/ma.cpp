#include <iostream>
#include <string>
#include <fstream>
#include <set>

int main() {
    std::ifstream reader("input.txt");
    std::set<long long> set;

    long long number = 0;
    while (!reader.eof()) {
        reader >> number;
        set.insert(number);
    }

    int sum = 0;
    for (const auto& el : set) {
        sum += el;
    }
    std::ofstream writer("output.txt");
    writer << sum;
    return 0;
}