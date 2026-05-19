# PPP3 C++ Practice

This repository contains my C++ practice programs while studying *Programming: Principles and Practice Using C++* by Bjarne Stroustrup, Third Edition.

The setup is designed for C++23 examples using GNU GCC on macOS, including support for PPP3's `import std;`, `PPP.h`, and `PPP_support.h`.

## Why this setup exists

PPP3's support header uses modules:

```cpp
import PPP;
```

That means a program such as `name.cpp` does not only need to compile its own source file. It also needs the already-built module artifacts:

- `gcm.cache/std.gcm` and `std.o` for `import std;`
- `gcm.cache/PPP.gcm` and `PPP.o` for `import PPP;`

If those object files are not linked, GCC can compile the source but the linker will fail with:

```text
Undefined symbols for architecture arm64:
  "initializer for module PPP"
```

## macOS note

On macOS, `g++` may resolve to Apple `clang++`. Apple Clang does not understand GCC's `-fmodules-ts` option. Use GNU GCC instead, usually installed with Homebrew as `gcc-15`, `gcc-14`, or `gcc`.

## First-time shortcut setup

From the repository root:

```bash
mkdir -p ~/bin
cp scripts/pppgcc ~/bin/pppgcc
chmod +x ~/bin/pppgcc
echo 'alias gcc="$HOME/bin/pppgcc"' >> ~/.zshrc
source ~/.zshrc
type gcc
```

After that, this should work for normal PPP3 exercises:

```bash
gcc name.cpp
./name
```

The wrapper automatically uses C++23 module flags, builds `std.o` and `PPP.o` when needed, links them, and names the executable after the source file.

## Manual build path

The shortcut expands to the same basic shape as this:

```bash
gcc -std=c++23 -fmodules-ts -c PPP.cxx
gcc -std=c++23 -fmodules-ts name.cpp PPP.o std.o -o name -lstdc++
./name
```

If `std.o` is missing, build the GCC standard-library module first. The wrapper tries to find GCC's `bits/std.cc` automatically.

## PPP module source

`PPP.cxx` intentionally starts with:

```cpp
module;
#include <cstddef>

export module PPP;
export import std;
```

The global module fragment gives `PPP_support.h` access to `size_t` without placing `#include <cstddef>` inside `PPP_support.h`. Putting `<cstddef>` inside `PPP_support.h` after `import std;` can cause GCC module redefinition errors.

## Example program

```cpp
#include "PPP.h"

int main()
{
    cout << "Please enter your first name (followed by 'enter'):\n";
    string first_name;
    cin >> first_name;
    cout << "Hello, " << first_name << "!\n";
}
```

Build and run:

```bash
gcc name.cpp
./name
```

## More detail

See [`docs/PPP3_GCC_MODULES.md`](docs/PPP3_GCC_MODULES.md) for the detailed troubleshooting notes from the macOS/GCC setup.
