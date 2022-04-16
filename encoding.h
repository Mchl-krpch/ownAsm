#ifndef _encode_
#define _encode_

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

#define x64_REX_W 0x48

typedef s32 (*identity_s32)();

typedef enum {
	MOD_Displacement_0    =  0b00, //               <0x00>
	MOD_Displacement_s8   =  0b01, //               <0x01>
	MOD_Displacement_s32  =  0b10, //               <0x02>
	MOD_Register          =  0b11, //               <0x03>
} MOD;

typedef enum {
	// Prefixies for opcode to get extension to some modes
	// It can be used for instruction 'mov rax, 15' f.e. 
	REX    =  0b01000000,
	REX_W  =  0b01001000, // 64-bit mode            <0x48>
	REX_R  =  0b01000100, // ModR/M register field  <0x44>
	REX_X  =  0b01000010, // SiB index field        <0x42>
	REX_B  =  0b01000001, // MpdR/M, r/m, SIM/opc   <0x41>
} REX_BYTE;
 
typedef enum {
	Operand_Type_None,
	Operand_Type_Register,
} Operand_Type;

typedef struct {
	u8 index;
} Register;

typedef struct {
	Operand_Type type;
	union {
		Register reg;
	};
} Operand;

typedef struct {
	u8 *memory;
	u64 occupied;
	u64 capacity;
} Buffer;

typedef enum {
	Operand_Encoding_Type_None,
	Operand_Encoding_Type_Register, 
	Operand_Encoding_Type_Register_Memory,
} Operand_Encoding_Type;

typedef enum {
	Instruction_Extension_Type_Register = 1,
	Instruction_Extension_Type_Op_Code,
	Instruction_Extension_Type_Plus_Register,
} Instruction_Extension_Type;

#endif