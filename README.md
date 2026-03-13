# qt_sandbox
A basic Qt Hello World application **to learn and practice** the fundamentals of Qt GUI development.
Qt Widgets app (`hello_world` -> to be renamed) that integrates the `bkk_api` C++ library as part of a single CMake build.

## Requirements (Linux)

- Ubuntu/Debian-based Linux
- CMake 3.16+
- GCC/G++ with C++17 support
- Qt development package (Qt5 or Qt6 Widgets)
- libcurl development package

Install dependencies on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install -y build-essential cmake qtbase5-dev libcurl4-openssl-dev
```

## Repository Setup

This repo uses `bkk_api` as a submodule.

If cloning fresh:

```bash
git clone --recurse-submodules <your-repo-url>
cd qt_sandbox
```

If already cloned:

```bash
git submodule update --init --recursive
```

## Build (Top-Level CMake)

From repository root:

```bash
cmake -S . -B cmake-build
cmake --build cmake-build -j
```

This builds both:

- `bkk_api_shared` from `bkk_api/cpp`
- `hello_world` from the root project

## Run

Always run the binary from the CMake build directory:

```bash
./cmake-build/hello_world
```

## Rebuild After Code Changes

For normal rebuilds:

```bash
cmake --build cmake-build -j
```

If you changed any `CMakeLists.txt` file, reconfigure first:

```bash
cmake -S . -B cmake-build
cmake --build cmake-build -j
```

For a full clean rebuild:

```bash
rm -rf cmake-build
cmake -S . -B cmake-build
cmake --build cmake-build -j
```