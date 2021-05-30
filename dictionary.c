// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

//how many words were loaded to dictionary
unsigned int dictionary_size = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        dictionary_size ++;
        //Get the word's position on a zero indexed abecedary
        int position = hash(word);

        //memory for new word
        node *newword = malloc(sizeof(node));
        memset(newword->word, '\0', sizeof(word));


        //insert words into hashtable
        if (hashtable[position])
        {
            //pass word from buffer to node
            for (int i = 0; word[i]; i++)
            {
                newword->word[i] = word[i];
            }

            //insert node in the linked list
            newword->next = hashtable[position];
            hashtable[position] = newword;

        }
        else
        {
            hashtable[position] = newword;

            //pass word from buffer to node
            for (int i = 0; word[i]; i++)
            {
                newword->word[i] = word[i];
            }

            newword->next = NULL;
        }

    }


    printf("Loaded %u words\n", dictionary_size);
    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //memory for copy of the passed word
    char tmpword[LENGTH + 1] = {'\0'};

    //copies the passed word
    for (int i = 0; word[i]; i++)
    {
        tmpword[i] = word[i];
    }

    //convert word to lower case
    for (int i = 0; tmpword[i]; i++)
    {
        tmpword[i] = tolower(tmpword[i]);
    }

    //get the word's position on a zero indexed abecedary
    int position = hash(tmpword);

    //pointer to the word's position on the abecedary
    node *ptr = hashtable[position];


    while (true)
    {
        //iterates through all characters
        for (int i = 0; i <= LENGTH; i++)
        {

            //compares characters one by one
            if (ptr->word[i] != tmpword[i])
            {
                break;
            }

            //if all characters in the array match, return true.
            if (i == LENGTH)
            {
                return true;
            }

        }

        if (!ptr->next)
        {
            //could not find a matching word in the dictionary
            return false;
        }

        ptr = ptr->next;
    }


}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{


    for (int i = 0; i < N; i++)
    {

        node *ptr = hashtable[i];

        while (ptr)
        {
            node *temp = ptr;
            ptr = ptr->next;
            free(temp);
        }
    }
    return true;
}
