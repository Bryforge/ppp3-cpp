# PPP3 C++ Practice

This repository contains my C++ practice programs while studying *Programming: Principles and Practice Using C++* by Bjarne Stroustrup, Third Edition.

The setup is designed for C++23 examples using GNU GCC on macOS, including support for PPP3's `import std;`, `PPP.h`, and `PPP_support.h`.

## Repository organization

Chapter work belongs in folders named `chNUM`:

```text
ch1/   # Chapter 1
ch2/   # Chapter 2
ch3/   # Chapter 3
```

PPP support files stay at the repository root and are shared by all chapter folders:

```text
PPP.h
PPP.cxx
PPP_support.h
PPPheaders.h
scripts/pppgcc
```

Example Chapter 1 files:

```text
ch1/hello.cpp
ch1/name.cpp
ch1/README.md
```

Build chapter programs from the repository root:

```bash
gcc ch1/hello.cpp
./hello

gcc ch1/name.cpp
./name
```

## Why this setup exists

PPP3's support header uses modules:

```cpp
import PPP;
```

That means a program such as `ch1/name.cpp` needs the already-built module artifacts:

- `gcm.cache/std.gcm` and `std.o` for `import std;`
- `gcm.cache/PPP.gcm` and `PPP.o` for `import PPP;`

The `scripts/pppgcc` wrapper automates the build and link flags needed for these files.

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
gcc ch1/name.cpp
./name
```

## PPP module source

`PPP.cxx` intentionally starts with:

```cpp
module;
#include <cstddef>

export module PPP;
export import std;
```

The global module fragment gives `PPP_support.h` access to `size_t` without placing `#include <cstddef>` inside `PPP_support.h`.

## More detail

See [`docs/PPP3_GCC_MODULES.md`](docs/PPP3_GCC_MODULES.md) for the detailed troubleshooting notes from the macOS/GCC setup.
