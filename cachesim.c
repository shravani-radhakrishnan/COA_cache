/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "cachesim.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <direct> <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    char* trace_file_name = argv[2];

		/* Declare Our caches of struct */
    struct direct_mapped_cache 					d_cache;
		struct fully_associative_cache 			f_cache;
		struct n_way_set_associative_cache 	n_cache;

		/* Leave alone stuff */
    char mem_request[20];
    uint64_t address;
    FILE *fp;
		FILE *outfp;

    /* Initialization  our three structs of cache*/

		/* Direct-associative cache */
    for (int i=0; i<NUM_BLOCKS; i++) {
        d_cache.valid_field[i] = 0;
        d_cache.dirty_field[i] = 0;
        d_cache.tag_field[i] = 0;
    }
    d_cache.hits = 0;
    d_cache.misses = 0;

		/* Fully-associative cache */
		for (int i=0; i<NUM_BLOCKS; i++) {
        f_cache.valid_field[i] = 0;
        f_cache.dirty_field[i] = 0;
        f_cache.tag_field[i] = 0;
    }
    f_cache.hits = 0;
    f_cache.misses = 0;

		/* N-Way-set-associative cache */
		for (int i=0; i<NUM_SETS; i++) {
        n_cache.valid_field[i] = 0;
        n_cache.dirty_field[i] = 0;
				
        
				// Initialize all tag fields to zero
				for (int k=0; k<WAY_SIZE; k++){
					n_cache.tag_field[i][k] = 0;
				}
    }
    n_cache.hits = 0;
    n_cache.misses = 0;

    /* Variables used for [PART 1] */
    float total = 0;
    float cache_miss_rate = 0;
    float cache_hit_rate = 0;

		/* For Random Policy [PART 2 & 3] */
		srand(time(0)); 

    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");
		outfp = fopen("[Part-3]full_64KB-output.txt", "w");
     printf(outfp,"\n==================================\n");
        printf(outfp,"Cache type:    Direct-Mapped Cache\n");
				printf(outfp,"Block Size:		%d bytes\n", BLOCK_SIZE);
				printf(outfp,"Cache Size:		%d KB\n", (CACHE_SIZE / 1024));
				printf(outfp,"File:					%s\n", trace_file_name);
        printf(outfp,"==================================\n");
        printf(outfp,"Cache Hits:    %d\n", d_cache.hits);
        printf(outfp,"Cache Misses:  %d\n", d_cache.misses);
        printf(outfp,"\n");

    if (strncmp(argv[1], "direct", 6)==0) { /* Simulating direct-mapped cache */
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            direct_mapped_cache_access(&d_cache, address);
        }
        /*Print out the results*/
        fprintf(outfp,"\n==================================\n");
        fprintf(outfp,"Cache type:    Direct-Mapped Cache\n");
				fprintf(outfp,"Block Size:		%d bytes\n", BLOCK_SIZE);
				fprintf(outfp,"Cache Size:		%d KB\n", (CACHE_SIZE / 1024));
				fprintf(outfp,"File:					%s\n", trace_file_name);
        fprintf(outfp,"==================================\n");
        fprintf(outfp,"Cache Hits:    %d\n", d_cache.hits);
        fprintf(outfp,"Cache Misses:  %d\n", d_cache.misses);
        fprintf(outfp,"\n");

        /* Manipulated Code to calculate hit rate & miss rate of a cache [PART 1]*/
        total = d_cache.misses + d_cache.hits;
        cache_hit_rate = d_cache.hits / total;
        cache_miss_rate = d_cache.misses / total;

        fprintf(outfp,"Hit Rate:       %% %.2f Percent\n", cache_hit_rate);
        fprintf(outfp,"Miss Rate:      %% %.2f Percent\n", cache_miss_rate);
        fprintf(outfp,"\n");

    }


		else if (strncmp(argv[1], "full", 6)==0) { /* Simulating Fully-associative cache */
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            fully_associative_cache_access(&f_cache, address);
        }
        /*Print out the results*/
        fprintf(outfp,"\n==================================\n");
        fprintf(outfp,"Cache type:    Fully-associative Cache\n");
				fprintf(outfp,"Block Size:		%d bytes\n", BLOCK_SIZE);
				fprintf(outfp,"Cache Size:		%d KB\n", (CACHE_SIZE / 1024));
				fprintf(outfp,"File:					%s\n", trace_file_name);
        fprintf(outfp,"==================================\n");
        fprintf(outfp,"Cache Hits:    %d\n", f_cache.hits);
        fprintf(outfp,"Cache Misses:  %d\n", f_cache.misses);
        fprintf(outfp,"\n");

        /* Manipulated Code to calculate hit rate & miss rate of a cache [PART 2]*/
        total = f_cache.misses + f_cache.hits;
        cache_hit_rate = f_cache.hits / total;
        cache_miss_rate = f_cache.misses / total;

        fprintf(outfp,"Hit Rate:       %% %.2f Percent\n", cache_hit_rate);
        fprintf(outfp,"Miss Rate:      %% %.2f Percent\n", cache_miss_rate);
        fprintf(outfp,"\n");
        
    }

		

		else if (strncmp(argv[1], "n-way", 6)==0) { /* Simulating N-Way cache */
        /* Read the memory request address and access the cache */

        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            n_way_set_associative_cache_access(&n_cache, address);
        }
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    %d N-Way Cache\n", WAY_SIZE);
				printf("Block Size:		%d bytes\n", BLOCK_SIZE);
				printf("Cache Size:		%d KB\n", (CACHE_SIZE / 1024));
				printf("File:					%s\n", trace_file_name);
        printf("==================================\n");
        printf("Cache Hits:    %d\n", n_cache.hits);
        printf("Cache Misses:  %d\n", n_cache.misses);
        printf("\n");

        /* Manipulated Code to calculate hit rate & miss rate of a cache [PART 2]*/
        total = n_cache.misses + n_cache.hits;
        cache_hit_rate = n_cache.hits / total;
        cache_miss_rate = n_cache.misses / total;

        printf("Hit Rate:       %% %.2f Percent\n", cache_hit_rate);
        printf("Miss Rate:      %% %.2f Percent\n", cache_miss_rate);
        printf("\n");

    }

    fclose(fp);
		fclose(outfp);
    return 0;
}


/* Leave alone */
uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" 
  (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    //printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}


void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address)
{
    uint64_t block_addr 	= address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index 				= block_addr % NUM_BLOCKS;
    uint64_t tag 					= block_addr >> (unsigned)log2(NUM_BLOCKS);
		

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu \n", address, block_addr, index, tag);
		printf("\tValid_field of this index = %d\n", cache->valid_field[index]);
		printf("\tTags at this set/index: \n");
		printf("\t\tcache->tag_field[index][0] = %llu\n",  cache->tag_field[index]);
#endif

    if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
        cache->hits += 1;
#ifdef DBG
        printf("Hit!\n");
				printf("\t\tcache->valid_field[index]: %d, cache->tag_field[index]: %llu \n\n", cache->valid_field[index], cache->tag_field[index]);
#endif
    } else {
        /* Cache miss */
        cache->misses += 1;
/*
#ifdef DBG
        printf("Miss!\n");
				printf("\t[BEFORE] cache->valid_field[index]: %d, cache->tag_field[index]: %llu \n", cache->valid_field[index], cache->tag_field[index]);
#endif*/
#ifdef DBG
      printf("\nMiss!\n");
			printf("\t[BEFORE]\n\t\t cache->valid_field[index]: %d\n", cache->valid_field[index]);
			for( int tag_i = 0; tag_i < WAY_SIZE; tag_i++){
      printf("\t\tcache->tag_field[index][%d] = %llu\n", tag_i, cache->tag_field[index]);
		}
#endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        cache->tag_field[index] = tag;
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;

/*
#ifdef DBG
				printf("\t[AFTER] cache->valid_field[index]: %d, cache->tag_field[index]: %llu \n\n", cache->valid_field[index], cache->tag_field[index]);
#endif*/
#ifdef DBG
			printf("\t[AFTER]\n\t\t cache->valid_field[index]: %d\n", cache->valid_field[index]);
			for( int tag_i = 0; tag_i < WAY_SIZE; tag_i++){
      printf("\t\tcache->tag_field[index][%d] = %llu\n", tag_i, cache->tag_field[index]);
		}
		printf("\n");
#endif


    }
}

/* N WAY Mapped - FUNCTION */
void n_way_set_associative_cache_access(struct n_way_set_associative_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_SETS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_SETS);

		
	

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu \n", address, block_addr, index, tag);
		printf("\tValid_field of this index = %d\n", cache->valid_field[index]);
		printf("\tTags at this set/index: \n");
#endif

		int valid_tag = 0;
		int tag_index = 0;
		/* Check if tag is in tagfields at this index */
		for( tag_index = 0; tag_index < WAY_SIZE; tag_index++){
			if(cache->tag_field[index][tag_index] == tag){
				valid_tag = 1;
				break;
			}
		}


#ifdef DBG
		for( int tag_i = 0; tag_i < WAY_SIZE; tag_i++){
      printf("\t\tcache->tag_field[index][%d] = %llu\n", tag_i, cache->tag_field[index][tag_i]);
		}
#endif		
		
    if (cache->valid_field[index] &&    valid_tag) { /* Cache hit */
        cache->hits += 1;



#ifdef DBG
        printf("\nHit!\n");
				printf("\t\tAT this location...cache->valid_field[index]: %d, cache->tag_field[index][%d]: %llu \n\n", cache->valid_field[index], tag_index, cache->tag_field[index][tag_index]);
#endif



    } else {
        /* Cache miss */
        cache->misses += 1;



#ifdef DBG
      printf("\nMiss!\n");
			printf("\t[BEFORE]\n\t\t cache->valid_field[index]: %d\n", cache->valid_field[index]);
			for( int tag_i = 0; tag_i < WAY_SIZE; tag_i++){
      printf("\t\tcache->tag_field[index][%d] = %llu\n", tag_i, cache->tag_field[index][tag_i]);
		}
#endif

        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }

				
				int filled_in_tag = 0;

				/* Else it means that none of the tags in that cache line is empty
				// Nor does any of the tags at that position match the tag genereated
				// Use our random policy to pick a tag to evicted/replace with the current
				 generated tag*/
				if( filled_in_tag == 0){

					int tagged_picked = random_policy();
					cache->tag_field[index][tagged_picked] = tag;
	
				}
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;

#ifdef DBG
			printf("\t[AFTER]\n\t\t cache->valid_field[index]: %d\n", cache->valid_field[index]);
			for( int tag_i = 0; tag_i < WAY_SIZE; tag_i++){
      printf("\t\tcache->tag_field[index][%d] = %llu\n", tag_i, cache->tag_field[index][tag_i]);
		}
		printf("\n");
#endif
    }

}

/* Fully associatied Mapped - FUNCTION */
void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address){
	
	uint64_t block_addr 	= address >> (unsigned)log2(BLOCK_SIZE);
  //uint64_t index 				= block_addr % NUM_BLOCKS;  // We Dont need to generate a index
  uint64_t tag 	= block_addr >> (unsigned)log2(NUM_BLOCKS);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif
	
		int cache_hit = 0;
	

		// Check to see if any of the tagfield has a similar tag
		for(int index = 0; index < NUM_BLOCKS; index++){
			// if this tag value is same; It's a HIT 
			if(cache->tag_field[index] == tag){
				cache->hits += 1;
				cache_hit = 1;
				break;}
		}


		// Else, there is a miss
		if(cache_hit == 0){cache->misses += 1;}

		// If no cache_hit, pick a random entry to replace
		if(cache_hit == 0){

			// For example, If random index is 512, we need to access index 511
			int random_index = ( rand() % (NUM_BLOCKS - 1 + 1)) + 1;
			random_index = random_index - 1;

			cache->valid_field[random_index] = 1;
			cache->tag_field[random_index] = tag;
			cache->dirty_field[random_index] = 0;
		} 
}