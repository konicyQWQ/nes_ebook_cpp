#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <utils/types.h>
#include <vector>

namespace nes_simulator {

enum class AddressingMode {
  Immediate,
  ZeroPage,
  Absolute,
  ZeroPage_X,
  ZeroPage_Y,
  Absolute_X,
  Absolute_Y,
  Indirect_X,
  Indirect_Y,
  Relative,
  Implied,
};

enum class flag {
  CarryFlag = 0,
  ZeroFlag = 1,
  InterruptDisable = 2,
  DecimalModeFlag = 3,
  BreakCommand = 4,
  BreakCommand2 = 5,
  OverflowFlag = 6,
  NegativeFlag = 7,
};

constexpr uint16 STACK = 0x0100;
constexpr uint16 STACK_RESET = 0xFD;

class cpu {
public:
  using callback_t = std::function<void(cpu &cpu)>;

  cpu();

  uint8 mem_read(uint16 addr);
  uint16 mem_read_uint16(uint16 addr);

  void mem_write(uint16 addr, uint8 val);
  void mem_write_uint16(uint16 addr, uint16 val);

  void stack_push(uint8 data);
  void stack_push_uint16(uint16 data);

  uint8 stack_pop();
  uint16 stack_pop_uint16();

  uint16 get_addr(AddressingMode mode);

  void load_and_run(const uint8 *program, int length,
                    callback_t callback = nullptr);
  void load(const uint8 *program, int length);
  void run(callback_t &&callback = nullptr);
  void reset();

public:
  bool status_bit_get(flag flag);
  void status_bit_set(flag flag, bool v);
  void update_zero_negative_flag(uint8 reg);

public:
  uint8 reg_a, reg_x, reg_y, sp, status;
  uint16 pc;
  uint8 memory[0xFFFF];
};

} // namespace nes_simulator