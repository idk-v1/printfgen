#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct Line
{
	char* str;
	uint64_t length;
} Line;

typedef struct TextFile
{
	Line* lines;
	uint64_t lineCount;
} TextFile;

typedef struct File
{
	uint8_t* data;
	uint64_t size;
} File;

static File readFile(const char* name)
{
	File f = { 0 };

	FILE* file = fopen(name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		f.size = ftell(file);
		fseek(file, 0, SEEK_SET);

		f.data = (uint8_t*)malloc(f.size);
		if (f.data)
			fread(f.data, 1, f.size, file);
		else
			f.size = 0;

		fclose(file);
	}

	return f;
}

static void freeFile(File* file)
{
	if (file)
	{
		free(file->data);
		file->data = NULL;
		file->size = 0;
	}
}