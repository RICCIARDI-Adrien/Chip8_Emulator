/** @file Main.c
 * Emulator entry point and main loop.
 * @author Adrien RICCIARDI
 */
#include <Memory.h>
#include <Processor.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Check parameters
	if (argc != 2)
	{
		printf("Usage : %s Chip8_Program\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// Load the requested program
	if (MemoryRAMLoadFromFile(argv[1]) != 0) return EXIT_FAILURE;
	
	srand(time(NULL));
	
	// Execute the program
	while (1) ProcessorExecuteNextInstruction();
	
	return EXIT_SUCCESS;
}
