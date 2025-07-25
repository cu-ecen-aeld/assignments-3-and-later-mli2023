#include <syslog.h>
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // For opendir, readdir, closedir
#include <sys/stat.h> // For stat (to check file type)

int count_lines(const char *filename) {
    FILE *fp;
    int ch;
    int lines = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1; // Indicate an error
    }

    // Read character by character until end of file
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    fclose(fp);
    return lines;
}

#define BUFFER_SIZE 1000

/// @brief Get the line count containing the string
/// @param filename 
/// @param str 
/// @return 
size_t findstr(const char *filename, const char *str)
{
    FILE *fp;
    size_t lineno = 0;
    size_t matchlines = 0;
    char buf[BUFFER_SIZE];
    char *search_string = "example";

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        lineno++;
        if (strstr(buf, search_string) != NULL) {
            matchlines++;
        }
    }

    fclose(fp);
    return matchlines;
}

void listFilesRecursively(const char *basePath, const char *str, size_t *filecnt, size_t *linecnt) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir) {
        // Handle error: could not open directory
        perror("opendir");
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            // Construct full path
            snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) { // Check if it's a directory
                    //printf("Directory: %s\n", path);
                    listFilesRecursively(path, str, filecnt, linecnt); // Recurse
                } else { // It's a regular file
                    
                    // Add your file processing logic here
                    size_t lines = findstr(path, str);
                    if (lines)
                    {
                        // printf("File: %s\n", path);
                        (*filecnt)++;
                        *linecnt += lines;
                    }
                }
            } else {
                perror("stat"); // Handle stat error
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    argc = 3;
    argv[1] = "/home/ampac/Training";
    argv[2] = "main";
    if (argc != 3)
    {
        printf("Usage: %s <filesdir> <searchstr>\r\n", argv[0]);
    }

    char logFlag = LOG_PID;

    
    size_t num_files = 0, num_matching_lines = 0;
    listFilesRecursively(argv[1], argv[2], &num_files, &num_matching_lines);
    printf("The number of files are %ld and the number of matching lines are %ld\r\n",
        num_files, num_matching_lines);


    openlog("mylog", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Writing %s to ");
    closelog();
    return 1;
}