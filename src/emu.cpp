#include <stdio.h>
#include <string.h>
#include <fstream>
#include "emu.h"
#include "inst.h"
/*
  loadRom
  This function will load the rom data into the romData buffer
*/
#define IP (reg[PC])
#define OFCHECK(A) (A)&=0xFF
int Emu::loadVal()
{
  return loadAddr();
}
int Emu::loadAddr()
{
  ++IP;
  int addr = romData[IP];
  return addr;
}
int Emu::loadAbsAddr()
{
  ++IP;
  int addr = romData[IP];
  addr += (romData[++IP]<<8);
  printf("addr: %d\n", addr);
  return addr;
}

void Emu::adc(int *dst, int src)
{
  int temp;
  if(src >= 0 && *dst & src) flags |= CARRY; 
  if(src < 0 && (temp = *dst ^ src))
    if(temp & src) flags |= CARRY;
  *dst += src; 
  if(*dst > 0xFF)
  {
    *dst &= 0xFF;
    flags |= OFLOW;
  }
  if(*dst&0x80) flags |= SIGN;
  if(*dst&0x7F == 0) flags |= ZERO;
  flags |= (0x20&(*dst));
}

void Emu::_and(int *dst, int src)
{
  *dst &= src;
  if(!*dst) flags |= ZERO;
  if(*dst&0x80) flags |= SIGN;
  OFCHECK(*dst);
}

void Emu::asl(int *dst)
{
  *dst <<= 1;
  flags |= CARRY;
  if(*dst&0x80) flags |= SIGN;
  if(*dst&0x7F == 0) flags |= ZERO;
  OFCHECK(*dst);
}

void Emu::lsr(int *dst, int src)
{
  flags |= (*dst)&1;
  *dst >>= src;
  if(*dst&0x80) flags |= SIGN;
  if(*dst&0x7F == 0) flags |= ZERO;
  OFCHECK(*dst);
}

void Emu::loadRom(std::string rom)
{
  std::ifstream in;
  std::filebuf *pbuf = in.rdbuf();
  in.open(rom.c_str(), std::ifstream::in | std::ifstream::binary);
  int size = pbuf->pubseekoff(0, in.end, in.in);
  pbuf->pubseekpos(0, in.in);
  romData = new char[size];
  this->romSize = size;
  pbuf->sgetn((char *)romData, size);
  in.close();
}

void Emu::runRom()
{
  for(reg[PC] = 0; reg[PC] < romSize; ++reg[PC]) execInst();
}

void Emu::execInst()
{
  printInst();
  switch((unsigned char)romData[IP])
  {
    case ADC_IMM:
      cycles = 2;
      adc(&reg[A], loadVal());
      break;
    case ADC_ZP: 
      cycles = 3;
      adc(&reg[A], mem[loadAddr()]);
      break;
    case ADC_ZP_X:
      cycles = 4;
      adc(&reg[A], mem[loadAddr()+reg[X]]);
      break;
    case ADC_ABS:
      cycles = 4;
      adc(&reg[A], mem[loadAbsAddr()]);
      break;
    case ADC_ABS_X:
      {
        int addr = loadAbsAddr();
        cycles = (addr > 0xFF)?5:4;
        adc(&reg[A], mem[addr+reg[X]]);
      }
      break;
    case ADC_ABS_Y:
      {
        int addr = loadAbsAddr();
        cycles = (addr > 0xFF)?5:4;
        adc(&reg[A], mem[addr+reg[Y]]);
      }
      break;
    case ADC_INDIR_X:
      {
      }
      break;
    case ADC_INDIR_Y:
      break;
    case AND_IMM:
      cycles = 2;
      _and(&reg[A], mem[loadVal()]); 
      break;
    case AND_ZP:
      cycles = 3;
      _and(&reg[A], mem[loadAddr()]);
      break;
    case AND_ZP_X:
      cycles = 4;
      _and(&reg[A], mem[loadAddr()+reg[X]]);
      break;
    case AND_ABS:
      cycles = 4;
      _and(&reg[A], mem[loadAbsAddr()]);
      break;
    case AND_ABS_X:
      {
        int addr = loadAbsAddr();
        cycles = (addr > 0xFF)?5:4;
        _and(&reg[A], mem[addr+reg[X]]);
      }
      break;
    case AND_ABS_Y:
      {
        int addr = loadAbsAddr();
        cycles = (addr > 0xFF)?5:4;
        _and(&reg[A], mem[addr+reg[Y]]);
      }
      break;
    case AND_INDIR_X:
      break;
    case AND_INDIR_Y:
      break;
    case ASL_ACC:
      cycles = 2;
      asl(&reg[A]);
      break;
    case ASL_ZP:
      cycles = 5;
      asl(mem[loadAddr()]);
      break;
    case ASL_ZP_X:
      break;
    case ASL_ABS:
      break;
    case ASL_ABS_X:
      break;
    case BCC:
      break;
    case BCS:
      break;
    case BEQ:
      break;
    case BIT_ZP:
      break;
    case BIT_ABS:
      break;
    case BMI:
      break;
    case BNE:
      break;
    case BPL:
      break;
    case BRK:
      break;
    case BVC:
      break;
    case BVS:
      break;
    case CLC:
      break;
    case CLD:
      break;
    case CLI:
      break;
    case CLV:
      break;
    case CMP_IMM:
      break;
    case CMP_ZP:
      break;
    case CMP_ZP_X:
      break;
    case CMP_ABS:
      break;
    case CMP_ABS_X:
      break;
    case CMP_ABS_Y:
      break;
    case CMP_INDIR_X:
      break;
    case CMP_INDIR_Y:
      break;
    case CPX_IMM:
      break;
    case CPX_ZP:
      break;
    case CPX_ABS:
      break;
    case CPY_IMM:
      break;
    case CPY_ZP:
      break;
    case CPY_ABS:
      break;
    case DEC_ZP:
      break;
    case DEC_ZP_X:
      break;
    case DEC_ABS:
      break;
    case DEC_ABS_X:
      break;
    case DEX:
      break;
    case DEY:
      break;
    case EOR_IMM:
      break;
    case EOR_ZP:
      break;
    case EOR_ZP_X:
      break;
    case EOR_ABS:
      break;
    case EOR_ABS_X:
      break;
    case EOR_ABS_Y:
      break;
    case EOR_INDIR_X:
      break;
    case EOR_INDIR_Y:
      break;
    case INC_ZP:
      break;
    case INC_ZP_X:
      break;
    case INC_ABS:
      break;
    case INC_ABS_X:
      break;
    case INX:
      break;
    case INY:
      break;
    case JMP_INDIR:
      break;
    case JMP_ABS:
      break;
    case JSR:
      break;
    case LDA_IMM:
      break;
    case LDA_ZP:
      break;
    case LDA_ZP_X:
      break;
    case LDA_ABS:
      break;
    case LDA_ABS_X:
      break;
    case LDA_ABS_Y:
      break;
    case LDA_INDIR_X:
      break;
    case LDA_INDIR_Y:
      break;
    case LDX_ZP:
      break;
    case LDX_ZP_Y:
      break;
    case LDX_ABS:
      break;
    case LDX_ABS_Y:
      break;
    case LDX_IMM:
      break;
    case LDY_IMM:
      break;
    case LDY_ZP:
      break;
    case LDY_ZP_X:
      break;
    case LDY_ABS:
      break;
    case LDY_ABS_X:
      break;
    case LSR_ACC:
      break;
    case LSR_ZP:
      break;
    case LSR_ZP_X:
      break;
    case LSR_ABS:
      lsr(&mem[loadAbsAddr()], 1);
      break;
    case LSR_ABS_X:
      break;
    case NOP:
      break;
    case ORA_IMM:
      break;
    case ORA_ZP:
      break;
    case ORA_ZP_X:
      break;
    case ORA_ABS:
      break;
    case ORA_ABS_X:
      break;
    case ORA_ABS_Y:
      break;
    case ORA_INDIR_X:
      break;
    case ORA_INDIR_Y:
      break;
    case PHA:
      break;
    case PHP:
      break;
    case PLA:
      break;
    case PLP:
      break;
    case ROL_ACC:
      break;
    case ROL_ZP:
      break;
    case ROL_ZP_X:
      break;
    case ROL_ABS:
      break;
    case ROL_ABS_X:
      break;
    case ROR_ACC:
      break;
    case ROR_ZP:
      break;
    case ROR_ZP_X:
      break;
    case ROR_ABS:
      break;
    case ROR_ABS_X:
      break;
    case RTI:
      break;
    case RTS:
      break;
    case SBC_IMM:
      break;
    case SBC_ZP:
      break;
    case SBC_ZP_X:
      break;
    case SBC_ABS:
      break;
    case SBC_ABS_X:
      break;
    case SBC_ABS_Y:
      break;
    case SBC_INDIR_X:
      break;
    case SBC_INDIR_Y:
      break;
    case SEC:
      break;
    case SED:
      break;
    case SEI:
      break;
    case STA_ZP:
      break;
    case ST_ZP_X:
      break;
    case STA_ABS:
      break;
    case STA_ABS_X:
      break;
    case STA_ABS_Y:
      break;
    case STA_INDIR_X:
      break;
    case STA_INDIR_Y:
      break;
    case STX_ZP:
      break;
    case STX_ZP_Y:
      break;
    case STX_ABS:
      break;
    case STY_ZP:
      break;
    case STY_ZP_X:
      break;
    case STY_ABS:
      break;
    case TAX:
      break;
    case TAY:
      break;
    case TSX:
      break;
    case TXA:
      break;
    case TXS:
      break;
    case TYA:
      break;
  }
}

void  Emu::printInst()
{
  switch((unsigned char)romData[reg[PC]])
  {
    case ADC_IMM: printf("ADC_IMM\n");
      break;
    case ADC_ZP: printf("ADC_ZP\n");
      break;
    case ADC_ZP_X: printf("ADC_ZP_X\n");
      break;
    case ADC_ABS: printf("ADC_ABS\n");
      break;
    case ADC_ABS_X: printf("ADC_ABS_X\n");
      break;
    case ADC_ABS_Y: printf("ADC_ABS_Y\n");
      break;
    case ADC_INDIR_X: printf("ADC_INDIR_X\n");
      break;
    case ADC_INDIR_Y: printf("ADC_INDIR_Y\n");
      break;
    case AND_IMM: printf("AND_IMM\n");
      break;
    case AND_ZP: printf("AND_ZP\n");
      break;
    case AND_ZP_X: printf("AND_ZP_X\n");
      break;
    case AND_ABS: printf("AND_ABS\n");
      break;
    case AND_ABS_X: printf("AND_ABS_X\n");
      break;
    case AND_ABS_Y: printf("AND_ABS_Y\n");
      break;
    case AND_INDIR_X: printf("AND_INDIR_X\n");
      break;
    case AND_INDIR_Y: printf("AND_INDIR_Y\n");
      break;
    case ASL_ACC: printf("ASL_ACC\n");
      break;
    case ASL_ZP: printf("ASL_ZP\n");
      break;
    case ASL_ZP_X: printf("ASL_ZP_X\n");
      break;
    case ASL_ABS: printf("ASL_ABS\n");
      break;
    case ASL_ABS_X: printf("ASL_ABS_X\n");
      break;
    case BCC: printf("BCC\n");
      break;
    case BCS: printf("BCS\n");
      break;
    case BEQ: printf("BEQ\n");
      break;
    case BIT_ZP: printf("BIT_ZP\n");
      break;
    case BIT_ABS: printf("BIT_ABS\n");
      break;
    case BMI: printf("BMI\n");
      break;
    case BNE: printf("BNE\n");
      break;
    case BPL: printf("BPL\n");
      break;
    case BRK: printf("BRK\n");
      break;
    case BVC: printf("BVC\n");
      break;
    case BVS: printf("BVS\n");
      break;
    case CLC: printf("CLC\n");
      break;
    case CLD: printf("CLD\n");
      break;
    case CLI: printf("CLI\n");
      break;
    case CLV: printf("CLV\n");
      break;
    case CMP_IMM: printf("CMP_IMM\n");
      break;
    case CMP_ZP: printf("CMP_ZP\n");
      break;
    case CMP_ZP_X: printf("CMP_ZP_X\n");
      break;
    case CMP_ABS: printf("CMP_ABS\n");
      break;
    case CMP_ABS_X: printf("CMP_ABS_X\n");
      break;
    case CMP_ABS_Y: printf("CMP_ABS_Y\n");
      break;
    case CMP_INDIR_X: printf("CMP_INDIR_X\n");
      break;
    case CMP_INDIR_Y: printf("CMP_INDIR_Y\n");
      break;
    case CPX_IMM: printf("CPX_IMM\n");
      break;
    case CPX_ZP: printf("CPX_ZP\n");
      break;
    case CPX_ABS: printf("CPX_ABS\n");
      break;
    case CPY_IMM: printf("CPY_IMM\n");
      break;
    case CPY_ZP: printf("CPY_ZP\n");
      break;
    case CPY_ABS: printf("CPY_ABS\n");
      break;
    case DEC_ZP: printf("DEC_ZP\n");
      break;
    case DEC_ZP_X: printf("DEC_ZP_X\n");
      break;
    case DEC_ABS: printf("DEC_ABS\n");
      break;
    case DEC_ABS_X: printf("DEC_ABS_X\n");
      break;
    case DEX: printf("DEX\n");
      break;
    case DEY: printf("DEY\n");
      break;
    case EOR_IMM: printf("EOR_IMM\n");
      break;
    case EOR_ZP: printf("EOR_ZP\n");
      break;
    case EOR_ZP_X: printf("EOR_ZP_X\n");
      break;
    case EOR_ABS: printf("EOR_ABS\n");
      break;
    case EOR_ABS_X: printf("EOR_ABS_X\n");
      break;
    case EOR_ABS_Y: printf("EOR_ABS_Y\n");
      break;
    case EOR_INDIR_X: printf("EOR_INDIR_X\n");
      break;
    case EOR_INDIR_Y: printf("EOR_INDIR_Y\n");
      break;
    case INC_ZP: printf("INC_ZP\n");
      break;
    case INC_ZP_X: printf("INC_ZP_X\n");
      break;
    case INC_ABS: printf("INC_ABS\n");
      break;
    case INC_ABS_X: printf("INC_ABS_X\n");
      break;
    case INX: printf("INX\n");
      break;
    case INY: printf("INY\n");
      break;
    case JMP_INDIR: printf("JMP_INDIR\n");
      break;
    case JMP_ABS: printf("JMP_ABS\n");
      break;
    case JSR: printf("JSR\n");
      break;
    case LDA_IMM: printf("LDA_IMM\n");
      break;
    case LDA_ZP: printf("LDA_ZP\n");
      break;
    case LDA_ZP_X: printf("LDA_ZP_X\n");
      break;
    case LDA_ABS: printf("LDA_ABS\n");
      break;
    case LDA_ABS_X: printf("LDA_ABS_X\n");
      break;
    case LDA_ABS_Y: printf("LDA_ABS_Y\n");
      break;
    case LDA_INDIR_X: printf("LDA_INDIR_X\n");
      break;
    case LDA_INDIR_Y: printf("LDA_INDIR_Y\n");
      break;
    case LDX_ZP: printf("LDX_ZP\n");
      break;
    case LDX_ZP_Y: printf("LDX_ZP_Y\n");
      break;
    case LDX_ABS: printf("LDX_ABS\n");
      break;
    case LDX_ABS_Y: printf("LDX_ABS_Y\n");
      break;
    case LDX_IMM: printf("LDX_IMM\n");
      break;
    case LDY_IMM: printf("LDY_IMM\n");
      break;
    case LDY_ZP: printf("LDY_ZP\n");
      break;
    case LDY_ZP_X: printf("LDY_ZP_X\n");
      break;
    case LDY_ABS: printf("LDY_ABS\n");
      break;
    case LDY_ABS_X: printf("LDY_ABS_X\n");
      break;
    case LSR_ACC: printf("LSR_ACC\n");
      break;
    case LSR_ZP: printf("LSR_ZP\n");
      break;
    case LSR_ZP_X: printf("LSR_ZP_X\n");
      break;
    case LSR_ABS: printf("LSR_ABS\n");
      break;
    case LSR_ABS_X: printf("LSR_ABS_X\n");
      break;
    case NOP: printf("NOP\n");
      break;
    case ORA_IMM: printf("ORA_IMM\n");
      break;
    case ORA_ZP: printf("ORA_ZP\n");
      break;
    case ORA_ZP_X: printf("ORA_ZP_X\n");
      break;
    case ORA_ABS: printf("ORA_ABS\n");
      break;
    case ORA_ABS_X: printf("ORA_ABS_X\n");
      break;
    case ORA_ABS_Y: printf("ORA_ABS_Y\n");
      break;
    case ORA_INDIR_X: printf("ORA_INDIR_X\n");
      break;
    case ORA_INDIR_Y: printf("ORA_INDIR_Y\n");
      break;
    case PHA: printf("PHA\n");
      break;
    case PHP: printf("PHP\n");
      break;
    case PLA: printf("PLA\n");
      break;
    case PLP: printf("PLP\n");
      break;
    case ROL_ACC: printf("ROL_ACC\n");
      break;
    case ROL_ZP: printf("ROL_ZP\n");
      break;
    case ROL_ZP_X: printf("ROL_ZP_X\n");
      break;
    case ROL_ABS: printf("ROL_ABS\n");
      break;
    case ROL_ABS_X: printf("ROL_ABS_X\n");
      break;
    case ROR_ACC: printf("ROR_ACC\n");
      break;
    case ROR_ZP: printf("ROR_ZP\n");
      break;
    case ROR_ZP_X: printf("ROR_ZP_X\n");
      break;
    case ROR_ABS: printf("ROR_ABS\n");
      break;
    case ROR_ABS_X: printf("ROR_ABS_X\n");
      break;
    case RTI: printf("RTI\n");
      break;
    case RTS: printf("RTS\n");
      break;
    case SBC_IMM: printf("SBC_IMM\n");
      break;
    case SBC_ZP: printf("SBC_ZP\n");
      break;
    case SBC_ZP_X: printf("SBC_ZP_X\n");
      break;
    case SBC_ABS: printf("SBC_ABS\n");
      break;
    case SBC_ABS_X: printf("SBC_ABS_X\n");
      break;
    case SBC_ABS_Y: printf("SBC_ABS_Y\n");
      break;
    case SBC_INDIR_X: printf("SBC_INDIR_X\n");
      break;
    case SBC_INDIR_Y: printf("SBC_INDIR_Y\n");
      break;
    case SEC: printf("SEC\n");
      break;
    case SED: printf("SED\n");
      break;
    case SEI: printf("SEI\n");
      break;
    case STA_ZP: printf("STA_ZP\n");
      break;
    case ST_ZP_X: printf("ST_ZP_X\n");
      break;
    case STA_ABS: printf("STA_ABS\n");
      break;
    case STA_ABS_X: printf("STA_ABS_X\n");
      break;
    case STA_ABS_Y: printf("STA_ABS_Y\n");
      break;
    case STA_INDIR_X: printf("STA_INDIR_X\n");
      break;
    case STA_INDIR_Y: printf("STA_INDIR_Y\n");
      break;
    case STX_ZP: printf("STX_ZP\n");
      break;
    case STX_ZP_Y: printf("STX_ZP_Y\n");
      break;
    case STX_ABS: printf("STX_ABS\n");
      break;
    case STY_ZP: printf("STY_ZP\n");
      break;
    case STY_ZP_X: printf("STY_ZP_X\n");
      break;
    case STY_ABS: printf("STY_ABS\n");
      break;
    case TAX: printf("TAX\n");
      break;
    case TAY: printf("TAY\n");
      break;
    case TSX: printf("TSX\n");
      break;
    case TXA: printf("TXA\n");
      break;
    case TXS: printf("TXS\n");
      break;
    case TYA: printf("TYA\n");
      break;
    default: printf("NOT AN INSTRUCTION\n");
      break;
  }
}
