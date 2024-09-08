#include "my-lib.h"
#include "funcoesFornecidas.h"
#define ENTRY_OFFSET 131

void data_insertion(char* address, int numberOfEntrys){
    FILE* file = fopen(address, "ab");
    entry* data = (entry*)calloc(1, sizeof(entry));
    if(file == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    for(int i=0;i < numberOfEntrys; i++){
        if(i == 187)
            printf("alo\n");
        //colecting entry data
         data_collect(data);
        //inserting entry to binary file
        data_write(data, file);
    }
    free(data);
    fclose(file);
}

void data_collect(entry* tmpEntry){
    char* input = (char*)calloc(20, sizeof(char));

    readline(input);
    tmpEntry->SPECIES_ID = atoi(input);

    readline(tmpEntry->NAME);
    // strcpy(tmpEntry->NAME, input);

    readline(tmpEntry->SCIENTIFIC_NAME);
    // strcpy(tmpEntry->SCIENTIFIC_NAME, input);

    readline(input);
    tmpEntry->POPULATION = atoi(input);

    readline(tmpEntry->STATUS);
    // strcpy(tmpEntry->STATUS, input);
       
    readline(input);
    tmpEntry->LOCATION[0] = atof(input);

    readline(input);
    tmpEntry->LOCATION[1] = atof(input);
    
    readline(input);
    tmpEntry->HUMAN_IMPACT = atoi(input);
    
    free(input);
}



void data_write(entry* tmpEntry, FILE* file){
    fwrite(&tmpEntry->SPECIES_ID, sizeof(int), 1, file);

    fwrite(tmpEntry->NAME, sizeof(char), strlen(tmpEntry->NAME) + 1, file);
    for(int i =0; i < (41 - strlen(tmpEntry->NAME) - 1); i++)
        fputc('$', file);

    fwrite(tmpEntry->SCIENTIFIC_NAME, sizeof(char), strlen(tmpEntry->SCIENTIFIC_NAME) + 1, file);
    for(int i =0; i < (61 - strlen(tmpEntry->SCIENTIFIC_NAME) - 1); i++)
        fputc('$', file);

    fwrite(&tmpEntry->POPULATION, sizeof(int), 1, file);

    fwrite(&tmpEntry->STATUS, sizeof(char), strlen(tmpEntry->STATUS) + 1, file);
    for(int i =0; i < (9 - strlen(tmpEntry->STATUS) - 1); i++)
        fputc('$', file);

    fwrite(&tmpEntry->LOCATION[0], sizeof(float), 1, file);

    fwrite(&tmpEntry->LOCATION[1], sizeof(float), 1, file);

    fwrite(&tmpEntry->HUMAN_IMPACT, sizeof(int), 1, file);

}

void print_all_register(char* address){
    FILE* file = fopen(address, "rb");
    if(file == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    entry data;
    while(1){
        if(eof_verification(file))
            break;
        read_from_binary(file, &data);
        print_entry(data);
    }
    fclose(file);
}

void read_from_binary(FILE* file, entry* data){
    fread(&data->SPECIES_ID, sizeof(int), 1, file);
    fread(data->NAME, sizeof(char), 41, file);
    fread(data->SCIENTIFIC_NAME, sizeof(char), 61, file);
    fread(&data->POPULATION, sizeof(int), 1, file);
    fread(data->STATUS, sizeof(char), 9, file);
    fread(&data->LOCATION[0], sizeof(float), 1, file);
    fread(&data->LOCATION[1], sizeof(float), 1, file);
    fread(&data->HUMAN_IMPACT, sizeof(int), 1, file);
}

void print_entry(entry data){
    printf("ID: %d\n", data.SPECIES_ID);
    printf("Nome: %s\n", data.NAME);
    printf("Nome Científico: %s\n", data.SCIENTIFIC_NAME);
    if(data.POPULATION == 0)
        printf("População: NULO\n");
    else
        printf("População: %d\n", data.POPULATION);
    printf("Status: %s\n", data.STATUS);
    printf("Localização: (%.2f, %.2f)\n", data.LOCATION[0], data.LOCATION[1]);
    if(data.HUMAN_IMPACT == 0)
        printf("Impacto Humano: NULO\n\n");
    else
        printf("Impacto Humano: %d\n\n", data.HUMAN_IMPACT);
}

entry get_rrn_data(int rrn, char* address, int* flag){
    FILE* file = fopen(address, "rb");
    entry data;
    if(file == NULL){
        printf("Falha no processamento do arquivo\n");
        *flag = 1;
        fclose(file);
        return data;
    }
    for(int i = 0; i < rrn; i++){
        if(eof_verification(file)){
            *flag = 1;
            printf("Espécie não encontrada\n");
            fclose(file); 
            return data;
        } //verifica fim do arquivo
        
        fseek(file, ENTRY_OFFSET, SEEK_CUR);
    }
    if(eof_verification(file)){ //verifica fim do arquivo
        *flag = 1;
        printf("Espécie não encontrada\n");
        return data;
    }
    read_from_binary(file, &data);
    fclose(file);
    return data;
}

void update_data(char* address){
    FILE* file = fopen(address, "rb+");
    char* input = (char*)calloc(20, sizeof(char));
    int id, n, flag = 0;
    if(file == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    readline(input);
    id = atoi(input);

    readline(input);
    n = atoi(input);

    flag = find_id(file, id);
    if(flag){
        printf("Espécie não encontrada\n");
        fclose(file);
        return;
    }
    for(int i = 0; i < n; i++){
        readline(input);
        if(!strcmp(input, "HUMAN IMPACT"))
            update_human_impact(file, id);
        else if (!strcmp(input, "POPULATION"))
            update_population(file, id);
        else if(!strcmp(input, "STATUS"))
            update_status(file, id);
        else{
            printf("Falha no processamento do arquivo\n");
            return;
        }
    }    
    fclose(file);
}

int find_id(FILE* file, int id){
    fseek(file, 0, SEEK_SET);
    int binary_id;
    while(1){
        if(eof_verification(file))
            break;
        fread(&binary_id, 1, sizeof(int), file);
        if(binary_id == id){
            fseek(file, -sizeof(int), SEEK_CUR);
            return 0;
        }
        fseek(file, ENTRY_OFFSET - sizeof(int), SEEK_CUR);
    }
    return -1;
}

void update_human_impact(FILE* file, int id){
    int file_info;
    char* input = (char*)calloc(20, sizeof(char));
    readline(input);
    int num = atoi(input);
    fseek(file, ENTRY_OFFSET - sizeof(int), SEEK_CUR);
    fread(&file_info, 1, sizeof(int), file);
    if(file_info != 0){
        printf("Informação já inserida no arquivo\n");
        fseek(file, - ENTRY_OFFSET, SEEK_CUR);
        return;
    }
    fseek(file, - sizeof(int), SEEK_CUR);
    fwrite(&num, sizeof(int), 1, file);
    fseek(file, - ENTRY_OFFSET, SEEK_CUR);
    free(input); 
}

void update_population(FILE* file, int id){
    int file_info;
    char* input = (char*)calloc(20, sizeof(char));
    readline(input);
    int num = atoi(input);
    fseek(file, 106, SEEK_CUR);

    fread(&file_info, 1, sizeof(int), file);
    if(file_info != 0){
        printf("Informação já inserida no arquivo\n");
        fseek(file, -110, SEEK_CUR);
        return;
    }
    fseek(file, -sizeof(int), SEEK_CUR);
    fwrite(&num, sizeof(int), 1, file); 
    fseek(file, -110, SEEK_CUR);
    free(input);
}

void update_status(FILE* file, int id){
    char* file_info = (char*)calloc(20, sizeof(char));
    char* input = (char*)calloc(20, sizeof(char));
    readline(input);
    fseek(file, ENTRY_OFFSET - 21, SEEK_CUR);
    fread(file_info, 9, sizeof(char),file);
    if(strcmp(file_info, "NULO") != 0){
        printf("Informação já inserida no arquivo\n");
        fseek(file, 21 - ENTRY_OFFSET, SEEK_CUR);
        return;
    }
    fseek(file, -9, SEEK_CUR);
    fwrite(input, sizeof(char), strlen(input) + 1, file);
    fwrite("$", sizeof(char), 9 - strlen(input) - 1, file);
    fseek(file, sizeof(int) - ENTRY_OFFSET, SEEK_CUR); 
    free(input);
    free(file_info);
}

int eof_verification(FILE* file){
    int flag;
    if(fread(&flag, 1, 1, file) == 0) //verifica fim do arquivo
        return 1;
    
    fseek(file, -1, SEEK_CUR);
    return 0;
}