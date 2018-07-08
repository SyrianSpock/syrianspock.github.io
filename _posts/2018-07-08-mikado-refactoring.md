---
layout: post
title: Refactoring&#58 Mikado, a powerful tool
categories: ['Software']
tags: ['software', 'programming', 'tools', 'refactoring']
image: /images/mikado/mikado.jpg
---

<center>
    <img src="/images/mikado/mikado.png" alt="We've all seen code bases that look like this">
    <figcaption>
        <small>We've all seen code bases that look like this :(</small>
    </figcaption>
</center>

# What is refactoring and why do I care?
Refactoring is major part of any software engineer's work.
We constantly refine our abstractions to better model the problems we are trying to solve.
This reflects in implementation changes.
The activity of making these changes are what we call refactoring.

Bad abstractions can slow us down when we want to solve new problems, i.e. implement features.
They can manifest through coupled code, i.e. unnecessary dependencies.
Worse, they can even halt your ability to modify code, i.e. make your code base rigid and brittle to change.
Refactoring helps us groom the code, refine abstractions, and pay back technological debt.
It is a way to cut bad technological debt (bad abstractions, entanglement) and build up software capital (good abstractions, modularity).
This is what makes refactoring a core task in programming, I would argue this is something you want to do continuously, and it should occupy somewhere between 20% and 40% of your programming time.

<center>
    <img src="/images/mikado/good-abstraction.jpg" alt="One does not simply find a good abstraction">
</center>

Many great authors have dealt with the topic in length.
I can recommend the following books: [Refactoring by Martin Fowler][Refactoring Fowler] and [Working Effectively with Legacy Code by Michael Feathers][Legacy Code Feathers].
Although these mostly deal with low-level design pattern and practical methods for refactoring, I still think you need to rethink your abstractions to make your refactoring as effective as possible, I may write on the topic in the future.

In practice, you may have little knowledge of the codebase you are trying to refactor (or parts of it).
Or maybe the amount of changes is so huge that you may feel overwhelmed just keeping track of it.
Worry no more, Mikado is a method that may help you refactor code in a more focused and efficient way.
It applies divide and conquer to refactoring tasks that works best for typed languages, and it has worked wonders for me.
For those who don't know about Mikado, I highly recommend reading the [original article published on the Pragmatic Programmer website back in 2010][The Mikado Method].

# Introducing the toy problem
Let's look at an example of bad abstraction, how it makes the code entagled, and how we can use Mikado to get rid of it.
Consider the following C++ application.

{% gist 1fc2e453d359472eda24e63d815ea033 %}

I'm using C++17, so you can compile and run the example using Clang 6.0
```
$ clang++ -std=c++17 code.cpp -o app
$ ./app 42 9
Inputs:
  a: 42
  b: 9
Outputs:
  mul: 378
  div:
    quotient: 4
    remainder: 6
```

Can you smell it?
The entaglement in this example stems from a bad abstraction: the `Data` struct.
`Data` contains both inputs and outputs, and it groups outputs together although they are completely indepedent.

```cpp
struct Data {                               // The root of all evil
    struct {
        int a;
        int b;
    } in;                                   // Program inputs
    struct {
        int mul;                            // First computation output
        std::optional<DivisionResult> div;  // Second computation output
    } out;
};
```

Subsequently `Data` is propagated all over the place to get things done: in input parsing, output generation, and result display.
This smells big time!
Our refactoring goal will be to get rid of `Data`.

# Building a Mikado graph
We can start building up our Mikado graph.
For that I'll use a [Python package I wrote a few months back][Mikado Graph Python] and regularly use to automate graph drawing and tracking (drawing on paper is not practical for Mikado).
You can get it by running `pip install mikado-graph`.
We start a new file to describe and track our Mikado graph `code.txt` and generate the graph by running `mikado code.txt --view --watch`
```
_ Remove Data coupling
    _ Remove Data from parse_inputs()
    _ Remove Data from run_computation()
    _ Remove Data from show_results()
```
<center>
    <img src="/images/mikado/graph_0.png" alt="Graph v0">
</center>

Next, let's look inside `parse_inputs` and find a way of removing `Data` from `parse_inputs`.
```cpp
std::optional<Data> parse_inputs(int argc, char** argv)
{
    if (argc != 3) {
        return {};
    }

    Data p;
    p.in.a = atoi(argv[1]);
    p.in.b = atoi(argv[2]);
    return p;
}
```
We see that this function is filling the inputs part of `Data` with what the user provided.
So we could introduce a `UserInput` struct to model this.
Let's complete the graph.
```
_ Remove Data coupling
    _ Remove Data from parse_inputs()
        _ Return UserInput instead of Data
    _ Remove Data from run_computation()
    _ Remove Data from show_results()
```
<center>
    <img src="/images/mikado/graph_1.png" alt="Graph v1">
</center>

Next, we look at the output generation done by `run_computation`.
```cpp
Data run_computation(Data p)
{
    p.out.mul = multiply(p);
    p.out.div = divide(p);
    return p;
}
```
It passes `Data` to two functions: `multiply` and `divide`.
Let's update our graph and jump inside each of these functions.
```
_ Remove Data coupling
    _ Remove Data from parse_inputs()
        _ Return UserInput instead of Data
    _ Remove Data from run_computation()
        _ Remove Data from multiply()
        _ Remove Data from divide()
    _ Remove Data from show_results()
```
<center>
    <img src="/images/mikado/graph_2.png" alt="Graph v2">
</center>

The child functions `multiply` and `divide` both use values from the inputs of `Data`.
```cpp
int multiply(const Data& p)
{
    return p.in.a * p.in.b;
}

std::optional<DivisionResult> divide(const Data& p)
{
    if (p.in.b == 0) return {};
    else             return DivisionResult{p.in.a / p.in.b, p.in.a % p.in.b};
}
```
So we can remove the dependency to `Data` by injecting the required arguments directly (inputs `a` and `b`).
No need to update the graph for that, although you could add an entry to each branch with this information.

Finally, we look at `show_results`.
```cpp
int show_results(const Data& p)
{
    std::cout << "Inputs:" << std::endl;
    std::cout << "  a: " << p.in.a << std::endl;
    std::cout << "  b: " << p.in.b << std::endl;
    std::cout << "Outputs:" << std::endl;
    std::cout << "  mul: " << p.out.mul << std::endl;
    std::cout << "  div: " << std::endl;
    std::cout << "    quotient: " << p.out.div.value().quotient << std::endl;
    std::cout << "    remainder: " << p.out.div.value().remainder << std::endl;
    return all_successful(p) ? 0 : 1;
}
```
That is both displaying input and outputs, so this is something we can split up.
It also returns the program exit code, computed by `all_successful` which should be split up and should only get outputs from `Data`.
```cpp
bool all_successful(const Data& p)
{
    return bool(p.out.div);
}
```
Now the graph is complete.
```
_ Remove Data coupling
    _ Remove Data from parse_inputs()
        _ Return UserInput instead of Data
    _ Remove Data from run_computation()
        _ Remove Data from multiply()
        _ Remove Data from divide()
    _ Remove Data from show_results()
        _ Split display of inputs and outputs
        _ Extract all_successful()
            _ Remove Data from all_successful()
```
<center>
    <img src="/images/mikado/graph_3.png" alt="Graph v3">
</center>

Building the graph is the first step in Mikado.
It is done incrementally by analysis and/or leveraging your compiler, read the [original Mikado method article][The Mikado Method] for more details.
The graph is a crucial tool that will organize our work and keeps track of all the changes needed.

# Getting down to business

We built our graph top-down, we're going to work our way bottom-up, which means solving each leaf (node) starting from the top of the graph.

## Dealing with inputs
We need to define a new data type to represent user input:
```cpp
struct UserInput {
    int a;
    int b;
};
```

Now we can make `parse_inputs` return such a type.
To keep the rest of the code intact, we can use our `UserInput` inside `Data`:
```cpp
struct Data {
    struct UserInput in;
    struct {
        int mul;
        std::optional<DivisionResult> div;
    } out;
};
```
We can then easily cut `Data` from `parse_inputs` and update client code:
```cpp
std::optional<UserInput> parse_inputs(int argc, char** argv)
{
    if (argc != 3) {
        return {};
    }

    UserInput in;
    in.a = atoi(argv[1]);
    in.b = atoi(argv[2]);
    return in;
}
// [...]
int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        Data data;
        data.in = input.value();
        auto output = run_computation(data);
        return show_results(output);
// [...]
```
Then update the graph with the first green leaves!
```
_ Remove Data coupling
    x Remove Data from parse_inputs()
        x Return UserInput instead of Data
    _ Remove Data from run_computation()
        _ Remove Data from multiply()
        _ Remove Data from divide()
    _ Remove Data from show_results()
        _ Split display of inputs and outputs
        _ Extract all_successful()
            _ Remove Data from all_successful()
```
<center>
    <img src="/images/mikado/graph_done_0.png" alt="Graph done v0">
</center>

## Another branch bites the dust
Refactoring `multiply` and `divide` is easy, just inject the required arguments:
```cpp
int multiply(const int a, const int b)
{
    return a * b;
}

std::optional<DivisionResult> divide(const int a, const int b)
{
    if (b == 0) return {};
    else        return DivisionResult{a / b, a % b};
}

Data run_computation(Data p)
{
    p.out.mul = multiply(p.in.a, p.in.b);
    p.out.div = divide(p.in.a, p.in.b);
    return p;
}
```
Notice how `run_computation` got slightly less readable, but that will get better next.
For now we can update our graph, then move down the branch.
<center>
    <img src="/images/mikado/graph_done_1.png" alt="Graph done v1">
</center>

We can make `run_computation` take a `UserInput` and return some `Output` struct.
That will make `run_computation` input/output more explicit in the function signature.
```cpp
struct Output {
    int mul;
    std::optional<DivisionResult> div;
};

struct Data {
    struct UserInput in;
    struct Output out;
};
// [...]
Output run_computation(const UserInput& input)
{
    Output out;
    out.mul = multiply(input.a, input.b);
    out.div = divide(input.a, input.b);
    return out;
}
// [...]
int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        Data data;
        data.in = input.value();
        data.out = run_computation(data.in);
        return show_results(data);
// [...]
```
The graph keeps on getting greener and greener.
<center>
    <img src="/images/mikado/graph_done_2.png" alt="Graph done v2">
</center>

## Splitting displays
The `all_successful` helper could work by only feeding it `Output`, and let it figure out if all outputs were generated.
```cpp
bool all_successful(const Output& res)
{
    return bool(res.div);
}

int show_results(const Data& p)
{
// [...]
    return all_successful(p.out) ? 0 : 1;
}
```
<center>
    <img src="/images/mikado/graph_done_3.png" alt="Graph done v3">
</center>

Now moving out `all_successful` from `show_results`.
```cpp
void show_results(const Data& p)
{
// [...]
}

int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        Data data;
        data.in = input.value();
        data.out = run_computation(data.in);
        show_results(data);
        return all_successful(data.out) ? 0 : 1;
// [...]
```
<center>
    <img src="/images/mikado/graph_done_4.png" alt="Graph done v4">
</center>

Then, splitting inputs and ouputs display.
```cpp
void show_inputs(const UserInput& in)
{
    std::cout << "Inputs:" << std::endl;
    std::cout << "  a: " << in.a << std::endl;
    std::cout << "  b: " << in.b << std::endl;
}

void show_outputs(const Output& out)
{
    std::cout << "Outputs:" << std::endl;
    std::cout << "  mul: " << out.mul << std::endl;
    std::cout << "  div: " << std::endl;
    std::cout << "    quotient: " << out.div.value().quotient << std::endl;
    std::cout << "    remainder: " << out.div.value().remainder << std::endl;
}

void show_results(const Data& p)
{
    show_inputs(p.in);
    show_outputs(p.out);
}
```
<center>
    <img src="/images/mikado/graph_done_5.png" alt="Graph done v5">
</center>

And getting rid of `show_results` altogether.
```cpp
int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        Data data;
        data.in = input.value();
        data.out = run_computation(data.in);
        show_inputs(data.in);
        show_outputs(data.out);
        return all_successful(data.out) ? 0 : 1;
// [...]
```
<center>
    <img src="/images/mikado/graph_done_6.png" alt="Graph done v6">
</center>

## Slaying down the dragon
The only remaining red node is the end goal.
We are finally ready to cut down `Data` completely from `main`.
```cpp
int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        auto result = run_computation(input.value());
        show_inputs(input.value());
        show_outputs(result);
        return all_successful(result) ? 0 : 1;
// [...]
```
Once that is done, we are left with a cleaner, less entagled code.

{% gist d65e9a81dfedf69f2f2ea71be8102002 %}

# Conclusion
Although the result of our toy problem refactoring is better than the initial state of the code, some things can still be done to improve it, such as decoupling outputs.
Remember: refactoring is a continuous process.

Mikado makes it much easier to tackle small and big refactoring tasks with good scalability.
It makes your work incremental.
I highly recommend you solve leaves one by one.
Each leaf should be a separate commit.
And each commit should compile with green unit tests.

Although it is a very powerful tool for day-to-day programming, Mikado is no silver bullet.
You still need to understand the codebase and what problem it is trying to solve in order to design better abstractions.
Mikado only helps you explore and organize your refactoring.


[The Mikado Method]: https://pragprog.com/magazines/2010-06/the-mikado-method
[Refactoring Fowler]: https://martinfowler.com/books/refactoring.html
[Legacy Code Feathers]: https://www.goodreads.com/book/show/44919.Working_Effectively_with_Legacy_Code
[Mikado Graph Python]: https://github.com/syrianspock/mikado-graph
