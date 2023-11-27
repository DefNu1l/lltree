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
* lltree - Enhanced Tree Listing, v1.0.1
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


static char *owner(char *filename) {
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


static char *modd(char *filename) {
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


static char *perm(char *filename) {
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


static void dirwalk(counter_t *counter, char *path, char *prefix) {
	DIR *dir = opendir(path);
	if (!dir) {
		fprintf(stderr, "opendir: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct dirent *dir_ptr;
	struct dirent **child_dir;


	int sc = scandir(path, &child_dir, NULL, alphasort);
	if (sc < 0) {
		fprintf(stderr, "scandir: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


	
	counter->dir_count++;


	for (int iter = 0; iter < sc; iter++) {
		dir_ptr = child_dir[iter];


		char *ca = "│   ", *cb = "├── ";

		if (iter == (sc - 1)) {
			cb = "└── ";
			ca = "    ";
		}

		char *entry = dir_ptr->d_name;
		if (entry[0] == '.') {
			continue;
		}


		char child_path[1024];
		snprintf(child_path, sizeof(child_path), "%s/%s", path, entry);
		char new_prefix[1024];
		snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, ca);



		char *permission = perm(child_path);
		char *modify_date = modd(child_path);
		char *file_owner = owner(child_path);


		int eval = strcmp(entry, ".") != 0 && strcmp(entry, "..");
		if (eval != 0) {
			if (dir_ptr->d_type == DT_DIR) {
				printf("%s%s%s %s %s \033[34m%s\033[0m\n", 
					prefix, cb, permission, file_owner, modify_date, entry);
			}
			else {
				printf("%s%s%s %s %s %s\n", 
					prefix, cb, permission, file_owner, modify_date, entry);
			}
		} 


		free(modify_date);
		free(permission);

		if (dir_ptr->d_type == DT_DIR && eval) {
			dirwalk(counter, child_path, new_prefix);
		} else {
			counter->file_count++;
		}


		free(dir_ptr);
	}

	closedir(dir);
	free(child_dir);

}


	

int main(int argc, char **argv) {
	char *dir;
	argc > 1 ? (dir = argv[1]) : (dir = ".");

	counter_t counter = {0, 0};
	clock_t start = clock();

	printf("\nLLTree, v1.0.1\n");
	printf("\n --- Permissions - Owner - Last Modified - File\n\n");
	printf("\033[32m%s\033[0m\n", dir);
	dirwalk(&counter, dir, "");


	clock_t end = clock();
	double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("\n%d directories, %d files, time needed: %fs\n", 
			counter.dir_count - 1, counter.file_count, elapsed);

	return 0;
}
