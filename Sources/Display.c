/** @file Display.c
 * @see Display.h for description.
 * @author Adrien RICCIARDI
 */
#include <assert.h>
#include <Display.h>
#include <Log.h>
#include <Memory.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

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

/** The video memory. One array cell stands for one pixel as seen by the Chip-8 program. */
static unsigned char Display_Video_Memory[DISPLAY_HEIGHT_PIXELS][DISPLAY_WIDTH_PIXELS] = {0};

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

int DisplayDrawSprite(int X, int Y, int RAM_Address, int Size)
{
	unsigned char Byte, Is_Pixel_Currently_Set;
	int i, Is_Pixel_Previouly_Set, Return_Value = 0;
	
	assert(X < DISPLAY_WIDTH_PIXELS);
	assert(Y < DISPLAY_HEIGHT_PIXELS);
	assert(RAM_Address < MEMORY_RAM_TOTAL_SIZE);
	assert(Size < 16);
	
	// Copy the sprite to the video memory
	while (Size > 0)
	{
		// Load the sprite next 8 horizontal pixels
		Byte = MemoryRAMReadByte(RAM_Address);
		
		// Expand each bit on a byte
		for (i = 0; i < 8; i++)
		{
			// Check if current pixel is set to tell if a collision occurred
			Is_Pixel_Previouly_Set = Display_Video_Memory[Y][X + i];
			
			// Is this bit set ?
			if (Byte & (0x80 >> i)) Is_Pixel_Currently_Set = 1;
			else Is_Pixel_Currently_Set = 0;
			Display_Video_Memory[Y][X + i] = Is_Pixel_Currently_Set;
			
			// There is a collision if a turned on pixel is turned on another time
			if (Is_Pixel_Previouly_Set && Is_Pixel_Currently_Set) Return_Value = 1;
		}
		
		Y++;
		// Do not cross display lower border
		if (Y >= DISPLAY_HEIGHT_PIXELS) break;
		
		RAM_Address++;
		Size--;
	}
	
	return Return_Value;
}

void DisplayUpdate(void)
{
	int Coordinate_Y, Coordinate_X;
	SDL_Rect Rectangle;
	
	// Clear the display with a blue background (like white-on-blue LCD modules)
	if (SDL_SetRenderDrawColor(Pointer_Display_Main_Renderer, 0, 0, 200, 255) != 0)
	{
		LOG_ERROR("Could not set rendering draw color (%s).", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	if (SDL_RenderClear(Pointer_Display_Main_Renderer) != 0)
	{
		LOG_ERROR("Failed to clear the rendering area (%s).", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	// Draw a white rectangle for each set pixel
	if (SDL_SetRenderDrawColor(Pointer_Display_Main_Renderer, 255, 255, 255, 255) != 0)
	{
		LOG_ERROR("Could not set rendering draw color (%s).", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	for (Coordinate_Y = 0; Coordinate_Y < DISPLAY_HEIGHT_PIXELS; Coordinate_Y++)
	{
		for (Coordinate_X = 0; Coordinate_X < DISPLAY_WIDTH_PIXELS; Coordinate_X++)
		{
			// Do nothing if the pixel is not set
			if (!Display_Video_Memory[Coordinate_Y][Coordinate_X]) continue;
			
			// Display the rectangle
			Rectangle.x = Coordinate_X * DISPLAY_SCALING_FACTOR;
			Rectangle.y = Coordinate_Y * DISPLAY_SCALING_FACTOR;
			Rectangle.w = DISPLAY_SCALING_FACTOR;
			Rectangle.h = DISPLAY_SCALING_FACTOR;
			if (SDL_RenderFillRect(Pointer_Display_Main_Renderer, &Rectangle) != 0)
			{
				LOG_ERROR("Failed to render a rectangle at video memory coordinates (%d,%d) (%s).", Coordinate_X, Coordinate_Y, SDL_GetError());
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// Update screen
	SDL_RenderPresent(Pointer_Display_Main_Renderer);
}
