#include "common.h"
// seg 20
void fatalError(int type, const char* name)
{
	//  freeScene();
	freeAll();
	printf("Error: %s\n", name);
	FITD_throwFatal(); // assert(0);
}

extern "C" {
	extern char homePath[512];
}

char* loadFromItd(const char* name)
{
	FILE* fHandle;
	char* ptr;

	char filePath[512];
	strcpy(filePath, homePath);
	strcat(filePath, name);

	fHandle = fopen(filePath, "rb");
	if (!fHandle) {
		fatalError(0, name); // TODO: Improve error message
		return NULL;
	}
	fseek(fHandle, 0, SEEK_END);
	fileSize = ftell(fHandle);
	fseek(fHandle, 0, SEEK_SET);
	ptr = (char*)malloc(fileSize);

	if (!ptr) {
		fatalError(1, name); // TODO: Improve error message
		return NULL;
	}
	fread(ptr, fileSize, 1, fHandle);
	fclose(fHandle);
	return(ptr);
}

/// @brief Simply a wrapper for `loadPak` that ensures the memory is allocated.
/// @param name 
/// @param index 
/// @return 
char* CheckLoadMallocPak(const char* name, int index)
{
	char* ptr;
	ptr = loadPak(name, index);
	if (!ptr) {
		fatalError(0, name); // TODO: Improve error message
	}
	return ptr;
}
