---
layout: post
title: Adventures in functional C++&#58 either error handling
categories: ['Functional programming', 'Software']
tags: ['c++', 'cpp', 'functional', 'programming', 'linux']
---

<script type="text/javascript" async
  src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML">
</script>

My previous posts have focused on using the [optional monad for error handling](/functional%20programming/software/2018/01/22/functional-cpp-adventures-optional/) and [composing functions that return an optional](/functional%20programming/software/2018/02/03/functional-cpp-adventures-function-composition-on-steroids/).
Today, I would like to discuss another monad, known to Haskellers as `Either`, and to Rust / Ocaml programmers as `Result`.

`Either` is a special case of union types that can hold one of two types:

- a valid result (a.k.a. `Right` in Haskell), or
- an error (a.k.a. `Left` in Haskell).

A common usage is to return an error message on failure, that can then be propagated down the chain and displayed at some point.

# "I want Either, I do; and I want it now!" - Antigone

Antigone might not have pronounced those exact words, but we get the excitement for `Either`.
How do we make it happen?
Luckily in C++17, we have access to sane and usable union types, through `std::variant`.
Using this we can pretty straightforwardly implement `Either` in few lines:

```cpp
#include <variant>

template <typename E, typename T> class Either
{
private:
    std::variant<E, T> m_data;

public:
    Either(std::variant<E, T> data) : m_data(data) {}

    bool isLeft() const {
        return std::holds_alternative<E>(m_data);
    }

    bool isRight() const {
        return std::holds_alternative<T>(m_data);
    }

    E fromLeft() const {
        return std::get<E>(m_data);
    }

    T fromRight() const {
        return std::get<T>(m_data);
    }
};

template <typename E, typename T> auto Left(E left)
{
    return Either<E, T>(left);
}

template <typename E, typename T> auto Right(T right)
{
    return Either<E, T>(right);
}
```

`E` is the type of the error, and `T` is the type of the result to be returned.
The basic idea is to store a union of `E` and `T` and provide methods to access the content and two factories to construct an `Either` type from a value or error type.

We can define a pretty printer for the `Either` class:

```cpp
template <typename E, typename T> inline
std::ostream& operator<<(std::ostream& os, const Either<E, T>& x)
{
    if (x.isRight())    { os << "Valid result '" << x.fromRight() << "'" << std::endl; }
    else                { os << "Error '" << x.fromLeft() << "'" << std::endl; }
    return os;
}
```

and try out this new type:

```cpp
int main(int, char **)
{
    std::cout << Right<std::string, int>(10);      // Valid result '10'
    std::cout << Left<std::string, int>("failed"); // Error 'failed'
}
```

# Functional sweetness

Let's consider our beloved square root function.
Remember that the square root of a number is only defined for positive numbers (only considering real numbers).
Now instead of using `optional` as we did in previous posts to return an invalid result, we can use `Either`.
This time we get to return an error message giving more insight on the cause of error:

```cpp
Either<std::string, double> squareRoot(double x)
{
    if (x >= 0) { return Right<std::string, double>(sqrt(x)); }
    else        { return Left<std::string, double>("Negative number has no square root"); }
}
```

We can try out our new square root implementation

```cpp
int main(int, char **)
{
    std::cout << squareRoot(10);  // Valid result '3.16228'
    std::cout << squareRoot(-10); // Error 'Negative number has no square root'
}
```

# There's no free lunch

All of that is nice but not very useful.
When you get your hands dirty with code, you end up chaining multiple functions.
If we do it with the current implementation, the code quickly gets too verbose.

For example, if I want to compute

$$
\begin{align*}
  f \colon \{\forall x | x \gt 0 \} &\to \mathbb{R}\\
  x &\mapsto \frac{1}{\sqrt{x}}
\end{align*}
$$

then I can write:

```cpp
Either<std::string, double> squareRoot(double x)
{
    if (x >= 0) { return Right<std::string, double>(sqrt(x)); }
    else        { return Left<std::string, double>("Negative number has no square root"); }
}

Either<std::string, double> divide(double x, double y)
{
    if (y != 0) { return Right<std::string, double>(x / y); }
    else        { return Left<std::string, double>("Division by zero"); }
}

Either<std::string, double> f(double x)
{
    auto oneOver = [](auto x){ return divide(1, x); };
    auto root = squareRoot(x);
    if (root.isRight()) { return oneOver(root.fromRight()); }
    else                { return root; }
}

int main(int, char **)
{
    std::cout << f(10);  // Valid result '0.316228'
    std::cout << f(0);   // Error 'Division by zero'
    std::cout << f(-10); // Error 'Negative number has no square root'
}
```

This is working code, but it's still ugly.
We have to explicitly check what the first layer returns, and select if we run the next function or return.
And this is just with 2 layers of composition, imagine 3, 4, or 10 layers!

# Taking it to the next level

One way to address this problem is to define the monadic `bind` operator for `Either`.
Again, this is easily implemented as:

```cpp
template <typename E, typename T, typename funcType>
auto operator>>=(funcType &&f, const Either<E, T>& x)
{
    if (x.isRight())    { return f(x.fromRight()); }
    else                { return x; }
}
```

Which simplifies things greatly:

```cpp
Either<std::string, double> squareRoot(double x)
{
    if (x >= 0) { return Right<std::string, double>(sqrt(x)); }
    else        { return Left<std::string, double>("Negative number has no square root"); }
}

Either<std::string, double> divide(double x, double y)
{
    if (y != 0) { return Right<std::string, double>(x / y); }
    else        { return Left<std::string, double>("Division by zero"); }
}

Either<std::string, double> f(double x)
{
    auto oneOver = [](auto x){ return divide(1, x); };
    return oneOver >>= squareRoot(x);
}
```

Now that makes it more readable IMO.
You see the two functions `oneOver` and `squareRoot` being chained, and are not bothered by the lower lever things going one with the `Either` type.

# Summary

`Either` is a another example of the power of monads as a programming idiom.
They allow us to abstract away error handling, and focus on building components that work independently, that we can then glue together easily and achieve high readability.

There are most probably more improvements that can be made to this "naive" implementation of `Either` in C++.
However, it remains an interesting experience to play with this in practice to better understand it and grasp the extent of its power.
