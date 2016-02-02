//
//  main.c
//  TriePrediction
//
//  Created by Neal Nguyen on 11/21/15.
//  Copyright © 2015 Neal Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"
TrieNode *createTrieNode(void);
TrieNode *insertString(char* buffer,TrieNode *root,int len);
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
    int i;
    
    if (root == NULL)
        return;
    
    // If we encounter the terminal node of an actual word that is represented
    // in the trie, we'll print it out, along with its frequency of occurrence.
    if (root->count > 0)
        printf("%s (%d)\n", buffer, root->count);
    
    // We'll be writing a character to buffer[k], so make sure the string is
    // terminated immediately thereafter.
    buffer[k+1] = '\0';
    
    // Now take each path, noe by one, to the child nodes of the current node.
    // This might lead us to a NULL pointer, in which case the base case will
    // cause us to return immediately from the next recursive call.
    for (i = 0; i < 26; i++)
    {
        // Write the corresponding character to the buffer. (0 is 'a', 1 is 'b',
        // and so on.)
        buffer[k] = 'a' + i;
        
        // Perform the recursive call.
        printTrieHelper(root->children[i], buffer, k + 1);
    }
    
    // We're finished writing to buffer[k]. Clean up by placing a null sentinel
    // in that position before returning to the previous call.
    buffer[k] = '\0';
}

// Wrapper function to the recursive printTrieHelper() function. Takes the root
// of the trie and kicks off the process of printing all the words it contains.
void printTrie(TrieNode *root)
{
    // Create a buffer where we can build up strings as we traverse the trie.
    // This assumes no word is greater than length 1023 (with an extra character
    // to hold the null sentinel).
    char buffer[1024];
    // We might initialize that first index to '\0', just in case the empty
    // string is in the trie and we want to print the buffer right away.
    buffer[0] = '\0';

    // No make the initial call.
    printTrieHelper(root, buffer, 0);

    // NOTE:
    // The 0 in that function call can be interpreted a few different ways,
    // depending how you view the problem (which is often the case with
    // parameters to recursive functions that change with each call):
    // - It's the index where we want to write the next character in buffer.
    // - It's the number of characters we have already written/processed so far.
}
TrieNode *createTrieNode(void)
{
    int i;
    TrieNode *root = malloc(sizeof(TrieNode));
    root->count = 0;
    root->subtrie = NULL;
    for (i = 0; i<26; i++) {
        root->children[i] = NULL;
    }
    return root;
}
TrieNode *insertString(char* buffer,TrieNode *root,int len)
{
    int index;
    if(root == NULL)
        root = createTrieNode();
    if(len == 0)
        (root->count)++;
    if(*buffer == '\0')
    return root ;

    if(*buffer > 64 && *buffer < 91)
    {
        index = *buffer - 'A';
        root->children[index] = insertString(buffer+1,root->children[index],len-1);
    }
    
    else if(*buffer > 96 && *buffer < 123 )
    {
        index =*buffer - 'a';
        root->children[index] = insertString(buffer+1,root->children[index],len-1);
    }
    else
        root =insertString(buffer+1,root,len-1);

    
    return root;
}
TrieNode *buildTrie(char *filename){
    TrieNode *root = NULL;
    FILE *fp = NULL;
    int len;
    char buffer[1024] = "\0";
    if((fp = fopen(filename,"r")) == NULL)
       {
           printf("%s",filename);
           printf("INVALID FILE NAME\n ");
           exit(1);
       }
    while (fscanf(fp,"%s",buffer) != EOF)
    {
        len =(int)strlen(buffer);

            root = insertString(buffer,root,len);

        
    }
    return root;
}
int main(int argc,char * argv[]) {
    TrieNode *root = NULL;
    //printf("%s\n",argv[1]);
    root = buildTrie(argv[1]);
    printTrie(root);
    return 0;
}
