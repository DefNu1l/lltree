#ifndef FDET_H
#define FDET_H

#include "externs.h"
#include "lltree.h"


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



int isowner(char *owner);
char *owner(char *filename);
char *modd(char *filename);
char *perm(char *filename);


#endif
