# The ICS: Data Lab
Directions to Students

Your goal is to modify your copy of bits.c so that it passes all the
tests in btest without violating any of the coding guidelines.

***********************************************************
1. Modifying bits.c and checking it for compliance with dlc
***********************************************************

Carefully read the instructions in the bits.c file before you
start. These give the coding rules that you will need to follow if you
want full credit. 

Use the dlc compiler (./dlc) to automatically check your version of
bits.c for compliance with the rules:

	unix> ./dlc bits.c

dlc returns silently if there are no problems with your code.
Otherwise it prints messages that flag any problems.  Once you have a
legal solution, you can test it for correctness using the ./btest
program.

*************************************
2. Testing for correctness with btest
*************************************

The Makefile in this directory compiles your version of bits.c with
additional code to create a program (or test harness) named btest.

Give the command:

	unix> make

to compile the code.

Run the program with the command:

	unix> ./btest [optional command line arguments]

When moving from one platform to another, you want to get rid of the
old version of btest and generate a new one.  Use the commands:

	unix> make clean
	unix> make

*******************
3. What btest does
*******************

Btest tests your procedures for correctness by running a number of test
cases.  It does not guarantee exhaustive evaluation.  In addition, it does
not check your code for compliance with the coding guidelines.  Use dlc to
do that.

If you think the functions provided in btest are incorrect send mail
to the lead person for this assignment.

Here are the command line options for btest:
   -e N     Limit number of errors to report for single function to N
            (Default unbounded)
   -f Name  Check only the named function
   -g       Prints concise report (implies -v 0 and -e 0)
   -h       Print the list of options
   -v N     Set verbosity to level N
            N=0 Only give final scores
            N=1 Also report individual correctness scores (default)
