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