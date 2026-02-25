# Redis from Scratch

## Requirements

- Linux or WSL2 (Ubuntu)
- g++ with C++20 support
- cmake
- ninja (optional, can use the default make generator instead)

## Build

```bash
cmake -S . -B build -G Ninja
cmake --build build
```