#ifndef __EMU__H
#define __EMU__H
#include <string>
class Emu
{
  public:
  Emu()
  {
    for(int i = 0x0; i < 0x800; ++i) ram[i] = &mem[i];
    for(int i = 0x2000, j = 0; i < 0x2008; ++i) reg[j] = &mem[i];
    for(int i = 0x4000, j = 0; i < 0x4020; ++i, ++j) regAPUIO[j] = &mem[i];
    romData = NULL;
  }
  Emu(std::string rom)
  {
    for(int i = 0x0; i < 0x800; ++i) ram[i] = &mem[i];
    for(int i = 0x2000, j = 0; i < 0x2008; ++i) reg[j] = &mem[i];
    for(int i = 0x4000, j = 0; i < 0x4020; ++i, ++j) regAPUIO[j] = &mem[i];
    romData = NULL;
    loadRom(rom);
  }
  ~Emu()
  {
    if(romData) delete []romData;
  }
  void loadRom(std::string);
  private:
  unsigned char *romData;
  std::string romName;
  int mem[0x10000];
  int *reg[0x8], *regAPUIO[0x20], *ram[0x800];
};
#endif
