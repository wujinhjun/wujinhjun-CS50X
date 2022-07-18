// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// void transUpToLow(const char *word, char *newWord, int length);
void freeNode(node *n);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

int sizeTotal = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // int length = strlen(word);
    // char newWord[length + 1];
    // strcpy(newWord, word);
    // transUpToLow(word, newWord, length);
    int index = hash(word);
    node *n = table[index];

    while (n != NULL)
    {
        if (strcasecmp(n->word, word) != 0)
        {
            n = n->next;
        }
        else
        {
            return true;
        }
    }

    return false;
}

// void transUpToLow(const char *wordOld, char *newWord, int length)
// {
//     string word;
//     strcpy(word, wordOld);
//     for (int i = 0; i < length; i++)
//     {
//         newWord[i] = word[i].tolower();
//     }

//     return;
// }

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *filePtr = fopen(dictionary, "r");
    if (filePtr == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(filePtr, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);

        int index = hash(word);
        n->next = table[index];
        table[index] = n;

        sizeTotal++;
    }

    fclose(filePtr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return sizeTotal;
}

void freeNode(node *n)
{
    if (n->next != NULL)
    {
        freeNode(n->next);
    }
    free(n);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            freeNode(table[i]);
        }

        if (table[i] == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
