#include <fstream>

int main() {
    std::ofstream fout("input.txt");

    for(int i = 0; i < 1000; i++) {
        fout << i << '\n';
    }

    return 0;
}