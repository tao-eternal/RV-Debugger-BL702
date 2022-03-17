	.file	"ker.c"
	.option nopic
	.option norelax
	.text
	.data
	.align	2
	.type	p, @object
	.size	p, 4
p:
# fill it with address of real printf
# take a look at ./readme.md
	.word	0x23004296
	.text
	.align	1
	.globl	printf
	.type	printf, @function
printf:
  lw a5, p
	jr a5
	.size	printf, .-printf
	.align	1
	.globl	puts
	.type	puts, @function
puts:
	j	printf
	.size	puts, .-puts
	.ident	"GCC: (T-HEAD RISCV Tools V1.10.4 B20210120) 8.4.0"
