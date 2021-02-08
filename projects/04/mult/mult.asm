// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@i
M=1 // i = 1

@res
M=0 // res = 0

(LOOP)
@i
D=M // D = i

@0
D=D-M // D = i - M[0]

@END
D;JGT // if (i - M[0]) > 0 goto END

@1
D=M // D = M[1]

@res
M=D+M // res = M[1] + res

@i
M=M+1 // i = i + 1

@LOOP
0;JMP // goto LOOP

(END)

@res
D=M

@2
M=D

@END
0;JMP // endless loop
