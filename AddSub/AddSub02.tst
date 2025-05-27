load AddSub.vm,
output-file AddSub02.out,
compare-to AddSub02.cmp,
output-list RAM[16]%D1.6.1;

set sp 256,
set local 300,
set RAM[300] 50,     // local 0 = 50
set RAM[301] 25,     // local 1 = 25
set RAM[16] 10,      // static 0 = 10
