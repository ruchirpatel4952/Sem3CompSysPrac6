load Mult.vm,
output-file Mult01.out,
compare-to Mult01.cmp,
output-list RAM[300]%D1.6.1;

set sp 256,
set local 300,
set static 0 6,
set static 1 5,

repeat 100 {
  vmstep;
}
output;
