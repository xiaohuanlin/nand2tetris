# Notes

## Important concept
- Assembler
- Specification

## What we need do:
- Write assembler using C++.

## The things we need to care:
- Try to make static vector: I chose to build it in function by using lambda expression.
- Fields could be empty, so we define it as a "null" symbol.
- Bitset is a good way to represent binary data.

## Usage
- Build project

```shell
$ cd projects/06/compiler
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./main [input_file] [output_file]
```

- Test project

```shell
$ cd projects/06/compiler
$ cd build
$ cmake ..
$ make parser_test 
$ ./test/parser_test
```

- Debug project

```shell
$ cd projects/06/compiler
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make 
$ gdb ./main [input_file] [output_file]
```