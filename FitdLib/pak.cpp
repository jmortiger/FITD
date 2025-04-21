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
    fread(&pPakInfo->discSize,4,1,fileHandle);
    fread(&pPakInfo->uncompressedSize,4,1,fileHandle);
    fread(&pPakInfo->compressionFlag,1,1,fileHandle);
    fread(&pPakInfo->info5,1,1,fileHandle);
    fread(&pPakInfo->offset,2,1,fileHandle);

#ifdef FITD_DEBUGGER
	printf("Raw pakInfo read:\n");
	printf("\tdiscSize: %li\n", pPakInfo->discSize);
	printf("\tuncompressedSize: %li\n", pPakInfo->uncompressedSize);
	printf("\tcompressionFlag: %hhu\n", pPakInfo->compressionFlag);
	printf("\tinfo5: %hhu\n", pPakInfo->info5);
	printf("\toffset: %i\n", pPakInfo->offset);
#endif
	pPakInfo->discSize = READ_LE_U32(&pPakInfo->discSize);
	pPakInfo->uncompressedSize = READ_LE_U32(&pPakInfo->uncompressedSize);
	pPakInfo->offset = READ_LE_U16(&pPakInfo->offset);
	
#ifndef WIN32
	
#endif
#ifdef FITD_DEBUGGER
	printf("Resultant pakInfo:\n");
	printf("\tdiscSize: %li\n", pPakInfo->discSize);
	printf("\tuncompressedSize: %li\n", pPakInfo->uncompressedSize);
	printf("\tcompressionFlag: %hhu\n", pPakInfo->compressionFlag);
	printf("\tinfo5: %hhu\n", pPakInfo->info5);
	printf("\toffset: %i\n", pPakInfo->offset);
#endif
}

unsigned int PAK_getNumFiles(const char* name)
{
    char bufferName[512];
    FILE* fileHandle;
    u32 fileOffset;

    strcpy(bufferName, homePath);
    strcat(bufferName, name); // temporary until makeExtention is coded
    strcat(bufferName,".PAK");

    fileHandle = fopen(bufferName,"rb");

    if (!fileHandle)
        return 0;

    ASSERT(fileHandle);

	// Move past the initial 4 empty bytes
    fseek(fileHandle,4,SEEK_CUR);
	// Read 1 set of 4 bytes into variable `fileOffset` as an unsigned 32-bit (4 byte) integer
    fread(&fileOffset,4,1,fileHandle);
#ifdef FITD_DEBUGGER
	printf("PAK_getNumFiles: Initial: %s has %lu files (Raw value: %lu)\n", name, (fileOffset/4)-2, fileOffset);
#endif
    fclose(fileHandle);

#ifdef FITD_DEBUGGER
	printf("PAK_getNumFiles: Final: %s has %lu files (Raw value: %lu)\n", name, (fileOffset/4)-2, fileOffset);
#endif
    return((fileOffset/4)-2);
}

int LoadPak(const char* name, int index, char* ptr)
{
#ifdef USE_UNPACKED_DATA
    char buffer[256];
    FILE* fHandle;
    int size;

    sprintf(buffer,"%s/%04X.OUT",name,index);

    fHandle = fopen(buffer,"rb");

    if(!fHandle)
        return(0);

    fseek(fHandle,0L,SEEK_END);
    size = ftell(fHandle);
    fseek(fHandle,0L,SEEK_SET);

    fread(ptr,size,1,fHandle);
    fclose(fHandle);

    return(1);
#else
    char* lptr;

    lptr = loadPak(name,index);

    memcpy(ptr,lptr,getPakSize(name,index));

    free(lptr);

    return(1);
#endif
}

int getPakSize(const char* name, int index)
{
#ifdef USE_UNPACKED_DATA
    char buffer[256];
    FILE* fHandle;
    int size;

    sprintf(buffer,"%s/%04X.OUT",name,index);

    fHandle = fopen(buffer,"rb");

    if(!fHandle)
        return(0);

    fseek(fHandle,0L,SEEK_END);
    size = ftell(fHandle);
    fseek(fHandle,0L,SEEK_SET);

    fclose(fHandle);

    return (size);
#else
    char bufferName[512];
    FILE* fileHandle;
    s32 fileOffset;
    s32 additionalDescriptorSize;
    pakInfoStruct pakInfo;
    s32 size=0;

#ifdef FITD_DEBUGGER
	printf("getPakSize: Retrieving size of index %i from %s\n", index, name);
#endif

    strcpy(bufferName, homePath);
    strcat(bufferName, name); // temporary until makeExtention is coded
    strcat(bufferName,".PAK");

    fileHandle = fopen(bufferName,"rb");

    if(fileHandle) // a bit stupid, should return NULL right away
    {
        fseek(fileHandle,(index+1)*4,SEEK_SET);

        fread(&fileOffset,4,1,fileHandle);
#ifdef FITD_DEBUGGER
		printf("\tinitial fileOffset: %li\n", fileOffset);
#endif
#ifdef MACOSX
        fileOffset = READ_LE_U32(&fileOffset);
#endif
#ifdef FITD_DEBUGGER
		printf("\tfinal fileOffset: %li\n", fileOffset);
#endif
        fseek(fileHandle,fileOffset,SEEK_SET);

        fread(&additionalDescriptorSize,4,1,fileHandle);
#ifdef FITD_DEBUGGER
		printf("\tinitial additionalDescriptorSize: %li\n", additionalDescriptorSize);
#endif
#ifdef MACOSX
        additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif
#ifdef FITD_DEBUGGER
		printf("\tfinal additionalDescriptorSize: %li\n", additionalDescriptorSize);
#endif

        readPakInfo(&pakInfo,fileHandle);

        fseek(fileHandle,pakInfo.offset,SEEK_CUR);

        if(pakInfo.compressionFlag == 0) // uncompressed
        {
            size = pakInfo.discSize;
        }
        else if(pakInfo.compressionFlag == 1) // compressed
        {
            size = pakInfo.uncompressedSize;
        }
        else if(pakInfo.compressionFlag == 4)
        {
            size = pakInfo.uncompressedSize;
        }

        fclose(fileHandle);
    } else {
		printf("getPakSize: Couldn't open file %s", name);
		// printf("getPakSize: Index out of bounds; %s only has %u files, can't get requested index %i\n", name, PAK_getNumFiles(name), index);
	}

    return size;
#endif
}

char* loadPak(const char* name, int index)
{
#ifdef FITD_DEBUGGER
	printf("loadPak: Retrieving file of index %i from %s\n", index, name);

	unsigned int numFiles = PAK_getNumFiles(name);
	if(numFiles < index) {
		printf("loadPak: Index out of bounds; %s only has %u files, can't get requested index %i\n", name, PAK_getNumFiles(name), index);
		return NULL;
	}
#else
	if(PAK_getNumFiles(name) < index) 
        return NULL;
#endif
    
    
    //dumpPak(name);
#ifdef USE_UNPACKED_DATA
    char buffer[256];
    FILE* fHandle;
    int size;
    char* ptr;

    sprintf(buffer,"%s/%04X.OUT",name,index);

    fHandle = fopen(buffer,"rb");

    if(!fHandle)
        return NULL;

    fseek(fHandle,0L,SEEK_END);
    size = ftell(fHandle);
    fseek(fHandle,0L,SEEK_SET);

    ptr = (char*)malloc(size);

    fread(ptr,size,1,fHandle);
    fclose(fHandle);

    return ptr;
#else
    char bufferName[512];
    FILE* fileHandle;
    u32 fileOffset;
    u32 additionalDescriptorSize;
    pakInfoStruct pakInfo;
    char* ptr=0;


    //makeExtention(bufferName, name, ".PAK");
    strcpy(bufferName, homePath);
    strcat(bufferName, name); // temporary until makeExtention is coded
    strcat(bufferName,".PAK");

    fileHandle = fopen(bufferName,"rb");

    if(fileHandle) // a bit stupid, should return NULL right away
    {
        char nameBuffer[256] = "";

        fseek(fileHandle,(index+1)*4,SEEK_SET);

        fread(&fileOffset,4,1,fileHandle);

#ifdef MACOSX
        fileOffset = READ_LE_U32(&fileOffset);
#endif

        fseek(fileHandle,fileOffset,SEEK_SET);

        fread(&additionalDescriptorSize,4,1,fileHandle);

#ifdef MACOSX
        additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif
		if(additionalDescriptorSize)
		{
			fseek(fileHandle, additionalDescriptorSize-4, SEEK_CUR);
		}

		readPakInfo(&pakInfo,fileHandle);

		if(pakInfo.offset)
		{
			ASSERT(pakInfo.offset<256);

			fread(nameBuffer,pakInfo.offset,1,fileHandle);
#ifdef FITD_DEBUGGER
			printf("Loading %s/%s\n", name,nameBuffer+2);
#endif
		}
		else
		{
			fseek(fileHandle,pakInfo.offset,SEEK_CUR);
		}

		switch(pakInfo.compressionFlag)
		{
		case 0:
			{
				ptr = (char*)malloc(pakInfo.discSize);
				fread(ptr,pakInfo.discSize,1,fileHandle);
				break;
			}
		case 1:
			{
				char * compressedDataPtr = (char *) malloc(pakInfo.discSize);
				fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
				ptr = (char *) malloc(pakInfo.uncompressedSize);

                PAK_explode((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);

                free(compressedDataPtr);
                break;
            }
        case 4:
            {
                char * compressedDataPtr = (char *) malloc(pakInfo.discSize);
                fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
                ptr = (char *) malloc(pakInfo.uncompressedSize);

                PAK_deflate((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize);

                free(compressedDataPtr);
                break;
            }
        default:
#ifdef FITD_DEBUGGER
			printf("Failed to load pak:\n\tfile: %s\n\tresource: %s\n\treason: Unexpected Value; pakInfo.compressionFlag should be 0, 1, or 4; was \n%x", name,nameBuffer+2,(unsigned int)pakInfo.compressionFlag);
#endif
            assert(false);
            break;
        }
        fclose(fileHandle);
    }

    return ptr;
#endif
}

void dumpPak(const char* name)
{
#ifdef WIN32 
    unsigned int numEntries = PAK_getNumFiles(name);

    for (unsigned int index = 0; index < numEntries; index++)
    {
        char bufferName[512];
        FILE* fileHandle;
        u32 fileOffset;
        u32 additionalDescriptorSize;
        pakInfoStruct pakInfo;
        char* ptr = 0;


        //makeExtention(bufferName, name, ".PAK");
        strcpy(bufferName, homePath);
        strcat(bufferName, name); // temporary until makeExtention is coded
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
            if (additionalDescriptorSize)
            {
                fseek(fileHandle, additionalDescriptorSize - 4, SEEK_CUR);
            }

            readPakInfo(&pakInfo, fileHandle);

            if (pakInfo.offset)
            {
                ASSERT(pakInfo.offset < 256);

                fread(nameBuffer, pakInfo.offset, 1, fileHandle);
#ifdef FITD_DEBUGGER
                printf("Loading %s/%s\n", name, nameBuffer + 2);
#endif
            }
            else
            {
                fseek(fileHandle, pakInfo.offset, SEEK_CUR);
            }

            switch (pakInfo.compressionFlag)
            {
            case 0:
            {
                ptr = (char*)malloc(pakInfo.discSize);
                fread(ptr, pakInfo.discSize, 1, fileHandle);
                break;
            }
            case 1:
            {
                char * compressedDataPtr = (char *)malloc(pakInfo.discSize);
                fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
                ptr = (char *)malloc(pakInfo.uncompressedSize);

                PAK_explode((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);

                free(compressedDataPtr);
                break;
            }
            case 4:
            {
                char * compressedDataPtr = (char *)malloc(pakInfo.discSize);
                fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle);
                ptr = (char *)malloc(pakInfo.uncompressedSize);

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
                if (foutputHandle)
                {
                    fwrite(ptr, pakInfo.uncompressedSize, 1, foutputHandle);
                    fclose(foutputHandle);
                }
            }
        }
    }
#endif
}
