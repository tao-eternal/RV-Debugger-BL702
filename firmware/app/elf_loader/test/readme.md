## compile

----------
```sh
riscv64-unknown-elf-gcc -march=rv32imafc -mabi=ilp32f

-mno-relax -c xxx.c -o xxx.o

nm -g xxx.elf |grep printf
```

then modify **ker.S** and re-compile it

copy **ker.o** and **func.o** into the `msc`

## serial

---------
usb cdc provides shell for input

uart0 (tx:TCK, rx:TDI) provides debug info and is also `printf` 's output