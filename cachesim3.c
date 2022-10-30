#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char *trace_file_name;
/*The data structure of l1 cache*/
struct l1_cache
{
    unsigned valid_field[1024]; /* Valid field */
    unsigned dirty_field[1024];/* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[1024];  /* Tag field */
    char data_field[1024][64]; /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;  /* Hit count */
    int misses;/* Miss count */
};
/*The data structure of l2 cache*/
struct l2_cache
{
    unsigned valid_field[16384]; /* Valid field */
    unsigned dirty_field[16384];/* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[16384];/* Tag field */
    char data_field[16384][64];/* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits; /* Hit count */
    int misses;/* Miss count */
};

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n')
    {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0')
        {
            binary = (binary * 16) + (memory_addr[i] - '0');
        }
        else
        {
            if (memory_addr[i] == 'a' || memory_addr[i] == 'A')
            {
                binary = (binary * 16) + 10;
            }
            if (memory_addr[i] == 'b' || memory_addr[i] == 'B')
            {
                binary = (binary * 16) + 11;
            }
            if (memory_addr[i] == 'c' || memory_addr[i] == 'C')
            {
                binary = (binary * 16) + 12;
            }
            if (memory_addr[i] == 'd' || memory_addr[i] == 'D')
            {
                binary = (binary * 16) + 13;
            }
            if (memory_addr[i] == 'e' || memory_addr[i] == 'E')
            {
                binary = (binary * 16) + 14;
            }
            if (memory_addr[i] == 'f' || memory_addr[i] == 'F')
            {
                binary = (binary * 16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}
/*Check that data that exists in cache L1 method */
int check_data_exists_in_cahcel1(uint64_t address, int nway, struct l1_cache *l1)
{
    uint64_t block_addr = address >> (unsigned)log2(64);
    int setNumber = block_addr % 512;
    uint64_t tag = block_addr >> (unsigned)log2(512);
    int startIndex = ((int)setNumber) * nway;
    int nwayTemp = nway;
    int loopIndex = startIndex;
    while (nwayTemp > 0)
    {
        if (l1->valid_field[loopIndex] && l1->tag_field[loopIndex] == tag)
        {
            return 1;
        }
        loopIndex += 1;
        nwayTemp--;
    }
    return 0;
}
/*Check that data that exists in cache L2 method */
int check_data_exists_in_cahcel2(uint64_t address, int nway, struct l2_cache *l2)
{
    uint64_t block_addr = address >> (unsigned)log2(64);
    int setNumber = block_addr % 2048;
    uint64_t tag = block_addr >> (unsigned)log2(2048);
    int startIndex = ((int)setNumber) * nway;
    int nwayTemp = nway;
    int loopIndex = startIndex;
    while (nwayTemp > 0)
    {
        if (l2->valid_field[loopIndex] && l2->tag_field[loopIndex] == tag)
        {
            return 1;
        }
        loopIndex += 1;
        nwayTemp--;
    }
    return 0;
}

void data_in_l1_cache(uint64_t address, int nway, struct l1_cache *l1)
{
    uint64_t block_addr = address >> (unsigned)log2(64);
    int setNumber = block_addr % 512;
    uint64_t tag = block_addr >> (unsigned)log2(512);
    int startIndex = ((int)setNumber) * nway;
    int nwayTemp = nway;
    int loopIndex = startIndex;
    int isAnySpaceEmpty = 0;
    int endIndex = startIndex + nway - 1;
    while (nwayTemp > 0)
    {
        if (l1->valid_field[loopIndex] == 0)
        {
            isAnySpaceEmpty = 1;
        }
        loopIndex++;
        nwayTemp--;
    }
    if (isAnySpaceEmpty > 0)
    {
        nwayTemp = nway;
        loopIndex = startIndex;
        while (nwayTemp > 0)
        {
            if (l1->valid_field[loopIndex] == 0)
            {
                l1->valid_field[loopIndex] = 1;
                l1->tag_field[loopIndex] = tag;
                break;
            }

            loopIndex += 1;
            nwayTemp--;
        }
    }
    else
    {
        int randomIndex = (rand() % (endIndex - startIndex + 1)) + startIndex;
        l1->valid_field[randomIndex] = 1;
        l1->tag_field[randomIndex] = tag;
    }
}

void data_in_l2_cache(uint64_t address, int nway, struct l2_cache *l2)
{

    uint64_t block_addr = address >> (unsigned)log2(64);
    int setNumber = block_addr % 2048;
    uint64_t tag = block_addr >> (unsigned)log2(2048);
    int startIndex = ((int)setNumber) * nway;
    int nwayTemp = nway;
    int loopIndex = startIndex;
    int isAnySpaceEmpty = 0;
    int endIndex = startIndex + nway - 1;
    while (nwayTemp > 0)
    {
        if (l2->valid_field[loopIndex] == 0)
        {
            isAnySpaceEmpty = 1;
        }
        loopIndex++;
        nwayTemp--;
    }
    if (isAnySpaceEmpty > 0)
    {
        nwayTemp = nway;
        loopIndex = startIndex;
        while (nwayTemp > 0)
        {
            if (l2->valid_field[loopIndex] == 0)
            {
                l2->valid_field[loopIndex] = 1;
                l2->tag_field[loopIndex] = tag;
                break;
            }

            loopIndex += 1;
            nwayTemp--;
        }
    }
    else
    {
        /*Random method to pick random index and replace*/
        int randomIndex = (rand() % (endIndex - startIndex + 1)) + startIndex;
        l2->valid_field[randomIndex] = 1;
        l2->tag_field[randomIndex] = tag;
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    trace_file_name = argv[2];
    char mem_request[20];
    uint64_t address;
    struct l1_cache l1;
    struct l2_cache l2;
    /* setting all the variable values*/
    int numberOfBlocksinl1 = 1024;
    int numberOfBocksinl2 = 16384;
    int l1nway = 2;
    int l2nway = 8;
    int numberOfSetsl1 = 512;
    int numberOfSetsl2 = 2048;
    /* For loop for block L1*/
    for (int i = 0; i < numberOfBlocksinl1; i++)
    {
        l1.valid_field[i] = 0;
        l1.dirty_field[i] = 0;
        l1.tag_field[i] = 0;
    }
    /* For loop for block L2*/
    for (int i = 0; i < numberOfBocksinl2; i++)
    {
        l2.valid_field[i] = 0;
        l2.dirty_field[i] = 0;
        l2.tag_field[i] = 0;
    }
     /*  Initializing the l1 and l2 hits and miss variable values*/
    l1.hits = 0;
    l1.misses = 0;
    l2.hits = 0;
    l2.misses = 0;

    fp = fopen(trace_file_name, "r");  /*  Opening the trace file*/

    if (strncmp(argv[1], "direct", 6) == 0)
    {
        while (fgets(mem_request, 20, fp) != NULL)
        {
            address = convert_address(mem_request);
            int dataInL1 = check_data_exists_in_cahcel1(address, l1nway, &l1);
            if (dataInL1 == 1)
            {
                l1.hits++;
                l2.hits++;
            }
            else
            {
                l1.misses++;
                int dataInL2 = check_data_exists_in_cahcel2(address, l2nway, &l2);
                if (dataInL2)
                {
                    l2.hits += 1;
                }
                else
                {
                    l2.misses++;
                    data_in_l2_cache(address, l2nway, &l2);
                }
                data_in_l1_cache(address, l1nway, &l1);
            }
        }
      
        printf("Cache type:     l1\n");
        printf("Cache Hits:    %d\n", l1.hits);
        printf("Cache Misses:  %d\n", l1.misses);
        printf("Cache Hit Rate : %0.2f%%\n", ((float)l1.hits / (float)(l1.hits + l1.misses)));
        printf("Cache Miss Rate : %0.2f%%\n", ((float)l1.misses / (float)(l1.hits + l1.misses)));
        printf("\n");
        printf("Cache type:     l2\n");
        printf("Cache Hits:    %d\n", l2.hits);
        printf("Cache Misses:  %d\n", l2.misses);
        printf("Cache Hit Rate : %0.2f%%\n", ((float)l2.hits / (float)(l2.hits + l2.misses)));
        printf("Cache Miss Rate : %0.2f%%\n", ((float)l2.misses / (float)(l2.hits + l2.misses)));
        printf("\n");
    }

    fclose(fp);

    return 0;
}