/** @file Processor.c
 * @see Processor.h for description.
 * @author Adrien RICCIARDI
 */
#include <Log.h>
#include <Memory.h>
#include <Processor.h>
#include <stdlib.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** All available Vk registers. */
#define PROCESSOR_VK_REGISTERS_COUNT 16

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold PC register (this register can't be directly accessed from Chip-8 instructions, so do not consider it as a real register). */
static int Processor_Register_Program_Counter = MEMORY_RAM_PROGRAM_ENTRY_POINT;

/** I is a special 16-bit register, different from Vk. */
static unsigned short Processor_Register_I;

/** All Vk registers. */
static unsigned char Processor_Registers_Vk[PROCESSOR_VK_REGISTERS_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ProcessorExecuteNextInstruction(void)
{
	unsigned short Instruction, Temporary_Word, Temporary_Byte_1, Temporary_Byte_2;
	
	// Fetch instruction from memory
	LOG_DEBUG("Loading next instruction at address 0x%04X...", Processor_Register_Program_Counter);
	Instruction = MemoryRAMReadWord(Processor_Register_Program_Counter);
	LOG_DEBUG("Instruction code : 0x%04X.", Instruction);
	
	// Decode and execute instruction
	switch (Instruction >> 12) // Extract opcode
	{
		case 0:
			// CLS
			if (Instruction == 0x00E0)
			{
				LOG_DEBUG("Decoded instruction : CLS.");
				// TODO
			}
			// RET
			else if (Instruction == 0x00EE)
			{
				LOG_DEBUG("Decoded instruction : RET.");
				// TODO
			}
			else goto Unknown_Instruction;
			break;
			
		// JP addr
		case 1:
			// Decode instruction
			Temporary_Word = Instruction & 0x0FFF;
			LOG_DEBUG("Decoded instruction : JP 0x%04X.", Temporary_Word);
			// Execute instruction
			Processor_Register_Program_Counter = Temporary_Word;
			break;
			
		// CALL addr
		case 2:
			// Decode instruction
			Temporary_Word = Instruction & 0x0FFF;
			LOG_DEBUG("Decoded instruction : CALL 0x%04X.", Temporary_Word);
			// Execute instruction
			// TODO
			break;
			
		// SE Vx, byte
		case 3:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (unsigned char) Instruction;
			LOG_DEBUG("Decoded instruction : SE V%X, %d.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			if (Processor_Registers_Vk[Temporary_Byte_1] == Temporary_Byte_2) Processor_Register_Program_Counter += 4;
			else Processor_Register_Program_Counter += 2;
			break;
			
		// SNE Vx, byte
		case 4:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (unsigned char) Instruction;
			LOG_DEBUG("Decoded instruction : SNE V%X, %d.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			if (Processor_Registers_Vk[Temporary_Byte_1] != Temporary_Byte_2) Processor_Register_Program_Counter += 4;
			else Processor_Register_Program_Counter += 2;
			break;
			
		// SE Vx, Vy
		case 5:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (Instruction & 0x00F0) >> 4;
			LOG_DEBUG("Decoded instruction : SE V%X, V%X.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			if (Processor_Registers_Vk[Temporary_Byte_1] == Temporary_Byte_2) Processor_Register_Program_Counter += 4;
			else Processor_Register_Program_Counter += 2;
			break;
			
		// LD Vx, byte
		case 6:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (unsigned char) Instruction;
			LOG_DEBUG("Decoded instruction : LD V%X, %d.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			Processor_Registers_Vk[Temporary_Byte_1] = Temporary_Byte_2;
			Processor_Register_Program_Counter += 2;
			break;
			
		// ADD Vx, byte
		case 7:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (unsigned char) Instruction;
			LOG_DEBUG("Decoded instruction : ADD V%X, %d.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			Processor_Registers_Vk[Temporary_Byte_1] += Temporary_Byte_2;
			Processor_Register_Program_Counter += 2;
			break;
			
		case 8:
			// There are several instructions, last nibble allows to differentiate them
			switch (Instruction & 0x000F)
			{
				// LD Vx, Vy
				case 0:
					// Decode instruction
					Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
					Temporary_Byte_2 = (Instruction & 0x00F0) >> 4;
					LOG_DEBUG("Decoded instruction : LD V%X, V%X.", Temporary_Byte_1, Temporary_Byte_2);
					// Execute instruction
					Processor_Registers_Vk[Temporary_Byte_1] = Processor_Registers_Vk[Temporary_Byte_2];
					Processor_Register_Program_Counter += 2;
					break;
					
				// OR Vx, Vy
				case 1:
					// Decode instruction
					Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
					Temporary_Byte_2 = (Instruction & 0x00F0) >> 4;
					LOG_DEBUG("Decoded instruction : OR V%X, V%X.", Temporary_Byte_1, Temporary_Byte_2);
					// Execute instruction
					Processor_Registers_Vk[Temporary_Byte_1] |= Processor_Registers_Vk[Temporary_Byte_2];
					Processor_Register_Program_Counter += 2;
					break;
					
				// AND Vx, Vy
				case 2:
					// Decode instruction
					Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
					Temporary_Byte_2 = (Instruction & 0x00F0) >> 4;
					LOG_DEBUG("Decoded instruction : AND V%X, V%X.", Temporary_Byte_1, Temporary_Byte_2);
					// Execute instruction
					Processor_Registers_Vk[Temporary_Byte_1] &= Processor_Registers_Vk[Temporary_Byte_2];
					Processor_Register_Program_Counter += 2;
					break;
					
				// XOR Vx, Vy
				case 3:
					// Decode instruction
					Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
					Temporary_Byte_2 = (Instruction & 0x00F0) >> 4;
					LOG_DEBUG("Decoded instruction : XOR V%X, V%X.", Temporary_Byte_1, Temporary_Byte_2);
					// Execute instruction
					Processor_Registers_Vk[Temporary_Byte_1] ^= Processor_Registers_Vk[Temporary_Byte_2];
					Processor_Register_Program_Counter += 2;
					break;
					
				// TODO
				
				default:
					goto Unknown_Instruction;
			}
			
		// SNE Vx, Vy
		case 9:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (Instruction & 0x00F0) >> 4;
			LOG_DEBUG("Decoded instruction : SNE V%X, V%X.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			if (Processor_Registers_Vk[Temporary_Byte_1] != Processor_Registers_Vk[Temporary_Byte_2]) Processor_Register_Program_Counter += 4;
			else Processor_Register_Program_Counter += 2;
			break;
			
		// LD I, addr
		case 0xA:
			// Decode instruction
			Temporary_Word = Instruction & 0x0FFF;
			LOG_DEBUG("Decoded instruction : LD I, 0x%04X.", Temporary_Word);
			// Execute instruction
			Processor_Register_I = Temporary_Word;
			Processor_Register_Program_Counter += 2;
			break;
			
		// JP V0, addr
		case 0xB:
			// Decode instruction
			Temporary_Word = Instruction & 0x0FFF;
			LOG_DEBUG("Decoded instruction : JP V0, 0x%04X.", Temporary_Word);
			// Execute instruction
			Processor_Register_Program_Counter = Processor_Registers_Vk[0] + Temporary_Word;
			break;
			
		// RND Vx, byte
		case 0xC:
			// Decode instruction
			Temporary_Byte_1 = (Instruction & 0x0F00) >> 8;
			Temporary_Byte_2 = (unsigned char) Instruction;
			LOG_DEBUG("Decoded instruction : RND V%X, 0x%02X.", Temporary_Byte_1, Temporary_Byte_2);
			// Execute instruction
			Processor_Registers_Vk[Temporary_Byte_1] = rand();
			Processor_Registers_Vk[Temporary_Byte_1] &= Temporary_Byte_2;
			Processor_Register_Program_Counter += 2;
			break;
			
		default:
			goto Unknown_Instruction;
	}
	
	LOG_DEBUG("New Program Counter value : 0x%04X.", Processor_Register_Program_Counter);
	
	getchar();
	
	return;
	
Unknown_Instruction:
	LOG_ERROR("Error : unknown instruction 0x%04X at PC=0x%04X, aborting program.", Instruction, Processor_Register_Program_Counter);
	exit(EXIT_FAILURE);
}
