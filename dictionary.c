// Implements a dictionary's functionality
#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

unsigned int countSize = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get hash index, make pointer to hash index
    int hashIndex = hash(word);
    node *n = table[hashIndex];

    // Compare strings
    while (n != NULL)
    {
        if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number
// Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = toupper(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    // Open dictionary file
    FILE *readPtr = fopen(dictionary, "r");

    // Ensure pointer is not NULL
    if (readPtr == NULL)
    {
        return false;
    }

    // Define buffer
    char thisWord[LENGTH + 1];

    // While reading, create node for each word, and count
    while (fscanf(readPtr, "%s", thisWord) != EOF)
    {
        // Make space for node in memory
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Copy current word into node
        strcpy(n->word, thisWord);

        // Get hash index save as int
        int hashIndex = hash(thisWord);
        // Assign pointer to hash index
        n->next = table[hashIndex];
        // Head is asigned to new pointer
        table[hashIndex] = n;
        // Add another word to counter
        countSize++;
    }
    fclose(readPtr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (countSize > 0)
    {
        return countSize;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop through hash table then free mem
    for (int i = 0; i < N; i++)
    {
        // Pointer to follow index
        node *n = table[i];

        // Free nodes memory one by one
        while (n != NULL)
        {
            node *tmp = n;
            n = n->next;
            free(tmp);
        }
        if (n == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}