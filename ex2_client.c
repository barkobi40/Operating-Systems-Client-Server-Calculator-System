#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/random.h>
#include <sys/wait.h>
#define TIMEOUT 30


int RandomNumber(int min, int max) 
{
    int range = max - min + 1;
    char random_bytes[sizeof(int)];

    ssize_t bytes_read = getrandom(random_bytes, sizeof(random_bytes), 0);

    if (bytes_read == -1) {
        perror("error in getrandom");
        exit(1);
    }

    int random_num = *(int*)random_bytes;
    int scaled_random = random_num % range;
    int final_result = scaled_random + min;

    return final_result;
}

void handler1() 
{
    char buff[20];
    int count = 0;
    char pid_c[60];
    pid_t currentPid = getpid();
    sprintf(pid_c, "%d", currentPid);
    char clientFIle[60];
    strcpy(clientFIle, "toClient_");
    strcat(clientFIle, pid_c);
    strcat(clientFIle, ".txt");
    int fd = open(clientFIle, O_RDONLY);
    if (fd < 0) 
    {
        perror("failed opening toClient.txt\n");
        exit(1);
    }
    while (1) 
    {
        int point = read(fd, &buff[count], 1);
        if (point < 0) {
            perror("failed opening toClient.txt\n");
            exit(1);
        }
        if (point == 0)
            break;
        else
            count++;
    }
    buff[count] = '\0';
    close(fd);

    int remove_pid = fork();
    if (remove_pid < 0) {
        perror("error creating a child for removing toClient.txt\n");
        exit(1);
    } 
    else if (remove_pid == 0) 
    {
        printf("End of stage 2f and 5j: delete the toClient_----.txt file\n");
        char *args[] = {"rm", clientFIle, NULL};
        execvp(args[0], args);
        perror("Exec2 failed\n");
        exit(-1);
    }
    wait(NULL);
    printf("The result is: %s\n", buff);
    exit(0);
}

void alarm_handler(int num)
{
    printf("End of stage 2e: client closed- no response from server after 30 seconds\n");
    exit(0);
}

int main(int argc, char* argv[]) 
{
    
    if (argc != 5) 
    {
        perror("error in number of arguments\n");
        exit(1);
    }
    sleep(13);//between clients	
    signal(SIGUSR1, handler1);
    signal(SIGALRM,alarm_handler);

    pid_t pidServer = atoi(argv[1]);
    pid_t ClientPid = getpid();
    int fd_toSrv;
    int x = 0, i;
    char space[] = " ";
    char client[10];
    char PID[20];
    char num1[20];
    char num2[20];
    char operator[20];

    sprintf(PID, "%d", ClientPid);
    strcpy(num1, argv[2]);
    strcpy(operator, argv[3]);
    strcpy(num2, argv[4]);
    
	
    for (i = 0; i < 10; i++) 
    {
        fd_toSrv = open("toServer.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
        if (fd_toSrv < 0) 
        {
            sleep(RandomNumber(1,6));
        }
        else
        {
            printf("End of stage 5d: creates the toServer.txt file\n");
            break;
        }
    }
    if (x == 10) 
    {
        perror("The server has not answered 10 times\n");
        exit(1);
    }
        sprintf(client, "%d", ClientPid);
        strcat(client, space);
        strcat(client, num1);
        strcat(client, space);
        strcat(client, operator);
        strcat(client, space);
        strcat(client, num2);
        strcat(client, "\n");
        write(fd_toSrv, client, strlen(client));
        close(fd_toSrv);

        alarm(TIMEOUT);
        printf("End of stage 2e: set timer for the server response\n");
        kill(pidServer, SIGUSR1);
        pause();

    return 0;
}
