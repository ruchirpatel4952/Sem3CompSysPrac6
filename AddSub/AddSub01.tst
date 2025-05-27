// AddSub01.tst

load AddSub.vm,
output-file AddSub01.out,
compare-to AddSub00.cmp,
output-list sp%D1.6.1 local%D1.6.1 argument%D1.8.1 this%D1.6.1 that%D1.6.1
            RAM[16]%D1.6.1 RAM[17]%D1.6.1 RAM[18]%D1.6.1
            local[0]%D1.8.1 local[1]%D1.8.1 local[2]%D1.8.1
            argument[0]%D1.11.1 argument[1]%D1.11.1 argument[2]%D1.11.1;

set sp 256,
set local 300,
set argument 400,
set this 3000,
set that 3010,

set RAM[16] 4,   // static 0
set RAM[17] 5,   // static 1
set RAM[18] 6,   // static 2

set local[0] 15,
set local[1] 25,
set local[2] 35,

set argument[0] 150,
set argument[1] 250,
set argument[2] 350,

run;
