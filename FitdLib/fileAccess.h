#ifndef _FILE_ACCESS_
#define _FILE_ACCESS_
void fatalError(int type, const char* name);
/// @brief Gets the filesize of the last file loaded using `loadFromItd`.
/// @return 
int getLastFileSize();
char* loadFromItd(const char* name);
char* CheckLoadMallocPak(const char* name, int index);
#endif
