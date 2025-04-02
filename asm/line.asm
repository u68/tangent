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
syscall render
mov r0, #00h
mov r3, #1fh
mov r9, #02h //gradient
mov r5, #0ah //y intercept
loop:
mov r6, r0
add r6, #a1h
mul r6, r9
add r6, r5
mov r1, r3
sub r1, r6
mov r2, r7
mov r3, r8
mov r4, #03h
syscall plot_line
syscall render
mov r7, r0
mov r8, r1
mov r3, #1fh
inc r0
b loop
