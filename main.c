#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define TrieNode structure
struct TrieNode {
    struct TrieNode *children[26];
    int is_end_of_word;
};

// Define Trie structure
struct Trie {
    struct TrieNode *root;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *node = pTrie->root;
    int n = strlen(word);

    for (int i = 0; i < n; i++) {
        int index = word[i] - 'a';
        if (!node->children[index]) {
            node->children[index] = (struct TrieNode *)malloc(sizeof(struct TrieNode));
            memset(node->children[index], 0, sizeof(struct TrieNode));
        }
        node = node->children[index];
    }
    node->is_end_of_word = 1;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct TrieNode *node = pTrie->root;
    int n = strlen(word);

    for (int i = 0; i < n; i++) {
        int index = word[i] - 'a';
        if (!node->children[index]) {
            return 0;
        }
        node = node->children[index];
    }

    if (node && node->is_end_of_word) {
        return 1;
    }

    return 0;
}

// Deallocate the trie structure
void deallocateTrie(struct TrieNode *node) {
    if (!node) {
        return;
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            deallocateTrie(node->children[i]);
        }
    }

    free(node);
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    memset(pTrie->root, 0, sizeof(struct TrieNode));

    return pTrie;
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    int numWords = 0;
    size_t bufsize = 64;
    char *buffer = (char *)malloc(bufsize * sizeof(char));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (getline(&buffer, &bufsize, fp) != -1) {
        // Remove newline character from the end of buffer
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        strcpy(pInWords[numWords], buffer);
        numWords++;
    }

    fclose(fp);
    free(buffer);

    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // Parse line-by-line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie->root);
    free(pTrie);

    return 0;
}