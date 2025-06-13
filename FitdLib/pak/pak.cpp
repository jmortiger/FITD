// seg 55

#include "common.h"

#ifdef WIN32
#include <direct.h>
#endif

extern "C" { extern char homePath[512]; }

enum PakCompressionType : char {
	PCT_Uncompressed = 0,
	PCT_Explode = 1,
	PCT_Deflate = 4,
}; typedef enum PakCompressionType PakCompressionType;

typedef struct pakInfoStruct // warning: alignement unsafe
{
	s32 discSize;
	s32 uncompressedSize;
	PakCompressionType compressionFlag;
	char info5; /// @brief Explode Compression Flags
	s16 offset;
}; typedef struct pakInfoStruct pakInfoStruct;

//#define USE_UNPACKED_DATA

/// @brief 
/// @param bufferName 
/// @param name 
/// @param ext 
/// @param prependHomePath 
/// @todo Move to utility file?
void makeExtention(char* bufferName, const char* name, const char* ext, bool prependHomePath = true)
{
	if (prependHomePath) strcpy(bufferName, homePath);
	strcat(bufferName, name);
	strcat(bufferName, ".PAK");
}

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
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "PAK_getNumFiles(%s)", name);
	FILE* fileHandle;
	u32 fileOffset;

	char bufferName[512];
	makeExtention(bufferName, name, ".PAK");
	/* strcpy(bufferName, homePath);
	strcat(bufferName, name); // TODO: temporary until makeExtention is coded
	strcat(bufferName, ".PAK"); */
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
	char* lPtr = loadPak(name, index);

	// if (!lPtr) return 0;

	memcpy(ptr, lPtr, getPakSize(name, index));

	free(lPtr);
#endif

	return(1);
}

int getPakSize(const char* name, int index, bool getCompressedSize)
{
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "getPakSize(%s, %i, %s)", name, index, getCompressedSize ? "true" : "false");
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;

	sprintf(buffer, "%s/%04X.OUT", name, index);

	fHandle = fopen(buffer, "rb");

	if (!fHandle) return(0);

	fseek(fHandle, 0L, SEEK_END);
	size = ftell(fHandle);
	fseek(fHandle, 0L, SEEK_SET);

	fclose(fHandle);
	
	DebugEndSection();
	return (size);
#else
	FILE* fileHandle;
	s32 fileOffset;
	s32 additionalDescriptorSize;
	pakInfoStruct pakInfo;
	s32 size = 0;

	char bufferName[512];
	makeExtention(bufferName, name, ".PAK");
	/* strcpy(bufferName, homePath);
	strcat(bufferName, name); // TODO: temporary until makeExtention is coded
	strcat(bufferName, ".PAK"); */

	fileHandle = fopen(bufferName, "rb");

	if (!fileHandle) { // Failed to open file
		DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "File Error: Failed to open %s (full path: %s); returning %i", name, bufferName, size);
		DebugEndSection();
		return size;
	}

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

	if (getCompressedSize) {
		size = pakInfo.discSize;
	} else {
		switch (pakInfo.compressionFlag) {
			case PCT_Uncompressed:
				size = pakInfo.discSize;
				break;
			case PCT_Explode:
			case PCT_Deflate:
				size = pakInfo.uncompressedSize;
				break;
			default:
				DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Expected compression flag of 0, 1, or 4; got %hhi; returning %i", pakInfo.compressionFlag, size);
				break;
		}
	}

	fclose(fileHandle);

	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "%s[%i] is %i bytes (%scompressed)", bufferName, index, size, getCompressedSize ? "" : "un");
	DebugEndSection();
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
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "loadPak(%s, %i)", name, index);
	if (PAK_getNumFiles(name) < index) { // IndexOutOfBounds
		DebugDisableOutput(); int temp = PAK_getNumFiles(name); DebugEnableOutput();
		DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "IndexOutOfBounds: index %i exceeds the number of files in %s (%i); returning NULL", name, temp);
		DebugEndSection();
		return NULL;
	}

	//dumpPak(name);
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	sprintf(buffer, "%s/%04X.OUT", name, index);

	FILE* fHandle = fopen(buffer, "rb");

	if (!fHandle) return NULL;

	fseek(fHandle, 0L, SEEK_END);
	int size = ftell(fHandle);
	fseek(fHandle, 0L, SEEK_SET);

	char* ptr = (char*)malloc(size);

	fread(ptr, size, 1, fHandle);
	fclose(fHandle);

	DebugEndSection();
	return ptr;
#else
	FILE* fileHandle;
	u32 fileOffset;
	u32 additionalDescriptorSize;
	pakInfoStruct pakInfo;
	char* ptr = 0;

	char bufferName[512];
	makeExtention(bufferName, name, ".PAK");
	/* strcpy(bufferName, homePath);
	strcat(bufferName, name); // TODO: temporary until makeExtention is coded
	strcat(bufferName, ".PAK"); */
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Full Path: %s", bufferName);

	fileHandle = fopen(bufferName, "rb");

	if (!fileHandle) { // Failed to open file
		// TODO: Return NULL instead?
		// DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "File Error: Failed to open %s (full path: %s); returning NULL", name, bufferName);
		DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "File Error: Failed to open %s (full path: %s); returning address 0x%lX", name, bufferName, (unsigned long)ptr);
		DebugEndSection();
		return ptr;
	}

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
	if (additionalDescriptorSize)
		fseek(fileHandle, additionalDescriptorSize - 4, SEEK_CUR);

	readPakInfo(&pakInfo, fileHandle);

	char nameBuffer[256] = "";
	if (pakInfo.offset) {
		ASSERT(pakInfo.offset < 256);

		fread(nameBuffer, pakInfo.offset, 1, fileHandle);
#ifdef FITD_DEBUGGER
		/* printf( */ DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Loading %s/%s", name, nameBuffer + 2);
#endif
	} else { fseek(fileHandle, pakInfo.offset, SEEK_CUR); }

	switch (pakInfo.compressionFlag) {
		case PCT_Uncompressed:
		{
			ptr = (char*)malloc(pakInfo.discSize);
			fread(ptr, pakInfo.discSize, 1, fileHandle);
			break;
		}
		case PCT_Explode:
		case PCT_Deflate:
		{
			char* compressedDataPtr = (char*)malloc(pakInfo.discSize);
			fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
			ptr = (char*)malloc(pakInfo.uncompressedSize);

			PAK_decompress((u8*)compressedDataPtr, (u8*)ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5, pakInfo.compressionFlag == PCT_Deflate);

			free(compressedDataPtr);
			break;
		}
		default:
			DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Failed to load %s/%s; expected compression flag of 0, 1, or 4; got %hhi", name, nameBuffer + 2, pakInfo.compressionFlag);
			FITD_throwFatal();
			break;
	}
	fclose(fileHandle);

	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Successfully loaded %s to address 0x%lX", bufferName, (unsigned long)ptr);
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
					FITD_throwFatal();
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
