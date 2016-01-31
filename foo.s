	.arch armv6
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu vfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"foo.c"
	.text
	.align	2
	.global	foo
	.type	foo, %function
foo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2
	ldr	r2, .L2+4
	mov	r0, #0
	ldr	r1, [r3]
	str	r0, [r3]
	add	r3, r1, #1
	str	r3, [r2]
	bx	lr
.L3:
	.align	2
.L2:
	.word	B
	.word	A
	.size	foo, .-foo
	.comm	B,4,4
	.comm	A,4,4
	.ident	"GCC: (Raspbian 4.8.4-1) 4.8.4"
	.section	.note.GNU-stack,"",%progbits
