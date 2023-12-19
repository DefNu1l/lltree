#include "headers/fdet.h"


/**
* lltree - Enhanced Tree Listing, v1.0.3
* 
* lltree is a command-line utility that combines the functionality of the tree
* and ll (long listing) commands. It provides an extended and comprehensive view
* of the file structure, including details such as file permissions, user information,
* and creation dates. The goal is to offer a quick and dependable solution for visualizing
* folder contents while ensuring no crucial elements are overlooked.
*  
* Author: Def0x00
* License: BSD 3-Clause License
* GitHub Repository: https://github.com/DefNu1l/lltree
*/



struct stat file_stat;


int isowner(char *owner) {
	int is_owner;
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	


	if (pw == NULL) {
		return -1;
	} 

	pw->pw_name == owner ? (is_owner = 1) : (is_owner = 0);
	
	
	return is_owner;
}


char *owner(char *filename) {
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


char *modd(char *filename) {
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


char *perm(char *filename) {
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

