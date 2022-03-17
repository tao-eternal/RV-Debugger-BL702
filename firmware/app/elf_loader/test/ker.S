
ker.o:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <printf>:
   0:	715d                	addi	sp,sp,-80
   2:	d606                	sw	ra,44(sp)
   4:	d422                	sw	s0,40(sp)
   6:	1800                	addi	s0,sp,48
   8:	fca42e23          	sw	a0,-36(s0)
   c:	c04c                	sw	a1,4(s0)
   e:	c410                	sw	a2,8(s0)
  10:	c454                	sw	a3,12(s0)
  12:	c818                	sw	a4,16(s0)
  14:	c85c                	sw	a5,20(s0)
  16:	01042c23          	sw	a6,24(s0)
  1a:	01142e23          	sw	a7,28(s0)
  1e:	02040793          	addi	a5,s0,32
  22:	1791                	addi	a5,a5,-28
  24:	fef42623          	sw	a5,-20(s0)
  28:	000007b7          	lui	a5,0x0
  2c:	0007a703          	lw	a4,0(a5) # 0 <printf>
  30:	fec42783          	lw	a5,-20(s0)
  34:	85be                	mv	a1,a5
  36:	fdc42503          	lw	a0,-36(s0)
  3a:	9702                	jalr	a4
  3c:	4781                	li	a5,0
  3e:	853e                	mv	a0,a5
  40:	50b2                	lw	ra,44(sp)
  42:	5422                	lw	s0,40(sp)
  44:	6161                	addi	sp,sp,80
  46:	8082                	ret

00000048 <puts>:
  48:	1101                	addi	sp,sp,-32
  4a:	ce06                	sw	ra,28(sp)
  4c:	cc22                	sw	s0,24(sp)
  4e:	1000                	addi	s0,sp,32
  50:	fea42623          	sw	a0,-20(s0)
  54:	fec42503          	lw	a0,-20(s0)
  58:	00000097          	auipc	ra,0x0
  5c:	000080e7          	jalr	ra # 58 <puts+0x10>
  60:	4781                	li	a5,0
  62:	853e                	mv	a0,a5
  64:	40f2                	lw	ra,28(sp)
  66:	4462                	lw	s0,24(sp)
  68:	6105                	addi	sp,sp,32
  6a:	8082                	ret
