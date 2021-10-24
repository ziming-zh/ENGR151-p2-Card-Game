//
// Created by Ziming on 2021/10/16.
//
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"


FILE* create_log(char* file_name,FILE* log){
    log=fopen(file_name,"w");
    if(log==NULL){
        fprintf(stderr,"Error#001:Unable to open file %s",file_name);
        exit(EXIT_FAILURE);
    }
    /// add initial message
    fprintf(log,"########################\n"
                "# #\n"
                "# Welcome to One Card! #\n"
                "# #\n"
                "########################\n");

    return log;
}

void close_log(FILE *log)
{
    //check if everything goes well
    fclose(log);
}
void read_log(char* file_name)
{
    FILE* log;
    char line[MAX_LINE_NUM];
    int i;
    char o;
    log=fopen(file_name,"r");
    fgets(line,MAX_LINE_NUM,log);
    fgets(line,MAX_LINE_NUM,log);
    fgets(line,MAX_LINE_NUM,log);

    if(log==NULL){
        fprintf(stderr,"Error#001:Unable to open file %s",file_name);
        exit(EXIT_FAILURE);
    }
    while(fgets(line,MAX_LINE_NUM,log)!=NULL)
    {
        for(i=0;i<strlen(line);i++)
        {
            if(line[i]=='#'){ ask_command(log); break;}
            printf("%c",line[i]);
        }
        printf("\n");
    }
    printf("Now Start the game or not <y/n>:");
    scanf("%c",&o);
    if(o!='y')  exit(EXIT_SUCCESS);
}