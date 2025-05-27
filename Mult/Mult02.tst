load Mult.vm,
output-file Mult02.out,
compare-to Mult02.cmp,
output-list RAM[300]%D1.6.1;

set sp 256,
set local 300,
set static 0 7,      // multiplicand (x)
set static 1 3,      // multiplier (y)
