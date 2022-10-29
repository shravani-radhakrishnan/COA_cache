/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#include <stdlib.h>
#include <time.h>

#define BLOCK_SIZE  64        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define WAY_SIZE    1        /* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  65536    /* Cache capacity in bytes (must be power of 2)*/

#define NUM_BLOCKS  (CACHE_SIZE / BLOCK_SIZE)
#define NUM_SETS    (NUM_BLOCKS / WAY_SIZE)


/*STRUCTS - for different cache implementation */



/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};


/* Fully-associative Cache Struct for [PART 3] */
struct fully_associative_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field  */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};


/* n_way_set-associative Cache Struct for [PART 3] */
struct n_way_set_associative_cache {
    unsigned valid_field[NUM_SETS];   /* Valid field */
    unsigned dirty_field[NUM_SETS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_SETS][WAY_SIZE];     /* Tag field , with "WAY_SIZE" amounts of tags */
    char data_field[NUM_SETS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};


/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/* FUNCTION PROTOTYPES - to simulate cache */


/*Simulate the direct-mapped cache*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address);

/*Simulate the direct-mapped cache*/
void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address);

/*Simulate the direct-mapped cache*/
void n_way_set_associative_cache_access(struct n_way_set_associative_cache *cache, uint64_t address);

/*RANDOM POLICY - PART 2 & 3 */
/* Picks a tag to replace randomly */
int random_policy(){

	int tag_picked = 0;
	tag_picked = (rand() % (WAY_SIZE - 1 + 1)) + 1;
    
	// If taged picked is 8, we need to access index 7... so minus 1
	tag_picked = tag_picked - 1;

	return tag_picked;
}