#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include <string>
#include <map>

class Bus;
class CPU;

struct INSTRUCTION
{
    std::string name;
    uint8_t(CPU::*operate)(void) = nullptr;
    uint8_t(CPU::*addrmode)(void) = nullptr;
    uint8_t cycles = 0;
};

class CPU
{
public:
    CPU();
    ~CPU();

    void connectBus(Bus *n) { bus = n; }

private:
    Bus* bus = nullptr;

private: //flags
    enum FLAGS
    {
        C = (1 << 0), //carry
        Z = (1 << 1), //zero
        I = (1 << 2), //disable interrupts
        D = (1 << 3), //decimal mode
        B = (1 << 4), //break
        U = (1 << 5), //unused
        V = (1 << 6), //overflow
        N = (1 << 7), //negative
    };

private: //registers
    uint16_t pc = 0x0000; //program counter
    uint8_t sp = 0x00;    //stack pointer (calculated from 0x0100)
    uint8_t a = 0x00;     //accumulator
    uint8_t x = 0x00;     //x register
    uint8_t y = 0x00;     //y register
    uint8_t status = 0x00; //status register

private: //fetched data
    uint8_t fetched = 0x00;
    uint16_t addr_abs = 0x0000;
    uint16_t addr_rel = 0x0000;
    uint8_t opcode = 0x00;
    uint8_t cycles = 0;

private: //lookup table
    std::vector<INSTRUCTION> lookup;

public: //read and write
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

public: //flag operations
    void setFlag(FLAGS f, bool v);
    bool getFlag(FLAGS f);

public: //addressing modes
    uint8_t IMP(); //implied
    uint8_t IMM(); //immediate
    uint8_t ZP0(); //zero page
    uint8_t ZPX(); //zero page, X
    uint8_t ZPY(); //zero page, Y
    uint8_t REL(); //relative
    uint8_t ABS(); //absolute
    uint8_t ABX(); //absolute, X
    uint8_t ABY(); //absolute, Y
    uint8_t IND(); //indirect
    uint8_t IZX(); //indirect, X
    uint8_t IZY(); //indirect, Y

public: //instructions
    uint8_t ADC(); //add with carry
    uint8_t AND(); //and (with accumulator)
    uint8_t ASL(); //arithmetic shift left
    uint8_t BCC(); //branch if carry clear
    uint8_t BCS(); //branch if carry set
    uint8_t BEQ(); //branch if equal
    uint8_t BIT(); //bit test
    uint8_t BMI(); //branch if minus
    uint8_t BNE(); //branch if not equal
    uint8_t BPL(); //branch if plus
    uint8_t BRK(); //force break
    uint8_t BVC(); //branch if overflow clear
    uint8_t BVS(); //branch if overflow set
    uint8_t CLC(); //clear carry flag
    uint8_t CLD(); //clear decimal mode
    uint8_t CLI(); //clear interrupt disable
    uint8_t CLV(); //clear overflow flag
    uint8_t CMP(); //compare (with accumulator)
    uint8_t CPX(); //compare with X register
    uint8_t CPY(); //compare with Y register
    uint8_t DEC(); //decrement memory
    uint8_t DEX(); //decrement X register
    uint8_t DEY(); //decrement Y register
    uint8_t EOR(); //exclusive or (with accumulator)
    uint8_t INC(); //increment memory
    uint8_t INX(); //increment X register
    uint8_t INY(); //increment Y register
    uint8_t JMP(); //jump
    uint8_t JSR(); //jump to subroutine
    uint8_t LDA(); //load accumulator
    uint8_t LDX(); //load X register
    uint8_t LDY(); //load Y register
    uint8_t LSR(); //logical shift right
    uint8_t NOP(); //no operation
    uint8_t ORA(); //or with accumulator
    uint8_t PHA(); //push accumulator
    uint8_t PHP(); //push processor status
    uint8_t PLA(); //pull accumulator
    uint8_t PLP(); //pull processor status
    uint8_t ROL(); //rotate left
    uint8_t ROR(); //rotate right
    uint8_t RTI(); //return from interrupt
    uint8_t RTS(); //return from subroutine
    uint8_t SBC(); //subtract with carry
    uint8_t SEC(); //set carry flag
    uint8_t SED(); //set decimal flag
    uint8_t SEI(); //set interrupt disable
    uint8_t STA(); //store accumulator
    uint8_t STX(); //store X register
    uint8_t STY(); //store Y register
    uint8_t TAX(); //transfer accumulator to X
    uint8_t TAY(); //transfer accumulator to Y
    uint8_t TSX(); //transfer stack pointer to X
    uint8_t TXA(); //transfer X to accumulator
    uint8_t TXS(); //transfer X to stack pointer
    uint8_t TYA(); //transfer Y to accumulator

    uint8_t XXX(); //illegal instruction

public: //clock
    void clock();

public: //asynchronous
    void reset(); //reset
    void irq();  //interrupt request
    void nmi(); //non-maskable interrupt

public: //fetching data
    uint8_t fetch();

public: //helper functions
    bool complete() { return cycles == 0; }
};

#endif