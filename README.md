
## Project Overview

The goal of this project is to implement an architectural simulator capable of assessing the performance of a simplified out-of-order 16-bit RISC processor that uses **Tomasulo’s algorithm without speculation**. This document details the instruction set to be supported, the inputs to the simulator, and the expected outputs 

## Implementation language: 

C++

## Instruction set architecture (ISA) 

The simulator assumes a simplified RISC ISA inspired by the ISA of the Ridiculously Simple Computer (RiSC-16) proposed by Bruce Jacob. As implied by its name, the word size of this computer is 16-bit. The processor has 8 general-purpose registers R0 to R7 (16-bit each). The register R0 always contains the value 0 and cannot be changed. Memory is **word addressable** and uses a 16-bit address (as such the memory capacity is 128 KB). The instruction format itself is not very important to the simulation and therefore is not described here. However, the simulator should support the following set of instructions (16-bit each): 

1. **Load/store**  
- Load word: Loads value from memory into rd. Memory address is formed by adding imm with contents of rs1, where imm is a 7-bit signed immediate value (ranging from -64 to 63).   
  * LW rd, imm(rs1) 
- Store word: Stores value from rs2 into memory. Memory address is computed as in the case of the load word instruction 
  * SW rs2, imm(rs1) 
2. **Conditional branch**  
- Branch if equal: branches to the address PC+imm if rs1=rs2 
  * BEQ rs1, rs2, imm 
3. **Call/Return** 
- Jump and link register: Stores the value of PC+1 in R1 and branches (unconditionally) to the address in rs1.  
  * JALR rs1  
- Return: branches (unconditionally) to the address stored in R1 
  * RET 
4. **Arithmetic** 
- Add: Adds the value of rs1 and rs2 storing the result in rd 
  * ADD rd, rs1, rs2 
- Negates: Inverts the sign of rs1 storing the result in rd 
  * NEG rd, rs1 
- Add immediate: Adds the value of rs1 to imm storing the result in rd 
  * ADDI rd, rs1, imm 
- Div: Divides rs1 by rs2 storing the result in rd 
  * DIV rd, rs1, rs2 

## Simulator inputs:  

1. The assembly program to be simulated. The user should also specify its starting address. Put the RISC code you want to run in “test2.txt”.  
1. Data required by the program to be initially loaded in the memory. For each data item both its value and its memory address should be specified in “mem.txt” file (Note that 0001 must be proceeded with the program starting 
address).

## Simulation

The simulator simulates the program’s execution on a **multiple-issue** processor. It follows the non-speculative version of Tomasulo’s algorithm. The simulator only takes the **3 backend stages** into account: issue (1 cycle), execute (N cycles depending on the functional unit involved as detailed in the table below), and write (1 cycle).  


## Assumptions

* Fetching and decoding take 0 cycles and the instruction queue is already filled with all the instructions to be simulated. 
* The reorder buffer starts with index 1, while index 0 is assumed to always be empty. (not used)
* When Qj or Qk are empty, their values equal to -1 not 0. ( because of assumption 2)
* No floating-point instructions, registers, or functional units 
* No interrupts or exceptions are to be handled 
* There is a one-to-one mapping between reservation stations and functional units. i.e., Each reservation station has a functional unit dedicated to it 
* No cache or virtual memory 
* The following is also assumed about the functional units available: 

||**Number of reservation stations available** |**Number of cycles needed** |
| :- | :-: | - |
|LW unit |2 |1 + 1 (compute address) |
|SW unit |2 |1 (compute address) + 1 (writing to memory) |
|BEQ unit |1 |1 (compute target and compare operands) |
|JALR/RET unit |1 |1 (compute target) |
|ADD/NEG/ADDI unit |2 |2 |
|DIV unit |1 |8 |

**For conditional branches, the processor uses an always not taken predictor. Recall that in the non-speculative version of Tomasulo, instructions can only be issued (but not executed) based on a prediction.** 

While simulating the execution, the program records the number of instructions completed, the number of branches encountered, the number of cycles spanned, and the number of branch mispredictions. 

## Testing

In the testing folder, you can find 12 different test cases implemented and verified with their respective results. To verify the results yourself, follow the step-by-step guide in the documentation


## Simulator output

At the end, the simulator prints the following performance metrics: 

1. A table listing the clock cycle time at which each instruction was: issued, started execution, finished execution, and was written. 
2. The total execution time expressed as the number of cycles spanned 
3. The IPC  
4. The branch misprediction percentage 
5. The new memory contents.
6. The state of the reservation stations.
7. The value of the registers after execution.

***For more information regarding the implementation and the full simulation step-by-step user guide, refer to the documentation "report.pdf" file***
