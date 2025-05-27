load Sys.vm,
output-file Fib02.out,
compare-to Fib02.cmp,
output-list RAM[300]%D1.6.1;

set sp 256,

push constant 6,
call Fib.fib 1,
pop RAM[300],

repeat 300 {
  vmstep;
}
output;
