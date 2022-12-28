
Names:  Bishoy Sabry - Omar Elayat


RiSC-16


A C++ simulator that assess simplified superscalar out-of-order 16-bit dual issue RISC processor that uses Tomasulo’s algorithm with speculation.

Supports:

	- Arithmetic: ADDI, ADD, MUL, NAND, SUB
	- LOAD and STORE
	- JMP, JAL, and RET
	- BEQ

Features: 


	- Writes the a “.csv” file that contains a table listing:
		1) The clock cycle time at which each instruction was: issued, started execution, finished
		execution, and was written.
		2) The IPC.
		3) The branch misprediction percentage.
		4) The total execution time expressed as the number of cycles spanned.

	- Writes:
		1) The new memory contents.
		2) The state of the reservation stations.
		3) The value of the registers after execution.


Assumptions: 

	- Fetching and decodng take 0 cycles and the instruction queue is already filled with all the instructions to be simulated.
	- The reorder buffer starts with index 1, while index 0 is assumed to always be empty. (not used)
	- When Qj or Qk are empty, their values equal to -1 not 0. ( because of assumption 2)


What Works: All RISC-16 instructions mentioned above.


Bonuses:

a) 12 different test cases implemented (feature 6)
b) implemented and integrated parser for the labels (feature 5)
