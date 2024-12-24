#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;

typedef struct File
{
	byte* data;
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

		f.data = (byte*)malloc(f.size);
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