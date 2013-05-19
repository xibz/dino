#include <string.h>
#include <fstream>
#include "emu.h"
/*
  loadRom
  This function will load the rom data into the romData buffer
*/
void Emu::loadRom(std::string rom)
{
  std::ifstream in;
  std::filebuf *pbuf = in.rdbuf();
  in.open(rom.c_str(), std::ifstream::in);
  int size = pbuf->pubseekoff(0, in.end, in.in);
  pbuf->pubseekpos(0, in.in);
  romData = new unsigned char[size];
  pbuf->sgetn((char *)romData, size);
  in.close();
}
