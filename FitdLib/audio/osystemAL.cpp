#include <stdlib.h>
#include <assert.h>

//#include "OpenAL/al.h"
//#include "OpenAL/alc.h"

#include "soloud.h"
#include "soloud_file.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

#include "common.h"
#include "osystemAL.h"

void osystemAL_mp3_Update();
void osystemAL_adlib_Update();

SoLoud::Soloud* gSoloud = NULL;

void osystemAL_init()
{
	gSoloud = new SoLoud::Soloud();
	gSoloud->init();
}

class ITD_AudioSource : public SoLoud::AudioSource
{
public:
	char* m_samples;
	int m_size;

	ITD_AudioSource(char* samplePtr, int size);
	virtual ~ITD_AudioSource();
	virtual SoLoud::AudioSourceInstance* createInstance();
};

class ITD_AudioInstance : public SoLoud::AudioSourceInstance
{
	ITD_AudioSource* mParent;
	unsigned int mOffset;
public:
	ITD_AudioInstance(ITD_AudioSource* aParent) : SoLoud::AudioSourceInstance()
	{
		mOffset = 0;
		mParent = aParent;
	}

	virtual unsigned int getAudio(float* aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize) override
	{
		for (int i = 0; i < aSamplesToRead; i++) {
			if (mOffset > mParent->m_size) {
				aBuffer[i] = 0.f;
			} else {
				aBuffer[i] = (((float)((unsigned char*)mParent->m_samples)[mOffset]) - 128.f) / 128.f;
				mOffset++;
			}
		}
		return aSamplesToRead;
	}

	virtual bool hasEnded()
	{
		if (mOffset > mParent->m_size) {
			return true;
		}

		return false;
	}
};

/// @brief 
/// @param samplePtr The address of the loaded sample to play.
/// @param size The size of the sample in bits(?); CURRENTLY UNUSED (size is determined by reading info from the start of `samplePtr`).
/// @details AITD uses compressed [VOC](https://en.wikipedia.org/wiki/Creative_Voice_file) files. This function seems to check that the version bytes 
ITD_AudioSource::ITD_AudioSource(char* samplePtr, int size) : SoLoud::AudioSource()
{
	DebugPrintfLnCategory(DBO_L_INFO, DBO_SOUND, "ITD_AudioSource::ITD_AudioSource(char* samplePtr, %i):%s", size, samplePtr[26] != 1 ? " early exit (expected samplePtr[26] to be 1; was not)." : "");
	assert(samplePtr[26] == 1); //assert first block is of sound data type
	DebugBeginSection(DBO_SOUND);
	// #region VOC Header
	DebugPrintfLn(DBO_L_DEBUG, "VOC Header: ");
	DebugBeginSection(DBO_SOUND);
	DebugPrintfLn(DBO_L_DEBUG, "0x%X (%u): %s (expecting \"Creative Voice File\")", 0x00, 0x00, samplePtr + 0x00);
	DebugPrintfLn(DBO_L_DEBUG, "0x%X (%u): 0x%hhX (expecting 0x1A) (EOF)", 0x13, 0x13, *(u8*)(samplePtr + 0x13));
	DebugPrintfLn(DBO_L_DEBUG, "0x%X (%u): 0x%hX/%hu (Usually 0x001A/26)(Raw: 0x%hX/%hu) (Starting byte of first data block)", 0x14, 0x14, READ_LE_U16(samplePtr + 0x14), READ_LE_U16(samplePtr + 0x14), *(u16*)(samplePtr + 0x14), *(u16*)(samplePtr + 0x14));
	u16 version = READ_LE_U16(samplePtr + 0x16);
	DebugPrintfLn(DBO_L_DEBUG, "0x%X (%u): v. %hhu.%hhu (Raw: 0x%hX) (expecting 0x010A/0x0114)", 0x16, 0x16, *(u8*)(samplePtr + 0x17), *(u8*)(samplePtr + 0x16), *(u16*)(samplePtr + 0x16));
	bool correct = READ_LE_U16(samplePtr + 0x18) == (u16)(~version + 0x1234);
	DebugPrintfLn(correct ? DBO_L_DEBUG : DBO_L_WARN, "0x%X (%u): ~0x%hX + 0x1234 = 0x%hX + 0x1234 = 0x%hX %s 0x%hX (Raw: 0x%hX) (expecting 0x1129)", 0x18, 0x18, version, ~version, ~version + 0x1234, correct ? "=" : "!=", READ_LE_U16(samplePtr + 0x18), *(u16*)(samplePtr + 0x18));
	DebugEndSection();
	// #endregion VOC Header
	// DebugBPrintf(DBO_L_DEBUG, "sampleSize: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData + 4)); DebugBFlushLn();
	DebugPrintfLn(DBO_L_DEBUG, "sampleSize: (*(samplePtr + 26) >> 8) - 2 => (%u >> 8) - 2 => %u (Raw: (%u >> 8) - 2 => %u)", READ_LE_U32(samplePtr + 26), (READ_LE_U32(samplePtr + 26) >> 8) - 2, *(u32*)(samplePtr + 26), (*(u32*)(samplePtr + 26) >> 8) - 2);
	int sampleSize = (READ_LE_U32(samplePtr + 26) >> 8) - 2;
	if (sampleSize != size - 33) {
		DebugPrintfLn(DBO_L_WARN, "Expected sampleSize (%i) to be %i - 26 (header) - 1 (type byte) - 3 (3 size bytes) - 1 (sample rate byte) - 1 (codec byte) - 1 (terminator byte)");
	}

	DebugPrintfLn(DBO_L_DEBUG, "frequencyDiv: %hhu (*(samplePtr + 30))", *(unsigned char*)(samplePtr + 30));
	int frequencyDiv = *(unsigned char*)(samplePtr + 30);
	DebugPrintfLn(DBO_L_DEBUG, "codecID (compression) (unused): %hhi/%hhu (samplePtr[31])", samplePtr[31], (u8)samplePtr[31]);
	// int codecId = samplePtr[31];
	
	// TODO: Print sample data
	// DebugPrintfLn(DBO_L_DEBUG, "frequencyDiv: %hhu (*(samplePtr + 30))", *(unsigned char*)(samplePtr + 30));
	char* sampleData = samplePtr + 32;

	DebugPrintfLn(DBO_L_DEBUG, "sampleRate: %i (1000000/(256-frequencyDiv) => 1000000/(256-%i) => 1000000/%i)", 1000000 / (256 - frequencyDiv), frequencyDiv, 256 - frequencyDiv);
	int sampleRate = 1000000 / (256 - frequencyDiv);

	m_samples = sampleData;
	m_size = sampleSize - 1;

	mBaseSamplerate = sampleRate;
	DebugEndSection();
}

ITD_AudioSource::~ITD_AudioSource() {}

SoLoud::AudioSourceInstance* ITD_AudioSource::createInstance() { return new ITD_AudioInstance(this); }

void osystem_playSample(char* samplePtr, int size)
{
	if (g_gameId >= TIMEGATE) {
		SoLoud::Wav* pAudioSource = new SoLoud::Wav();
		pAudioSource->loadMem((u8*)samplePtr, size, true);
		gSoloud->play(*pAudioSource);
	} else {
		ITD_AudioSource* pAudioSource = new ITD_AudioSource(samplePtr, size);
		gSoloud->play(*pAudioSource);
	}
}

extern float gVolume;

/// @brief Just updates the Soloud engine's global volume to `gVolume`;
void osystemAL_update() { gSoloud->setGlobalVolume(gVolume); }

SoLoud::DiskFile* pFile = NULL;
SoLoud::WavStream* pWavStream = NULL;

/// @brief 
/// @param trackId 
/// @return 0 always??
int osystem_playTrack(int trackId)
{
	DebugPrintfLnCategory(DBO_L_INFO, DBO_SOUND, "osystem_playTrack(%i):", trackId);
	DebugBeginSection(DBO_SOUND);
	// NOTE: Does this get freed?
	if (pWavStream) { pWavStream->stop(); }

	char filename[256];
	sprintf(filename, "%02d.wav", trackId);

	FILE* fHandle = fopen(filename, "rb");
	SoLoud::WAVSTREAM_FILETYPE filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_WAV;
	if (fHandle == NULL) {
		// DebugPrintfLn(DBO_L_WARN, "File %s failed to open; exiting early w/ a value of 0", filename);
		// DebugEndSection();
		// return 0;
		DebugPrintfLn(DBO_L_WARN, "File %s failed to open; trying .mp3", filename);
		filename[0] = '\000';
		sprintf(filename, "%02d.mp3", trackId);

		FILE* fHandle = fopen(filename, "rb");
		filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_MP3;
		if (fHandle == NULL) {
			DebugPrintfLn(DBO_L_WARN, "File %s failed to open; trying .ogg", filename);
			filename[0] = '\000';
			sprintf(filename, "%02d.ogg", trackId);
	
			FILE* fHandle = fopen(filename, "rb");
			filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_OGG;
			if (fHandle == NULL) {
				DebugPrintfLn(DBO_L_WARN, "File %s failed to open; trying .flac", filename);
				filename[0] = '\000';
				sprintf(filename, "%02d.flac", trackId);
		
				FILE* fHandle = fopen(filename, "rb");
				filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_FLAC;
				if (fHandle == NULL) {
					DebugPrintfLn(DBO_L_WARN, "File %s failed to open; exiting early w/ a value of 0", filename);
					DebugEndSection();
					return 0;
				}
			}
		}
	}

	pFile = new SoLoud::DiskFile(fHandle);
	pWavStream = new SoLoud::WavStream();
	// pWavStream->loadFile(pFile);
	pWavStream->parse(pFile);
	/* switch (filetype)
	{
	case SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_WAV:
		pWavStream->loadwav(pFile);
		break;
	
	default:
		break;
	} */
	gSoloud->play(*pWavStream);

	DebugEndSection();
	return 0;
}

// void osystem_playSampleFromName(char* sampleName)
// {
// 	DebugPrintfLnCategory(DBO_L_INFO, DBO_SOUND, "osystem_playSampleFromName(%s):", sampleName);
// 	DebugBeginSection(DBO_SOUND);

// 	FILE* fHandle = fopen(sampleName, "rb");
// 	SoLoud::WAVSTREAM_FILETYPE filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_WAV;
// 	if (fHandle == NULL) {
// 		// DebugPrintfLn(DBO_L_WARN, "File %s failed to open; exiting early w/ a value of 0", sampleName);
// 		// DebugEndSection();
// 		// return 0;
// 		DebugPrintfLn(DBO_L_WARN, "File %s failed to open; trying .mp3", sampleName);
// 		sampleName[0] = '\000';
// 		sprintf(sampleName, "%02d.mp3", trackId);

// 		FILE* fHandle = fopen(sampleName, "rb");
// 		filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_MP3;
// 		if (fHandle == NULL) {
// 			DebugPrintfLn(DBO_L_WARN, "File %s failed to open; trying .ogg", sampleName);
// 			sampleName[0] = '\000';
// 			sprintf(sampleName, "%02d.ogg", trackId);
	
// 			FILE* fHandle = fopen(sampleName, "rb");
// 			filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_OGG;
// 			if (fHandle == NULL) {
// 				DebugPrintfLn(DBO_L_WARN, "File %s failed to open; trying .flac", sampleName);
// 				sampleName[0] = '\000';
// 				sprintf(sampleName, "%02d.flac", trackId);
		
// 				FILE* fHandle = fopen(sampleName, "rb");
// 				filetype = SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_FLAC;
// 				if (fHandle == NULL) {
// 					DebugPrintfLn(DBO_L_WARN, "File %s failed to open; exiting early w/ a value of 0", sampleName);
// 					DebugEndSection();
// 					return 0;
// 				}
// 			}
// 		}
// 	}

// 	pFile = new SoLoud::DiskFile(fHandle);
// 	pWavStream = new SoLoud::WavStream();
// 	// pWavStream->loadFile(pFile);
// 	pWavStream->parse(pFile);
// 	/* switch (filetype)
// 	{
// 	case SoLoud::WAVSTREAM_FILETYPE::WAVSTREAM_WAV:
// 		pWavStream->loadwav(pFile);
// 		break;
	
// 	default:
// 		break;
// 	} */
// 	gSoloud->play(*pWavStream);

// 	DebugEndSection();
// 	return 0;
// }
