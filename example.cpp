/*
 * Usage example for enumerate.hpp
 *
 */

#include <iostream>
#include <stdexcept>
#include "enumerate.hpp"


enum class Fruit {
    BEGIN,          // Define the beginning of the enum range.
    Apple = BEGIN,
    Orange,
    Pear,
    END             // Define the end of the enum range.
};


const char* name(const Fruit f) {
    switch (f) {
    case Fruit::Apple:
        return "Apple";
    case Fruit::Orange:
        return "Orange";
    case Fruit::Pear:
        return "Pear";
    default:
        throw std::out_of_range("Fruit");
    }
}


int main() {
#ifdef __cpp_variable_templates
    using enumerate::enumerate;

    // Forward ...
    for (const auto fruit : enumerate<Fruit>) {
        std::cout << name(fruit) << std::endl;
    }

    // ... and backwards!
    for (auto it = enumerate<Fruit>.rbegin(); it != enumerate<Fruit>.rend(); ++it) {
        std::cout << name(*it) << std::endl;
    }
#else
    // For C++11, the syntax is slightly different.
    using enumerate::Enumerate;

    // Mind the braces.
    for (const auto fruit : Enumerate<Fruit>{}) {
        std::cout << name(fruit) << std::endl;
    }

    // The `Enumerate` object is a compile-time constant.
    constexpr auto range = Enumerate<Fruit>{};
    for (auto it = range.rbegin(); it != range.rend(); ++it) {
        std::cout << name(*it) << std::endl;
    }
#endif
    return 0;
}
