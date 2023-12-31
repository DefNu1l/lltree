#ifndef MAIN_H
#define MAIN_H

/**
* This is the header file of the lltree main file.
*
*
* lltree - Enhanced Tree Listing, v1.0.3
*
* Author: Def0x00
* License: BSD 3-Clause License
* GitHub Repository: https://github.com/DefNu1l/lltree
*/


#include "fdet.h"


typedef struct {
	int dir_count;
	int file_count;	
} counter_t;


void dirwalk(counter_t *counter, char *path, char *prefix);


#endif
