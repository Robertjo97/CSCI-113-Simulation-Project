# Memory Simulation Project

## Overview

This is a project I created for my Computer Architecture class at California State Univeristy, Fresno. It is a C++ program that simulates the behavior of a memory hierarchy, including registers, cache, and main memory. This simulation is designed to work with a specific set of machine instructions and provides insight into how memory operations are handled by the cache and the overall system.

## Project Structure

The project is organized into a single C++ source file, which includes the following components:

1. **Structures:**
   - `cache_block`: Represents the structure of a cache block, including set, valid bit, history, tag, and data.
   - `cache`: Defines a 2-way set-associative cache using two arrays of `cache_block` structures.
   - `instruction`: Stores information about a machine instruction, including opcode, source registers, target register, and an offset.

2. **Utility Functions:**
   - `compute_mem_addr(instruction instr)`: Computes the memory address based on an instruction's information.
   - `conversion(int array[], int len)`: Converts binary arrays into decimal integers.
   - `FourBitBin(int n)`, `ThirtyTwoBitBin(int n)`, `SevenBitBin(int n)`: Convert decimal integers into 4-bit, 32-bit, and 7-bit binary representations, respectively.

3. **Memory Operations:**
   - `load_operation(int memory_address, int& rt, int (&regs)[8], cache& cache, int (&memory)[128])`: Handles load operations (LW) and simulates cache behavior, including cache hits and misses.
   - `store_operation(int memory_address, int& rt, int (&regs)[8], cache& cache, int (&memory)[128])`: Handles store operations (SW) and simulates cache behavior, including cache hits and misses.

4. **Instruction Decoding:**
   - `decode(string instr)`: Decodes a machine instruction represented as a binary string and returns an `instruction` structure with the opcode, source registers, target register, and offset.

5. **Execution:**
   - `execute(instruction instr, int (&regs)[8], cache& cache, int (&memory)[128])`: Executes instructions based on their opcode, calling the appropriate load or store operation.

6. **Main Function:**
   - The `main()` function initializes the cache, memory, and registers. It reads machine instructions from an input file, executes them, and then displays the contents of the registers, cache, and memory after execution.

## Usage

To run the Memory Simulation Project, compile the C++ source file and execute the resulting program. Ensure that you have an input file containing machine instructions named "02-Input-object-code" in the same directory as the executable. The program will simulate memory operations, cache behavior, and display the state of the system after execution.

## Input

The program expects machine instructions in binary format as input. Each instruction is represented as a single line in the input file, and the program will read and process these instructions one by one.

## Output

After executing the instructions, the program will display the following information:

- The machine instructions that were executed.
- The contents of registers after execution.
- The state of the cache (both cache block 0 and cache block 1).
- The contents of main memory.

## Conclusion

The Memory Simulation Project provides a comprehensive simulation of memory operations, cache behavior, and the overall system's state after executing machine instructions. It serves as an educational tool to understand how memory hierarchy and cache management work in a computer system.
