#ifndef SPENCER_AUDIO_H
#define SPENCER_AUDIO_H

#include "I2S.h"
#include "AudioFileSourceSerialFlash.h"
#include "CompositeAudioFileSource.h"
#include <AudioFileSource.h>
#include <AudioOutputI2S.h>
#include <AudioGeneratorWAV.h>
#include <AudioGeneratorMP3.h>
#include "SampleStore.h"

class Audio
{
public:
	Audio();
	~Audio();
	void begin();
	/**
	 * Starts WAV audio playback from an AudioFileSource pointer.
	 * @param file AudioFileSource pointer to the desired file.
	 */
	void playWAV(AudioFileSource* file);
	/**
	 * Starts WAV audio playback of a file from the serial flash chip.
	 * @param path Path of the file on the flash chip.
	 */
	void playWAV(const char* path);
	/**
	 * Starts MP3 audio playback from an AudioFileSource pointer.
	 * @param file AudioFileSource pointer to the desired file.
	 */
	void playMP3(AudioFileSource* file);
	/**
	 * Starts MP3 audio playback of a file from the serial flash chip.
	 * @param path Path of the file on the flash chip.
	 */
	void playMP3(const char* path);
	void stopPlayback();
	/**
	 * Starts audio recording. File is saved on the flash chip as "recording.wav"
	 * @param callback Callback to be executed after done recording.
	 */
	void record(void (*callback)(void)); // 16bit, monoral, 16000Hz,  linear PCM
	void loop();

private:
	//playback
	I2S* i2s;
	AudioGeneratorWAV *wav = nullptr;
	AudioGeneratorMP3 *mp3;
	AudioOutputI2S *out;
	AudioFileSource *file;
	//recording
	void CreateWavHeader(byte* header, int waveDataSize);
	void (*recordCallback)(void) = nullptr;
};

extern Audio audio;

#endif
