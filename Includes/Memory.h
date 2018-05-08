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
#define MEMORY_TOTAL_RAM_SIZE 4096
/** Chip-8 stack size in words. */
#define MEMORY_TOTAL_STACK_SIZE 16

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

unsigned char MemoryRAMReadByte(int Address);
unsigned short MemoryRAMReadWord(int Address);
void MemoryRAMWriteByte(int Address, unsigned char Data);
void MemoryRAMWriteWord(int Address, unsigned short Data);

#endif
