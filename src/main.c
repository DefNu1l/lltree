#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>



/**
* lltree - Enhanced Tree Listing, v1.0.0
* 
* lltree is a command-line utility that combines the functionality of the tree
* and ll (long listing) commands. It provides an extended and comprehensive view
* of the file structure, including details such as file permissions, user information,
* and creation dates. The goal is to offer a quick and dependable solution for visualizing
* folder contents while ensuring no crucial elements are overlooked.
* 
* Usage:
*  - Run:    $ ./build/lltree 
*  - Specify directory: $ ./build/lltree /path/to/your/directory/
*  - Install system-wide: $ sudo make install
*  - Run after installation: $ lltree
* 
* Author: Def0x00
* License: BSD 3-Clause License
* GitHub Repository: https://github.com/DefNu1l/lltree
*/



#include "headers/main.h"


static char *owner(const char *filename) {
	memset(&file_stat, 0, sizeof(struct stat));


	if (stat(filename, &file_stat) == -1) {
		fprintf(stderr, "stat: %s\n", strerror(errno));
		return "";
	}	

	
	uid_t oid = file_stat.st_uid;
	struct passwd *owner_inf = getpwuid(oid);
	if (owner_inf == NULL) {
		fprintf(stderr, "getpwuid: %s\n", strerror(errno));
		return "";
	}

	return owner_inf->pw_name;
}


static char *modd(const char *filename) {
	memset(&file_stat, 0, sizeof(struct stat));

	
	if (stat(filename, &file_stat) == -1) {
		fprintf(stderr, "stat: %s\n", strerror(errno));
		return "";
	}


	time_t lmodt = file_stat.st_mtime;
	struct tm *time_info = localtime(&lmodt);
	if (time_info == NULL) {
		fprintf(stderr, "localtime: %s\n", strerror(errno));
		return "";
	}

	
	char buf[1024];
	snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d\n", 
			time_info->tm_year + 1900, time_info->tm_mon + 1, 
			time_info->tm_mday, time_info->tm_hour, 
			time_info->tm_min, time_info->tm_sec);


	char *fmt_time = (char*)malloc(strlen(buf) + 1);
	if (fmt_time == NULL) {
		fprintf(stderr, "malloc: %s\n", strerror(errno));
		exit(-1);
	}	


	memmove(fmt_time, buf, strlen(buf) + 1);	
	fmt_time[strlen(fmt_time) - 1] = '\0';

	return fmt_time;
}


static char *perm(const char *filename) {
	memset(&file_stat, 0, sizeof(struct stat));

	
	if (stat(filename, &file_stat) == -1) {
		printf("%s\n", filename);
		fprintf(stderr, "stat: %s\n", strerror(errno));
		return "";
	}

	char *result = (char*)malloc(sizeof(char) *12);
	if (result == NULL) {
		fprintf(stderr, "malloc: %s\n", strerror(errno));
		exit(-1);
	}


	result[0] = (S_ISDIR(file_stat.st_mode)) ? 'd' : '-';

	result[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
	result[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
	result[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
	
	result[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
	result[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
	result[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';

	result[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
	result[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
	result[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
	
	result[10] = '\0';

	
	return result;
}


static void dirwalk(const char *path, int depth) {
	DIR *dir;
	if ((dir = opendir(path)) == NULL) {
		fprintf(stderr, "opendir: %s\n", strerror(errno));
		return;
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			for (int iter = 0; iter < depth; iter++) {
				printf("|   ");
			}
			if (strncmp(entry->d_name, ".", 1) == 0) {
				continue;
			}

			printf("|-- ");

			char fupa[1024];
			snprintf(fupa, sizeof(fupa), "%s/%s", path, entry->d_name);


			char *permission = perm(fupa);
			char *file_owner = owner(fupa);
			char *mod_date = modd(fupa);
			if (entry->d_type == DT_DIR) {
				printf("%s  %s  %s  \033[34m%s\033[0m\n", permission, file_owner, mod_date, entry->d_name);
			}
			else {
				printf("%s  %s  %s  %s\n", permission, file_owner, mod_date, entry->d_name);
			}

			idx++;
			free(permission);
			free(mod_date);

			if (entry->d_type == DT_DIR) {
				char new[1024];
				snprintf(new, sizeof(new), "%s/%s", path, entry->d_name);
				dirwalk(new, depth + 1);
			}
		}
	}

	closedir(dir);
}


int main(int argc, char **argv) {
	if (argc > 2) {
		fprintf(stderr, "Usage: %s <path>\n", argv[0]);
		return -1;
	}


	char *path = NULL;
	if (argc != 1) {
		path = argv[1];
	}
	else {
		size_t buffer = 1024;
		char cwd[buffer];
		if (getcwd(cwd, (size_t)buffer) == NULL) {
			fprintf(stderr, "getcwd: %s\n", strerror(errno));
			return -1;
		}

		path = strdup(cwd);
	}


	clock_t start = clock();

	printf("\nLLTree, v1.0.0\n");
	printf("\n --- Permissions - Owner - Last Modified - File\n\n\033[34m.\033[0m\n");


	dirwalk(path, 0);


	clock_t end = clock();
	double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\nDone, Total: %d, Time needed: %fs\n", idx, elapsed);
	
	return 0;
}
