#ifndef MAIN_H
#define MAIN_H

/**
* This is the header file of the lltree main file.
*
*
* lltree - Enhanced Tree Listing, v1.0.0
*
* Author: Def0x00
* License: BSD 3-Clause License
* GitHub Repository: https://github.com/DefNu1l/lltree
*/


static int dir_count = 0;
static int file_count = 0;
static struct stat file_stat;


static char *owner(const char *filename);
static char *modd(const char *filename);
static char *perm(const char *filename);
static void dirwalk(const char *path, char *prefix);


#endif
