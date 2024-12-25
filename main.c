#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

#include "file.h"
#include "list.h"

List(char, string)
List(char*, strList)

List(uint8_t, u8List)
List(void*, anyList)

typedef struct tokenTree
{
	char* name;
	uint64_t length;
	u8List tokenTypes;
	anyList tokens;
} tokenTree;

strList tokenize(string list)
{
	strList tokens = strList_create();
	uint64_t count = 0;
	bool whitespace = true;
	bool slComment = false;
	bool mlComment = false;

	for (uint64_t i = 0; i < list.size; i++)
	{
		bool special = false;

		switch (list.data[i])
		{
		case '/':
			if (!slComment && !mlComment)
			{
				if (list.data[i + 1] == '/')
					slComment = true;
				else if (list.data[i + 1] == '*')
					mlComment = true;
				special = true;
			}
			break;

		case '*':
			if (list.data[i + 1] == '/' && mlComment)
			{
				mlComment = false;
				whitespace = true;
				special = true;
			}
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
			special = true;
			break;
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

static tokenTree makeTree(strList tokens)
{
	tokenTree tree = { 0 };
	tree.tokens = anyList_create();
	tree.tokenTypes = u8List_create();

	for (uint64_t i = 0; i < tokens.size; i++)
	{

	}

	return tree;
}

static string reformat(File* file)
{
	string newData = string_create();

	if (file)
	{
		for (uint64_t i = 0; i < file->size; i++)
			switch (file->data[i])
			{
			// Fix crlf by ignoring it
			case '\r':
				break;

			// Add space around special characters to make it easier later
			case ',':
			case ';':
			case '*':
			case '{':
			case '}':
			case '[':
			case ']':
				string_push(&newData, ' ');
				string_push(&newData, file->data[i]);
				string_push(&newData, ' ');
				break;

			default:
				string_push(&newData, file->data[i]);
			}

		string_push(&newData, 0);
	}

	return newData;
}


void tree_free(tokenTree* tree)
{
	for (uint64_t i = 0; i < tree->length; i++)
	{
		if (tree->tokenTypes.data[i] == 1)
			tree_free(tree->tokens.data[i]);
	}
	basicList_free(&tree->tokenTypes);
	basicList_free(&tree->tokens);
	if (tree->name)
		free(tree->name);

	tree->length = 0;
	tree->name = NULL;
}

void strList_free(strList* list)
{
	for (uint64_t i = 0; i < list->size; i++)
#pragma warning(suppress: 6001) // MSVC wrongly reports this as using uninitialised variable.
		if (list->data[i])
			free(list->data[i]);
	basicList_free(list);
}


int main()
{
	File file = readFile("HIDP_VALUE_CAPS.txt");

	string list = reformat(&file);
	freeFile(&file);

	strList tokens = tokenize(list);
	basicList_free(&list);

	tokenTree tree = makeTree(tokens);
	strList_free(&tokens);

	tree_free(&tree);

	return 0;
}