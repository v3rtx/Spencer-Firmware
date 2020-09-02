#include "Audio.h"
#include <SerialFlash.h>
Audio::Audio()
{
}

Audio::~Audio()
{
	delete i2s;
}
void Audio::begin()
{
	i2s = new I2S();
}
void Audio::record(void (*callback)(byte*, size_t))
{
	wavData = new char*[wavDataSize/dividedWavDataSize + 1];
	wavData[0] = new char[headerSize];
	for (int i = 1; i < wavDataSize/dividedWavDataSize + 1; ++i){
		wavData[i] = new char[dividedWavDataSize];
	}
	recordCallback = callback;
	CreateWavHeader((byte*)(wavData[0]), wavDataSize);
	
	for (uint32_t j = 1; j < wavDataSize/dividedWavDataSize + 1; j++) {
		i2s->Read(i2sBuffer, i2sBufferSize);
		for (int i = 0; i < i2sBufferSize/8; i++) {
			wavData[j][2*i] = i2sBuffer[4*i + 2];
			wavData[j][2*i + 1] = i2sBuffer[4*i + 3];
		}
	}
	if(SerialFlash.exists("recording.wav")){
		Serial.println("removing recording.wav...");
		if(SerialFlash.remove("recording.wav"))
		{
			Serial.println("removed file");
		}
		else{
			Serial.println("failed to remove file");
		}
	}
	SerialFlash.create("recording.wav", 70000);
	SerialFlashFile f = SerialFlash.open("recording.wav");
	Serial.println((bool)f ? "file open" : "file error");

	for(uint8_t i = 0; i < wavDataSize/dividedWavDataSize + 1; i++)
	{
		f.write(wavData[i], sizeof(wavData[i]));
	}
	f.close();
}
void Audio::CreateWavHeader(byte* header, int waveDataSize){
	header[0] = 'R';
	header[1] = 'I';
	header[2] = 'F';
	header[3] = 'F';
	unsigned int fileSizeMinus8 = waveDataSize + 44 - 8;
	header[4] = (byte)(fileSizeMinus8 & 0xFF);
	header[5] = (byte)((fileSizeMinus8 >> 8) & 0xFF);
	header[6] = (byte)((fileSizeMinus8 >> 16) & 0xFF);
	header[7] = (byte)((fileSizeMinus8 >> 24) & 0xFF);
	header[8] = 'W';
	header[9] = 'A';
	header[10] = 'V';
	header[11] = 'E';
	header[12] = 'f';
	header[13] = 'm';
	header[14] = 't';
	header[15] = ' ';
	header[16] = 0x10;  // linear PCM
	header[17] = 0x00;
	header[18] = 0x00;
	header[19] = 0x00;
	header[20] = 0x01;  // linear PCM
	header[21] = 0x00;
	header[22] = 0x01;  // monoral
	header[23] = 0x00;
	header[24] = 0x80;  // sampling rate 16000
	header[25] = 0x3E;
	header[26] = 0x00;
	header[27] = 0x00;
	header[28] = 0x00;  // Byte/sec = 16000x2x1 = 32000
	header[29] = 0x7D;
	header[30] = 0x00;
	header[31] = 0x00;
	header[32] = 0x02;  // 16bit monoral
	header[33] = 0x00;
	header[34] = 0x10;  // 16bit
	header[35] = 0x00;
	header[36] = 'd';
	header[37] = 'a';
	header[38] = 't';
	header[39] = 'a';
	header[40] = (byte)(waveDataSize & 0xFF);
	header[41] = (byte)((waveDataSize >> 8) & 0xFF);
	header[42] = (byte)((waveDataSize >> 16) & 0xFF);
	header[43] = (byte)((waveDataSize >> 24) & 0xFF);
}
