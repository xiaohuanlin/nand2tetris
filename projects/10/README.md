# Notes

## Important concept
- Syntax analysis

## What we need do:
- Write a jack compiler(syntax analysis part) using C++.

## The things we need to care:
- Generally speaking, we seldom review the tokens we have parsed. I think that is why we use Jack as the language need to be interpreted.
- If we can't correctly parse the code, this program just throws the exception. So it is impossible that figure out what's happened the gdb assistance.


## Usage
- Build project

```shell
$ cd projects/10/jack_compiler
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./main [("token" | "notoken")] [input_file_or_dir]
```

- Test project

```shell
$ cd projects/10/jack_compiler
$ cd build
$ cmake ..
$ make [compilation_engine_test | jack_tokenizer_test | xml_writer_test]
$ ./test/[compilation_engine_test | jack_tokenizer_test | xml_writer_test]
```

- Debug project

```shell
$ cd projects/10/jack_compiler
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make 
$ gdb ./main [("token" | "notoken")] [input_file_or_dir]
```

"token" option: Only show the token record(in xml format).
"notoken" option: Show the syntax tree(in xml format).

## Output file for checking
1. The file for checking tokens: "xxx_token_analyze.xml"
1. The file for checking final result: "xxx_analyze.xml"