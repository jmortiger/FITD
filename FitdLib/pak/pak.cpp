// seg 55

#include "common.h"

#ifdef WIN32
#include <direct.h>
#endif

extern "C" {
	extern char homePath[512];
}

typedef struct pakInfoStruct // warning: allignement unsafe
{
	s32 discSize;
	s32 uncompressedSize;
	char compressionFlag;
	char info5;
	s16 offset;
};

typedef struct pakInfoStruct pakInfoStruct;

//#define USE_UNPACKED_DATA

void readPakInfo(pakInfoStruct* pPakInfo, FILE* fileHandle)
{
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	fread(&pPakInfo->discSize, 4, 1, fileHandle);
	fread(&pPakInfo->uncompressedSize, 4, 1, fileHandle);
	fread(&pPakInfo->compressionFlag, 1, 1, fileHandle);
	fread(&pPakInfo->info5, 1, 1, fileHandle);
	fread(&pPakInfo->offset, 2, 1, fileHandle);
	
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "discSize (initial): %i", pPakInfo->discSize);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "uncompressedSize (initial): %i", pPakInfo->uncompressedSize);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "offset (initial): %i", pPakInfo->offset);
	pPakInfo->discSize = READ_LE_U32(&pPakInfo->discSize); // TODO: Change to S32?
	pPakInfo->uncompressedSize = READ_LE_U32(&pPakInfo->uncompressedSize); // TODO: Change to S32?
	pPakInfo->offset = READ_LE_U16(&pPakInfo->offset); // TODO: Change to S32?
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "discSize: %i", pPakInfo->discSize);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "uncompressedSize: %i", pPakInfo->uncompressedSize);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "compressionFlag: %i", pPakInfo->compressionFlag);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "info5: %i", pPakInfo->info5);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "offset: %i", pPakInfo->offset);
	DebugEndSection();
}

unsigned int PAK_getNumFiles(const char* name)
{
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "PAK_getNumFiles(%s)", name);
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	char bufferName[512];
	FILE* fileHandle;
	u32 fileOffset;

	//makeExtention(bufferName, name, ".PAK");
	strcpy(bufferName, homePath);
	strcat(bufferName, name); // TODO: temporary until makeExtention is coded
	strcat(bufferName, ".PAK");
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Full Path: %s", bufferName);

	fileHandle = fopen(bufferName, "rb");

	if (!fileHandle) {
		DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Failed to open %s; returning 0...", bufferName);
		DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Returns: %u", 0);
		DebugEndSection();
		return 0;
	}

	ASSERT(fileHandle);

	fseek(fileHandle, 4, SEEK_CUR);
	fread(&fileOffset, 4, 1, fileHandle);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "fileOffset (initial): %u", fileOffset);
#ifdef MACOSX
	fileOffset = READ_LE_U32(&fileOffset);
#endif
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "fileOffset (final): %u", fileOffset);
	fclose(fileHandle);
	
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Returns: %u", (fileOffset / 4) - 2);
	DebugEndSection();
	return((fileOffset / 4) - 2);
}

int loadPakTo(const char* name, int index, char* ptr)
{
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;

	sprintf(buffer, "%s/%04X.OUT", name, index);

	fHandle = fopen(buffer, "rb");

	if (!fHandle)
		return(0);

	fseek(fHandle, 0L, SEEK_END);
	size = ftell(fHandle);
	fseek(fHandle, 0L, SEEK_SET);

	fread(ptr, size, 1, fHandle);
	fclose(fHandle);
#else
	char* lptr;

	lptr = loadPak(name, index);

	// if (!lptr) return 0;

	memcpy(ptr, lptr, getPakSize(name, index));

	free(lptr);
#endif

	return(1);
}

int getPakSize(const char* name, int index)
{
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;

	sprintf(buffer, "%s/%04X.OUT", name, index);

	fHandle = fopen(buffer, "rb");

	if (!fHandle)
		return(0);

	fseek(fHandle, 0L, SEEK_END);
	size = ftell(fHandle);
	fseek(fHandle, 0L, SEEK_SET);

	fclose(fHandle);

	return (size);
#else
	char bufferName[512];
	FILE* fileHandle;
	s32 fileOffset;
	s32 additionalDescriptorSize;
	pakInfoStruct pakInfo;
	s32 size = 0;

	strcpy(bufferName, homePath);
	strcat(bufferName, name); // TODO: temporary until makeExtention is coded
	strcat(bufferName, ".PAK");

	fileHandle = fopen(bufferName, "rb");

	if (fileHandle) // a bit stupid, should return NULL right away
	{
		fseek(fileHandle, (index + 1) * 4, SEEK_SET);

		fread(&fileOffset, 4, 1, fileHandle);
#ifdef MACOSX
		fileOffset = READ_LE_U32(&fileOffset);
#endif
		fseek(fileHandle, fileOffset, SEEK_SET);

		fread(&additionalDescriptorSize, 4, 1, fileHandle);
#ifdef MACOSX
		additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif

		readPakInfo(&pakInfo, fileHandle);

		fseek(fileHandle, pakInfo.offset, SEEK_CUR);

		if (pakInfo.compressionFlag == 0) // uncompressed
		{
			size = pakInfo.discSize;
		} else if (pakInfo.compressionFlag == 1) // compressed
		{
			size = pakInfo.uncompressedSize;
		} else if (pakInfo.compressionFlag == 4) {
			size = pakInfo.uncompressedSize;
		}

		fclose(fileHandle);
	}

	return size;
#endif
}

/// @brief 
/// @param name 
/// @param index 
/// @return 
/// @details Uses `malloc`.
char* loadPak(const char* name, int index)
{
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "loadPak(%s, %i)", name, index);
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	if (PAK_getNumFiles(name) < index)
		return NULL;

	//dumpPak(name);
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;
	char* ptr;

	sprintf(buffer, "%s/%04X.OUT", name, index);

	fHandle = fopen(buffer, "rb");

	if (!fHandle)
		return NULL;

	fseek(fHandle, 0L, SEEK_END);
	size = ftell(fHandle);
	fseek(fHandle, 0L, SEEK_SET);

	ptr = (char*)malloc(size);

	fread(ptr, size, 1, fHandle);
	fclose(fHandle);

	return ptr;
#else
	char bufferName[512];
	FILE* fileHandle;
	u32 fileOffset;
	u32 additionalDescriptorSize;
	pakInfoStruct pakInfo;
	char* ptr = 0;

	//makeExtention(bufferName, name, ".PAK");
	strcpy(bufferName, homePath);
	strcat(bufferName, name); // TODO: temporary until makeExtention is coded
	strcat(bufferName, ".PAK");
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Full Path: %s", bufferName);

	fileHandle = fopen(bufferName, "rb");

	if (fileHandle) // a bit stupid, should return NULL right away
	{
		char nameBuffer[256] = "";

		fseek(fileHandle, (index + 1) * 4, SEEK_SET);

		fread(&fileOffset, 4, 1, fileHandle);
		DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "fileOffset (initial): %u", fileOffset);

#ifdef MACOSX
		fileOffset = READ_LE_U32(&fileOffset);
#endif

		DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "fileOffset (final): %u", fileOffset);
		fseek(fileHandle, fileOffset, SEEK_SET);

		fread(&additionalDescriptorSize, 4, 1, fileHandle);
		DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "additionalDescriptorSize (initial): %u", additionalDescriptorSize);

#ifdef MACOSX
		additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif
		DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "additionalDescriptorSize (final): %u", additionalDescriptorSize);
		if (additionalDescriptorSize) {
			fseek(fileHandle, additionalDescriptorSize - 4, SEEK_CUR);
		}

		readPakInfo(&pakInfo, fileHandle);

		if (pakInfo.offset) {
			ASSERT(pakInfo.offset < 256);

			fread(nameBuffer, pakInfo.offset, 1, fileHandle);
#ifdef FITD_DEBUGGER
			printf("Loading %s/%s\n", name, nameBuffer + 2);
			DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Loading %s/%s", name, nameBuffer + 2);
#endif
		} else {
			fseek(fileHandle, pakInfo.offset, SEEK_CUR);
		}

		switch (pakInfo.compressionFlag) {
			case 0:
			{
				ptr = (char*)malloc(pakInfo.discSize);
				fread(ptr, pakInfo.discSize, 1, fileHandle);
				break;
			}
			case 1:
			{
				char* compressedDataPtr = (char*)malloc(pakInfo.discSize);
				fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
				ptr = (char*)malloc(pakInfo.uncompressedSize);

				PAK_explode((u8*)compressedDataPtr, (u8*)ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);

				free(compressedDataPtr);
				break;
			}
			case 4:
			{
				char* compressedDataPtr = (char*)malloc(pakInfo.discSize);
				fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
				ptr = (char*)malloc(pakInfo.uncompressedSize);

				PAK_deflate((u8*)compressedDataPtr, (u8*)ptr, pakInfo.discSize, pakInfo.uncompressedSize);

				free(compressedDataPtr);
				break;
			}
			default:
				DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Failed to load %s/%s; expected compression flag of 0, 1, or 4; got %hhi", name, nameBuffer + 2, pakInfo.compressionFlag);
				assert(false);
				break;
		}
		fclose(fileHandle);
	}

	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Successfully loaded %s at %i", bufferName, (long)ptr);
	DebugEndSection();
	return ptr;
#endif
}

void dumpPak(const char* name)
{
#ifdef WIN32 
	unsigned int numEntries = PAK_getNumFiles(name);

	for (unsigned int index = 0; index < numEntries; index++) {
		char bufferName[512];
		FILE* fileHandle;
		u32 fileOffset;
		u32 additionalDescriptorSize;
		pakInfoStruct pakInfo;
		char* ptr = 0;


		//makeExtention(bufferName, name, ".PAK");
		strcpy(bufferName, homePath);
		strcat(bufferName, name); // TODO: temporary until makeExtention is coded
		strcat(bufferName, ".PAK");

		fileHandle = fopen(bufferName, "rb");

		if (fileHandle) // a bit stupid, should return NULL right away
		{
			char nameBuffer[256] = "";

			fseek(fileHandle, (index + 1) * 4, SEEK_SET);

			fread(&fileOffset, 4, 1, fileHandle);

#ifdef MACOSX
			fileOffset = READ_LE_U32(&fileOffset);
#endif

			fseek(fileHandle, fileOffset, SEEK_SET);

			fread(&additionalDescriptorSize, 4, 1, fileHandle);

#ifdef MACOSX
			additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif
			if (additionalDescriptorSize) {
				fseek(fileHandle, additionalDescriptorSize - 4, SEEK_CUR);
			}

			readPakInfo(&pakInfo, fileHandle);

			if (pakInfo.offset) {
				ASSERT(pakInfo.offset < 256);

				fread(nameBuffer, pakInfo.offset, 1, fileHandle);
#ifdef FITD_DEBUGGER
				printf("Loading %s/%s\n", name, nameBuffer + 2);
#endif
			} else {
				fseek(fileHandle, pakInfo.offset, SEEK_CUR);
			}

			switch (pakInfo.compressionFlag) {
				case 0:
				{
					ptr = (char*)malloc(pakInfo.discSize);
					fread(ptr, pakInfo.discSize, 1, fileHandle);
					break;
				}
				case 1:
				{
					char* compressedDataPtr = (char*)malloc(pakInfo.discSize);
					fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
					ptr = (char*)malloc(pakInfo.uncompressedSize);

					PAK_explode((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);

					free(compressedDataPtr);
					break;
				}
				case 4:
				{
					char* compressedDataPtr = (char*)malloc(pakInfo.discSize);
					fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
					ptr = (char*)malloc(pakInfo.uncompressedSize);

					PAK_deflate((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize);

					free(compressedDataPtr);
					break;
				}
				default:
					assert(false);
					break;
			}
			fclose(fileHandle);

			{
				mkdir(name);
				char outputName[256];
				sprintf(outputName, "%s/%02d_%s", name, index, nameBuffer + 2);
				FILE* foutputHandle = fopen(outputName, "wb+");
				if (foutputHandle) {
					fwrite(ptr, pakInfo.uncompressedSize, 1, foutputHandle);
					fclose(foutputHandle);
				}
			}
		}
	}
#endif
}
