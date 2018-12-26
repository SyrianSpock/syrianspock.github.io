---
layout: post
title: Improve your CMake with functions
categories: ['Software']
tags: ['software', 'programming', 'tools', 'refactoring', 'c++', 'cpp']
---

Writing CMake code can be sometimes exhausting.
Most of the pain comes from the complexity inherent to the dependency graph of your project.
But sometimes, complexity accumulates and you're faced with dozens or even hundreds of lines of CMake code in a single file.
That may be expected, or it may be a smell.
When it's a smell, remember that you can do something about it: refactor!

Do you think it's time to encapsulate complexity?
Do you wish there was a way to define functions to simplify your CMake code?
Do not worry, CMake has got you covered.

Writing CMake functions can be tedious, but we can make it easier than it sounds.

# Problem

Let's consider the basic example we all encounter at some point.
We're writing libraries, and they have unit tests.
The code is usually repetitive and may look like this

```cmake
add_library(
    foobar
    foobar.h
    foobar.c
)
target_link_libraries(foobar m)

if(${BUILD_TESTS})
    add_executable(
        test_unit_foobar
        test/main.cpp
        test/test_foobar.cpp
    )
    target_link_libraries(test_unit_foobar foobar CppUTest CppUTestExt)
    add_test(test_unit_foobar test_unit_foobar)
endif()
```

for some source code looking like that

```
.
├── foobar.c
├── foobar.h
└── test
    ├── main.cpp
    └── test_foobar.cpp
```

This may not bother you, but when you start having this repeated for all your libraries, and you want to change the way you link your unit tests, because maybe you're moving from CppUTest to GoogleTest.
It can be a pain to refactor all of it.
So it helps to define a function to hide this.

# Solution

Let's define two functions: one to create a library, one to create a test target.

```cmake
function(custom_add_library)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES DEPENDENCIES)
    cmake_parse_arguments(TEST_EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

endfunction()

function(custom_add_library)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES DEPENDENCIES)
    cmake_parse_arguments(TEST_EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

endfunction()
```

so our previous CMake code becomes

```cmake
custom_add_library(
    TARGET
        foobar
    SOURCES
        foobar.h
        foobar.c
    DEPENDENCIES
        m
)

custom_add_test(
    ENABLE ${BUILD_TESTS}
    TARGET test_unit_foobar
    SOURCES
        test/main.cpp
        test/test_foobar.cpp
    DEPENDENCIES
        foobar
        CppUTest
        CppUTestExt
)
```

Notice that we used `cmake_parse_arguments` to parse the function arguments.
This avoids the mess of dealing with arguments parsing in CMake that makes it painful to define functions (to the point where you prefer writing macros).
Functions are still more powerful and scalable than macros in CMake, and `cmake_parse_arguments` is a key component to make functions easy to write and read in CMake.

With the introductions of the `custom_add_library` and `custom_add_test` functions, we see the intent of our CMake code more clearly without getting lost in implementation details.
Any changes to the way we build a library or a test target is encapsulated in the functions we defined.
Furthermore, our codebase is less fragile to future changes, and our mental health is preserved.
