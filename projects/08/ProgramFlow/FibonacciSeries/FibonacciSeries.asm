@1
D=A
@2
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
@0
AM=M-1
D=M
@4
M=D
@0
D=A
@0
A=M
M=D
@0
M=M+1
@0
D=A
@4
D=D+M
@13
M=D
@0
AM=M-1
D=M
@13
A=M
M=D
@1
D=A
@0
A=M
M=D
@0
M=M+1
@1
D=A
@4
D=D+M
@13
M=D
@0
AM=M-1
D=M
@13
A=M
M=D
@0
D=A
@2
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
@2
D=A
@0
A=M
M=D
@0
M=M+1
@0
A=M-1
D=M
@0
M=M-1
A=M-1
M=M-D
@0
D=A
@2
D=D+M
@13
M=D
@0
AM=M-1
D=M
@13
A=M
M=D
(MAIN_LOOP_START)
@0
D=A
@2
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
@0
AM=M-1
D=M
@COMPUTE_ELEMENT
D;JNE
@END_PROGRAM
0;JMP
(COMPUTE_ELEMENT)
@0
D=A
@4
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
@1
D=A
@4
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
@0
A=M-1
D=M
@0
M=M-1
A=M-1
M=M+D
@2
D=A
@4
D=D+M
@13
M=D
@0
AM=M-1
D=M
@13
A=M
M=D
@4
D=M
@0
A=M
M=D
@0
M=M+1
@1
D=A
@0
A=M
M=D
@0
M=M+1
@0
A=M-1
D=M
@0
M=M-1
A=M-1
M=M+D
@0
AM=M-1
D=M
@4
M=D
@0
D=A
@2
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
@1
D=A
@0
A=M
M=D
@0
M=M+1
@0
A=M-1
D=M
@0
M=M-1
A=M-1
M=M-D
@0
D=A
@2
D=D+M
@13
M=D
@0
AM=M-1
D=M
@13
A=M
M=D
@MAIN_LOOP_START
0;JMP
(END_PROGRAM)
