#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie
{	
    struct TrieNode *root;
};

// Creates a new Trie node and returns a pointer to it
struct TrieNode *newNode()
{
    struct TrieNode *pNode = NULL;
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (pNode)
    {
        pNode->count = 0;
        for (int i=0; i<ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
    return pNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    int len = strlen(word);
    struct TrieNode *pCrawl = pTrie->root;
    for (int i=0; i<len; i++)
    {
        int index = word[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = newNode();
        pCrawl = pCrawl->children[index];
    }
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    int len = strlen(word);
    struct TrieNode *pCrawl = pTrie->root;
    for (int i=0; i<len; i++)
    {
        int index = word[i] - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }
    return pCrawl->count;
}

// Recursive function to deallocate memory of the Trie nodes
void deallocateNodes(struct TrieNode *node)
{
    for (int i=0; i<ALPHABET_SIZE; i++)
    {
        if (node->children[i])
            deallocateNodes(node->children[i]);
    }
    free(node);
}

// Deallocates the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        deallocateNodes(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = NULL;
    pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie)
        pTrie->root = newNode();
    return pTrie;
}

// Reads the dictionary file and stores all the words
// Also returns the number of words read
int readDictionary(char *filename, char **pInWords)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening the dictionary file\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        line[strcspn(line, "\r\n")] = 0; // Removing trailing newline character
        pInWords[count] = (char*)malloc((strlen(line)+1) * sizeof(char));
        strcpy(pInWords[count], line);
        count++;
    }

    fclose(fp);
    if (line)
        free(line);

    return count;
}

int main(void)
{
    char *inWords[256];
    struct Trie *pTrie;
    int numWords;

    // Read the number of the words in the dictionary
    numWords = readDictionary("dictionary.txt", inWords);
    printf("Words in the dictionary:\n");
    for (int i=0; i<numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // Create the Trie data structure and insert words from dictionary
    pTrie = createTrie();
    for (int i=0; i<numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test the implementation
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    printf("\nNumber of occurrences of each word:\n");
    for (int i=0; i<5; i++)
    {
        printf("%s: %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the Trie data structure
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    // Deallocate the words array
    for (int i=0; i<numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}
