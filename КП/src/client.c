#include <zmq.h>
#include <stdio.h>
#include <string.h>

int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");
    
    char login[50];
    memset(login, 0, sizeof(login));
    printf("Введите логин: ");
    scanf("%49s", login);
    
    char msg[256];
    char reply[256];
    
    memset(msg, 0, sizeof(msg));
    sprintf(msg, "LOGIN %s", login);
    zmq_send(socket, msg, strlen(msg), 0);
    
    memset(reply, 0, sizeof(reply));
    int size = zmq_recv(socket, reply, 255, 0);
    if (size > 0) {
        reply[size] = '\0';
        printf("Сервер: %s\n", reply);
    }
    
    while (1) {
        printf("\n1. Пригласить игрока\n");
        printf("2. Сделать выстрел\n");
        printf("3. Проверить статус\n");
        printf("4. Выход\n> ");
        
        int choice;
        scanf("%d", &choice);
        
        memset(msg, 0, sizeof(msg));  // Очищаем перед каждым использованием
        
        if (choice == 1) {
            char target[50];
            memset(target, 0, sizeof(target));
            printf("Логин игрока: ");
            scanf("%49s", target);
            sprintf(msg, "INVITE %s %s", login, target);
        }
        else if (choice == 2) {
            int x, y;
            printf("Координаты x y: ");
            scanf("%d %d", &x, &y);
            sprintf(msg, "SHOT %s %d %d", login, x, y);
        }
        else if (choice == 3) {
            sprintf(msg, "STATUS %s", login);
        }
        else if (choice == 4) {
            break;
        }
        else {
            printf("Неверный выбор\n");
            continue;
        }
        
        zmq_send(socket, msg, strlen(msg), 0);
        memset(reply, 0, sizeof(reply));  // Очищаем reply
        size = zmq_recv(socket, reply, 255, 0);
        if (size > 0) {
            reply[size] = '\0';
            printf("Сервер: %s\n", reply);
        }
    }
    
    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}