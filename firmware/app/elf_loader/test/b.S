
b.o:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <test_extern_add>:
   0:	1101                	addi	sp,sp,-32
   2:	ce06                	sw	ra,28(sp)
   4:	cc22                	sw	s0,24(sp)
   6:	1000                	addi	s0,sp,32
   8:	fea42623          	sw	a0,-20(s0)
   c:	feb42423          	sw	a1,-24(s0)
  10:	fe842583          	lw	a1,-24(s0)
  14:	fec42503          	lw	a0,-20(s0)
  18:	00000097          	auipc	ra,0x0
  1c:	000080e7          	jalr	ra # 18 <test_extern_add+0x18>
  20:	87aa                	mv	a5,a0
  22:	853e                	mv	a0,a5
  24:	40f2                	lw	ra,28(sp)
  26:	4462                	lw	s0,24(sp)
  28:	6105                	addi	sp,sp,32
  2a:	8082                	ret

0000002c <test_static_add>:
  2c:	1101                	addi	sp,sp,-32
  2e:	ce06                	sw	ra,28(sp)
  30:	cc22                	sw	s0,24(sp)
  32:	1000                	addi	s0,sp,32
  34:	fea42623          	sw	a0,-20(s0)
  38:	feb42423          	sw	a1,-24(s0)
  3c:	000007b7          	lui	a5,0x0
  40:	0007a783          	lw	a5,0(a5) # 0 <test_extern_add>
  44:	fe842583          	lw	a1,-24(s0)
  48:	fec42503          	lw	a0,-20(s0)
  4c:	9782                	jalr	a5
  4e:	87aa                	mv	a5,a0
  50:	853e                	mv	a0,a5
  52:	40f2                	lw	ra,28(sp)
  54:	4462                	lw	s0,24(sp)
  56:	6105                	addi	sp,sp,32
  58:	8082                	ret

0000005a <test_local_add>:
  5a:	7179                	addi	sp,sp,-48
  5c:	d606                	sw	ra,44(sp)
  5e:	d422                	sw	s0,40(sp)
  60:	1800                	addi	s0,sp,48
  62:	fca42e23          	sw	a0,-36(s0)
  66:	fcb42c23          	sw	a1,-40(s0)
  6a:	000007b7          	lui	a5,0x0
  6e:	0007a783          	lw	a5,0(a5) # 0 <test_extern_add>
  72:	fef42623          	sw	a5,-20(s0)
  76:	fec42783          	lw	a5,-20(s0)
  7a:	fd842583          	lw	a1,-40(s0)
  7e:	fdc42503          	lw	a0,-36(s0)
  82:	9782                	jalr	a5
  84:	87aa                	mv	a5,a0
  86:	853e                	mv	a0,a5
  88:	50b2                	lw	ra,44(sp)
  8a:	5422                	lw	s0,40(sp)
  8c:	6145                	addi	sp,sp,48
  8e:	8082                	ret
