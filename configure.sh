#! /bin/sh

# cmake -S . -B build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
