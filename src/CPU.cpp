#include "../include/Bus.h"

CPU::CPU()
{
    using a = CPU;
	lookup = 
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU::~CPU()
{
}

//READ AND WRITE
uint8_t CPU::read(uint16_t addr)
{
    return bus->read(addr, false);
}

void CPU::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

//FLAG OPERATIONS
void CPU::setFlag(FLAGS f, bool v)
{
    if(v)
    {
        status |= f;
    }
    else
    {
        status &= ~f;
    }
}

bool CPU::getFlag(FLAGS f)
{
    return status & f;
}

//CLOCK
void CPU::clock()
{
	if(cycles == 0)
	{
		opcode = read(pc++);

		cycles = lookup[opcode].cycles;
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		cycles += (additional_cycle1 & additional_cycle2);
	}

	cycles--;
}

//ADDRESSING MODES

uint8_t CPU::IMP() //implied
{
	fetched = a;
	return 0;
}

uint8_t CPU::IMM() //immediate
{
	addr_abs = pc++; //absolute address is the program counter
	return 0;
}

uint8_t CPU::ZP0() //zero page
{
	addr_abs = read(pc++); //absolute address is the value at the address of the program counter
	addr_abs &= 0x00FF; //mask the address to 8 bits so it fits in the zero page
	return 0;
}

uint8_t CPU::ZPX() //zero page X indexed
{
	addr_abs = (read(pc++) + x); //absolute address is the value at the address of the program counter + the value of the X register
	addr_abs &= 0x00FF; //mask the address to 8 bits so it fits in the zero page
	return 0;
}

uint8_t CPU::ZPY() //zero page Y indexed
{
	addr_abs = (read(pc++) + y); //absolute address is the value at the address of the program counter + the value of the Y register
	addr_abs &= 0x00FF; //mask the address to 8 bits so it fits in the zero page
	return 0;
}

uint8_t CPU::ABS() //absolute
{
	uint16_t low = read(pc++); //low byte of the absolute address is the value at the address of the program counter
	uint16_t high = read(pc++); //high byte of the absolute address is the value at the address of the program counter + 1
	addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	return 0;
}

uint8_t CPU::ABX() //absolute X indexed
{
	uint16_t low = read(pc++); //low byte of the absolute address is the value at the address of the program counter
	uint16_t high = read(pc++); //high byte of the absolute address is the value at the address of the program counter + 1
	addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	addr_abs = addr_abs + x; //add the value of the X register to the absolute address

	if((addr_abs & 0xFF00) != (high << 8)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
	{
		return 1; //return 1 to indicate an additional cycle is needed
	}
	else
	{
		return 0; //return 0 to indicate no additional cycle is needed
	}
}

uint8_t CPU::ABY() //absolute Y indexed
{
	uint16_t low = read(pc++); //low byte of the absolute address is the value at the address of the program counter
	uint16_t high = read(pc++); //high byte of the absolute address is the value at the address of the program counter + 1
	addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	addr_abs = addr_abs + y; //add the value of the Y register to the absolute address

	if((addr_abs & 0xFF00) != (high << 8)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
	{
		return 1; //return 1 to indicate an additional cycle is needed
	}
	else
	{
		return 0; //return 0 to indicate no additional cycle is needed
	}
}

uint8_t CPU::IND() //indirect
{
	uint16_t addr_low = read(pc++); //low byte of the address to jump to is the value at the address of the program counter
	uint16_t addr_high = read(pc++); //high byte of the address to jump to is the value at the address of the program counter + 1
	uint16_t addr = (addr_high << 8) | addr_low; //combine the two bytes to form the address to jump to
	uint16_t low = read(addr); //low byte of the absolute address is the value at the address to jump to
	//adding 1 to addr may cause a page boundary to be crossed, so we need to check for this
	//if the page boundary is crossed, the location of the high byte of the absolute address is looked for within the same page with rollover of addr
	if(addr_low == 0x00FF) //simulate page boundary hardware bug
	{
		uint16_t high = read(addr & 0xFF00); //high byte of the absolute address is the value at the address to jump to + 1 but looked for in the same page with rollover
		addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	}
	else //normal behaviour
	{
		uint16_t high = read(++addr); //high byte of the absolute address is the value at the address to jump to + 1
		addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	}
	return 0;
}

uint8_t CPU::IZX() //indirect X indexed (looked for in zero page)
{
	uint16_t addr = read(pc++); //address to jump to is the value at the address of the program counter in the zero page
	uint16_t low = read((addr + x) & 0x00FF); //low byte of the absolute address is the value at the address to jump to + the value of the X register in the zero page
	uint16_t high = read((addr + x + 1) & 0x00FF); //high byte of the absolute address is the value at the address to jump to + the value of the X register + 1 in the zero page
	addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	return 0;
}

uint8_t CPU::IZY() //indirect Y indexed (looked for in zero page)
{
	uint16_t addr = read(pc++); //address to jump to is the value at the address of the program counter in the zero page
	uint16_t low = read(addr & 0x00FF); //low byte of the absolute address is the value at the address to jump to in the zero page
	uint16_t high = read((addr + 1) & 0x00FF); //high byte of the absolute address is the value at the address to jump to + 1 in the zero page
	addr_abs = (high << 8) | low; //combine the two bytes to form the absolute address
	addr_abs = addr_abs + y; //add the value of the Y register to the absolute address

	if((addr_abs & 0xFF00) != (high << 8)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
	{
		return 1; //return 1 to indicate an additional cycle is needed
	}
	else
	{
		return 0; //return 0 to indicate no additional cycle is needed
	}
}

uint8_t CPU::REL() //relative
{
	addr_rel = read(pc++); //relative address is the value at the address of the program counter
	if(addr_rel & 0x80) //if the high bit is of the lower byte is set
	{
		addr_rel |= 0xFF00; //set the high byte to 1s to form a negative 16 bit number (two's complement)
	}
	return 0; 
}

//INSTRUCTIONS

//fetching
uint8_t CPU::fetch()
{
	if(!(lookup[opcode].addrmode == &CPU::IMP)) //if the addressing mode is not implied
	{
		fetched = read(addr_abs); //fetched is the value at the absolute address
	}
	return fetched;
}

//AND
uint8_t CPU::AND()
{
	fetch();
	a = a & fetched; //AND the value in the accumulator with the fetched value
	setFlag(Z, a == 0x00); //set the zero flag if the result is 0
	setFlag(N, a & 0x80); //set the negative flag if the high bit of the result is set
	return 1; //return 1 to indicate an additional cycle may be needed
}

//ADC
uint8_t CPU::ADC()
{
	//decimal mode not implemented: TODO
	fetch();
	uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)getFlag(C); //add the value in the accumulator with the fetched value and the carry flag
	setFlag(C, temp > 255); //set the carry flag if the result is greater than 255
	setFlag(Z, (temp & 0x00FF) == 0); //set the zero flag if the result is 0
	setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080); //set the overflow flag if the sign bit of the result is different to the sign bit of the accumulator and the fetched value
	a = temp & 0x00FF; //set the accumulator to the lower byte of the result
	setFlag(N, a & 0x80); //set the negative flag if the high bit of the result is set
	return 1; //return 1 to indicate an additional cycle may be needed
}

//ASL
uint8_t CPU::ASL()
{
	fetch();
	uint16_t temp = (uint16_t)fetched << 1; //shift the fetched value left by 1
	setFlag(C, (temp & 0xFF00) > 0); //set the carry flag if the high bit of the result is set
	setFlag(Z, (temp & 0x00FF) == 0); //set the zero flag if the result is 0
	setFlag(N, temp & 0x80); //set the negative flag if the high bit of the result is set
	if(lookup[opcode].addrmode == &CPU::IMP) //if the addressing mode is implied
	{
		a = temp & 0x00FF; //set the accumulator to the lower byte of the result
	}
	else
	{
		write(addr_abs, temp & 0x00FF); //write the lower byte of the result to the absolute address
	}
	return 0; //return 0 to indicate that no additional cycle is needed
}

//BCC
uint8_t CPU::BCC()
{
	if(getFlag(C) == 0) //if the carry flag is not set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BCS
uint8_t CPU::BCS()
{
	if(getFlag(C) == 1) //if the carry flag is not set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BEQ
uint8_t CPU::BEQ()
{
	if(getFlag(Z) == 1) //if the zero flag is set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BIT
uint8_t CPU::BIT()
{
	fetch();
	uint8_t temp = a & fetched; //AND the value in the accumulator with the fetched value
	setFlag(Z, temp == 0x00); //set the zero flag if the result is 0
	setFlag(N, fetched & (1 << 7)); //set the negative flag if the high bit of the fetched value is set
	setFlag(V, fetched & (1 << 6)); //set the overflow flag if the second highest bit of the fetched value is set
	return 0; //return 0 to indicate no additional cycle is needed
}

//BMI
uint8_t CPU::BMI()
{
	if(getFlag(N) == 1) //if the negative flag is set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BNE
uint8_t CPU::BNE()
{
	if(getFlag(Z) == 0) //if the zero flag is not set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BPL
uint8_t CPU::BPL()
{
	if(getFlag(N) == 0) //if the negative flag is not set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BRK
uint8_t CPU::BRK()
{
	pc++; //increment the program counter
	setFlag(I, 1); //set the interrupt flag
	write(0x0100 + sp, (pc >> 8) & 0x00FF); //write the high byte of the program counter to the stack
	sp--; //decrement the stack pointer
	write(0x0100 + sp, pc & 0x00FF); //write the low byte of the program counter to the stack
	sp--; //decrement the stack pointer
	setFlag(B, 1); //set the break flag
	write(0x0100 + sp, status); //write the status register to the stack
	sp--; //decrement the stack pointer
	setFlag(B, 0); //clear the break flag
	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8); //set the program counter to the address stored at 0xFFFE
	return 0; //return 0 to indicate no additional cycle is needed
}

//BVC
uint8_t CPU::BVC()
{
	if(getFlag(V) == 0) //if the overflow flag is not set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//BVS
uint8_t CPU::BVS()
{
	if(getFlag(V) == 1) //if the overflow flag is set
	{
		cycles++; //add an additional cycle
		addr_abs = pc + addr_rel; //set the absolute address to the program counter + the relative address
		if((addr_abs & 0xFF00) != (pc & 0xFF00)) //if the high byte of the absolute address has changed (i.e. the address has crossed a page boundary)
		{
			cycles++; //add an additional cycle
		}
		pc = addr_abs; //set the program counter to the absolute address
	}
	return 0; //return 0 to indicate no additional cycle is needed
}

//CLC
uint8_t CPU::CLC()
{
	setFlag(C, false); //set carry flag to 0
	return 0;
}

//CLD
uint8_t CPU::CLD()
{
	setFlag(D, false); //set decimal mode flag to 0
	return 0;
}

//CLI
uint8_t CPU::CLI()
{
	setFlag(I, false); //set interrupt disable flag to 0
	return 0;
}

//CLV
uint8_t CPU::CLV()
{
	setFlag(V, false); //set overflow flag to 0
	return 0;
}

//CMP
uint8_t CPU::CMP()
{
	fetch();
	uint16_t temp = (uint16_t) a - (uint16_t) fetched;
	setFlag(C, a >= fetched); //set carry flag if the accumulator is greater than or equal to the fetched value
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set zero flag if the result is 0
	setFlag(N, temp & 0x0080); //set negative flag if the high bit of the result is set
	return 1;
}

//CPX
uint8_t CPU::CPX()
{
	fetch();
	uint16_t temp = (uint16_t) x - (uint16_t) fetched;
	setFlag(C, x >= fetched); //set carry flag if the x register is greater than or equal to the fetched value
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set zero flag if the result is 0
	setFlag(N, temp & 0x0080); //set negative flag if the high bit of the result is set
	return 0;
}

//CPY
uint8_t CPU::CPY()
{
	fetch();
	uint16_t temp = (uint16_t) y - (uint16_t) fetched;
	setFlag(C, y >= fetched); //set carry flag if the y register is greater than or equal to the fetched value
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set zero flag if the result is 0
	setFlag(N, temp & 0x0080); //set negative flag if the high bit of the result is set
	return 0;
}

//DEC
uint8_t CPU::DEC()
{
	fetch();
	uint16_t temp = fetched - 1;
	write(addr_abs, temp & 0x00FF); //write the result to the address
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set zero flag if the result is 0
	setFlag(N, temp & 0x0080); //set negative flag if the high bit of the result is set
	return 0;
}

//DEX
uint8_t CPU::DEX()
{
	x--;
	setFlag(Z, x == 0x00); //set zero flag if the x register is 0
	setFlag(N, x & 0x80); //set negative flag if the high bit of the x register is set
	return 0;
}

//DEY
uint8_t CPU::DEY()
{
	y--;
	setFlag(Z, y == 0x00); //set zero flag if the y register is 0
	setFlag(N, y & 0x80); //set negative flag if the high bit of the y register is set
	return 0;
}

//EOR
uint8_t CPU::EOR()
{
	fetch();
	a = a ^ fetched; //perform bitwise exclusive or on the accumulator and the fetched value
	setFlag(Z, a == 0x00); //set zero flag if the result is 0
	setFlag(N, a & 0x80); //set negative flag if the high bit of the result is set
	return 1;
}

//INC
uint8_t CPU::INC()
{
	fetch();
	uint16_t temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set zero flag is the result is 0
	setFlag(N, temp & 0x0080);  //set negative flag is the high bit of the result is set
	return 0;
}

//INX
uint8_t CPU::INX()
{
	x++;
	setFlag(Z, x == 0x00); //set zero flag if the x register is 0
	setFlag(N, x & 0x80); //set negative flag if the high bit of the x register is set
	return 0;
}

//INY
uint8_t CPU::INY()
{
	y++;
	setFlag(Z, y == 0x00); //set zero flag if the y register is 0
	setFlag(N, y & 0x80); //set negative flag if the high bit of the y register is set
	return 0;
}

//JMP
uint8_t CPU::JMP()
{
	pc = addr_abs; //set the program counter to point to the address specified
	return 0;
}

//JSR
uint8_t CPU::JSR()
{
	pc--;
	write(0x0100 + sp, (pc >> 8) & 0x00FF); //push the high byte of the program counter onto the stack
	sp--;
	write(0x0100 + sp, pc & 0x00FF); //push the low byte of the program counter onto the stack
	sp--;
	pc = addr_abs; //set the program counter to point to the address specified
	return 0;
}

//LDS
uint8_t CPU::LDA()
{
	fetch();
	a = fetched; //load the accumulator with the value fetched
	return 1;
}

//LDX
uint8_t CPU::LDX()
{
	fetch();
	x = fetched; //load the x register with the value fetched
	return 1;
}

//LDY
uint8_t CPU::LDY()
{
	fetch();
	y = fetched; //load the y register with the value fetched
	return 1;
}

//LSR
uint8_t CPU::LSR()
{
	fetch();
	uint8_t temp = fetched >> 1;
	setFlag(N, false); //clear the negative flag
	setFlag(Z, temp == 0x00); //set the zero flag if the result is 0
	setFlag(C, fetched & 0x01); //set the carry flag with the least significant bit of the fetched value
	return 0;
}

//NOP
uint8_t CPU::NOP()
{
	return 0;
}

//ORA
uint8_t CPU::ORA()
{
	fetch();
	a = a | fetched;
	setFlag(Z, a == 0x00); //set zero flag if the value in the accumulator is 0
	setFlag(N, a & 0x80); //set the negative flag if the high bit of the accumulator is set
	return 0;
}

//PHA
uint8_t CPU::PHA()
{
	write(0x0100 + sp, a); //write the value in the accumulator to the top of the stack
	sp--; //decrement the stack pointer
	return 0;
}

//PHP
uint8_t CPU::PHP()
{
	write(0x0100 + sp, status); //write the value in the status register to the top of the stack
	sp--; //decrement the stack pointer
	return 0;
}

//PLA
uint8_t CPU::PLA()
{
	a = read(0x0100 + sp); //read the value on the top of the stack into the accumulator
	sp++; //increment the stack pointer
	setFlag(Z, a == 0x00); //set the zero flag if the value in the accumulator is set to 0
	setFlag(N, a & 0x80); //set the negative flag if the high bit of the accumulator is set
	return 0;
}

//PLP
uint8_t CPU::PLP()
{
	status = read(0x0100 + sp); //read the value on the top of the stack into the accumulator
	sp++; //increment the stack pointer
	setFlag(Z, status == 0x00); //set the zero flag if the value in the accumulator is set to 0
	setFlag(N, status & 0x80); //set the negative flag if the high bit of the accumulator is set
	return 0;
}

//ROL
uint8_t CPU::ROL()
{
	fetch();
	uint16_t temp = fetched << 1; //shift the fetched value by 1 place
	if(getFlag(C))
	{
		temp += 1; //add 1 if the carry flag is set
	}
	setFlag(C, fetched & 0x80); //set the carry flag if the high bit of the fetched value was set
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set the zero flag if the lower byte of the result is 0
	setFlag(N, temp & 0x0080); //set the negative flag if the high bit of the lower byte of the result is 1
	if(lookup[opcode].addrmode == &CPU::IMP) //if the addressing mode is implied
	{
		a = temp & 0x00FF; //load the accumulator with the result
	}
	else
	{
		write(addr_abs, temp & 0x00FF); //write the result into the address specified
	}
	return 0;
}

//ROR
uint8_t CPU::ROR()
{
	fetch();
	uint8_t temp = (fetched >> 1) | (getFlag(C) << 7); //shift the fetched value left by 1 place and fill the most significant bit with the value of the carry flag
	setFlag(Z, temp == 0x00); //set the zero flag if the result is 0
	setFlag(C, fetched & 0x01); //set the carry flag if the least significant bit of the fetched value is 1
	setFlag(N, temp & 0x80); //set the negative flag is the high bit of the result is 1
	if(lookup[opcode].addrmode == &CPU::IMP) //if the addressing mode is implied
	{
		a = temp; //load the accumulator with the result
	}
	else
	{
		write(addr_abs, temp); //write the result into the address specified
	}
	return 0;
}

//RTI
uint8_t CPU::RTI()
{
	sp++; //increment the stack pointer
	status = read(0x0100 + sp); //read the value on the top of the stack into the status register
	status &= ~B; //clear the break flag
	status &= ~U; //clear the unused flag
	sp++; //increment the stack pointer
	pc = (uint16_t) read(0x0100 + sp) | ((uint16_t) read(0x0100 + sp + 1) << 8); //read the value on the top of the stack into the program counter
	sp++; //increment the stack pointer
	return 0;
}

//RTS
uint8_t CPU::RTS()
{
	sp++; //increment the stack pointer
	pc = (uint16_t) read(0x0100 + sp) | ((uint16_t) read(0x0100 + sp + 1) << 8); //read the value on the top of the stack into the program counter
	sp++; //increment the stack pointer
	pc++; //increment the program counter
	return 0;
}

//SBC
uint8_t CPU::SBC()
{
	fetch();
	uint16_t temp = (uint16_t) a - (uint16_t) fetched - (1 - (uint16_t) getFlag(C)); //subtract the fetched value from the accumulator and the carry flag
	setFlag(C, temp > 0x00FF); //set the carry flag if the result is greater than 255
	setFlag(Z, (temp & 0x00FF) == 0x0000); //set the zero flag if the lower byte of the result is 0
	setFlag(N, temp & 0x0080); //set the negative flag if the high bit of the lower byte of the result is 1
	setFlag(V, ((uint16_t) a ^ temp) & ((uint16_t) fetched ^ temp) & 0x0080); //set the overflow flag if the sign of the accumulator and the fetched value are different and the sign of the result is different
	a = temp & 0x00FF; //load the accumulator with the lower byte of the result
	return 1;
}

//SEC
uint8_t CPU::SEC()
{
	setFlag(C, true); //set the carry flag
	return 0;
}

//SED
uint8_t CPU::SED()
{
	setFlag(D, true); //set the decimal flag
	return 0;
}

//SEI
uint8_t CPU::SEI()
{
	setFlag(I, true); //set the interrupt flag
	return 0;
}

//STA
uint8_t CPU::STA()
{
	write(addr_abs, a); //write the value in the accumulator to the address specified
	return 0;
}

//STX
uint8_t CPU::STX()
{
	write(addr_abs, x); //write the value in the x register to the address specified
	return 0;
}

//STY
uint8_t CPU::STY()
{
	write(addr_abs, y); //write the value in the y register to the address specified
	return 0;
}

//TAX
uint8_t CPU::TAX()
{
	x = a; //load the x register with the value in the accumulator
	setFlag(Z, x == 0x00); //set the zero flag if the value in the x register is 0
	setFlag(N, x & 0x80); //set the negative flag if the high bit of the x register is set
	return 0;
}

//TAY
uint8_t CPU::TAY()
{
	y = a; //load the y register with the value in the accumulator
	setFlag(Z, y == 0x00); //set the zero flag if the value in the y register is 0
	setFlag(N, y & 0x80); //set the negative flag if the high bit of the y register is set
	return 0;
}

//TSX
uint8_t CPU::TSX()
{
	x = sp; //load the x register with the value in the stack pointer
	setFlag(Z, x == 0x00); //set the zero flag if the value in the x register is 0
	setFlag(N, x & 0x80); //set the negative flag if the high bit of the x register is set
	return 0;
}

//TXA
uint8_t CPU::TXA()
{
	a = x; //load the accumulator with the value in the x register
	setFlag(Z, a == 0x00); //set the zero flag if the value in the accumulator is 0
	setFlag(N, a & 0x80); //set the negative flag if the high bit of the accumulator is set
	return 0;
}

//TXS
uint8_t CPU::TXS()
{
	sp = x; //load the stack pointer with the value in the x register
	return 0;
}

//TYA
uint8_t CPU::TYA()
{
	a = y; //load the accumulator with the value in the y register
	setFlag(Z, a == 0x00); //set the zero flag if the value in the accumulator is 0
	setFlag(N, a & 0x80); //set the negative flag if the high bit of the accumulator is set
	return 0;
}

//XXX
uint8_t CPU::XXX()
{
	return 0;
}

//ASYNCHRONOUS

//Reset the CPU
void CPU::reset()
{
	a = 0x00; //clear the accumulator
	x = 0x00; //clear the x register
	y = 0x00; //clear the y register
	sp = 0xFD; //set the stack pointer to 0xFD
	status = 0x00 | U; //clear the status register and set the unused flag
	addr_abs = 0xFFFC; //set the address to 0xFFFC
	uint16_t low = read(addr_abs + 0); //read the low byte of the address
	uint16_t high = read(addr_abs + 1); //read the high byte of the address
	pc = (high << 8) | low; //set the program counter to the address specified
	addr_rel = 0x0000; //clear the relative address
	addr_abs = 0x0000; //clear the absolute address
	fetched = 0x00; //clear the fetched value
	cycles = 8; //set the number of cycles to 8
}

//Interrupt request
void CPU::irq()
{
	if(getFlag(I) == 0) //if the interrupt flag is not set
	{
		write(0x0100 + sp, (pc >> 8) & 0x00FF); //write the high byte of the program counter to the stack
		sp--; //decrement the stack pointer
		write(0x0100 + sp, pc & 0x00FF); //write the low byte of the program counter to the stack
		sp--; //decrement the stack pointer
		setFlag(B, 0); //clear the break flag
		setFlag(U, 1); //set the unused flag
		setFlag(I, 1); //set the interrupt flag
		write(0x0100 + sp, status); //write the status register to the stack
		sp--; //decrement the stack pointer
		addr_abs = 0xFFFE; //set the address to 0xFFFE
		uint16_t low = read(addr_abs + 0); //read the low byte of the address
		uint16_t high = read(addr_abs + 1); //read the high byte of the address
		pc = (high << 8) | low; //set the program counter to the address specified
		cycles = 7; //set the number of cycles to 7
	}
}

//Non-maskable interrupt
void CPU::nmi()
{
	write(0x0100 + sp, (pc >> 8) & 0x00FF); //write the high byte of the program counter to the stack
	sp--; //decrement the stack pointer
	write(0x0100 + sp, pc & 0x00FF); //write the low byte of the program counter to the stack
	sp--; //decrement the stack pointer
	setFlag(B, 0); //clear the break flag
	setFlag(U, 1); //set the unused flag
	setFlag(I, 1); //set the interrupt flag
	write(0x0100 + sp, status); //write the status register to the stack
	sp--; //decrement the stack pointer
	addr_abs = 0xFFFA; //set the address to 0xFFFE
	uint16_t low = read(addr_abs + 0); //read the low byte of the address
	uint16_t high = read(addr_abs + 1); //read the high byte of the address
	pc = (high << 8) | low; //set the program counter to the address specified
	cycles = 7; //set the number of cycles to 7
}