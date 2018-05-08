/** @file Processor.h
 * Chip-8 Central Processing Unit emulator.
 * @author Adrien RICCIARDI
 */
#ifndef H_PROCESSOR_H
#define H_PROCESSOR_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Execute the instruction pointed by Program Counter register and update RAM, stack and registers accordingly. */
void ProcessorExecuteNextInstruction(void);

#endif
