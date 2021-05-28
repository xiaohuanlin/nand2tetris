# Notes

## Important concept
- Arithmetic and Logical Commands
- Memory Access Commands

## What we need do:
- Write a vm translator(without program flow and function calling commands) using C++.

## The things we need to care:
- It is necessary to record the current line number of the code. It makes the jump to a certain position possible. OK, actually, I finally find jumping by using a label is another more reasonable solution.

## Usage
- Build project

```shell
$ cd projects/07/vmtranslator
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./main [input_file_or_dir] [output_file]
```

- Test project

```shell
$ cd projects/07/vmtranslator
$ cd build
$ cmake ..
$ make [parser_test | code_writer_test]
$ ./test/[parser_test | code_writer_test]
```

- Debug project

```shell
$ cd projects/07/vmtranslator
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make 
$ gdb ./main [input_file_or_dir] [output_file]
```