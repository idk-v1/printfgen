#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

#include "file.h"
#include "list.h"
List(char, charList)
List(char*, strList)

strList tokenize(charList list)
{
	strList tokens = strList_create();

	uint64_t count = 0;

	bool whitespace = true;
	bool slComment = false;
	bool mlComment = false;
	bool special = false;

	for (uint64_t i = 0; i < list.size; i++)
	{
		special = true;

		switch (list.data[i])
		{
		case '/':
			if (slComment || mlComment)
				special = false;
			else
			{
				if (list.data[i + 1] == '/')
					slComment = true;
				else if (list.data[i + 1] == '*')
					mlComment = true;
			}
			break;

		case '*':
			if (list.data[i + 1] == '/' && mlComment)
			{
				mlComment = false;
				whitespace = true;
			}
			else
				special = false;
			break;

		case '{':
		case '}':
			special = false;
			break;

		case '\n':
			slComment = false;
		case ' ':
		case '\t':
			if (!whitespace) // save token
			{
				char* token = malloc(count + 1);
				if (token)
				{
					memcpy(token, list.data + i - count, count);
					token[count] = '\0';
				}
				strList_push(&tokens, token);
			}
			whitespace = true;
			break;

		default:
			special = false;
		}

		if (!special && !(slComment || mlComment))
		{
			if (whitespace) // start token
				count = 0;
			count++;
			whitespace = false;
		}
	}

	return tokens;
}

static void parseTokens(strList tokens)
{
	uint32_t depth = 0;
	bool nameAtEnd = false;

	char* type = NULL;
	char* name = NULL;

	// Loop

	// Check if next token is "typedef"
	// If so, get name from end
	// Then check if token is "struct" or "union"
	// If so and not typedef'd, get next token as name
	//
}

static charList normalizeFile(File* file)
{
	charList newData = charList_create();

	if (file)
	{
		for (uint64_t i = 0; i < file->size; i++)
			switch (file->data[i])
			{
			case '\r':
				break;

			case ',':
			case ';':
			case '*':
			case '{':
			case '}':
			case '[':
			case ']':
				charList_push(&newData, ' ');
				charList_push(&newData, file->data[i]);
				charList_push(&newData, ' ');
				break;

			default:
				charList_push(&newData, file->data[i]);
			}

		charList_push(&newData, 0);
	}

	return newData;
}


int main()
{
	File file = readFile("HIDP_VALUE_CAPS.txt");
	charList list = normalizeFile(&file);
	freeFile(&file);

	strList tokens = tokenize(list);
	charList_free(&list);

	for (uint64_t i = 0; i < tokens.size; i++)
		printf("%s\n", tokens.data[i]);

	parseTokens(tokens);


	for (uint64_t i = 0; i < tokens.size; i++)
		if (tokens.data[i])
			free(tokens.data[i]);
	strList_free(&tokens);

	return 0;
}