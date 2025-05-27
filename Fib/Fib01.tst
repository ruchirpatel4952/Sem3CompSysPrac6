load Sys.vm,
output-file Fib01.out,
compare-to Fib01.cmp,
output-list RAM[300]%D1.6.1;

set sp 256,

push constant 4,
call Fib.fib 1,
pop RAM[300],  // Store result

repeat 200 {
  vmstep;
}
output;
