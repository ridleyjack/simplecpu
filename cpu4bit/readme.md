## 4 Bit CPU Simulator
Simulates a 4 bit CPU for educational purposes.

### Components
- Registers: Two 4 bit registers, RegisterA and RegisterB. (Currently 8 bit).
- Memory: Capacity of 16 addressable words a 4 bits each. Addresses 0x0 to 0xF.
- ALU: Supports add and Subtract.
- Flags: Zero, Overflow, Negative.
- Internal 4bit Registers: Instruction Store(IS), Program Counter(PC), Alu Result Buffer (Currently 8 bits).


### Operation
On startup the CPU runs the instruction at memory address 0x0. It then increments the Program Counter(PC) until a 
Halt instruction (OpCode:0) is reached.

### Instruction Set
 The CPU runs operations specified by a 4bit OpCode, some instructions require one 4 bit argument or two 2 bit arguments. Arguments are 
 specified on the next line of memory, after the op code.

#### Calculations
 Some instructions require the operand containing registers to be specified as two 2 bit arguments. In this case Register A would be 
 specified as 00 and Register B would be specified as 01. The arugment would then be 0001 or 0100 if the oposite order is desired. In some 
 operations order matters, such as subtraction. The result of a calculation is always placed in Register A.

#### OpCodes

| Name   | #  | Code   | Args (4bit)  | Description
|--------|----|--------|--------------|---------------------------|
| Halt   | 00 |  0000  | N/A          | Halt CPU                  |
| LoadA  | 01 |  0001  | Src Address  | Load RegA from Mem[src]   |
| LoadB  | 02 |  0010  | Src Address  | Load RegB from Mem[src]   |
| StoreA | 03 |  0011  | Dest Address | Store RegA to Mem[dest]   |
| Mov    | 04 |  0100  | Reg0, Reg1   | src: Reg0, dest: Reg1     |
| Add    | 05 |  0101  | Reg0, Reg1   | RegA = [Reg0] + [Reg1]    |  
| Sub    | 06 |  0110  | Reg0. Reg1   | RegA = [Reg0] - [Reg1]    |
| Jump   | 07 |  0111  | Ins Address  | Jump PC to address        |
| JumpZ  | 08 |  1000  | Ins Address  | Jump if prev ALU rslt ==0 |
| ZumpNZ | 09 |  1001  | Ins Address  | Jump if prev ALU rslt !=0 |

#### Sample Program
The following program loads 5 into registerA and 2 into registerB. It then adds the registers for a sum of 7 and places this sum into memory location 0xB.

```
Mem
0x0 0001 # Load A
0x1 1001 # Arg=0x9. Register A <- Mem(0x9) = 5
0x2 0002 # Load B
0x3 1010 # Arg=0xA. Register B <- Mem(0xA) = 2
0x4 0101 # Add two registers
0x5 0001 # Arg0=0,Arg1=1 -> RegA,RegB -> RegA + RegB = RegA
0x6 0011 # Store A
0x7 1010 # Arg=0xB. Mem(0xB) = RegA
0x8 0000 Halt # Halt program, data follows:
0x9 0101 # Data: 5 
0xA 0010 # Data: 2
0xB 0000 # Result goes here. Will be 7 after program completes.
...
0xF
