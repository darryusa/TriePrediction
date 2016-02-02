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


// prototype
TrieNode *createTrieNode(void);
TrieNode *insertString(char* buffer,TrieNode *root,int len,char *next);
TrieNode *insertSubTree(char* buffer, TrieNode* root,int len);
int printTrieHelper(TrieNode *root, char *buffer, int k,char* key,int count,int* n);
void printSubTrieHelper(TrieNode *root, char *buffer, int k);
void printTrie(TrieNode *root,char* key,int n);
void printSubTrie(TrieNode *root);
void textPredictionHelper(TrieNode *root, char *buffer, int k,char** test,char* key,int sub,int* count);
void textPrediction(TrieNode*,int,char**,char*,int*);
void preTextPrediction(TrieNode* root,int n,char* key);
void inputHandling(char* inputFile, TrieNode* root);
void destroy(TrieNode *root);


// modified from Dr.Szumlanski printTrieHelper on webcourse
int printTrieHelper(TrieNode *root, char *buffer, int k,char* key,int count,int* n)
{
    int i;
    //base case
    if (root == NULL)
        return count;
    

    if (root->count > 0) // found a word
    {

        if(key == NULL) // print out entire trie
        {
            count = 0;
            printf("%s (%d)\n", buffer, root->count);
        }
        
        else if(!strcmp(key,buffer)) // compare key with words in trie
        {

            if(root->subtrie == NULL) // if NULL update count to 1  inorder to print out empty
                count = 1;
            
            else
            {
                printSubTrie(root->subtrie); //print out sub tree
                count = 0;
            }
        }
        else if(!strcmp(key, buffer)) // if not found update n to print out invalid word
        {
            *n = -1;
        }
    }
    
       buffer[k+1] = '\0';
      for (i = 0; i < 26; i++) //traverse through 26 branch
    {
        buffer[k] = 'a' + i;
        count = printTrieHelper(root->children[i], buffer, k + 1,key,count,n); //recursive call
    }
       buffer[k] = '\0'; // set buffer back to normal
    return count;
}

// copied and modified from Dr. Szulanski printTrie function on webcourse
void printTrie(TrieNode *root,char* key,int n)
{
    int i = 0;
    char buffer[1024];
    buffer[0] = '\0';

    if((i = printTrieHelper(root, buffer, 0,key,-1,&n)) > 0) // if i > 0 then tree is empty
        printf("(EMPTY)\n");
    
    else if( i < 0 && n != 0) // if i is unchanged == -1 and n is -1 then the string is invalid
    {
        printf("(INVALID STRING)\n");
    }


}
//copied and modified from Dr Szulanski
void printSubTrieHelper(TrieNode *root, char *buffer, int k)
{
    int i;
    
    if (root == NULL) // base case
        return;
      if (root->count > 0)
    {
        printf("- %s (%d)\n", buffer, root->count); // print subtree
    }
    
      buffer[k+1] = '\0';
       for (i = 0; i < 26; i++) // traverse all branches
    {
        buffer[k] = 'a' + i;
        printSubTrieHelper(root->children[i], buffer, k + 1); // recursive call
    }
       buffer[k] = '\0'; // reinitialize buffer back
    
}

void printSubTrie(TrieNode *root)
{
    char buffer[1024];
   
    buffer[0] = '\0';
    
    printSubTrieHelper(root, buffer, 0);
    
}

TrieNode *createTrieNode(void) // create tree node
{
    //initialize components
    int i;
    TrieNode *root = malloc(sizeof(TrieNode));
    root->count = 0; // set cound to 0
    root->subtrie = NULL; // initialize subtree node to NULL
    for (i = 0; i<26; i++) {
        root->children[i] = NULL; // initialize children node to NULL
    }
    return root;
}
TrieNode *insertString(char* buffer,TrieNode *root,int len,char *next) // insert string
{
    int index;
    int lenNext;
    
    if(root == NULL) // base case
        root = createTrieNode();
    if(len == 0) // when reached the end of len
    {
        (root->count)++; // increasement count
        if (next != NULL && !(*buffer == '.' || *buffer == '!' || *buffer == '?')) // check for next word to be not NULL or not ending in . ! or ?
        {
            lenNext = (int)strlen(next); // get lenth of next word
            root->subtrie = insertString(next, root->subtrie,lenNext,NULL); // insert next work to subtrie
        }
            buffer  = buffer+1; //move buffer forward to skip over the punctuation
        }
    
    if(*buffer == '\0') // check if end of buffer then return root
    return root ;

    if(*buffer > 64 && *buffer < 91) // check for uppercase
    {
        index = *buffer - 'A';
        root->children[index] = insertString(buffer+1,root->children[index],len-1,next);
        
    }
    
    else if(*buffer > 96 && *buffer < 123 ) // check for lower case
    {
        index =*buffer - 'a';
        root->children[index] = insertString(buffer+1,root->children[index],len-1,next);
    }
    else if(*buffer == '.' || *buffer == '!' || *buffer == '?') // check if the punctuation is . ! or ? then call the fuction recursively
        root =insertString(buffer,root,len-1,next);
    else // if normal punctuation then skip over
        root =insertString(buffer+1,root,len-1,next);
    
    return root;
}

TrieNode *buildTrie(char *filename)
{
    TrieNode *root = NULL; //initialize root
    FILE *fp = NULL; // initialize file pointer
    int len,i = 0,numWord = 0;
    char **current,buffer[1024] = "\0";
    if((fp = fopen(filename,"r")) == NULL) // check make sure file is open correctly
       {
           printf("INVALID FILE NAME\n ");
           exit(1); // exit program if not open correctly
       }
    while(fscanf(fp,"%s",buffer) != EOF) // loop through file to get number of words
    {
        numWord++;
    }
    
    current = calloc(numWord,sizeof(char*)); // malloc 2D array to hold words
                     
    for (i = 0 ;i <numWord; i++)
    {
        current[i] = calloc(1024,sizeof(char));
    }
    
    rewind(fp); // rewind file pointer
    i = 0; // initialize i back to 0
    
    while(fscanf(fp,"%s",buffer) != EOF) // copy words from file back to pointers
    {
        strcpy(current[i++], buffer);
    }
    
    for(i = 0; i <numWord;i ++) //loop through number of word to put it in trie
    {
        
        len = (int)strlen(current[i]); // get len of the current word
        if(i < numWord -1) // check to make sure its not out of bound
        root = insertString(current[i],root,len,current[i+1]); // insert current and next word as subtrie
        else
        root = insertString(current[i],root,len,NULL); // insert last word
    }
    for(i = 0; i < numWord;i++)
        free(current[i]);
    free(current);
    fclose(fp);
    return root;
}

// copied and modified from printtrie.txt
void textPredictionHelper(TrieNode *root, char *buffer, int k,char** test,char* key,int sub,int* count)
{
    int i;
    
    if (root == NULL) // base case
        return;
    
    if (root->count > 0) // found words
    {
        
        if((!strcmp(buffer, key)) || (sub)) // sub is a check point to know if its a subtree or noto
        {
            textPrediction(root->subtrie, 1,test,key,count); // call the function recursively pass 1 to signal its a subtree
            if(root->count > *count) // check and choose one with highest count
            {
                *count = root->count; // update count if it is a highest cout
                if(*buffer == '-')
                {
                    strcpy(*test, buffer+2); // copy subtree for next word
                }
            }
            
        }
        
    }
    
    buffer[k + 1] = '\0';
    
    for (i = 0; i < 26; i++)
    {
        buffer[k] = 'a' + i;
        
        textPredictionHelper(root->children[i], buffer, k + 1,test,key,sub,count);
    }
    
    buffer[k] = '\0';

}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0.
//copied and modified from printtrie.txt
void textPrediction(TrieNode *root, int useSubtrieFormatting,char** test,char *key,int* count)
{
    char buffer[1026];
    
    if (useSubtrieFormatting)
    {
        strcpy(buffer, "- ");
        textPredictionHelper(root, buffer, 2,test,key,1,count);
    }
    else
    {
        strcpy(buffer, "");
        textPredictionHelper(root, buffer, 0,test,key,0,count);
    }

}

void preTextPrediction(TrieNode* root,int n,char* key)
{

    int i,count;
    char* stringHolder=NULL; // temp value to hold the word predicted to predict the next word
    stringHolder = calloc(1024,sizeof(char)); // allocate word
    for(i = 0; i < n;i++) // loop through number of prediction specify
    {
        count = 0;
        textPrediction(root, 0,&stringHolder,key,&count);
        if(!strcmp(stringHolder,"\0")) // if string is empty skip
            continue;
        printf(" %s",stringHolder); // print out string
        strcpy(key,stringHolder); // copy from stringholder to key
        memset(stringHolder, '\0', 1024);// reset stringHolder back to 0
    }
    printf("\n");
    free(stringHolder);
}



void inputHandling(char* inputFile, TrieNode* root)
{
    FILE * fp = NULL; // declare file pointer
    int i,j,len,num = 0;
    int temp;
    char buffer[1024]= "\0";
    char **command =NULL; // 2D array to hold every input
    if((fp= fopen(inputFile,"r")) == NULL) // check if file open correctly
    {
        printf("FATAL ERROR\n");
        exit(1);
    }
    while(fscanf(fp,"%s",buffer) != EOF) // get number of word to malloc for 2D array
    {
        num++;
    }
    rewind(fp);
    command = malloc(num * sizeof(char*)); // malloc 2D array to store words
    
    for (i = 0; i < num; i++)
    {
        command[i] = calloc(1024,sizeof(char));
    }
    i = 0;
    while (fscanf(fp,"%s", buffer) != EOF)
    {
        strcpy(command[i++], buffer); // move words from file to 2D array
    }
    
    for( i = 0;i < num; i++)
    {

        if(!strcmp(command[i], "!")) // if key is ! then print out tree
            printTrie(root,NULL,0);
        
        else if(! strcmp(command[i], "@")) // if key is @
        {
            temp = atoi(command[i+2]); // convert string of number to number of prediction
            {
                printf("%s",command[i+1]);  // print out the first word without modification
                len = (int)strlen(command[i+1]); // get the length of the word
                for(j = 0;j <len;j++) // convert it to lower case if needed
                {
                    if(command[i+1][j] < 91 && command[i+1][j]>64)
                        command[i+1][j]+=32;
                }
                
                preTextPrediction(root,temp,command[i+1]); // call Preprediction fuction
            }
            i = i+2; // increase i accordingly
        }
        
        else
        {
            printf("%s\n",command[i]); // check for word and print what come after
            len = (int)strlen(command[i]); // get length of word
            for(j = 0;j <len;j++) // conver it back to lower if needed
            {
                if(command[i][j] < 91 && command[i][j]>64)
                    command[i][j]+=32;
            }
            printTrie(root, command[i],1); // call printtrie function
        }
        
    }
    
    for( i = 0; i < num;i++)
        free(command[i]);
    
    free(command);
    fclose(fp);
}

void destroy(TrieNode *root)
{
    int i;
    
    if (root == NULL)
        return;
    
    if (root->count > 0) // if there is a count greater than 0 go in and destroy subtree
        destroy(root->subtrie);
    
    
    for (i = 0; i < 26; i++)
    {
        
        destroy(root->children[i]);
    }
    
    free(root);
}



int main(int argc,char * argv[])
{
    TrieNode *root = NULL;
    root = buildTrie(argv[1]); // put words in tree
    inputHandling(argv[2],root); // handle input file
    destroy(root); // destroy tree
    root = NULL;
    return 0;
}
