#include <iostream>
#include <ranges>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    for (auto x : std::views::iota(0) | std::views::take((argc > 1) ? std::stoi(argv[1]) : 10) | 
            std::views::transform([](int i) { return 3.0 * pow(i, 4) - 7 * pow(i, 3) + 2.0; })) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}
