load Sys.vm,
output-file ArrMax01.out,
compare-to ArrMax01.cmp,
output-list RAM[16]%D1.6.1;

set sp 256,
set static 1 2048,      // base address of array
set static 2 4,         // length
set RAM[2048] 14,
set RAM[2049] 15,
set RAM[2050] 16,
set RAM[2051] 56,

push constant 2048,     // base address
push constant 4,        // length
call ArrMax.arrMax 2,
pop temp 0,

repeat 300 {
  vmstep;
}
output;
