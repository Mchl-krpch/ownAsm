#include <assert.h>
#include <stdio.h>
#include <memoryapi.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <io.h>
#include <fcntl.h>

#include "encoding.h"

typedef struct {
	u16 op_code;
	Instruction_Extension_Type extension_type;
	Operand_Encoding_Type operand_types[2];
} Instruction_Encoding;

typedef struct {
	Instruction_Encoding *encoding_list;
	u32 encoding_count;
} X64_Mnemonic;

typedef struct {
	X64_Mnemonic mnemonic;
	Operand operands[2];
} Instruction;

////////////////////////////////////
#define REG_DEF(name, id)          \
const Operand name = {.            \
	type = Operand_Type_Register,  \
	.reg = {id}                    \
};                                 \
////////////////////////////////////
REG_DEF(rax, 0b0000)
REG_DEF(rcx, 0b0001)
REG_DEF(rdx, 0b0010)
REG_DEF(rbx, 0b0011)
REG_DEF(rsp, 0b0100)
REG_DEF(rbp, 0b0101)
REG_DEF(rsi, 0b0110)
REG_DEF(rdi, 0b0111)
REG_DEF(r8,  0b1000)
REG_DEF(r9,  0b1001)
REG_DEF(r10, 0b1010)
REG_DEF(r11, 0b1011)
REG_DEF(r12, 0b1100)
REG_DEF(r13, 0b1101)
REG_DEF(r14, 0b1110)
REG_DEF(r15, 0b1111)
////////////////////////////////////

void buffer_append_s32	(Buffer *buffer, u8 value)
{
	buffer->memory[buffer->occupied] = value;
	buffer->occupied += sizeof(value);
}

void buffer_append_u8	(Buffer *buffer, u8 value)
{
	u8 *first_non_occupoed_add = buffer->memory + buffer->occupied;
	s32 *target = (s32 *)first_non_occupoed_add;
	assert(buffer->occupied + sizeof(value) <= buffer->capacity);
	*target = value;

	buffer->occupied += sizeof(value);
}

X64_Mnemonic mov {
	.encoding_list = 0,
	.encoding_count = 1,
};

X64_Mnemonic ret = {
	.encoding_list = 0,
	.encoding_count = 0
};

void encode		(Buffer *buffer, Instruction instruction)
{
	assert(buffer);

	for (u32 ind = 0; ind < instruction.mnemonic.encoding_count; ++ind) {
		Instruction_Encoding *encoding = &instruction.mnemonic.encoding_list[ind];

		/////////////////////////////////////////////////////////////////////////////

		//FIXME remove this hardcoding to instruction number of operands
		for (u32 operand_ind = 0; operand_ind < 2; ++operand_ind)
		{
			Operand_Encoding_Type encoding_type = encoding->operand_types[operand_ind];
			Operand_Type operand_type = instruction.operands[operand_ind].type;

			if (operand_type == Operand_Type_None &&
				encoding_type == Operand_Encoding_Type_None) {
				continue;
			}

			if (operand_type == Operand_Type_Register &&
				encoding_type == Operand_Encoding_Type_Register) {
				continue;
			}

			if (operand_type == Operand_Type_Register &&
				encoding_type == Operand_Encoding_Type_Register_Memory) {
				continue;
			}
			assert(false);
		}

		/////////////////////////////////////////////////////////////////////////////

		bool needs_mod_r_m = false;
		for (u32 operand_ind = 0; operand_ind < 2; ++operand_ind)
		{
			Operand_Type operand_type = instruction.operands[operand_ind].type;

			if (operand_type == Operand_Type_Register) {
				buffer_append_u8(buffer, REX_W);
				needs_mod_r_m = true;
				break;
			}
		}
		// FIXME if opcode is 2 bytes need different append
		buffer_append_u8(buffer, (u8)encoding->op_code);

		/////////////////////////////////////////////////////////////////////////////

		// FIXME Implement proper mod support
		// FIX mask register index
		if (needs_mod_r_m) {
			u8 mod_r_m = (
				(MOD_Register << 6) |
				(instruction.operands[0].reg.index << 3) |
				(instruction.operands[1].reg.index)
			);
			buffer_append_u8(buffer, mod_r_m);
		}
	}
}

const Buffer make_buffer	(u64 capacity, DWORD permission_flags)
{

	u8 *memory = (u8 *)VirtualAlloc(0, capacity, MEM_COMMIT | MEM_RESERVE, permission_flags);
	return (const Buffer) {
		.memory   = memory,
		.occupied = 0,
		.capacity = capacity,
	};
}

identity_s32 make_identity_s32	(s32 value)
{

	Buffer buffer = make_buffer(1024, PAGE_EXECUTE_READWRITE);
	encode(&buffer, (const Instruction){mov, {rcx, rax}});

	buffer_append_u8(&buffer, value);
	encode(&buffer, (const Instruction){ret, {}});
	// buffer_append_u8(&buffer, x64_ret);

	int discr = open("enc", O_WRONLY | O_CREAT);
	write(discr, buffer.memory, 8);
	close(discr);

	return (identity_s32)buffer.memory;
}

// #define x64_ret   0xc3
// 66 bb 2a
int main	(void)
{
	printf("compiler\n");
	mov.encoding_list = (Instruction_Encoding *)malloc(sizeof(Instruction_Encoding));
	*mov.encoding_list = {
		.op_code = 0x89,
		.extension_type = Instruction_Extension_Type_Register,
		.operand_types = {
			Operand_Encoding_Type_Register_Memory,
			Operand_Encoding_Type_Register,
		},
	};
	mov.encoding_count = 1;

	ret.encoding_list = (Instruction_Encoding *)malloc(sizeof(Instruction_Encoding));
	*ret.encoding_list = {
		.op_code = 0xc3,
		.extension_type = Instruction_Extension_Type_Register,
		.operand_types = {
			Operand_Encoding_Type_None,
			Operand_Encoding_Type_None,
		},
	};
	ret.encoding_count = 1;
	
	identity_s32 answer = make_identity_s32(0xBB);
	// make_identity_s32(0xBB);
	// printf("%d\n", (int)answer);
	// s64 result = answer();
	// printf("%X\n", (unsigned)result);

	return 0;
}