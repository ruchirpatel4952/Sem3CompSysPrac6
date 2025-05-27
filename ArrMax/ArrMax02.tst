load Sys.vm,
output-file ArrMax02.out,
compare-to ArrMax02.cmp,
output-list RAM[16]%D1.6.1;

set sp 256,
set static 1 2048,
set static 2 4,
set RAM[2048] 7,
set RAM[2049] 7,
set RAM[2050] 7,
set RAM[2051] 7,

push constant 2048,
push constant 4,
call ArrMax.arrMax 2,
pop temp 0,

repeat 300 {
  vmstep;
}
output;
