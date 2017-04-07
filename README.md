# Enumerate.hpp – C++ Templates to Iterate Over Enums

This header contains a function `enumerate()` that allows you to
iterate over an `enum`.

## Example

```c++
#include <iostream>
#include <stdexcept>
#include <enumerate.hpp>


enum class Fruit {
    BEGIN,          // Define the beginning of the enum range.
    Apples = BEGIN,
    Oranges,
    Pears,
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
    using enumerate::enumerate;
    for (const auto fruit : enumerate<Fruit>) {
        std::cout << name(fruit) << std::endl;
    }
    return 0;
}
```

For C++11, where variable templates don't exist yet, the syntax is
slightly different:
```c++
// Mind the capital E.
using enumerate::Enumerate;
for (const auto fruit : Enumerate<Fruit>()) {
    std::cout << name(fruit) << std::endl;
}
```


## Installing

This is only a small header file. Just drop `enumerate.hpp` in your
project folder or somewhere on your include path.


## Contributing

Issues and pull requests are greatly appreciated. If you'd like to
contribute, please fork the repository and use a feature branch.


## Licensing

This project is licensed under the MIT license. for more information,
see the file `LICENSE`.
