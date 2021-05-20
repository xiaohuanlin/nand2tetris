## Usage

-   For project use

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./main [("token" | "notoken")] [input_file_or_dir]
```


-   For project test

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make xml_writer_test
$ ./test/xml_writer_test
$ make jack_tokenizer_test
$ ./test/jack_tokenizer_test
$ make compilation_engine_test
$ ./test/compilation_engine_test
```

This testsuit contains all test files of this project.

# Output file for checking
1. The file for checking tokens: "xxx_token_analyze.xml"
1. The file for checking final result: "xxx_analyze.xml"


# note

| function type | format | call manner | definition |
| ---- | ---- | ---- | ---- |
| function | className.function | push arguments | use arguments |
| method | varName.method or (this.)method | push varName(or "this") and arguments | pop argument 0 to pointer segment |
| constructor | className.constructor | push arguments | create "this" pointer and pop to pointer segment |


In let statement, we should evaluate the right value before evaluate the index of array, otherwise the "that" segment will be overwrote.