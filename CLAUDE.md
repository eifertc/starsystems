# Claude Code Guide

See [agent.md](agent.md) for full project documentation.

## Quick Reference

- **Language:** C++11, GLSL shaders
- **Build:** `mkdir build && cd build && cmake .. && make`
- **Run tests:** `cd build && ctest`
- **Format:** `cmake --build build --target format` (clang-format, LLVM style, 4-space indent, 120 col)
- **Static analysis:** `cmake --build build --target cppcheck`
- **Style:** PascalCase classes, snake_case_ members/private methods, UPPER_CASE constants, `#ifndef` guards
- **No namespaces, no smart pointers** - uses raw new/delete and directory-based organization
