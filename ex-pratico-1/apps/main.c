#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcoesFornecidas.h"
#include "my-lib.h"


int main(){
    int menu;
    char* input = (char*)calloc(65, sizeof(char));
    char* address = (char*)calloc(20, sizeof(char));
    char* tmp;
    readline(input);
    tmp = strtok(input, " ");
    menu = atoi(tmp);
    tmp = strtok(NULL, " ");
    strcpy(address, tmp);
    tmp = strstr(address, ".bin");
    tmp[4] = '\0';

    switch(menu){
        default:{
            printf("Falha no processamento do arquivo");
        }break;
        case 1:{
            readline(input);
            int numOfEntrys = atoi(input);
            data_insertion(address,numOfEntrys);
            binarioNaTela(address);
        }break;
        case 2:{
            print_all_register(address);
        }break;
        case 3:{
            int rrn;
            int flag = 0;
            entry rrn_data;
            readline(input);
            rrn = atoi(input);
            rrn_data = get_rrn_data(rrn, address, &flag);
            if(flag)
                return 0;
            print_entry(rrn_data);
        }break;
        case 4:{
            update_data(address);
            binarioNaTela(address);
        }break;
    }
    free(input);
    free(address);
    return 0;
}