load AddSub.vm,
output-file AddSub01.out,
compare-to AddSub01.cmp,
output-list RAM[16]%D1.6.1;

set sp 256,
set local 300,
set RAM[300] 20,     // local 0 = 20
set RAM[301] 10,     // local 1 = 10
set RAM[16] 5,       // static 0 = 5

