#include <iostream>
#include <vector>
#include <memory>

class Number {
    int n;
public:
    Number(int m) : n(m) {}
    int get() const { return n; }
};

int main() {
    std::vector<Number> numbers {
        Number(1),
        Number(2),
        Number(3)
    };
    for (const auto& number : numbers) {
        std::cout << number.get() << '\n';
        std::cout << number.get() << '\n';
        std::cout << number.get() << '\n';
    }

    return 0;
}