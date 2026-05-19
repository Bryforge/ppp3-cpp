# PPP3 GCC Modules on macOS

This note records the working setup for using *Programming: Principles and Practice Using C++*, Third Edition, with GNU GCC modules on macOS.

## Goal

For PPP3 exercises, the desired workflow is:

```bash
gcc name.cpp
./name
```

The `gcc` command can be an alias to `scripts/pppgcc`, which expands the build into the required C++23 module commands.

## Why plain `gcc name.cpp` can fail

`PPP.h` contains:

```cpp
import PPP;
using namespace PPP;
using namespace std;
```

That means the PPP module must already be built before the program imports it.

If the compiled module interface is missing, GCC reports something like:

```text
PPP: error: failed to read compiled module: No such file or directory
PPP: note: compiled module file is 'gcm.cache/PPP.gcm'
PPP: note: imports must be built before being imported
```

If the module interface exists but `PPP.o` is not linked, the linker can report:

```text
Undefined symbols for architecture arm64:
  "initializer for module PPP", referenced from:
      __static_initialization_and_destruction_0()
ld: symbol(s) not found for architecture arm64
```

The fix is to compile and link with the PPP and standard-library module object files:

```bash
gcc -std=c++23 -fmodules-ts name.cpp PPP.o std.o -o name -lstdc++
```

## Apple Clang vs GNU GCC

On macOS, `g++` may actually be Apple Clang:

```text
clang++: error: unknown argument: '-fmodules-ts'
```

That means the command is not using GNU GCC. Use Homebrew GCC instead, usually one of:

```bash
gcc-15 --version
gcc-14 --version
gcc --version
```

The wrapper script searches for a real GNU GCC binary.

## PPP.cxx

This repository includes `PPP.cxx`:

```cpp
module;
#include <cstddef>

export module PPP;
export import std;

#define PPP_EXPORT export
#include "PPP_support.h"

using namespace PPP;
```

The `module;` section is the global module fragment. It lets the file include `<cstddef>` before starting `module PPP`.

That matters because `PPP_support.h` uses `size_t`. Adding `#include <cstddef>` directly inside `PPP_support.h` after `import std;` can trigger GCC module redefinition errors such as duplicate definitions of `std::size_t`, `std::ptrdiff_t`, `std::nullptr_t`, or `std::byte`.

## Manual clean rebuild

If the module cache gets confused, clean and rebuild:

```bash
rm -rf gcm.cache PPP.o std.o name

# Build std.o if needed; path varies by GCC version.
# Example Homebrew path:
gcc -std=c++23 -fmodules-ts -c /opt/homebrew/Cellar/gcc/15.2.0_1/include/c++/15/bits/std.cc -o std.o

gcc -std=c++23 -fmodules-ts -c PPP.cxx
gcc -std=c++23 -fmodules-ts name.cpp PPP.o std.o -o name -lstdc++
./name
```

The `scripts/pppgcc` wrapper automates this path.

## Installing the shortcut

From the repository root:

```bash
mkdir -p ~/bin
cp scripts/pppgcc ~/bin/pppgcc
chmod +x ~/bin/pppgcc
echo 'alias gcc="$HOME/bin/pppgcc"' >> ~/.zshrc
source ~/.zshrc
type gcc
```

Expected result:

```text
gcc is an alias for /Users/<you>/bin/pppgcc
```

Then compile PPP3 programs with:

```bash
gcc hello.cpp
./hello

gcc name.cpp
./name
```

## Example `name.cpp`

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

## Common beginner mistakes from Chapter 1

### Missing PPP include

If `cout`, `cin`, or `string` are not declared, make sure the file starts with:

```cpp
#include "PPP.h"
```

### Broken string literal

This is wrong:

```cpp
cout << "Please enter your first name (followed by "enter"):\n";
```

Use single quotes inside the string:

```cpp
cout << "Please enter your first name (followed by 'enter'):\n";
```

## Rule of thumb

For PPP3 module programs, do not think of the build as only:

```text
source file -> executable
```

Think of it as:

```text
std module -> std.o + gcm.cache/std.gcm
PPP module -> PPP.o + gcm.cache/PPP.gcm
source file + module objects -> executable
```
