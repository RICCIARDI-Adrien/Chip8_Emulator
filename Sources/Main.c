/** @file Main.c
 * Emulator entry point and main loop.
 * @author Adrien RICCIARDI
 */
#include <Display.h>
#include <Log.h>
#include <Memory.h>
#include <Processor.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The thread running the Chip-8 processor. */
//static SDL_Thread *Pointer_Main_Thread_Processor; TODO

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Gracefully stop SDL on program exit. */
static void MainExitUninitializeSDL(void)
{
	SDL_Quit();
	LOG_DEBUG("SDL has been uninitialized.");
}

/** Close display resources on program exit. */
static void MainExitUninitializeDisplay(void)
{
	DisplayUninitialize();
	LOG_DEBUG("Display has been uninitialized.");
}

// TODO
/*static void MainExitUninitializeProcessorThread(void)
{
	// TODO
}*/

/** Execute the Chip-8 program.
 * @param Pointer This parameter is unused.
 * @return Unused value.
 */
static int MainThreadProcessor(void __attribute__((unused)) *Pointer)
{
	while (1)
	{
		ProcessorExecuteNextInstruction();
		
		// TEST
		getchar();
	}
	
	// To make the compiler happy
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	SDL_Event Event;
	
	// Check parameters
	if (argc != 2)
	{
		printf("Usage : %s Chip8_Program\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// Load the requested program
	if (MemoryRAMLoadFromFile(argv[1]) != 0) return EXIT_FAILURE;
	
	srand(time(NULL));
	
	// Initialize needed SDL subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
	{
		LOG_ERROR("Failed to initialize SDL2 library (%s).", SDL_GetError());
		return EXIT_FAILURE;
	}
	atexit(MainExitUninitializeSDL);
	
	// Create the rendering area
	if (DisplayInitialize() != 0) return EXIT_FAILURE;
	atexit(MainExitUninitializeDisplay);
	
	// Execute the Chip-8 program in a separated thread to handle SDL events using the main thread
	if (SDL_CreateThread(MainThreadProcessor, "Chip-8 CPU", NULL) == NULL)
	{
		LOG_ERROR("Failed to create Chip-8 processor thread (%s).", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	while (1)
	{
		// Handle SDL events
		while (SDL_PollEvent(&Event))
		{
			switch (Event.type)
			{
				case SDL_QUIT:
					LOG_DEBUG("Received quit event.");
					return EXIT_SUCCESS;
					
				default:
					break;
			}
		}
		
		DisplayUpdate();
	}
	
	return EXIT_SUCCESS;
}
