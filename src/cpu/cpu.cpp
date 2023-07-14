#include "cpu/cpu.h"
#include "cpu/opcode.h"
#include "utils/types.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

namespace nes_simulator {

cpu::cpu()
    : reg_a(0), reg_x(0), reg_y(0), sp(STACK_RESET), status(0b100100), pc(0),
      memory() {
  memset(memory, 0, sizeof(memory));
}

uint8 cpu::mem_read(uint16 addr) { return memory[addr]; }
uint16 cpu::mem_read_uint16(uint16 addr) {
  return static_cast<uint16>(mem_read(addr + 1) << 8) | mem_read(addr);
}

void cpu::mem_write(uint16 addr, uint8 val) { memory[addr] = val; }
void cpu::mem_write_uint16(uint16 addr, uint16 val) {
  mem_write(addr, val & 0xFF);
  mem_write(addr + 1, val >> 8);
}

uint8 cpu::stack_pop() {
  sp++;
  return mem_read(STACK + sp);
}

uint16 cpu::stack_pop_uint16() {
  uint16 lo = stack_pop();
  uint16 hi = stack_pop();
  return hi << 8 | lo;
}

void cpu::stack_push(uint8 data) {
  mem_write(STACK + sp, data);
  sp--;
}

void cpu::stack_push_uint16(uint16 data) {
  stack_push(data >> 8);
  stack_push(data & 0xff);
}

uint16 cpu::get_addr(AddressingMode mode) {
  switch (mode) {
  case AddressingMode::Immediate:
  case AddressingMode::Relative:
    return pc;

  case AddressingMode::ZeroPage:
    return mem_read(pc);

  case AddressingMode::Absolute:
    return mem_read_uint16(pc);

  case AddressingMode::ZeroPage_X:
    return mem_read(pc) + reg_x;

  case AddressingMode::ZeroPage_Y:
    return mem_read(pc) + reg_y;

  case AddressingMode::Absolute_X:
    return mem_read_uint16(pc) + reg_x;

  case AddressingMode::Absolute_Y:
    return mem_read_uint16(pc) + reg_y;

  case AddressingMode::Indirect_X: {
    uint8 ptr = mem_read(pc) + reg_x;
    return static_cast<uint16>(mem_read(ptr + 1) << 8) | mem_read(ptr);
  }

  case AddressingMode::Indirect_Y: {
    uint8 ptr = mem_read(pc);
    return (static_cast<uint16>(mem_read(ptr + 1) << 8) | mem_read(ptr)) +
           reg_y;
  }
  case AddressingMode::Implied:
    break;
  }

  throw std::runtime_error("unknown AddressingMode: " +
                           std::to_string((int)mode));
}

bool cpu::status_bit_get(flag flag) {
  uint8 t = 1 << static_cast<int>(flag);
  return status & t;
}

void cpu::status_bit_set(flag flag, bool v) {
  uint8 t = 1 << static_cast<int>(flag);
  v ? status |= t : status &= ~t;
}

void cpu::update_zero_negative_flag(uint8 reg) {
  status_bit_set(flag::ZeroFlag, reg == 0);
  status_bit_set(flag::NegativeFlag, reg & 0x80);
}

void cpu::reset() {
  reg_a = reg_x = reg_y = 0;
  sp = STACK_RESET;
  status = 0b100100;
  pc = mem_read_uint16(0xFFFC);
}

void cpu::load_and_run(const uint8 *program, int length, callback_t callback) {
  load(program, length);
  reset();
  run(std::move(callback));
}

void cpu::load(const uint8 *program, int length) {
  memcpy(memory + 0x600, program, length);
  mem_write_uint16(0xFFFC, 0x600);
}

void cpu::run(callback_t &&callback) {
  while (true) {
    if (callback)
      callback(*this);

    const auto opcode = memory[pc++];
    const auto pc_before_op = pc;
    const auto &info = opcodes[opcode];

    switch (info.opcode) {
    case OpcodeType::LDA:
      reg_a = mem_read(get_addr(info.mode));
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::LDX:
      reg_x = mem_read(get_addr(info.mode));
      update_zero_negative_flag(reg_x);
      break;

    case OpcodeType::LDY:
      reg_y = mem_read(get_addr(info.mode));
      update_zero_negative_flag(reg_y);
      break;

    case OpcodeType::STA:
      mem_write(get_addr(info.mode), reg_a);
      break;

    case OpcodeType::STX:
      mem_write(get_addr(info.mode), reg_x);
      break;

    case OpcodeType::STY:
      mem_write(get_addr(info.mode), reg_y);
      break;

    case OpcodeType::ADC: {
      uint8 base = mem_read(get_addr(info.mode));
      uint16 tmp =
          static_cast<uint16>(reg_a) + base + status_bit_get(flag::CarryFlag);

      status_bit_set(flag::CarryFlag, tmp > 0xff);
      status_bit_set(flag::OverflowFlag, static_cast<uint8>(tmp ^ base) &
                                             static_cast<uint8>(tmp ^ reg_a) &
                                             0x80 != 0);

      reg_a = tmp & 0xff;
      update_zero_negative_flag(reg_a);
      break;
    }

    case OpcodeType::SBC: {
      uint8 base = -(mem_read(get_addr(info.mode)) + 1);
      uint16 tmp =
          static_cast<uint16>(reg_a) + base + status_bit_get(flag::CarryFlag);

      status_bit_set(flag::CarryFlag, tmp > 0xff);
      status_bit_set(flag::OverflowFlag, static_cast<uint8>(tmp ^ base) &
                                             static_cast<uint8>(tmp ^ reg_a) &
                                             0x80 != 0);

      reg_a = tmp & 0xff;
      update_zero_negative_flag(reg_a);
      break;
    }

    case OpcodeType::AND:
      reg_a &= mem_read(get_addr(info.mode));
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::BCC:
      if (!status_bit_get(flag::CarryFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BCS:
      if (status_bit_get(flag::CarryFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BEQ:
      if (status_bit_get(flag::ZeroFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BMI:
      if (status_bit_get(flag::NegativeFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BNE:
      if (!status_bit_get(flag::ZeroFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BPL:
      if (!status_bit_get(flag::NegativeFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BVC:
      if (!status_bit_get(flag::OverflowFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::BVS:
      if (status_bit_get(flag::OverflowFlag)) {
        pc += (int8)(mem_read(get_addr(info.mode))) + 1;
      }
      break;

    case OpcodeType::JMP_ABS:
      pc = mem_read_uint16(get_addr(info.mode));
      break;

    case OpcodeType::JMP_IND: {
      uint16 addr = mem_read_uint16(get_addr(info.mode));
      if ((addr & 0xFF) == 0xFF) {
        pc = (mem_read(addr & 0xFF00) << 8) | mem_read(addr);
      } else {
        pc = mem_read_uint16(addr);
      }
      break;
    }

    case OpcodeType::JSR: {
      stack_push_uint16(pc + 2 - 1);
      pc = mem_read_uint16(pc);
      break;
    }

    case OpcodeType::NOP:
      break;

    case OpcodeType::ORA:
      reg_a |= mem_read(get_addr(info.mode));
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::EOR:
      reg_a ^= mem_read(get_addr(info.mode));
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::INX:
      reg_x += 1;
      update_zero_negative_flag(reg_x);
      break;

    case OpcodeType::INY:
      reg_y += 1;
      update_zero_negative_flag(reg_y);
      break;

    case OpcodeType::DEX:
      reg_x -= 1;
      update_zero_negative_flag(reg_x);
      break;

    case OpcodeType::DEY:
      reg_y -= 1;
      update_zero_negative_flag(reg_y);
      break;

    case OpcodeType::ASL: {
      uint16 addr = get_addr(info.mode);
      uint8 data = mem_read(addr);
      status_bit_set(flag::CarryFlag, data & 0x80);

      data <<= 1;
      mem_write(addr, data << 1);
      update_zero_negative_flag(data);
      break;
    }

    case OpcodeType::ASL_ACC:
      status_bit_set(flag::CarryFlag, reg_a & 0x80);
      reg_a <<= 1;
      break;

    case OpcodeType::BIT: {
      uint8 data = mem_read(get_addr(info.mode));
      uint8 tmp = reg_a & data;
      status_bit_set(flag::ZeroFlag, tmp == 0);
      status_bit_set(flag::NegativeFlag, data & 0x80);
      status_bit_set(flag::OverflowFlag, data & 0x40);
      break;
    }

    case OpcodeType::CLC:
      status_bit_set(flag::CarryFlag, false);
      break;

    case OpcodeType::CLD:
      status_bit_set(flag::DecimalModeFlag, false);
      break;

    case OpcodeType::CLI:
      status_bit_set(flag::InterruptDisable, false);
      break;

    case OpcodeType::SLC:
      status_bit_set(flag::CarryFlag, true);
      break;

    case OpcodeType::SLD:
      status_bit_set(flag::DecimalModeFlag, true);
      break;

    case OpcodeType::SLI:
      status_bit_set(flag::InterruptDisable, true);
      break;

    case OpcodeType::CLV:
      status_bit_set(flag::OverflowFlag, false);
      break;

    case OpcodeType::CMP: {
      uint8 data = mem_read(get_addr(info.mode));
      status_bit_set(flag::CarryFlag, reg_a >= data);
      update_zero_negative_flag(reg_a - data);
      break;
    }

    case OpcodeType::CPX: {
      uint8 data = mem_read(get_addr(info.mode));
      status_bit_set(flag::CarryFlag, reg_x >= data);
      update_zero_negative_flag(reg_x - data);
      break;
    }

    case OpcodeType::CPY: {
      uint8 data = mem_read(get_addr(info.mode));
      status_bit_set(flag::CarryFlag, reg_y >= data);
      update_zero_negative_flag(reg_y - data);
      break;
    }

    case OpcodeType::LSR_ACC:
      status_bit_set(flag::CarryFlag, reg_a & 1);
      reg_a >>= 1;
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::LSR: {
      uint16 addr = get_addr(info.mode);
      uint8 data = mem_read(addr);
      status_bit_set(flag::CarryFlag, data & 1);
      data >>= 1;
      mem_write(addr, data);
      update_zero_negative_flag(data);
      break;
    }

    case OpcodeType::TAX:
      reg_x = reg_a;
      update_zero_negative_flag(reg_x);
      break;

    case OpcodeType::TAY:
      reg_y = reg_a;
      update_zero_negative_flag(reg_y);
      break;

    case OpcodeType::TSX:
      reg_x = sp;
      update_zero_negative_flag(reg_x);
      break;

    case OpcodeType::TXA:
      reg_a = reg_x;
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::TXS:
      sp = reg_x;
      update_zero_negative_flag(sp);
      break;

    case OpcodeType::TYA:
      reg_a = reg_y;
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::PHA:
      stack_push(reg_a);
      break;

    case OpcodeType::PHP:
      stack_push(status);
      break;

    case OpcodeType::PLA:
      reg_a = stack_pop();
      update_zero_negative_flag(reg_a);
      break;

    case OpcodeType::PLP:
      status = stack_pop();
      break;

    case OpcodeType::BRK:
      return;

    case OpcodeType::ROL_ACC: {
      uint8 old_carry = status_bit_get(flag::CarryFlag);
      status_bit_set(flag::CarryFlag, reg_a & 0x80);
      reg_a = (reg_a << 1) | old_carry;
      update_zero_negative_flag(reg_a);
      break;
    }

    case OpcodeType::ROL: {
      uint16 addr = get_addr(info.mode);
      uint8 data = mem_read(addr);
      uint8 old_carry = status_bit_get(flag::CarryFlag);
      status_bit_set(flag::CarryFlag, data & 0x80);
      data = (data << 1) | old_carry;
      mem_write(addr, data);
      update_zero_negative_flag(data);
      break;
    }

    case OpcodeType::ROR_ACC: {
      uint8 old_carry = status_bit_get(flag::CarryFlag);
      status_bit_set(flag::CarryFlag, reg_a & 1);
      reg_a = (reg_a >> 1) | (old_carry << 7);
      update_zero_negative_flag(reg_a);
      break;
    }

    case OpcodeType::ROR: {
      uint16 addr = get_addr(info.mode);
      uint8 data = mem_read(addr);
      uint8 old_carry = status_bit_get(flag::CarryFlag);
      status_bit_set(flag::CarryFlag, data & 1);
      data = (data >> 1) | (old_carry << 7);
      mem_write(addr, data);
      update_zero_negative_flag(data);
      break;
    }

    case OpcodeType::RTI:
      status = stack_pop();
      status_bit_set(flag::BreakCommand, false);
      status_bit_set(flag::BreakCommand2, true);
      pc = stack_pop_uint16();
      break;

    case OpcodeType::RTS:
      pc = stack_pop_uint16() + 1;
      break;

    case OpcodeType::INC: {
      uint16 addr = get_addr(info.mode);
      uint8 data = mem_read(addr);
      data++;
      mem_write(addr, data);
      update_zero_negative_flag(data);
      break;
    }

    case OpcodeType::DEC:
      uint16 addr = get_addr(info.mode);
      uint8 data = mem_read(addr);
      data--;
      mem_write(addr, data);
      update_zero_negative_flag(data);
      break;
    }

    if (pc_before_op == pc) {
      pc += info.bytes - 1;
    }
  }
}

} // namespace nes_simulator