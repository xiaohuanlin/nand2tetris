@0
D=A
@0
A=M
M=D
@0
M=M+1
@0
D=A
@1
M=D+M
@0
AM=M-1
D=M
@1
A=M
M=D
@0
D=A
@1
M=M-D
(LOOP_START)
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
D=A
@1
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
@0
D=A
@1
M=D+M
@0
AM=M-1
D=M
@1
A=M
M=D
@0
D=A
@1
M=M-D
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
M=D+M
@0
AM=M-1
D=M
@2
A=M
M=D
@0
D=A
@2
M=M-D
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
@LOOP_START
D;JNE
@0
D=A
@1
A=D+M
D=M
@0
A=M
M=D
@0
M=M+1
