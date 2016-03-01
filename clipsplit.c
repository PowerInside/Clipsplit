#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#define CTRL_press 0x1d
#define CTRL_release 0x9d

#define C_press 0x2e
#define C_release 0xae

#define V_press 0x2f
#define V_release 0xaf

#define seperator " "
#define BUFSIZE 64

#include "savesplit.h"

int fd;
int oldkbmode;
struct termios old;

static void clean_up(void)
{
    if (ioctl(fd, KDSKBMODE, oldkbmode)) {
        perror("KDSKBMODE");
        exit(1);
    }
    if (tcsetattr(fd, 0, &old) == -1)
        perror("tcsetattr");
    close(fd);

}

static void die(int x)
{
    printf("caught signal %d, cleaning up...\n", x);
    clean_up();
    exit(1);
}

int main()
{
    int n, i;
    fd = open("/dev/tty0", O_RDONLY);
    signal(SIGHUP, die);
    signal(SIGINT, die);
    signal(SIGQUIT, die);
    signal(SIGILL, die);
    signal(SIGTRAP, die);
    signal(SIGABRT, die);
    signal(SIGFPE, die);
    signal(SIGKILL, die);
    signal(SIGUSR1, die);
    signal(SIGUSR2, die);
    signal(SIGTERM, die);
    signal(SIGCONT, die);
    signal(SIGSTOP, die);
    signal(SIGTSTP, die);
    signal(SIGTTIN, die);
    signal(SIGTTOU, die);
    unsigned char buf[16];

    struct termios new;

    tcgetattr(fd, &old);
    tcgetattr(fd, &new);

    new.c_lflag &= ~(ICANON | ECHO | ISIG);
    new.c_lflag |= (ECHO | ECHOCTL);
    new.c_iflag = 0;
    new.c_cc[VMIN] = sizeof(buf);
    new.c_cc[VTIME] = 1;

    int show_keycodes = 0;
    if (ioctl(fd, KDSKBMODE, show_keycodes ? K_MEDIUMRAW : K_RAW)) {
        perror("KDSKBMODE");
        exit(1);
    }

    tcsetattr(fd, TCSAFLUSH, &new);
    bool ctrlp = false;

    pid_t pid;
    char *token;
    unsigned int inc = BUFSIZE;
    char *pasta; 
        pasta = malloc(inc);
    
    int link[2] = { 0 };
    puts("\nRunning..\n");
    while (1) {
        n = read(fd, buf, sizeof(buf));
        for (i = 0; i < n; i++) {
            if (buf[i] == CTRL_press) {
                ctrlp = true;
            }
            if (buf[i] == CTRL_release) {
                ctrlp = false;
            }
            if (ctrlp && buf[i] == C_press) {
                printf("\tClipboard copy!\n");
                if (pipe(link) == -1)
                    perror("pipe");
                if ((pid = fork()) == -1)
                    perror("fork");
                if (pid == 0) {
                    dup2(link[1], STDOUT_FILENO);
                    close(link[0]);
                    close(link[1]);
                    execl("/usr/bin/xsel", "xsel", "-ob",
                          (char *)0);
                    perror("execl");
                } else {
                    close(link[1]);
                    buffering(link[0], &pasta, BUFSIZE);
                    close(link[0]);
                    token = strtok(pasta, seperator);
                    int status;
                    wait(&status);
                }
            }
            if (ctrlp && buf[i] == V_press) {
                if (token != NULL) {
                    char *temp = malloc(strlen(token) + 2);
                    sprintf(temp, " %s", token); // prepend space
                    printf
                        ("\nSet inside the clipboard:%s\n",
                         temp);
                    if (pipe(link) == -1)
                        perror("\npipe error");
                    pid = fork();
                    switch (pid) {
                    case -1:
                        printf("\nfork error");
                        break;
                    case 0:
                        dup2(link[0], STDIN_FILENO);
                        close(link[1]);
                        execl("/usr/bin/xsel", "xsel",
                              "-ib", NULL);
                        break;
                    default:
                        close(link[0]);
                        write(link[1], temp,
                              strlen(temp));
                        close(link[1]);
                        wait(NULL);
                        free(temp);
                        break;
                    }
                    token = strtok(NULL, seperator);
                } else {
                    if (pipe(link) == -1)
                        perror("\npipe error\n");
                    pid = fork();
                    switch (pid) {
                    case -1:
                        printf("\nfork error\n");
                        break;
                    case 0:
                        dup2(link[0], STDIN_FILENO);
                        close(link[1]);
                        execl("/usr/bin/xsel", "xsel",
                              "-ib", NULL);
                        break;
                    default:
                        close(link[0]);
                        write(link[1], " ",
                              strlen(" "));
                        close(link[1]);
                        wait(NULL);
                        break;
                    }
                }
            }
        }
    }
    tcsetattr(fd, 0, &old);
    free(pasta);
    close(fd);
}
