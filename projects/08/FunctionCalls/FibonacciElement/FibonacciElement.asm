@256
D=A
@0
M=D
@Sys.init.1
D=A
@0
A=M
M=D
@0
M=M+1
@1
D=M
@0
A=M
M=D
@0
M=M+1
@2
D=M
@0
A=M
M=D
@0
M=M+1
@3
D=M
@0
A=M
M=D
@0
M=M+1
@4
D=M
@0
A=M
M=D
@0
M=M+1
@5
D=A
@0
D=M-D
@2
M=D
@0
D=M
@1
M=D
@Sys.init
0;JMP
(Sys.init.1)
(Main.fibonacci)
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
D=M-D
@82
D;JLT
@0
A=M-1
M=0
@85
0;JMP
@0
A=M-1
M=-1
@0
AM=M-1
D=M
@IF_TRUE
D;JNE
@IF_FALSE
0;JMP
(IF_TRUE)
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
D=M
@13
M=D
@5
A=D-A
D=M
@14
M=D
@0
A=M-1
D=M
@2
A=M
M=D
@2
D=M+1
@0
M=D
@13
D=M
@1
A=D-A
D=M
@4
M=D
@13
D=M
@2
A=D-A
D=M
@3
M=D
@13
D=M
@3
A=D-A
D=M
@2
M=D
@13
D=M
@4
A=D-A
D=M
@1
M=D
@14
A=M
0;JMP
(IF_FALSE)
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
@Main.fibonacci.2
D=A
@0
A=M
M=D
@0
M=M+1
@1
D=M
@0
A=M
M=D
@0
M=M+1
@2
D=M
@0
A=M
M=D
@0
M=M+1
@3
D=M
@0
A=M
M=D
@0
M=M+1
@4
D=M
@0
A=M
M=D
@0
M=M+1
@6
D=A
@0
D=M-D
@2
M=D
@0
D=M
@1
M=D
@Main.fibonacci
0;JMP
(Main.fibonacci.2)
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
@Main.fibonacci.3
D=A
@0
A=M
M=D
@0
M=M+1
@1
D=M
@0
A=M
M=D
@0
M=M+1
@2
D=M
@0
A=M
M=D
@0
M=M+1
@3
D=M
@0
A=M
M=D
@0
M=M+1
@4
D=M
@0
A=M
M=D
@0
M=M+1
@6
D=A
@0
D=M-D
@2
M=D
@0
D=M
@1
M=D
@Main.fibonacci
0;JMP
(Main.fibonacci.3)
@0
A=M-1
D=M
@0
M=M-1
A=M-1
M=M+D
@1
D=M
@13
M=D
@5
A=D-A
D=M
@14
M=D
@0
A=M-1
D=M
@2
A=M
M=D
@2
D=M+1
@0
M=D
@13
D=M
@1
A=D-A
D=M
@4
M=D
@13
D=M
@2
A=D-A
D=M
@3
M=D
@13
D=M
@3
A=D-A
D=M
@2
M=D
@13
D=M
@4
A=D-A
D=M
@1
M=D
@14
A=M
0;JMP
(Sys.init)
@4
D=A
@0
A=M
M=D
@0
M=M+1
@Main.fibonacci.4
D=A
@0
A=M
M=D
@0
M=M+1
@1
D=M
@0
A=M
M=D
@0
M=M+1
@2
D=M
@0
A=M
M=D
@0
M=M+1
@3
D=M
@0
A=M
M=D
@0
M=M+1
@4
D=M
@0
A=M
M=D
@0
M=M+1
@6
D=A
@0
D=M-D
@2
M=D
@0
D=M
@1
M=D
@Main.fibonacci
0;JMP
(Main.fibonacci.4)
(WHILE)
@WHILE
0;JMP