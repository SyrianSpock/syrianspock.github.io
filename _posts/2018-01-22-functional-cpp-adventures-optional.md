---
layout: post
title: Functional Programming in C++&#58 Optional
categories: ['Functional programming', 'Software']
tags: ['c++', 'cpp', 'functional', 'programming', 'linux']
---

<script type="text/javascript" async
  src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML">
</script>

I have been interested lately in functional programming, especially Haskell.
After delving for a few weeks in the realm of category theory, strong types, functors, and monads, I have reemerged with a changed mind.
Many concepts in functional programming seem complex on the surface because they rely on heavy mathematical formalism.
Once you understand them, you realize the formalism is what makes algebra applicable to types and functions.
The composability this brings to code is remarkably powerful.

<figure>
    <center>
    <img src="/images/haskell.png" alt="Haskell enlightenment">
    <figcaption>
            <small>
                Source: <a href="http://leftoversalad.com/c/015_programmingpeople/">http://leftoversalad.com/c/015_programmingpeople/</a>
            </small>
    </figcaption>
    </center>
</figure>

My day job involves writing, refactoring and maintaining C++ code that contains a lot of business logic and mathematical algorithms.
So I have been looking for ways to apply functional concepts to help make code more expressive, more readable and thus easier to maintain and understand.

C++ has been incorporating functional concepts since lambdas in C++11.
And various libraries and blog posts have considered implementing option types, union types, and even monads in C++.
For example, the Boost library has `optional` (option type) and `variant` (union type) for a while now.


# C++17 compiler support

Modern compilers (clang, gcc) are been shipping C++17 features last year, so [only newer versions have full support of C++17 features](http://en.cppreference.com/w/cpp/compiler_support#cpp17).
Using clang 5 or gcc 7 is a safe bet.
To simplify my life I set up a simple Docker image with clang 5 for my experiments.
The Dockerfile is straightforward and can be found on [my github](https://github.com/SyrianSpock/dockerfiles).
Now we're ready to build and run code!


# Functional sweetness: `std::optional`

C++17 introduces the option type `optional`, also known by Haskellers as `Maybe`, or `Option` for Rust / Ocaml programmers.
`optional` wraps an existing type to allow representation of the non-existence of the value: `{}`.
See the [C++ reference for more juicy details](http://en.cppreference.com/w/cpp/utility/optional).

For example, an `optional<char>` has 257 possible values:

- 256 for the `char`
- 1 for `{}` (nothing)

This enables us to represent no-values in a very intuitive manner that makes it explicit if there is a value or no in a given variable.
Let's say I want to perform some computation that may return no value, for example the inverse of a number.
The inverse function $$f$$ can be defined as:

$$
\begin{align*}
  f \colon \mathbb{R}^* &\to \mathbb{R}\\
  x &\mapsto \frac{1}{x}
\end{align*}
$$

So we get an output for each value $$x$$ in the set of real numbers $$\mathbb{R}$$ except 0 (noted $$\mathbb{R}^*$$), this is called the domain of $$f$$.

This translates in Haskell to:

```hs
inverse :: (Eq a, Fractional a) => a -> Maybe a
inverse 0 = Nothing
inverse x = Just(1 / x)
```

We inform the compiler that our input type `a` must define operation `==` (`Eq a`) and implement division (`Fractional a`).
Then we return `Nothing` in case $$x = 0$$, and return the inverse otherwise.

If we implement the inverse funtion in C++, we have three main choices for handling the case $$x = 0$$:

- Return a boolean that is `true` for all values of $$x$$ except $$0$$
- Throw an exception when $$x = 0$$
- Return an `optional<double>` that contains the result for $$x \neq 0$$ or nothing otherwise

Let's look at each solution and weigh their pros and cons.

# Returning a boolean
The first solution involves returning a boolean flag, this is something you see more in C codebases, but the practice is still present among C++ programmers.
An example implementation of inverse is:

```cpp
bool inverse(double x, double& result)
{
    if (x == 0) { return false; }

    result = 1.0 / x;
    return true;
}
```

I find this code unintuitive.
The return value is passed to the function by reference as input.
The resulting client code is less expressive:

```cpp
double result;
if (inverse(/* some value */, result))
{
    /* do something with */
}
else
{
    /* do something else to handle failure */
}
```

Client code can easily forget to check the returned flag to validate that the computed result is correct.
This can lead to silent errors of computation being propagated unnoticed which can be a cause of many headaches.

# Throwing an exception
The C++ way of handling errors is throwing an exception.
An example implementation of the inverse function would look like this:

```cpp
double inverse(double x)
{
    if (x != 0) { return 1.0 / x; }
    else        { throw std::invalid_argument("Cannot compute inverse of 0"); }
}
```

This is more readable, and the client code is less cluttered, we feed in `x` and get the result as return value.
No fiddling with error codes, and code reads naturally:

```cpp
double result = inverse(/* some value */);
```

However this has one big drawback: the client may not know that your code throws.
And there is no way to inform the client that your code throws.
Exceptions are implementation details that are not exposed through the function declaration that serves as interface (header file).
The way of guaranteeing exception safety is by using the `noexcept` keyword in C++.
Writing this for each function that doesn't throw quickly becomes tedious work.

Implicit throwing forces you to read `.cpp` files before deciding if you need to wrap function calls in try-catch blocks.
And that is similar to hidden side effects a function might have.
Side effects limit our ability to reason about correctness of code, and make it much harder to compose functions.

# Maybe return a result
`std::optional` is a new tool in the modern C++ programmer's toolbox that allow intuitive representation of no-values and (some) errors.
An example implementation of inverse looks like:

```cpp
std::optional<double> inverse(double x)
{
    if (x != 0) { return 1.0 / x; }
    else        { return {}; }
}
```

The `optional` is right in the function signature, and makes it explicit that this function can fail and return nothing.
Moreover, the client is forced to deal with the failing case, there is no going around it:

```cpp
if (auto result = inverse(/* some value */))
{
    /* do something with */ result.value();
}
else
{
    /* deal with failure */
}
```

If there is a default value associated to failed computation, you can use the `value_or()` method:

```cpp
double result = inverse(/* some input value */).value_or(/* default value for failure */);
```

You can also bypass this check by fetching the value of the `optional`:

```cpp
auto result = inverse(/* some value */);
/* do something with */ result.value();
```

And this will throw if the `optional` has no value, so you get the same behavior as if the function was throwing an exception to begin with.

# Conclusion
Functional programming will not magically make your code better, but it certainly brings new paradigms into play that make you think differently about code.
Many concepts allow better expression of intent making code easier to read and understand.
We spend more time reading code than writing it, so it's worth thinking about what we write and leveraging the right tools provided by the language to build good software.

Note that in practice you may consider replacing `x == 0` checks with something like `abs(x) < 1e-7`.
Numerical computations are usually applied to noisy inputs, so we consider any value small enough (say less that $$10^{-7}$$ in this case) to be equivalent to zero.

Here are a few that are worth reading if you're interested in functional programming and C++:

- Bartosz Milewski is an eminent figure in both C++ and Haskell who wrote a lot on the subject
    * [Monads in C++](https://bartoszmilewski.com/2011/07/11/monads-in-c/)
    * [The functional revolution in C++](https://bartoszmilewski.com/2014/06/09/the-functional-revolution-in-c/)
    * [C++17 I see a monad in your future](https://bartoszmilewski.com/2014/02/26/c17-i-see-a-monad-in-your-future/)
- Fluent C++ is also an interesting read, it addresses code readability issues with concrete examples
    * [Make your functions functional](https://www.fluentcpp.com/2016/11/22/make-your-functions-functional/)
    * [Clearer interfaces with optional\<T\>](https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt/)
- [Functional C++ blog](https://functionalcpp.wordpress.com)
