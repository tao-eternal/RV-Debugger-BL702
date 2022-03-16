
func.o:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <add>:
   0:	1101                	addi	sp,sp,-32
   2:	ce22                	sw	s0,28(sp)
   4:	1000                	addi	s0,sp,32
   6:	fea42623          	sw	a0,-20(s0)
   a:	feb42423          	sw	a1,-24(s0)
   e:	fec42703          	lw	a4,-20(s0)
  12:	fe842783          	lw	a5,-24(s0)
  16:	97ba                	add	a5,a5,a4
  18:	853e                	mv	a0,a5
  1a:	4472                	lw	s0,28(sp)
  1c:	6105                	addi	sp,sp,32
  1e:	8082                	ret

00000020 <fib>:
  20:	1101                	addi	sp,sp,-32
  22:	ce06                	sw	ra,28(sp)
  24:	cc22                	sw	s0,24(sp)
  26:	ca26                	sw	s1,20(sp)
  28:	1000                	addi	s0,sp,32
  2a:	fea42623          	sw	a0,-20(s0)
  2e:	fec42703          	lw	a4,-20(s0)
  32:	4785                	li	a5,1
  34:	00e7c463          	blt	a5,a4,3c <.L4>
  38:	4785                	li	a5,1
  3a:	a025                	j	62 <.L5>

0000003c <.L4>:
  3c:	fec42783          	lw	a5,-20(s0)
  40:	17fd                	addi	a5,a5,-1
  42:	853e                	mv	a0,a5
  44:	00000097          	auipc	ra,0x0
  48:	000080e7          	jalr	ra # 44 <.L4+0x8>
  4c:	84aa                	mv	s1,a0
  4e:	fec42783          	lw	a5,-20(s0)
  52:	17f9                	addi	a5,a5,-2
  54:	853e                	mv	a0,a5
  56:	00000097          	auipc	ra,0x0
  5a:	000080e7          	jalr	ra # 56 <.L4+0x1a>
  5e:	87aa                	mv	a5,a0
  60:	97a6                	add	a5,a5,s1

00000062 <.L5>:
  62:	853e                	mv	a0,a5
  64:	40f2                	lw	ra,28(sp)
  66:	4462                	lw	s0,24(sp)
  68:	44d2                	lw	s1,20(sp)
  6a:	6105                	addi	sp,sp,32
  6c:	8082                	ret

0000006e <hello>:
  6e:	1141                	addi	sp,sp,-16
  70:	c606                	sw	ra,12(sp)
  72:	c422                	sw	s0,8(sp)
  74:	0800                	addi	s0,sp,16
  76:	000007b7          	lui	a5,0x0
  7a:	00078513          	mv	a0,a5
  7e:	00000097          	auipc	ra,0x0
  82:	000080e7          	jalr	ra # 7e <hello+0x10>
  86:	4781                	li	a5,0
  88:	853e                	mv	a0,a5
  8a:	40b2                	lw	ra,12(sp)
  8c:	4422                	lw	s0,8(sp)
  8e:	0141                	addi	sp,sp,16
  90:	8082                	ret

00000092 <test_datarel>:
  92:	1101                	addi	sp,sp,-32
  94:	ce06                	sw	ra,28(sp)
  96:	cc22                	sw	s0,24(sp)
  98:	1000                	addi	s0,sp,32
  9a:	000007b7          	lui	a5,0x0
  9e:	0007a783          	lw	a5,0(a5) # 0 <add>
  a2:	fef42623          	sw	a5,-20(s0)
  a6:	fec42783          	lw	a5,-20(s0)
  aa:	863e                	mv	a2,a5
  ac:	87fd                	srai	a5,a5,0x1f
  ae:	86be                	mv	a3,a5
  b0:	000007b7          	lui	a5,0x0
  b4:	00078513          	mv	a0,a5
  b8:	00000097          	auipc	ra,0x0
  bc:	000080e7          	jalr	ra # b8 <test_datarel+0x26>
  c0:	fec42703          	lw	a4,-20(s0)
  c4:	000007b7          	lui	a5,0x0
  c8:	00078513          	mv	a0,a5
  cc:	9702                	jalr	a4
  ce:	4781                	li	a5,0
  d0:	853e                	mv	a0,a5
  d2:	40f2                	lw	ra,28(sp)
  d4:	4462                	lw	s0,24(sp)
  d6:	6105                	addi	sp,sp,32
  d8:	8082                	ret

Disassembly of section .data:

00000000 <l_puts>:
   0:	0000                	unimp
	...

Disassembly of section .rodata:

00000000 <.LC0>:
   0:	6548                	flw	fa0,12(a0)
   2:	6c6c                	flw	fa1,92(s0)
   4:	7266206f          	j	6272a <test_datarel+0x62698>
   8:	6f206d6f          	jal	s10,66fa <test_datarel+0x6668>
   c:	6a62                	flw	fs4,24(sp)
   e:	0021                	c.nop	8

00000010 <.LC1>:
  10:	20746567          	0x20746567
  14:	796d                	lui	s2,0xffffb
  16:	705f 7475 2073      	0x20737475705f
  1c:	203d                	jal	4a <.L4+0xe>
  1e:	7830                	flw	fa2,112(s0)
  20:	6c25                	lui	s8,0x9
  22:	0a78                	addi	a4,sp,284
  24:	0000                	unimp
	...

00000028 <.LC2>:
  28:	6574                	flw	fa3,76(a0)
  2a:	64207473          	csrrci	s0,0x642,0
  2e:	7461                	lui	s0,0xffff8
  30:	7261                	lui	tp,0xffff8
  32:	6c65                	lui	s8,0x19
  34:	6f20                	flw	fs0,88(a4)
  36:	000a216b          	0xa216b

Disassembly of section .comment:

00000000 <.comment>:
   0:	4700                	lw	s0,8(a4)
   2:	203a4343          	fmadd.s	ft6,fs4,ft3,ft4,rmm
   6:	5428                	lw	a0,104(s0)
   8:	482d                	li	a6,11
   a:	4145                	li	sp,17
   c:	2044                	fld	fs1,128(s0)
   e:	4952                	lw	s2,20(sp)
  10:	20564353          	0x20564353
  14:	6f54                	flw	fa3,28(a4)
  16:	20736c6f          	jal	s8,36a1c <test_datarel+0x3698a>
  1a:	3156                	fld	ft2,368(sp)
  1c:	312e                	fld	ft2,232(sp)
  1e:	2e30                	fld	fa2,88(a2)
  20:	2034                	fld	fa3,64(s0)
  22:	3242                	fld	ft4,48(sp)
  24:	3230                	fld	fa2,96(a2)
  26:	3031                	jal	fffff832 <test_datarel+0xfffff7a0>
  28:	3231                	jal	fffff934 <test_datarel+0xfffff8a2>
  2a:	2930                	fld	fa2,80(a0)
  2c:	3820                	fld	fs0,112(s0)
  2e:	342e                	fld	fs0,232(sp)
  30:	302e                	fld	ft0,232(sp)
	...

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	2d41                	jal	690 <test_datarel+0x5fe>
   2:	0000                	unimp
   4:	7200                	flw	fs0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <.riscv.attributes+0x14>
   c:	00000023          	sb	zero,0(zero) # 0 <.riscv.attributes>
  10:	7205                	lui	tp,0xfffe1
  12:	3376                	fld	ft6,376(sp)
  14:	6932                	flw	fs2,12(sp)
  16:	7032                	flw	ft0,44(sp)
  18:	5f30                	lw	a2,120(a4)
  1a:	326d                	jal	fffff9c4 <test_datarel+0xfffff932>
  1c:	3070                	fld	fa2,224(s0)
  1e:	615f 7032 5f30      	0x5f307032615f
  24:	3266                	fld	ft4,120(sp)
  26:	3070                	fld	fa2,224(s0)
  28:	635f 7032 0030      	0x307032635f
