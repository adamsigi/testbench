## 1. Using `using namespace std`
It is not worth polluting the namespace and introducing bugs due to name clashes just to save some typing.
Avoid using `using namespace std` in global level, and never use it in header files.
```c++
using namespace std;  // bad
```

## 2. Using `std::endl`
The `std::endl` manipulator performs two actions:
1. It inserts a newline character ('\n') into the output stream.
2. It flushed the buffer associated with the output stream, meaning it forces any buffered data to be immediately sent to the output device.
Flushing the buffer has a performance overhead compared to just using the newline character.
```c++
std::cout << "hello, world!" << std::endl;  // use '\n' instead
```

## 3. Not using a range based for-loop
The intent to serially loop over the elements in a container is best expressed by a range based for-loop.
Use an index based for-loop only when having the index is necessary.
```c++
// bad
for (std::size_t i = 0; i < data.size(); ++i) {
    model.update(data[i]);  // the index i is only used for serial access
}

// good
for (const auto &x : data) {  // expresses the indent better
    model.update(x);
}
```

## 4. Reimplementing std functionality
Using functions from std for common tasks is safer and more readable than reimplementing the same functionality.
```c++
const std::vector<int> data = {-1, -3, -5, 8, 15, -1};
std::size_t first_pos_idx;

// bad
for (std::size_t i = 0; i < data.size(); ++i) {
    if (data[i] > 0) {
        first_pos_idx = i;
        break;
    }
}

// good
const auto is_positive = [](const auto &x) { return x > 0; };
first_pos_idx = std::find_if(data.cbegin(), data.cend(), is_positive);
```

## 5. Using a C style array instead of a std array
C style arrays often decay into pointers, and require that you carry their length.
Moreover, they are often the cause of buffer overruns.
Use std arrays instead
```c++
const int n = 256;

// bad
int c_arr[n] = {0};
func(c_arr, n);

// good
std::array<int, n> std_arr{}
func(std_arr);
```

## 6. Any use of reinterpret_cast
Objects acquired from reinterpret_cast can (almost) only be reinterpret_cast back to the original type.
Most other operations are undefined behavior.
The same applies to C style casting.
```c++
long long x = 0;
auto xp = reinterpret_cast<char *>(x);
auto x2 = reinterpret_cast<long long>(xp);
```

## 7. Casting away `const`
Avoid removing the `const` qualification from variables.
```c++
// bad
const std::string &more_frequent(const std::unordered_map<std::string, int> &word_counts,
                                 const std::string &word1,
                                 const std::string &word2) {
    auto &counts = const_cast<std::unordered_map<std::string, int> &>(word_counts);  // bad
    return counts[word1] > counts[word2] ? word1 : word2;  // [] operator in not const
}

// good
const std::string &more_frequent(const std::unordered_map<std::string, int> &word_counts,
                                 const std::string &word1,
                                 const std::string &word2) {
    return counts.at(word1) > counts.at(word2) ? word1 : word2;  // .at is const
}
```

## 8. Not knowing operator functionality
The [] operator inserts the element into a map if it does not exist.
The .at operator throws if the element does not exist in a map.

## 9. Ignoring `const` correctness
When the content of a variable is not supposed to be changed is should be `const`.
```c++
void print_vect_one_per_line(const std::vector<int> &arr) {  // arr won't be modified, hence use const
    for (const auto &x : arr) {
        std::cout << x << '\n';
    }
}
```

## 10. Not knowing string literal lifetimes
String literals live for the entire lifetime of the program.
```c++
const char *string_literal_lifetime() {
    return "string literal";  // valid; not a reference to a local variable
}
```

## 11. Not using structured bindings
Structured bindings make the code more readable if descriptive names are used.
```c++
std::unordered_map<std::string, std::string> colors = {
    {"RED",   "#FF0000"},
    {"GREEN", "#00FF00"},
    {"BLUE",  "#0000FF"}
};

for (const auto &[name, hex] : colors) {
    std::cout << "name: " << name << ", hex: " << hex << '\n';
}
```

## 12. Using multiple out parameters in a function.
To return multiple values from a function create a `struct` containing the those values and return it.
```c++
// bad
void multiple_out_params(const int n, int &x, int &y) {
    x = n;
    y = n + 1;
}

// good
struct Values {
    int x, y;
};
Values return_struct(const int n) {
    return {n, n+1};
}
```

## 13. Doing work at runtime that could have been done at compile time.
If the parameters of a computation are know at compile time use the `constexpr` to perform it at compile time instead of runtime.
```c++
constexpr int sum_of_1_to_n(const int n) {
    return n * (n + 1) / 2;
}

void uses_sum() {
    const int limit = 1000;  // parameter known at compile time
    auto triangle_n = sum_of_1_to_n(limit);
}
```

## 14. Forgetting to mark destructors as `virtual` in a class hierarchy
Assume a base class with a non virtual destructor and a derived class that inherits from it.
If a derived class object gets deleted through a pointer to the base class, the derived class destructor won't be called.
```c++
class BaseWithNonvirtualDestructor {
public:
    void foo() {
        std::cout << "do foo\n";
    }
    ~BaseWithNonvirtualDestructor() {  // should be virtual
        std::cout << "called base destructor\n";
    }
};

class Derived : public BaseWithNonvirtualDestructor {
public:
    ~Derived() {  // should be marked with override
        std::cout << "called derived destructor\n";
    }
};

void consume_base(std::unique_ptr<BaseWithNonvirtualDestructor> p) {
    p->foo();
    // If object of Derived class is passed, ~Derived won't be called.
}

```

## 15. Not knowing class member initialization order
Class members are initialized in the order they are declared in, not in the order they appear in the initializer list.
```c++
// bad
class View {
public:
    View(char *start, std::size_t size) : m_start{start}, m_end{m_start + size} {}  // order is irrelevant
private:
    char *m_end;    // m_end will be initialized first, at that point m_start is still garbage!
    char *m_start;  // m_start will be initialized second after m_end
};
```

## 16. Overuse of magic numbers/string
Introduce and reuse a basic constant instead of copying magic numbers/strings in multiple places in the code.
```c++
double energy(double mass) {
    constexpr double SPEED_OF_LIGHT = 299792458.0;
    return mass * SPEED_OF_LIGHT * SPEED_OF_LIGHT;
}
```

## 17. Editing a container while looping over it with an iterator
Adding or removing elements in a container can invalidate existing iterators.
Resizing a container might require moving all its elements to a new location.
Use index based for-loop instead.
```c++
std::vector<int> v{1, 2, 3, 4};

// bad
for (auto x : v) {
    v.push_back(x);  // might break the iterators used for looping
}

// good
const std::size_t size = v.size();
for (std::size_t i = 0; i < size; ++i) {
    v.push_back(v[i]);
}
```

## 18. Returning a moved local variable
The compiler applies return value optimization (RVO) so that no copy and no move is necessary.
If RVO is not possible the compiler will automatically do a move.
When `std::move` is specified in the code it actively prevents RVO.
```c++
// bad
std::vector<int> make_vector(const int n) {
    std::vector<int> v{1, 2, 3, 4};
    return std::move(v);  // prevents RVO
}

// good
std::vector<int> make_vector(const int n) {
    std::vector<int> v{1, 2, 3, 4};
    return v;  // compiler can do copy elision / RVO
}
```

## 19. Thinking that `std::move` moves something
exactly equivalent to a static_cast to an rvalue reference type.


