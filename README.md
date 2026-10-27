# struct_reordering

A single-header C++26 library for reflection-based struct reordering.
The public header is `include/struct_reordering.hpp`, and the CMake target is
`struct_reordering::struct_reordering`.

## Reorder a struct

```cpp
#include <struct_reordering.hpp>

struct Input {
    char tag;
    double value;
    char flag;
    int count;
};

using Packed = struct_reordering::optimized_t<Input>;
// Member order: value, count, tag, flag.
Packed object{.value = 3.5, .count = 42, .tag = 'a', .flag = 'b'};
```

The generated type is a separate aggregate with the same member names and types,
sorted by decreasing member alignment. Equal alignments retain their original
declaration order. Explicit member `alignas` requirements are preserved. Sorting
can reduce padding, but does not guarantee the smallest possible size.

Inputs must be aggregate class types with named members and no base classes.
Unions, pointers, references, bit-fields, and mutable members are unsupported.
Member types may themselves be arrays, references, or nontrivial classes.
Methods, static members, default member initializers, attributes such as
`[[no_unique_address]]`, and alignment applied to the original class itself are
not copied. Initialize the generated aggregate directly in its new member order;
there is no automatic conversion from an original object.

## Build and run the tests

Requires CMake 3.30 or newer and a compiler and standard library that implement
C++26 reflection and `<meta>`, such as GCC 16. CMake enables `-freflection` for GCC 16
and newer.
Tests have no third-party dependencies.

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/gcc@16/bin/g++-16 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

For a multi-configuration generator, build with `--config Debug` and run CTest
with `-C Debug`.

The tests cover member ordering, equal-alignment stability, padding reduction,
explicit alignment, empty and single-member structs, unsupported input categories,
value initialization, reference bindings, move-only members, and generated type
identity across translation units. Compile-time checks validate layouts; runtime
checks remain active in Release builds.

Tests default to enabled when this is the top-level project and disabled when
included by another project. Disable them explicitly with
`-DSTRUCT_REORDERING_BUILD_TESTS=OFF`.

## Use in another CMake project

```cmake
add_subdirectory(path/to/struct_reordering)
target_link_libraries(my_app PRIVATE struct_reordering::struct_reordering)
```

```cpp
#include <struct_reordering.hpp>

static_assert(struct_reordering::version == "0.1.0");
```

You can also copy the single header into your project and compile in C++26 mode.
There is no compiled library to link. Keep future header definitions inline,
`constexpr`, or templated to support inclusion from multiple translation units.

## Reflection toolchain

C++26 mode alone does not guarantee reflection support. The public header includes
`<meta>` and uses a `consteval` block, so even the sample tests need a reflection
toolchain. The build command above uses Homebrew GCC 16 on macOS; on other systems,
replace the compiler path with your GCC 16 executable.

[GCC's support table](https://gcc.gnu.org/projects/cxx-status.html) lists reflection
in GCC 16 with `-freflection`:

```sh
cmake -S . -B build-reflection -DCMAKE_CXX_COMPILER=g++-16
cmake --build build-reflection
ctest --test-dir build-reflection --output-on-failure
```

The compiler executable name depends on your installation. Upstream
[Clang's support table](https://clang.llvm.org/cxx_status.html) currently lists
P2996 as unsupported. Use a separate build directory when changing compilers.
