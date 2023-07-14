#pragma once

#include <cpu/cpu.h>
#include <utils/types.h>

namespace nes_simulator {

enum class OpcodeType {
  ADC,
  LDA,
  AND,
  BCC,
  BCS,
  BEQ,
  NOP,
  ORA,
  EOR,
  INX,
  INY,
  ASL,
  ASL_ACC,
  BIT,
  BMI,
  BNE,
  BPL,
  BVC,
  BVS,
  CLC,
  CLD,
  CLI,
  CLV,
  CMP,
  CPX,
  CPY,
  SBC,
  SLC,
  SLD,
  SLI,
  LDX,
  LDY,
  LSR_ACC,
  LSR,
  TAX,
  TAY,
  TSX,
  TXA,
  TXS,
  TYA,
  JMP_ABS,
  JMP_IND,
  JSR,
  DEX,
  DEY,
  STA,
  STX,
  STY,
  PHA,
  PHP,
  PLA,
  PLP,
  BRK,
  ROL_ACC,
  ROL,
  ROR_ACC,
  ROR,
  RTI,
  RTS,
  INC,
  DEC,
};

struct opcode_info {
  OpcodeType opcode;
  uint8 bytes;
  uint8 cycle;
  AddressingMode mode;
};

const opcode_info opcodes[0xFF] = {
    [0x69] = {OpcodeType::ADC, 2, 2, AddressingMode::Immediate},
    [0x65] = {OpcodeType::ADC, 2, 3, AddressingMode::ZeroPage},
    [0x75] = {OpcodeType::ADC, 2, 4, AddressingMode::ZeroPage_X},
    [0x6D] = {OpcodeType::ADC, 3, 4, AddressingMode::Absolute},
    [0x7D] = {OpcodeType::ADC, 3, 4, AddressingMode::Absolute_X},
    [0x79] = {OpcodeType::ADC, 3, 4, AddressingMode::Absolute_Y},
    [0x61] = {OpcodeType::ADC, 2, 6, AddressingMode::Indirect_X},
    [0x71] = {OpcodeType::ADC, 2, 5, AddressingMode::Indirect_Y},

    [0xE9] = {OpcodeType::SBC, 2, 2, AddressingMode::Immediate},
    [0xE5] = {OpcodeType::SBC, 2, 3, AddressingMode::ZeroPage},
    [0xF5] = {OpcodeType::SBC, 2, 4, AddressingMode::ZeroPage_X},
    [0xED] = {OpcodeType::SBC, 3, 4, AddressingMode::Absolute},
    [0xFD] = {OpcodeType::SBC, 3, 4, AddressingMode::Absolute_X},
    [0xF9] = {OpcodeType::SBC, 3, 4, AddressingMode::Absolute_Y},
    [0xE1] = {OpcodeType::SBC, 2, 6, AddressingMode::Indirect_X},
    [0xF1] = {OpcodeType::SBC, 2, 5, AddressingMode::Indirect_Y},

    [0x0A] = {OpcodeType::ASL_ACC, 1, 2, AddressingMode::Implied},
    [0x06] = {OpcodeType::ASL, 2, 5, AddressingMode::ZeroPage},
    [0x16] = {OpcodeType::ASL, 2, 6, AddressingMode::ZeroPage_X},
    [0x0E] = {OpcodeType::ASL, 3, 6, AddressingMode::Absolute},
    [0x1E] = {OpcodeType::ASL, 3, 7, AddressingMode::Absolute_X},

    [0x29] = {OpcodeType::AND, 2, 2, AddressingMode::Immediate},
    [0x25] = {OpcodeType::AND, 2, 3, AddressingMode::ZeroPage},
    [0x35] = {OpcodeType::AND, 2, 4, AddressingMode::ZeroPage_X},
    [0x2D] = {OpcodeType::AND, 3, 4, AddressingMode::Absolute},
    [0x3D] = {OpcodeType::AND, 3, 4, AddressingMode::Absolute_X},
    [0x39] = {OpcodeType::AND, 3, 4, AddressingMode::Absolute_Y},
    [0x21] = {OpcodeType::AND, 2, 6, AddressingMode::Indirect_X},
    [0x31] = {OpcodeType::AND, 2, 5, AddressingMode::Indirect_Y},

    [0x90] = {OpcodeType::BCC, 2, 2, AddressingMode::Relative},
    [0xB0] = {OpcodeType::BCS, 2, 2, AddressingMode::Relative},
    [0xF0] = {OpcodeType::BEQ, 2, 2, AddressingMode::Relative},
    [0x30] = {OpcodeType::BMI, 2, 2, AddressingMode::Relative},
    [0xD0] = {OpcodeType::BNE, 2, 2, AddressingMode::Relative},
    [0x10] = {OpcodeType::BPL, 2, 2, AddressingMode::Relative},
    [0x50] = {OpcodeType::BVC, 2, 2, AddressingMode::Relative},
    [0x70] = {OpcodeType::BVS, 2, 2, AddressingMode::Relative},
    [0x4c] = {OpcodeType::JMP_ABS, 3, 3, AddressingMode::Immediate},
    [0x6c] = {OpcodeType::JMP_IND, 3, 5, AddressingMode::Immediate},

    [0x20] = {OpcodeType::JSR, 3, 6, AddressingMode::Immediate},

    [0x18] = {OpcodeType::CLC, 1, 2, AddressingMode::Implied},
    [0xD8] = {OpcodeType::CLD, 1, 2, AddressingMode::Implied},
    [0x58] = {OpcodeType::CLI, 1, 2, AddressingMode::Implied},
    [0xB8] = {OpcodeType::CLV, 1, 2, AddressingMode::Implied},

    [0x38] = {OpcodeType::SLC, 1, 2, AddressingMode::Implied},
    [0xF8] = {OpcodeType::SLD, 1, 2, AddressingMode::Implied},
    [0x78] = {OpcodeType::SLI, 1, 2, AddressingMode::Implied},

    [0xE6] = {OpcodeType::INC, 2, 5, AddressingMode::ZeroPage},
    [0xF6] = {OpcodeType::INC, 2, 6, AddressingMode::ZeroPage_X},
    [0xEE] = {OpcodeType::INC, 3, 6, AddressingMode::Absolute},
    [0xFE] = {OpcodeType::INC, 3, 7, AddressingMode::Absolute_X},

    [0xC6] = {OpcodeType::DEC, 2, 5, AddressingMode::ZeroPage},
    [0xD6] = {OpcodeType::DEC, 2, 6, AddressingMode::ZeroPage_X},
    [0xCE] = {OpcodeType::DEC, 3, 6, AddressingMode::Absolute},
    [0xDE] = {OpcodeType::DEC, 3, 7, AddressingMode::Absolute_X},

    [0xE8] = {OpcodeType::INX, 1, 2, AddressingMode::Implied},
    [0xC8] = {OpcodeType::INY, 1, 2, AddressingMode::Implied},
    [0xCA] = {OpcodeType::DEX, 1, 2, AddressingMode::Implied},
    [0x88] = {OpcodeType::DEY, 1, 2, AddressingMode::Implied},

    [0x24] = {OpcodeType::BIT, 2, 3, AddressingMode::ZeroPage},
    [0x2C] = {OpcodeType::BIT, 3, 4, AddressingMode::Absolute},

    [0xEA] = {OpcodeType::NOP, 1, 2, AddressingMode::Implied},

    [0x09] = {OpcodeType::ORA, 2, 2, AddressingMode::Immediate},
    [0x05] = {OpcodeType::ORA, 2, 3, AddressingMode::ZeroPage},
    [0x15] = {OpcodeType::ORA, 2, 4, AddressingMode::ZeroPage_X},
    [0x0D] = {OpcodeType::ORA, 3, 4, AddressingMode::Absolute},
    [0x1D] = {OpcodeType::ORA, 3, 4, AddressingMode::Absolute_X},
    [0x19] = {OpcodeType::ORA, 3, 4, AddressingMode::Absolute_Y},
    [0x01] = {OpcodeType::ORA, 2, 6, AddressingMode::Indirect_X},
    [0x11] = {OpcodeType::ORA, 2, 5, AddressingMode::Indirect_Y},

    [0x49] = {OpcodeType::EOR, 2, 2, AddressingMode::Immediate},
    [0x45] = {OpcodeType::EOR, 2, 3, AddressingMode::ZeroPage},
    [0x55] = {OpcodeType::EOR, 2, 4, AddressingMode::ZeroPage_X},
    [0x4D] = {OpcodeType::EOR, 3, 4, AddressingMode::Absolute},
    [0x5D] = {OpcodeType::EOR, 3, 4, AddressingMode::Absolute_X},
    [0x59] = {OpcodeType::EOR, 3, 4, AddressingMode::Absolute_Y},
    [0x41] = {OpcodeType::EOR, 2, 6, AddressingMode::Indirect_X},
    [0x51] = {OpcodeType::EOR, 2, 5, AddressingMode::Indirect_Y},

    [0xA9] = {OpcodeType::LDA, 2, 2, AddressingMode::Immediate},
    [0xA5] = {OpcodeType::LDA, 2, 3, AddressingMode::ZeroPage},
    [0xB5] = {OpcodeType::LDA, 2, 4, AddressingMode::ZeroPage_X},
    [0xAD] = {OpcodeType::LDA, 3, 4, AddressingMode::Absolute},
    [0xBD] = {OpcodeType::LDA, 3, 4, AddressingMode::Absolute_X},
    [0xB9] = {OpcodeType::LDA, 3, 4, AddressingMode::Absolute_Y},
    [0xA1] = {OpcodeType::LDA, 2, 6, AddressingMode::Indirect_X},
    [0xB1] = {OpcodeType::LDA, 2, 5, AddressingMode::Indirect_Y},

    [0xA2] = {OpcodeType::LDX, 2, 2, AddressingMode::Immediate},
    [0xA6] = {OpcodeType::LDX, 2, 3, AddressingMode::ZeroPage},
    [0xB6] = {OpcodeType::LDX, 2, 4, AddressingMode::ZeroPage_Y},
    [0xAE] = {OpcodeType::LDX, 3, 4, AddressingMode::Absolute},
    [0xBE] = {OpcodeType::LDX, 3, 4, AddressingMode::Absolute_Y},

    [0xA0] = {OpcodeType::LDY, 2, 2, AddressingMode::Immediate},
    [0xA4] = {OpcodeType::LDY, 2, 3, AddressingMode::ZeroPage},
    [0xB4] = {OpcodeType::LDY, 2, 4, AddressingMode::ZeroPage_X},
    [0xAC] = {OpcodeType::LDY, 3, 4, AddressingMode::Absolute},
    [0xBC] = {OpcodeType::LDY, 3, 4, AddressingMode::Absolute_X},

    [0x85] = {OpcodeType::STA, 2, 3, AddressingMode::ZeroPage},
    [0x95] = {OpcodeType::STA, 2, 4, AddressingMode::ZeroPage_X},
    [0x8D] = {OpcodeType::STA, 3, 4, AddressingMode::Absolute},
    [0x9D] = {OpcodeType::STA, 3, 4, AddressingMode::Absolute_X},
    [0x99] = {OpcodeType::STA, 3, 4, AddressingMode::Absolute_Y},
    [0x81] = {OpcodeType::STA, 2, 6, AddressingMode::Indirect_X},
    [0x91] = {OpcodeType::STA, 2, 5, AddressingMode::Indirect_Y},

    [0x86] = {OpcodeType::STX, 2, 3, AddressingMode::ZeroPage},
    [0x96] = {OpcodeType::STX, 2, 4, AddressingMode::ZeroPage_Y},
    [0x8E] = {OpcodeType::STX, 3, 4, AddressingMode::Absolute},

    [0x84] = {OpcodeType::STY, 2, 3, AddressingMode::ZeroPage},
    [0x94] = {OpcodeType::STY, 2, 4, AddressingMode::ZeroPage_X},
    [0x8C] = {OpcodeType::STY, 3, 4, AddressingMode::Absolute},

    [0xC9] = {OpcodeType::CMP, 2, 2, AddressingMode::Immediate},
    [0xC5] = {OpcodeType::CMP, 2, 3, AddressingMode::ZeroPage},
    [0xD5] = {OpcodeType::CMP, 2, 4, AddressingMode::ZeroPage_X},
    [0xCD] = {OpcodeType::CMP, 3, 4, AddressingMode::Absolute},
    [0xDD] = {OpcodeType::CMP, 3, 4, AddressingMode::Absolute_X},
    [0xD9] = {OpcodeType::CMP, 3, 4, AddressingMode::Absolute_Y},
    [0xC1] = {OpcodeType::CMP, 2, 6, AddressingMode::Indirect_X},
    [0xD1] = {OpcodeType::CMP, 2, 5, AddressingMode::Indirect_Y},

    [0xE0] = {OpcodeType::CPX, 2, 2, AddressingMode::Immediate},
    [0xE4] = {OpcodeType::CPX, 2, 3, AddressingMode::ZeroPage},
    [0xEC] = {OpcodeType::CPX, 3, 4, AddressingMode::Absolute},

    [0xC0] = {OpcodeType::CPY, 2, 2, AddressingMode::Immediate},
    [0xC4] = {OpcodeType::CPY, 2, 3, AddressingMode::ZeroPage},
    [0xCC] = {OpcodeType::CPY, 3, 4, AddressingMode::Absolute},

    [0x4A] = {OpcodeType::LSR_ACC, 1, 2, AddressingMode::Implied},
    [0x46] = {OpcodeType::LSR, 2, 5, AddressingMode::ZeroPage},
    [0x56] = {OpcodeType::LSR, 2, 6, AddressingMode::ZeroPage_X},
    [0x4E] = {OpcodeType::LSR, 3, 6, AddressingMode::Absolute},
    [0x5E] = {OpcodeType::LSR, 3, 7, AddressingMode::Absolute_X},

    [0xAA] = {OpcodeType::TAX, 1, 2, AddressingMode::Implied},
    [0x8A] = {OpcodeType::TXA, 1, 2, AddressingMode::Implied},
    [0xA8] = {OpcodeType::TAY, 1, 2, AddressingMode::Implied},
    [0x98] = {OpcodeType::TYA, 1, 2, AddressingMode::Implied},
    [0xBA] = {OpcodeType::TSX, 1, 2, AddressingMode::Implied},
    [0x9A] = {OpcodeType::TXS, 1, 2, AddressingMode::Implied},

    [0x48] = {OpcodeType::PHA, 1, 3, AddressingMode::Implied},
    [0x08] = {OpcodeType::PHP, 1, 3, AddressingMode::Implied},

    [0x68] = {OpcodeType::PLA, 1, 3, AddressingMode::Implied},
    [0x28] = {OpcodeType::PLP, 1, 3, AddressingMode::Implied},

    [0x00] = {OpcodeType::BRK, 1, 1, AddressingMode::Implied},

    [0x2A] = {OpcodeType::ROL_ACC, 1, 2, AddressingMode::Implied},

    [0x26] = {OpcodeType::ROL, 2, 5, AddressingMode::ZeroPage},
    [0x36] = {OpcodeType::ROL, 2, 6, AddressingMode::ZeroPage_X},
    [0x2E] = {OpcodeType::ROL, 3, 6, AddressingMode::Absolute},
    [0x3E] = {OpcodeType::ROL, 3, 7, AddressingMode::Absolute_X},

    [0x6A] = {OpcodeType::ROR_ACC, 1, 2, AddressingMode::Implied},

    [0x66] = {OpcodeType::ROR, 2, 5, AddressingMode::ZeroPage},
    [0x76] = {OpcodeType::ROR, 2, 6, AddressingMode::ZeroPage_X},
    [0x6E] = {OpcodeType::ROR, 3, 6, AddressingMode::Absolute},
    [0x7E] = {OpcodeType::ROR, 3, 7, AddressingMode::Absolute_X},

    [0x40] = {OpcodeType::RTI, 1, 6, AddressingMode::Implied},
    [0x60] = {OpcodeType::RTS, 1, 6, AddressingMode::Implied},
};

} // namespace nes_simulator