mov r1, #1fh //x axis
mov r2, #bfh
mov r3, #1fh
mov r4, #01h
syscall plot_line
mov r0, #5fh //y aixs
mov r1, #01h
mov r2, #5fh
mov r3, #3fh
syscall plot_line
mov r0, #00h
mov r3, #1fh

mov r5, #01h //gradient
mov r6, #05h //y intercept

mov r0, #00h //x1
mov r1, #1fh //y1
mov r2, #bfh //x2
mov r3, #1fh //y2

mov r7, #a0h //-96
mul r7, r5   //-96*m
add r7, r6   //-96*m+c

sub r1, r7   //invert it

mov r7, #5fh //96
mul r7, r5   //96*m
add r7, r6   //96*m+c

sub r3, r7   //invert it

mov r4, #03h

syscall plot_line
syscall render
break