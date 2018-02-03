---
layout: post
title: Adventures in functional C++&#58 function composition on steroids
categories: ['Functional programming', 'Software']
tags: ['c++', 'cpp', 'functional', 'programming', 'linux']
---

<script type="text/javascript" async
  src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML">
</script>

[In the previous post](/functional%20programming/software/2018/01/22/functional-cpp-adventures-optional/), we discussed the optional idiom introduced to C++ starting from C++17.
Although `optional` is not implemented as a monad in C++, we can treat it as such and implement the missing parts to achieve greater function composition.

> But what is a monad?

For the purpose of this article, think about monads as a design pattern that allows us to:

- define types,
- compose them,
- apply functions to them.

This pattern is powerful as it enables better function composition, you'll see why later, bear with me.

Many functions we write in our code can fail.
[We saw previously](/functional%20programming/software/2018/01/22/functional-cpp-adventures-optional/) how three patterns can be used to express failure.
Here we will focus on the solution involving `optional` because `optional` already allows us to define types, so that fulfills the first element in the description of the monad pattern I gave above.
Let's see how we can build up the other two missing elements of the monad pattern.

# Toy problem

Let's say I want to implement the following mathematical function:

$$
\begin{align*}
  f \colon \{\forall x | -1 \leq x \leq 1 \setminus 0\} &\to \mathbb{R}\\
  x &\mapsto \frac{1}{\arcsin{x}}
\end{align*}
$$

You might ask yourself why on earth would do we need to compute such an evil function that is only valid for non-zero numbers between -1 and 1?
Well.. life is unfair, get over it.

One approach to implementing this would be using a single function:

```cpp
std::optional<double> my_single_function(double x)
{
    if (x >= -1 && x <= 1 && x != 0) { return 1 / asin(x); }
    else                             { return {}; }
}
```

and that looks fine.

# Breaking it down

For the sake of discussion, let's assume I'm writing a math library and would like to reuse each building block (arcsine and inverse) with safe input domain checks.
The function can be defined as the composition of two functions, the inverse and the arcsine:

$$
\begin{align*}
  inverse \colon \mathbb{R}^* &\to \mathbb{R}\\
  x &\mapsto \frac{1}{x}
\end{align*}
$$

$$
\begin{align*}
  arcsine \colon x \in [-1, 1] &\to \mathbb{R}\\
  x &\mapsto \arcsin{x}
\end{align*}
$$

$$
\begin{align*}
  f = inverse \circ arcsine
\end{align*}
$$

The implementation can be broken down to reflect this:

```cpp
std::optional<double> inverse(double x)
{
    if (x != 0) { return 1 / x; }
    else        { return {}; }
}

std::optional<double> arcsine(double x)
{
    if (x >= -1 && x <= 1) { return asin(x); }
    else                   { return {}; }
}

std::optional<double> my_function(double x)
{
    if (const auto y = arcsine(x)) { return inverse(y.value()); }
    else                           { return {}; }
}
```

Well that looks uglier and less efficient.
We introduced smaller building blocks which makes the code more modular, but function composition sucks.
The return value of the first call to `inverse` needs to be checked, and only if it contains some value do we feed it to the following call to `arcsine`.
There is too much noise introduced by the use of `optional`, and noise builds up as we chain functions, what can we do about it?

# Bind operator in Haskell

In Haskell, this problem could be solved by writing the following:

```hs
inverse :: Float -> Maybe Float
inverse 0 = Nothing
inverse x = Just(1 / x)

arcsine :: Float -> Maybe Float
arcsine x
    | -1 <= x && x <= 1 = Just(asin x)
    | otherwise         = Nothing

f x = x >>= arcsine >>= inverse
```

Haskell provides the `>>=` operator for monads (including the `Maybe` monad) which is called the bind operator.
This allows us to chain functions of type signature `a -> M a`: functions that take a type as input and return a type wrapped in a monad.
Functions can be chained easily and if some step fails, the failure propagates through.

# Bind operator in C++

We can implement the bind operator for `optional` like this:

```cpp
template <typename T, typename funcType>
auto operator>>=(const std::optional<T>& x, funcType &&f)
{
    if (x)  { return f(x.value()); }
    else    { return decltype(f(std::declval<T>())){}; }
}
```

Then we can redefine `my_function` more efficiently as:

```cpp
std::optional<double> my_function(double x)
{
    return (std::optional<double>(x) >>= arcsine) >>= inverse;
}
```

which looks a lot like Haskell.
With the bind operator we can achieve more efficient function composition using the `optional` idiom.
This fulfills the 2 last elements of the monad pattern as defined above.

# Summary

Monads are a very powerful pattern that brings function composition to the next level.
Implementing the bind operator is crucial in using the monad pattern in C++.
This is clearly not perfect, and mostly an attempt to understand how the monad pattern can make C++ code more readable and maintainable.
A few interesting things to try next are:

- implementing more syntactic sugar (an equivalent to the [do notation in Haskell](https://en.wikibooks.org/wiki/Haskell/do_notation)),
- defining the bind operator for other templated types in C++ such as `std::variant`.
