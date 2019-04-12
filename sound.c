#include <stdio.h>
#include <math.h>
#include "sound.h"
#include "screen.h"

int findPeaks(int d[])
{				// this function gets in an array of decibel values and finds out the number of peaks in
	int i, c = 0;		// this array
	for(i = 1; i < 80; i++)
	{
		if(d[i] >= 75 && d[i-1]<75)
		{
			c++;
		}
	}
	if(d[0] >= 75)
	{
		c++;
	}
	return c;
}
// this function takes 1 second of samples (16000 in our case)
// and calculate 80 pieces of RMS value and then
// turn these valyes into decibels, and display them as
// a barchart
void displayWAVDATA(short s[])
{
	double rms[80];		// because we have 16000 samples, 16000 / 80 = 200
				// so every 200 samples makes one RMS
	int db[80];
	int i, j, k = 0;	// nested loop counters
	short *ptr = s;		// use pointers to point to the beginning of the samples

	for(i = 0; i < 80; i++)
	{
		double sum = 0;
		for(j = 0; j < 200; j++)
		{
		 	sum += (*ptr) * (*ptr);		// accumulate the sum
			ptr++;				// pointer increments
		}
		rms[i] = sqrt(sum/200);
		db[i] = 20*log10(rms[i]);		// decibel value

#ifdef DEBUG						// conditional compilation
		printf("RMS[%d] = %f\n", i, rms[i]);
#endif
	}	//end of for loop
#ifndef DEBUG
	barChart(db);
	int peaks = findPeaks(db);	// get the number of peaks
	setColors(WHITE, bg(BLACK));	// set the colors
	printf("\033[1;41H");		// go to row 1, col 41
	printf("Peaks: %d           \n", peaks);
#endif
}
void showID(char *name, char *value)
{
	printf("%s: ", name);
	for(int i = 0; i < 4; i++)
	{
		printf("%c", value[i]);
	}
	puts("");
}
// function definition
void displayWAVHDR(struct WAVHDR h)
{
#ifdef DEBUG
	showID("Chunk ID", h.ChunkID);
	printf("Chunk size: %d\n", h.ChunkSize);
	showID("Format", h.Format);
	showID("Subchunk1 ID", h.Subchunk1ID);
	printf("Subchunk1 size: %d\n", h.Subchunk1Size);
	printf("Audio format: %d\n", h.AudioFormat);
	printf("Num of Channels: %d\n", h.NumChannels);
	printf("Sample rate: %d\n", h.SampleRate);
	printf("Byte rate: %d\n", h.ByteRate);
	printf("Block align: %d\n", h.BlockAlign);
	printf("Bits per sample: %d\n", h.BitsPerSample);
	showID("Subchunk 2 ID: ", h.Subchunk2ID);
	printf("Subchunk 2 size: %d\n", h.Subchunk2Size);
#else
	setColors(RED, bg(BLUE));
	printf("\033[1;H");
	printf("test.wav            \n");
	setColors(YELLOW, bg(GREEN));
	printf("\033[1;21H");
	printf("Sample rate=%d      \n", h.SampleRate);
	setColors(WHITE, bg(MAGENTA));
	printf("\033[1;61H");
	printf("Duration=%.2f       \n", (float)h.Subchunk2Size/h.ByteRate);
#endif
}

void fillID(char *dst, const char *m)
{
	for(int i = 0; i < 4; i++)
	{
		*dst++ = *m++;
	}
}

void testTone(int c, int f, float d)
{
	if(f < 30 || f > 16000)
	{
		printf("Frequency is out of range.\n");
		return;
	}
	if(c < 1 || c > 2)
	{
		printf("Number of channels is not okay.\n");
		return;
	}
	if(d < 1 || d > 10)
	{
		printf("Duration is not okay.\n");
		return;
	}
	struct WAVHDR h;		// we need to prepare a WAV header
	fillID(h.ChunkID, "RIFF");
	fillID(h.Format, "WAVE");
	fillID(h.Subchunk1ID, "fmt ");
	fillID(h.Subchunk2ID, "data");
	h.Subchunk1Size = 16;		// for PCM
	h.AudioFormat = 1;
	h.NumChannels = c;
	h.SampleRate = 44100;
	h.BitsPerSample = 16;

	if(c == 1)			// for mono channel
	{
		h.ByteRate = h.SampleRate * c * h.BitsPerSample;
		h.BlockAlign = c * h.BitsPerSample / 16;
		h.Subchunk2Size = d * h.SampleRate * h.BlockAlign;
		h.ChunkSize = h.Subchunk2Size + 36;
	}

	// prepare sound data
	short data[441000];		//[d*h.SampleRate];
	for(int i = 0; i < d*h.SampleRate; i++)
	{
		data[i] = 32768 * sin(2*PI*i/44100);
	}

	FILE *fp = fopen("testTone.wav", "w");
	if(fp == NULL)
	{
		printf("We cannot open the file\n");
		return;
	}
	fwrite(&h, sizeof(h), 1, fp);	// write the header
	fwrite(data, d*h.SampleRate*sizeof(short), 1, fp);
	fclose(fp);
	printf("Test tone is generated!\n");
}
