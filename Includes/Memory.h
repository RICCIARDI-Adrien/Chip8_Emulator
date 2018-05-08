/** @file Memory.h
 * Handle Chip-8 RAM and stack.
 * @author Adrien RICCIARDI
 */
#ifndef H_MEMORY_H
#define H_MEMORY_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Chip-8 total RAM size in bytes. */
#define MEMORY_RAM_TOTAL_SIZE 4096
/** Chip-8 stack size in words. */
#define MEMORY_STACK_TOTAL_SIZE 16

/** Where to load programs and to start them. */
#define MEMORY_RAM_PROGRAM_ENTRY_POINT 512

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
void MemoryStackPush(unsigned short Address);
unsigned short MemoryStackPop(void);

/** Load the full RAM content from a file.
 * @param Pointer_String_File_Name The file to load.
 * @return -1 if an error occurred,
 * @return 0 if the file was successfully loaded.
 * @note If the provided file is larger than the RAM, only the amount of bytes corresponding to the RAM size will be loaded.
 */
int MemoryRAMLoadFromFile(char *Pointer_String_File_Name);

/** Read 8-bit data from the RAM.
 * @param Address The byte to read address.
 * @return The read data.
 */
unsigned char MemoryRAMReadByte(int Address);
void MemoryRAMWriteByte(int Address, unsigned char Data);

/** Read 16-bit data from the RAM and convert them to the emulator platform endianness.
 * @param Address The word to read address. Address will automatically be 16-bit aligned.
 * @return The read data converted to platform endianness.
 */
unsigned short MemoryRAMReadWord(int Address);

void MemoryRAMWriteWord(int Address, unsigned short Data);

#endif
