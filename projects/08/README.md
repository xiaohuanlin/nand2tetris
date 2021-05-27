# Notes

## Important concept
- Program flow
- Subroutine calling

## What we need do:
- Write a vm translator using C++.

## The things we need to care:
- Refactoring code makes the code clear and simple.


## Usage
- Build project

```shell
$ cd projects/08/vmtranslator
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./main [input_file_or_dir] [output_file]
```

- Test project

```shell
$ cd projects/08/vmtranslator
$ cd build
$ cmake ..
$ make [parser_test | code_writer_test | main_test]
$ ./test/[parser_test | code_writer_test | main_test]
```

- Debug project

```shell
$ cd projects/08/vmtranslator
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make 
$ gdb ./main [input_file_or_dir] [output_file]
```