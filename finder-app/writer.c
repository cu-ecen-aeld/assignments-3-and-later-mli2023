#include <syslog.h>
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // For opendir, readdir, closedir
#include <sys/stat.h> // For stat (to check file type)


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

int main(int argc, char *argv[])
{
    // argc = 3;
    // argv[1] = "/home/ampac/Training";
    // argv[2] = "main";
    if (argc != 3)
    {
        printf("Usage: %s <writefile> <writestr>\r\n", argv[0]);
        return 1;
    }
    
    openlog("mylog", LOG_PID, LOG_USER);
    FILE *fd;
    fd = fopen(argv[1], "w+");
    if (fd == NULL)
    {
        syslog(LOG_ERR, "Cannot open/create file\r\n");
        return 1;
    }

    fwrite(argv[2], strlen(argv[2]), 1, fd);
    fclose(fd);

    syslog(LOG_INFO, "Writing %s to %s", argv[2], argv[1]);
    closelog();

    return 0;
}