#include "cpu.h"

#include "bus.h"
#include "print_format.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define REG_HI(x) ((x) >> 8)
#define REG_LO(x) ((x)&0xff)

void print_registers(struct cpu *cpu) {
  printf("ax: " HEX_16 ", bx: " HEX_16 ", cx: " HEX_16 ", dx: " HEX_16 ", ip: " HEX_16 "\n",
         cpu->regs.reg_16[AX], cpu->regs.reg_16[BX], cpu->regs.reg_16[CX], cpu->regs.reg_16[DX],
         cpu->ip);
  printf("si: " HEX_16 ", di: " HEX_16 ", bp: " HEX_16 ", sp: " HEX_16 ", fl: " HEX_16 "\n",
         cpu->regs.reg_16[SI], cpu->regs.reg_16[DI], cpu->regs.reg_16[BP], cpu->regs.reg_16[SP],
         cpu->flags);
}

static void set_flags_sign_zero_8(struct cpu *cpu, u8 value) {
  if (!value) {
    cpu_set_flag(cpu, f_zero);
  } else {
    cpu_clear_flag(cpu, f_zero);
  }

  if (value & 0x80) {
    cpu_set_flag(cpu, f_sign);
  } else {
    cpu_clear_flag(cpu, f_sign);
  }
}

static void set_flags_subtract_8(struct cpu *cpu, u8 destination, u8 source) {
  u16 val = (u16)destination - (u16)source;

  set_flags_sign_zero_8(cpu, (u8)val);

  if (val & 0xff00) {
    cpu_set_flag(cpu, f_carry);
  } else {
    cpu_clear_flag(cpu, f_carry);
  }

  if ((val ^ destination) & (destination ^ source) & 0x80) {
    cpu_set_flag(cpu, f_overflow);
  } else {
    cpu_clear_flag(cpu, f_overflow);
  }

  if ((destination ^ source ^ val) & 0x10) {
    cpu_set_flag(cpu, f_adjust);
  } else {
    cpu_clear_flag(cpu, f_adjust);
  }
}

struct address get_operand_indirect_address(struct operand *operand, struct cpu *cpu) {
  struct address result;

  switch (operand->data.as_indirect.encoding) {
    case mod_rm_mem_bx_si:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, SI));
      break;

    case mod_rm_mem_bx_di:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, DI));
      break;

    case mod_rm_mem_bp_si:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, SI));
      break;

    case mod_rm_mem_bp_di:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, DI));
      break;

    case mod_rm_mem_si:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, SI));
      break;

    case mod_rm_mem_di:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, DI));
      break;

    case mod_rm_mem_bp:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BP));
      break;

    case mod_rm_mem_bx:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BX));
      break;

    default:
      assert(0);
      result = segment_offset(0, 0);
      break;
  }

  return result;
}

static u16 get_operand_indirect_value_16(struct operand *operand, struct cpu *cpu) {
  struct address address = get_operand_indirect_address(operand, cpu);

  return bus_fetch(cpu->bus, address) + (bus_fetch(cpu->bus, address_offset(address, 1)) << 8);
}

static u8 get_operand_indirect_value_8(struct operand *operand, struct cpu *cpu) {
  struct address address = get_operand_indirect_address(operand, cpu);

  return bus_fetch(cpu->bus, address);
}

static u8 get_operand_register_value_8(struct operand *operand, struct cpu *cpu) {
  return cpu->regs.reg_8[operand->data.as_register.reg_8];
}

static u16 get_operand_register_value_16(struct operand *operand, struct cpu *cpu) {
  return cpu->regs.reg_16[operand->data.as_register.reg_16];
}

static u8 get_operand_value_8(struct operand *operand, struct cpu *cpu) {
  switch (operand->type) {
    case ot_indirect:
      return get_operand_indirect_value_8(operand, cpu);

    case ot_displacement:
      assert(0);
      break;

    case ot_register:
      return get_operand_register_value_8(operand, cpu);

    case ot_direct:
      return bus_fetch(cpu->bus, segment_offset(0, operand->data.as_direct.address));

    case ot_immediate:
      return operand->data.as_immediate.immediate_8;

    case ot_none:
    default:
      assert(0);
  }

  return 0;
}

static u16 get_operand_value_16(struct operand *operand, struct cpu *cpu) {
  switch (operand->type) {
    case ot_indirect:
      return get_operand_indirect_value_16(operand, cpu);

    case ot_displacement:
      assert(0);
      break;

    case ot_register:
      return get_operand_register_value_16(operand, cpu);

    case ot_direct:
      return bus_fetch(cpu->bus, segment_offset(0, operand->data.as_direct.address)) +
             (bus_fetch(cpu->bus, segment_offset(0, operand->data.as_direct.address + 1)) << 8);

    case ot_immediate:
      return operand->data.as_immediate.immediate_16;

    default:
      assert(0);
  }

  return 0;
}

static void set_operand_indirect_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  struct address address = get_operand_indirect_address(operand, cpu);

  bus_store(cpu->bus, address, value);
}

static void set_operand_indirect_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  struct address address = get_operand_indirect_address(operand, cpu);

  bus_store(cpu->bus, address, value >> 8);
  bus_store(cpu->bus, address_offset(address, 1), value & 0xff);
}

static void set_operand_register_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  cpu->regs.reg_8[operand->data.as_register.reg_8] = value;
}

static void set_operand_register_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  cpu->regs.reg_16[operand->data.as_register.reg_16] = value;
}

static void set_operand_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  switch (operand->type) {
    case ot_indirect:
      set_operand_indirect_value_8(operand, cpu, value);
      break;

    case ot_displacement:
      assert(0);
      break;

    case ot_register:
      set_operand_register_value_8(operand, cpu, value);
      break;

    case ot_direct:
      bus_store(cpu->bus, segment_offset(0, operand->data.as_direct.address), value);
      break;

    case ot_immediate:
    case ot_none:
    default:
      assert(0);
      break;
  }
}

static void set_operand_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  switch (operand->type) {
    case ot_indirect:
      set_operand_indirect_value_16(operand, cpu, value);
      break;

    case ot_displacement:
      assert(0);
      break;

    case ot_register:
      set_operand_register_value_16(operand, cpu, value);
      break;

    case ot_direct:
      bus_store(cpu->bus, segment_offset(0, operand->data.as_direct.address), value >> 8);
      bus_store(cpu->bus, segment_offset(0, operand->data.as_direct.address + 1), value & 0xff);
      break;

    case ot_immediate:
    case ot_none:
    default:
      assert(0);
      break;
  }
}

static void interpret_add(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_add);

  u8 value8;
  u16 value16;

  switch (instruction->destination.size) {
    case os_8:
      value8 = get_operand_value_8(&instruction->destination, cpu);
      break;

    case os_16:
      value16 = get_operand_value_16(&instruction->destination, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->source.size) {
    case os_8:
      value8 += get_operand_value_8(&instruction->source, cpu);
      break;

    case os_16:
      value16 += get_operand_value_16(&instruction->source, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->destination.size) {
    case os_8:
      set_operand_value_8(&instruction->destination, cpu, value8);
      break;

    case os_16:
      set_operand_value_16(&instruction->destination, cpu, value16);
      break;

    default:
      assert(0);
  }
}

static void interpret_inc(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_inc);

  switch (instruction->destination.size) {
    case os_8: {
      switch (instruction->destination.type) {
        case ot_register: {
          u8 value = get_operand_value_8(&instruction->destination, cpu);
          set_operand_value_8(&instruction->destination, cpu, value + 1);
          break;
        }

        default:
          assert(0);
      }
      break;
    }

    case os_16: {
      switch (instruction->destination.type) {
        case ot_register: {
          u16 value = get_operand_value_16(&instruction->destination, cpu);
          set_operand_value_16(&instruction->destination, cpu, value + 1);
          break;
        }

        default:
          assert(0);
      }
      break;
    }
  }
}

static void interpret_mov(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_mov);

  u8 value8;
  u16 value16;

  switch (instruction->source.size) {
    case os_8:
      value8 = get_operand_value_8(&instruction->source, cpu);
      break;

    case os_16:
      value16 = get_operand_value_16(&instruction->source, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->destination.size) {
    case os_8:
      set_operand_value_8(&instruction->destination, cpu, value8);
      break;

    case os_16:
      set_operand_value_16(&instruction->destination, cpu, value16);
      break;

    default:
      assert(0);
  }
}

static void interpret_test(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_test);

  cpu_clear_flag(cpu, f_carry);
  cpu_clear_flag(cpu, f_overflow);

  switch (instruction->destination.size) {
    case os_8: {
      u8 destination = get_operand_value_8(&instruction->destination, cpu);
      u8 source = get_operand_value_8(&instruction->source, cpu);
      u8 r = destination & source;

      cpu_set_flag_value(cpu, f_sign, r & 0xff);
      cpu_set_flag_value(cpu, f_zero, !r);

      // TODO: More flags are affected by TEST
      break;
    }

    case os_16: {
      u16 destination = get_operand_value_16(&instruction->destination, cpu);
      u16 source = get_operand_value_16(&instruction->source, cpu);
      u16 r = destination & source;

      cpu_set_flag_value(cpu, f_sign, r & 0xffff);
      cpu_set_flag_value(cpu, f_zero, !r);
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_loop(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_loop);

  switch (instruction->destination.size) {
    case os_8: {
      u8 cl = cpu_get_register_8(cpu, CL) - 1;
      cpu_set_register_8(cpu, CL, cl);
      if (cl) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case os_16: {
      u16 cx = cpu_get_register_16(cpu, CX);
      if (cx) {
        cpu_set_register_16(cpu, CX, cx - 1);
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_push(struct cpu *cpu, struct instruction *instruction) {
  (void)cpu;

  assert(instruction->type == it_push);
}

static void interpret_pop(struct cpu *cpu, struct instruction *instruction) {
  (void)cpu;

  assert(instruction->type == it_pop);
}

static void interpret_jmp(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_jmp);

  switch (instruction->destination.type) {
    case ot_direct_with_segment: {
      struct address address = instruction->destination.data.as_direct_with_segment.address;
      cpu->segments[CS] = address.segment;
      cpu->ip = address.offset;
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_instruction(struct cpu *cpu, struct instruction *instruction) {
  switch (instruction->type) {
    case it_add:
      interpret_add(cpu, instruction);
      break;

    case it_hlt:
      // Handle this somewhere else.
      assert(0);
      break;

    case it_inc:
      interpret_inc(cpu, instruction);
      break;

    case it_loop:
      interpret_loop(cpu, instruction);
      break;

    case it_mov:
      interpret_mov(cpu, instruction);
      break;

    case it_test:
      interpret_test(cpu, instruction);
      break;

    case it_cmp: {
      if (instruction->destination.size == os_8) {
        u8 destination = get_operand_value_8(&instruction->destination, cpu);
        u8 source = get_operand_value_8(&instruction->source, cpu);
        set_flags_subtract_8(cpu, destination, source);
      }
      break;
    }

    case it_push:
      interpret_push(cpu, instruction);
      break;

    case it_pop:
      interpret_pop(cpu, instruction);
      break;

    case it_jmp:
      interpret_jmp(cpu, instruction);
      break;

    case it_jb: {
      assert(instruction->destination.type == ot_jump);
      if (cpu_flag_is_set(cpu, f_carry)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_jnb: {
      assert(instruction->destination.type == ot_jump);
      if (!cpu_flag_is_set(cpu, f_carry)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_jz: {
      assert(instruction->destination.type == ot_jump);
      if (cpu_flag_is_set(cpu, f_zero)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_jnz: {
      assert(instruction->destination.type == ot_jump);
      if (!cpu_flag_is_set(cpu, f_zero)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_jp: {
      assert(instruction->destination.type == ot_jump);
      if (cpu_flag_is_set(cpu, f_parity)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_jnp: {
      assert(instruction->destination.type == ot_jump);
      if (!cpu_flag_is_set(cpu, f_parity)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_js: {
      assert(instruction->destination.type == ot_jump);
      if (cpu_flag_is_set(cpu, f_sign)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_jns: {
      assert(instruction->destination.type == ot_jump);
      if (!cpu_flag_is_set(cpu, f_sign)) {
        cpu->ip += instruction->destination.data.as_jump.offset;
      }
      break;
    }

    case it_cli: {
      cpu_clear_flag(cpu, f_interrupt);
      break;
    }

    case it_sahf: {
      cpu->flags = (cpu->flags & 0xff00) | cpu->regs.reg_8[AH];
      break;
    }

    case it_lahf: {
      cpu->regs.reg_8[AH] = cpu->flags & 0xff;
      break;
    }

    case it_noop:
      break;

    default:
      printf("Unsupported instruction (" HEX_8 ")\n", instruction->type);
      assert(0);
  }
}

void cpu_init(struct cpu *cpu, struct bus *bus) {
  memset(cpu->regs.reg_16, 0, sizeof(cpu->regs.reg_16));
  cpu->ip = 0;
  cpu->flags = 0;
  memset(cpu->segments, 0, sizeof(cpu->segments));

  cpu->bus = bus;
}

void hex_dump(const u8 *data, unsigned data_size) {
  for (unsigned i = 0; i < data_size; ++i) {
    printf("%02x", data[i]);
    unsigned mod = i % 0x10;
    if (mod == 0x0f) {
      printf("\n");
    } else if (mod == 0x07) {
      printf("  ");
    } else {
      printf(" ");
    }
  }
}

unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size) {
  u16 cs = cpu->segments[CS];
  u16 ip = cpu->ip;

  u16 i = 0;
  for (; i < size; ++i) {
    buffer[i] = bus_fetch(cpu->bus, segment_offset(cs, ip + i));
  }

  return i;
}

u8 cpu_get_register_8(struct cpu *cpu, enum register_8 reg) {
  return cpu->regs.reg_8[reg];
}

void cpu_set_register_8(struct cpu *cpu, enum register_8 reg, u8 value) {
  cpu->regs.reg_8[reg] = value;
}

u16 cpu_get_register_16(struct cpu *cpu, enum register_16 reg) {
  return cpu->regs.reg_16[reg];
}

void cpu_set_register_16(struct cpu *cpu, enum register_16 reg, u16 value) {
  cpu->regs.reg_16[reg] = value;
}

u16 cpu_get_segment(struct cpu *cpu, enum segment_register reg) {
  return cpu->segments[reg];
}

#if defined(TESTING)
void test_simple_cpu(void);

void test_cpu(void) {
  test_simple_cpu();
}
#endif
