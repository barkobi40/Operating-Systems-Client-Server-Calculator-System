#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define TIMEOUT 60

#define SIZE 60


void helpingRead(int fd, char* buff, int size) {
    int i = 0;
    while (i < size) {
        int bytes_read = read(fd, &buff[i], 1);
        if (bytes_read < 0) {
            perror("failed reading toServer.txt file\n");
            exit(1);
        }
        if (bytes_read == 0)
            break;
        if (buff[i] == ' ')
            break;
        i++;
    }
    buff[i] = '\0'; // add null terminator
}

void execution(char* num1, char* operator, char* num2, pid_t clientPID) 
{
    char ClientFileName[SIZE];
    int op = atoi(operator);
    int sum = 0;
    char result[SIZE];

    switch (op) {
        case 1:
            sum = atoi(num1) + atoi(num2);
            break;
        case 2:
            sum = atoi(num1) - atoi(num2);
            break;
        case 3:
            sum = atoi(num1) * atoi(num2);
            break;
        case 4:
            if (atoi(num2) == 0) {
                printf("cannot divide by zero\n");
                break;
            }
            sum = atoi(num1) / atoi(num2);
            break;
        default:
            perror("wrong operator was inserted\n");
            return;
    }

    sprintf(ClientFileName, "toClient_%d.txt", clientPID);

    int fd_toClient = open(ClientFileName, O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd_toClient < 0) {
        perror("failed creating toClient.txt\n");
        exit(1);
    }
    printf("End of stage 5h: createing toClient_---.txt file for specific clientPID\n");
    sprintf(result, "%d", sum);
    write(fd_toClient, result, strlen(result));
    close(fd_toClient);
    printf("End of stage 5i: writing the result to the toClient_---.txt file\n");
    exit(0);
}

void handler1() 
{
    int size = SIZE;
    char num1[SIZE];
    char num2[SIZE];
    char operator[SIZE];
    char clientPid[SIZE];


    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork1 is failed\n");
        exit(1);
    } 
    else if (pid1 == 0)
     {
         int fd_toServer = open("toServer.txt", O_RDONLY);
         if (fd_toServer < 0) 
        {
        printf("failed opening toServer.txt\n");
        exit(1);
        }

    helpingRead(fd_toServer, clientPid, size);
    helpingRead(fd_toServer, num1, size);
    helpingRead(fd_toServer, operator, size);
    helpingRead(fd_toServer, num2, size);

    close(fd_toServer);
        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("fork2 is failed\n");
            exit(1);
        } else if (pid2 == 0) 
        {
            printf("End of stage 5g: delete the toServer.txt file\n");
            char *args[] = {"rm", "toServer.txt", NULL};
            execvp(args[0], args);
            perror("Exec1 failed\n");
            exit(-1);
        } 
        else 
        {
            wait(NULL);
            pid_t pid3 = fork();
            if (pid3 < 0) {
                perror("fork3 is failed\n");
                exit(1);
            } 
            else if (pid3 == 0) 
            {
                printf("End of stage 5f: creating child in order to calculate the request from server\n");
                execution(num1, operator, num2, atoi(clientPid));
            } else {
                wait(NULL);
                pid_t C_pid = atoi(clientPid);
                kill(C_pid, SIGUSR1);
                exit(0);
            }
        }
    } else {
        wait(NULL);
    }
}

void alarmhandler(int timeout)
{
    printf("End of stage 3: terminating the server process after 60 seconds\n");
    exit(0);
}

int main() 
{
   signal(SIGUSR1, handler1);
   signal(SIGALRM,alarmhandler);
    while (1) 
    {
        alarm(TIMEOUT);
        pause();
    }
    exit(0);
}