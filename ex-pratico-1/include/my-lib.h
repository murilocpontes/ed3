#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    int SPECIES_ID;
    char NAME[41];
    char SCIENTIFIC_NAME[61];
    int POPULATION;
    char STATUS[9];
    float LOCATION[2]; //[0] = x, [1] = y
    int HUMAN_IMPACT;
}entry;

// Collect all info to a entry
void data_collect(entry* tmpEntry);
// Functions related to a register in binary
void data_insertion(char* address, int numberOfEntrys);
void data_write(entry* tmpEntry, FILE* file);
// Print and reading in binary functions
void print_all_register(char* address);
void read_from_binary(FILE* file, entry* data);
void print_entry(entry data);
// Searching with rrn
entry get_rrn_data(int rrn, char* address, int* flag);
// Updating id data functions
void update_data(char* address);
void update_human_impact(FILE* file, int id);
void update_population(FILE* file, int id);
void update_status(FILE* file, int id);
int find_id(FILE* file, int id);

int eof_verification(FILE* file);
