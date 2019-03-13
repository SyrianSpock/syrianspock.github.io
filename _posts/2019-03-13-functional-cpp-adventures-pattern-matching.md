---
layout: post
title: Functional Programming in C++&#58 Pattern matching
categories: ['Functional programming', 'Software']
tags: ['c++', 'cpp', 'functional', 'programming', 'linux']
---

Last year, I wrote a few posts on functional programming patterns in C++.
[One of them](/functional%20programming/software/2018/03/12/functional-cpp-adventures-either-error-handling/) discussed how to express sum types using `std::variant` for error handling.
Today, we'll see how we can do pattern matching in C++ using `std::variant` and `std::visit`.
Pattern matching is a feature commonly found in the functional programming paradigm, and it's useful to express branching (think if/else or switch statements).

# C++ has a visitor

In C++17, the standard introduces `std::visit` to run different code paths for each type defined in the `std::variant` passed to it.
This allows us to define a C++ functor (a.k.a. a class with `operator()`) that has overloads of the `operator()` for each of the types of the given `std::variant`.

Using some helpers, we can have a pretty neat syntax that almost looks as easy as Haskell or Rust pattenr matching.
Here is an example:

```cpp
#include <iomanip>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

using var_t = std::variant<int, long, double, std::string>;

// helpers copied from the standard for nicer syntax
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main() {
    std::vector<var_t> vec = {10, 15l, 1.5, "hello"};

    for (auto& v: vec) {
        std::visit(overloaded {
            [](int arg) { std::cout << "this is a int: " << arg << std::endl; },
            [](long arg) { std::cout << "this is a long: " << arg << std::endl; },
            [](double arg) { std::cout << "this is a double: " << std::fixed << arg << std::endl; },
            [](const std::string& arg) { std::cout << "this is a string: " << std::quoted(arg) << std::endl; },
        }, v);
    }
}
```

that will print the following
```
this is a int: 10
this is a long: 15
this is a double: 1.500000
this is a string: "hello"
```

Or we can return something from our visitors to perform some fancy operation

```cpp
int main() {
    std::vector<var_t> vec = {10, 15l, 1.5, "hello"};

    double sum = std::accumulate(vec.begin(), vec.end(), 0.0, [](double a, auto v) {
        return a + std::visit(overloaded {
            [](int arg) -> double { return arg; },
            [](long arg) -> double { return arg; },
            [](double arg) -> double { return arg; },
            [](const std::string& arg) -> double { return 0; },
        }, v);
    });
    std::cout << "Computed sum: " << sum << std::endl;
}
```

That will compute the sum of all elements in the vector, valuing string as zero.
We get:
```
Computed sum: 26.500000
```
Cool!
Let's try something fancier.

# Visiting Either

Remember from [the previous blog](/functional%20programming/software/2018/03/12/functional-cpp-adventures-either-error-handling/) where we defined `Either` and `>>=` (bind) to compose operations that returned an `Either` type.
Let's pay our previous implementation a visit, and introduce a `match` method that would apply a different function to `Either` depending if it's `Left` or `Right`.
```cpp
#include <variant>

template <typename E, typename T> class Either
{
private:
    std::variant<E, T> m_data;

public:
    Either(std::variant<E, T> data) : m_data(data) {}

    template<typename ApplyLeft, typename ApplyRight>
    decltype(auto) match(ApplyLeft&& left, ApplyRight&& right) const
    {
        return std::visit(overloaded {
            std::forward<ApplyLeft>(left),
            std::forward<ApplyRight>(right)
        }, m_data);
    }

    // [...]
};
```

Since we don't know what the input functions we want to apply to left and right are, we use templated types.
And since we don't know what they will return, we let the type system deduce them using `decltype(auto)`.
In that case, the `auto` is replaced by the operand of the function's return statement, and the final return type is deduced by `decltype`.

With that, we can change our implementations of operators bind `>>=` and stream `<<` to use `match`
```cpp

template <typename E, typename T, typename funcType>
decltype(auto) operator>>=(funcType &&f, const Either<E, T>& x)
{
    return x.match(
        [&](const E& e){ return x; },
        [&](const T& t){ return f(t); }
    );
}

template <typename E, typename T>
inline std::ostream& operator<<(std::ostream& os, const Either<E, T>& x)
{
    x.match(
        [&](const E& e){ os << "Value '" << e << "' is left" << std::endl; },
        [&](const T& t){ os << "Value '" << t << "' is right" << std::endl; }
    );
    return os;
}
```
Fancy schmancy!
Way cooler than if/else statements checking for `std::holds_alternative`, isn't it?
That will also make the ["anti-if"](https://francescocirillo.com/pages/anti-if-campaign) activists very happy.

# Summary

In C++17, we have (type-safe) sum types with `std::variant` and pattern matching with `std::visit`.
Still far from the power of Rust or the poetry of Haskell, but it's very nice to see how C++ is evolving to allow us to write composable and expressive code more easily.
