#ifndef __EMU__H
#define __EMU__H
#include <string>
class Emu;
//The A register, also called the accumulator, is a general purpose
//register.
//The A, X, Y registers are a byte
//SP is one byte as well. However, since address space of an NES
//is two bytes, the SP registers must be in the 0x01** address area.
//So, is SP = 0x25, then it really points at 0x0125
//PC is two bytes
//ST is the status register. Each bit is used as flags.

//zero flag shows whether the result was zero

//sign flags shows whether the result was negative

//carry flag shows whether the arithmetic operation needed to carry
//or barrow outside of the result byte

//overflow flag reflects the seventh bit of the result

//interrupt disable flag tells the microprocessor whether it all right to
//process an interrupt immediately

//break flag shows whether an interrupt is caused by an external event
//or by a program command

//decimal flag tells the microprocessor whether it is doing binary
//or decimal arithmetic

//The first 0x0000-0x00FF are called zero page. The next page,
//with addresses from 0x01000-0x01FF is called page one
enum registers{A, X, Y, SP, PC, ST};
enum flag{ZERO = 1, SIGN = 2, CARRY = 4, OFLOW = 8, INTERRUPT = 16, BREAK = 32, DECIMAL = 64};
class Emu
{
  public:
  Emu()
  {
    for(int i = 0x0; i < 0x800; ++i) ram[i] = &mem[i];
    for(int i = 0x2000, j = 0; i < 0x2008; ++i, ++j) ppuReg[j] = &mem[i];
    for(int i = 0x4000, j = 0; i < 0x4020; ++i, ++j) regAPUIO[j] = &mem[i];
    romData = NULL;
    romSize = 0;
  }
  Emu(std::string rom)
  {
    for(int i = 0x0; i < 0x800; ++i) ram[i] = &mem[i];
    for(int i = 0x2000, j = 0; i < 0x2008; ++i) ppuReg[j] = &mem[i];
    for(int i = 0x4000, j = 0; i < 0x4020; ++i, ++j) regAPUIO[j] = &mem[i];
    romData = NULL;
    loadRom(rom);
  }
  ~Emu()
  {
    if(romData) delete []romData;
  }
  void initFuncs();
  void loadRom(std::string);
  void runRom();
  void printInst();
  void execInst();
  void adc(int *, int);
  void _and(int *, int);
  void asl(int *);
  void lsr(int *, int);
  int loadAddr();
  int loadAbsAddr();
  int loadVal();
  private:
  char *romData;
  std::string romName;
  int mem[0x10000];
  int *ppuReg[0x8], *regAPUIO[0x20], *ram[0x800];
  int reg[6];
  int romSize;
  int flags;
  short cycles;
};
#endif
