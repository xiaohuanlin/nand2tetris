# Notes

## Important concept
- Hardware simulation
- HDL

## What we need do:
- Write the code using HDL for simulating hardware.

## The things we need to care:
- Actually, HDL is not like a programming language. It just like a tool that can link all of the units while don't care about the order of operations. It means that you should not worry about failing the endless loop.

- For any parameter like "sel[3]", you could pick a certain bit by using tokens like "sel[i]". For example, "sel[0]" represents the first bit of parameter "sel". Besides, you can even get a slice from this kind of parameter, like using "sel[0..2]", which means take two bits from the origin parameter.

- Install Java runtime environment, which could help you run all tools the project supported, otherwise you can't know what's happened behind the code.

- Using the Hardware Simulator UI instead of using the command line. Massive information showed would assistant you debug your code. However, the command line only tell you are failed without any meaningful information.

- Follow the Specification step by step.

- Write the code in ".hdl" file. Test the code by ".tst" file. Check the code result by ".cmp" file.