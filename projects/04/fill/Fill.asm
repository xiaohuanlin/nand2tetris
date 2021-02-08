// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.


// address 24576 reflect keyboard
// address 16384 - 24575 reflect screen

(LOOP)
@16384
D=A // D = 16384

@i
M=D // i = 16384

@24576
D=M // D = M[24576]

@ELSE
D;JGT // if (M[24576] > 0) goto ELSE

(INNER_LOOP)
@i
D=M; // D = i

@24575
D=D-A; // D = i - 24575

@END
D;JGT // if (i - 24575) > 0 goto END

@i
D=M 
A=D
M=0 // M[i] = 0

@i
M=M+1 // i = i + 1

@INNER_LOOP
0;JMP

(ELSE)

(INNER_ELSE_LOOP)
@i
D=M; // D = i

@24575
D=D-A; // D = i - 24575

@END
D;JGT // if (i - 24575) > 0 goto END

@i
D=M 
A=D
M=-1 // M[i] = -1 this is 0x1111

@i
M=M+1 // i = i + 1

@INNER_ELSE_LOOP
0;JMP

(END)

@LOOP
0;JMP
