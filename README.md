# machineCode2Mips55
translate machine code back to mips.asm, supporting 55 instructions including 'MUL' other 54 instructions. (MARS MIPS simulator -settings-memoryConfig-default)

+ addi
+ addiu
+ addu
+ add
+ sub
+ subu 
+ sltiu
+ slti
+ sll 
+ sllv 
+ sltu 
+ sra
+ srl 
+ slt
+ srlv
+ srav
+ lui
+ xori
+ andi
+ ori
+ and
+ xor
+ nor
+ or
+ beq
+ bgez
+ bne
+ j
+ jal
+ jr
+ jalr
+ lw
+ lb
+ lbu
+ lhu
+ sw
+ sb
+ sh
+ lh
+ clz
+ eret
+ syscall
+ teq
+ break
+ mfc0
+ mfhi
+ mflo
+ mtc0
+ mthi
+ mtlo
+ mul
+ multu
+ mult
+ div
+ divu


##  同济大学 CS 计算机组成原理 

    当时给的只有机器码，然后就随便写了一个翻译，写得比较丑😣
  
    如果现在用的还是同一份测试文件的话就直接把example里的代码拿去用吧😘
