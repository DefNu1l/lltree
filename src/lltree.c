/**
* lltree - Enhanced Tree Listing, v1.0.3
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



#include "headers/lltree.h"



void dirwalk(counter_t *counter, char *path, char *prefix) {
	DIR *dir = opendir(path);
	if (!dir) {
		fprintf(stderr, "opendir: %s\n", strerror(errno));
		exit(-1);
	}

	struct dirent *dir_ptr;
	struct dirent **child_dir;


	int sc = scandir(path, &child_dir, NULL, alphasort);
	if (sc < 0) {
		fprintf(stderr, "scandir: %s\n", strerror(errno));
		exit(-1);
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


		if (path[strlen(path) - 1] == '/') {
			path[strlen(path) - 1] = '\0';
		}


		char child_path[PATH_MAX];
		snprintf(child_path, sizeof(child_path), "%s/%s", path, entry);
		char new_prefix[PATH_MAX];
		snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, ca);



		struct stat link_stat;
		if (lstat(child_path, &link_stat) == 0 && S_ISLNK(link_stat.st_mode)) {
			char symlink_dest[PATH_MAX];
			ssize_t len = readlink(child_path, symlink_dest, sizeof(symlink_dest) - 1);
			

			if (len != -1) {
				symlink_dest[len] = '\0'; 
				printf("%s%s [\033[31msymlink\033[0m] %s -> %s\n", prefix, cb, child_path, symlink_dest);
			}
			continue; // symbolic link: skip
		}
		
		
		char *permission = perm(child_path);
		char *modify_date = modd(child_path);
		char *file_owner = owner(child_path);

		char colorize_owner[50];
		int io = isowner(file_owner);
		if (io) {
			sprintf(colorize_owner, "\033[32m%s\033[0m", file_owner);
		}
		else if (io == -1) {
			sprintf(colorize_owner, "\033[31mFAILURE\033[0m");
		}
		else {
			sprintf(colorize_owner, "\033[31m%s\033[0m", file_owner);
		}
		

		int eval = strcmp(entry, ".") != 0 && strcmp(entry, "..");
		if (eval != 0) {
			if (dir_ptr->d_type == DT_DIR) {
				printf("%s%s%s %s \033[36m%s\033[0m \033[1;34m%s\033[0m\n", 
					prefix, cb, permission, colorize_owner, modify_date, entry);
			}
			else {
				printf("%s%s%s %s \033[36m%s\033[0m %s\n", 
					prefix, cb, permission, colorize_owner, modify_date, entry);
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

	printf("\nLLTree, v1.0.3\n");
	printf("\n --- Permissions - Owner - Last Modified - File\n\n");
	printf("Start: \033[32m%s\033[0m\n", dir);
	dirwalk(&counter, dir, "");


	clock_t end = clock();
	double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("\n%d directories, %d files, time needed: %fs\n", 
			counter.dir_count - 1, counter.file_count, elapsed);

	return 0;
}
