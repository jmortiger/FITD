#include "common.h"
// seg 20
void fatalError(int type, const char* name)
{
    //  freeScene();
    freeAll();
    printf("Error: %s\n", name);
    assert(0);
}

extern "C" {
    extern char homePath[512];
}

/// @brief Loads the given ITD file into memory & returns the loaded address.
/// @param name 
/// @return 
char* loadFromItd(const char* name)
{
#ifdef FITD_DEBUGGER
    printf("loadFromItd: Attempting to loading %s into memory...\n", name);
#endif
    FILE* fHandle;
    char* ptr;

    char filePath[512];
    strcpy(filePath, homePath);
    strcat(filePath, name);

    fHandle = fopen(filePath,"rb");
    if(!fHandle)
    {
        fatalError(0,name);
        return NULL;
    }
    fseek(fHandle,0,SEEK_END);
    fileSize = ftell(fHandle);
    fseek(fHandle,0,SEEK_SET);
#ifdef FITD_DEBUGGER
    printf("loadFromItd: Loading %s (%i bytes) into memory...\n", name, fileSize);
#endif
    ptr = (char*)malloc(fileSize);

    if(!ptr)
    {
#ifdef FITD_DEBUGGER
        printf("loadFromItd: Failed to allocate (%i bytes) into memory for %s\n", fileSize, name);
#endif
        fatalError(1,name);
        return NULL;
    }
    fread(ptr,fileSize,1,fHandle);
#ifdef FITD_DEBUGGER
#ifdef _M_X64
    printf("loadFromItd: Loaded %s (%i bytes) into memory at address %llu\n", name, fileSize, (unsigned long long)ptr);
#else
    printf("loadFromItd: Loaded %s (%i bytes) into memory at address %lu\n", name, fileSize, (unsigned long)ptr);
#endif
#endif
    fclose(fHandle);
    return(ptr);
}

char* CheckLoadMallocPak(const char* name, int index)
{
    char* ptr;
    ptr = loadPak(name, index);
    if(!ptr)
    {
        fatalError(0,name);
    }
    return ptr;
}
