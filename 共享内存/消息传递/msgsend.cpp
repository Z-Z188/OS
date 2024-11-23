#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct msg_st {
    long int msg_type;
    char text[MAX_TEXT];
};

int main() {
    int running = 1;
    struct msg_st data;
    char buffer[BUFSIZ];
    int msgid = -1;

    // 建立消息队列
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // 消息队列中写入消息，直到写入 end
    while (running) {
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);

        data.msg_type = 2; // 法定2
        strncpy(data.text, buffer, MAX_TEXT);

        // 向消息队列发送数据
        if (msgsnd(msgid, (void *)&data, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        // 输入 end 结束发送
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}
