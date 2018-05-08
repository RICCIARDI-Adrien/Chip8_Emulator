/** @file Memory.c
 * @see Memory.h for description.
 * @author Adrien RICCIARDI
 */
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <Log.h>
#include <Memory.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The whole RAM. */
static unsigned char Memory_RAM[MEMORY_TOTAL_RAM_SIZE];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int MemoryRAMLoadFromFile(char *Pointer_String_File_Name)
{
	int File_Descriptor;
	
	// Try to open the file
	LOG_DEBUG("Opening file '%s'...", Pointer_String_File_Name);
	File_Descriptor = open(Pointer_String_File_Name, O_RDONLY);
	if (File_Descriptor == -1)
	{
		LOG_ERROR("Failed to open '%s' file (%s).", Pointer_String_File_Name, strerror(errno));
		return -1;
	}
	
	// Load the file content
	LOG_DEBUG("Loading file content...");
	if (read(File_Descriptor, Memory_RAM, MEMORY_TOTAL_RAM_SIZE) <= 0)
	{
		LOG_ERROR("Could not read file (%s).", strerror(errno));
		close(File_Descriptor);
		return -1;
	}
	
	LOG_DEBUG("File successfully loaded.");
	close(File_Descriptor);
	return 0;
}
