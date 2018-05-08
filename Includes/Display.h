/** @file Display.h
 * Emulate the Chip-8 monochrome display.
 * @author Adrien RICCIARDI
 */
#ifndef H_DISPLAY_H
#define H_DISPLAY_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Create a window that will show the display content.
 * @return -1 if an error occurred,
 * @return 0 on success.
 */
int DisplayInitialize(void);

/** Free all display resources. */
void DisplayUninitialize(void);

/** Draw a specific sprite on the display.
 * @param X Drawing X coordinate.
 * @param Y Drawing Y coordinate.
 * @param RAM_Address Sprite starting address in Chip-8 RAM.
 * @param Size Sprite size in bytes.
 * @return 0 if none set pixel did override a previously set pixel (there was "no collision"),
 * @return 1 if one or more collision were detected.
 */
int DisplayDrawSprite(int X, int Y, int RAM_Address, int Size);

/** Render the Chip-8 video memory to the emulator window. */
void DisplayUpdate(void);

#endif
