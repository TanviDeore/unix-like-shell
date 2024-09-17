/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <string.h>

//Read input from user
ssize_t read;
size_t len = 0;
char* inputStr= NULL;
int is_interactive=0;
int is_batchmode=0;


void readInputFromUser(char*inputStr, size_t length){
    const char* delimiter = " ";    
    char *token, *saveptr;
    
    read = getline(&inputStr,&len,stdin);
    if(read == -1)
        return;     //need to add an exception
    
    inputStr[strcspn(inputStr,"\n")]='\0';
    
    token = strtok_r(inputStr,delimiter,&saveptr);
    
   
    
    while(token!=NULL){
        returnTokenFunction(token);
        token = strtok_r(NULL,delimiter,&saveptr);
    }
    
       return;
}

void returnTokenFunction(char* token){
    if(!is_batchmode && strcasecmp(token,"y")==0){
        is_batchmode = 1; // will need to change as if user inputs ./file -> it should enter batch mode
        printf("Entered batch mode\n");}
    if(!is_interactive && strcasecmp(token,"n")==0){
        is_interactive = 1;
        printf("Entered interactive mode\n");
        int x = interactiveMode();
    }
    else{printf("USe this token: %s\n",token);}
}

int interactiveMode(){
    printf("dash> ");
    readInputFromUser(inputStr,len);
    // if(inputStr==NULL){printf("returned string is NULL");}
    // printf("%s",inputStr);
    return 0;
}

int main()
{
    printf("Do you want to enter batch mode y/n ?\n");
    readInputFromUser(inputStr,len);
    
    
    return 0;
}
