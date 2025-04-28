#ifndef _PAK_
#define _PAK_

char* loadPak(const char* name, int index);
/// @brief Load the file at the given index in the given Pak into the given memory address.
/// @param name The `.PAK` file to load.
/// @param index The index in the file to load.
/// @param ptr The address to place the pak's data into.
/// @return 0 for failure, otherwise 1.
int loadPakTo(const char* name, int index, char* ptr);
int getPakSize(const char* name, int index);
unsigned int PAK_getNumFiles(const char* name);
void dumpPak(const char* name);

#endif
