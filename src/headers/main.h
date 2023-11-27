#ifndef MAIN_H
#define MAIN_H

/**
* This is the header file of the lltree main file.
*
*
* lltree - Enhanced Tree Listing, v1.0.1
*
* Author: Def0x00
* License: BSD 3-Clause License
* GitHub Repository: https://github.com/DefNu1l/lltree
*/


typedef struct {
	int dir_count;
	int file_count;	
} counter_t;

static struct stat file_stat;


static char *owner(char *filename);
static char *modd(char *filename);
static char *perm(char *filename);
static void dirwalk(counter_t *counter, char *path, char *prefix);


#endif
