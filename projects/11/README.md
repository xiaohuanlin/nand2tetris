# Notes

## Important concept
- Code Generation

## What we need do:
- Write a jack compiler using C++.

## The things we need to care:

| function type | format | call manner | definition |
| ---- | ---- | ---- | ---- |
| function | className.function | push arguments | use arguments |
| method | varName.method or (this.)method | push varName(or "this") and arguments | pop argument 0 to pointer segment |
| constructor | className.constructor | push arguments | create "this" pointer and pop to pointer segment |


- In the "let" statement, we should evaluate the right value before evaluating the index of the array, otherwise, the "that" segment will be overwrote.


## Usage
- Build project

```shell
$ cd projects/11/jack_compiler
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./main [input_file_or_dir] [("vm_writer" | "xml_writer")]
```

- Test project

```shell
$ cd projects/11/jack_compiler
$ cd build
$ cmake ..
$ make [compilation_engine_test | jack_tokenizer_test | symbol_table_test | xml_writer_test | vm_writer_test]
$ ./test/[compilation_engine_test | jack_tokenizer_test | symbol_table_test | xml_writer_test | vm_writer_test]
```

- Debug project

```shell
$ cd projects/11/jack_compiler
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make 
$ gdb ./main [input_file_or_dir] [("vm_writer" | "xml_writer")]
```

"vm_writer" option: Write VM code.
"xml_writer" option: Show the syntax tree(in xml format).