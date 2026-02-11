#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h>

#define PORT 9000
#define BACKLOG 1
#define BUFFER_SIZE 102400
#define DATA_FILE "/var/tmp/aesdsocketdata"

int server_fd = -1;
int client_fd = -1;

void handle_signal(int sig) {
    if (client_fd != -1) close(client_fd);
    if (server_fd != -1) close(server_fd);
    closelog();
    unlink(DATA_FILE);
    syslog(LOG_INFO, "Caught signal %d, exiting", sig);
    exit(0);
}

int main(int argc, char **argv) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];
    ssize_t bytes_read;
    FILE *fp;

    if ((argc > 1) && (strstr(argv[1], "-d")!= NULL))
    {
        // make this program a daemon

        int ret = fork();
        if (!ret) // child
        {
            printf("Child daemon\r\n");
        }
        else if (ret > 0) // parent, ret is child pid
        {
            printf("Parent exit!\r\n");
            return 0;
        }
    }

    openlog("aesdsocket", LOG_PID, LOG_USER);

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        syslog(LOG_ERR, "socket failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        syslog(LOG_ERR, "bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, BACKLOG) < 0) {
        syslog(LOG_ERR, "listen failed");
        close(server_fd);
        return -1;
    }

    while (1) {
        client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            syslog(LOG_ERR, "accept failed");
            continue;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        syslog(LOG_INFO, "Accepted connection from %s", client_ip);

        fp = fopen(DATA_FILE, "a");
        if (fp == NULL) {
            syslog(LOG_ERR, "fopen failed");
            close(client_fd);
            continue;
        }

        memset(buffer, 0, BUFFER_SIZE);
        while ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            printf("Received %zd bytes from %s: %s\n", bytes_read, client_ip, buffer);
            if (buffer[bytes_read - 1] != '\n') {
                buffer[bytes_read] = '\n';
                bytes_read++;
            }

            fwrite(buffer, 1, bytes_read, fp);
            fflush(fp);
            fclose(fp);

            memset(buffer, 0, BUFFER_SIZE);
            fp = fopen(DATA_FILE, "r");
            if (fp == NULL) {
                syslog(LOG_ERR, "fopen failed");
                close(client_fd);
                continue;
            }
            // fread(buffer, 1, BUFFER_SIZE, fp);
          //  printf("Sending data back to %s: %s\n", client_ip, buffer);
            while (fread(buffer, 1, BUFFER_SIZE, fp) > 0) {
                send(client_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }
            fseek(fp, 0, SEEK_END);
            
        }

        fclose(fp);
        close(client_fd);
        client_fd = -1;
        syslog(LOG_INFO, "Closed connection from %s", client_ip);
    }

    return 0;
}