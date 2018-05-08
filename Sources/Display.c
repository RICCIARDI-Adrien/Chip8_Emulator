/** @file Display.c
 * @see Display.h for description.
 * @author Adrien RICCIARDI
 */
#include <Display.h>
#include <Log.h>
#include <SDL2/SDL.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many columns the display has. */
#define DISPLAY_WIDTH_PIXELS 64
/** How many rows the display has. */
#define DISPLAY_HEIGHT_PIXELS 32

/** How many real screen pixel a Chip-8 pixel takes (to make the game visible on modern screens). */
#define DISPLAY_SCALING_FACTOR 16

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The window to render to. */
static SDL_Window *Pointer_Display_Window;

/** The renderer used to draw in the window. */
static SDL_Renderer *Pointer_Display_Main_Renderer;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int DisplayInitialize(void)
{
	// Create the window
	LOG_DEBUG("Creating window...");
	Pointer_Display_Window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH_PIXELS * DISPLAY_SCALING_FACTOR, DISPLAY_HEIGHT_PIXELS * DISPLAY_SCALING_FACTOR, 0);
	if (Pointer_Display_Window == NULL)
	{
		LOG_ERROR("Failed to create SDL window (%s).", SDL_GetError());
		return -1;
	}
	
	// Create the renderer
	LOG_DEBUG("Creating renderer...");
	Pointer_Display_Main_Renderer = SDL_CreateRenderer(Pointer_Display_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Pointer_Display_Main_Renderer == NULL)
	{
		LOG_ERROR("Failed to create SDL renderer (%s).", SDL_GetError());
		SDL_DestroyWindow(Pointer_Display_Window);
		return -1;
	}
	
	return 0;
}

void DisplayUninitialize(void)
{
	SDL_DestroyRenderer(Pointer_Display_Main_Renderer);
	SDL_DestroyWindow(Pointer_Display_Window);
}
