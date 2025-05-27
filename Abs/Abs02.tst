load Abs.vm,
output-file Abs02.out,
compare-to Abs02.cmp,
output-list RAM[17]%D1.6.1;

set sp 256,
set static 0 0,  // x = 0

repeat 25 {
  vmstep;
}
output;
