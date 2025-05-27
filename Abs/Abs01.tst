load Abs.vm,
output-file Abs01.out,
compare-to Abs01.cmp,
output-list RAM[17]%D1.6.1;

set sp 256,
set static 0 7,  // x = 7

repeat 25 {
  vmstep;
}
output;
