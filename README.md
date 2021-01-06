# cmdParser
This project implements a simple command line parsing utility for C programs.
I often find myself needing something like this for new projects; hopefully 
it can jumpstart such efforts in the future.  A small demo program is
included for reference.

To customize:
1) define the number of command line options to support in the header, along
with their maximum length if different from the default hardcoded values.
2) define the option letters and longform aliases at the top of the C file, 
along with whether or not the options take an argument.  Help text for each
can also be defined.
3) fill in else/if cases in applyOpt to transfer each string argument/option
to a structure that you define.

To use:
1) initialize the option parser with initOpt
2) call processOpts, passing in the options_t structure created by initOpt
and the arguments to main (argc and argv).
3) use the options as needed in the rest of your program.

Example linux compile of the demo:
gcc -O2 cmdOptions.c cmdOptions.h demo.c -o demo



