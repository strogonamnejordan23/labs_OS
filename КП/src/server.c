#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    zmq_bind(socket, "tcp://*:5555");
    printf("Сервер запущен\n");

    char game_player1[50] = "";
    char game_player2[50] = "";
    int game_active = 0;
    char turn[50] = "";
    int ships1[10][10] = {0};
    int ships2[10][10] = {0};

    while (1) {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        int size = zmq_recv(socket, buffer, 255, 0);
        
        char cmd[20] = "";
        char login[50] = "";
        char target[50] = "";
        char reply[256];
        memset(reply, 0, sizeof(reply));
        
        if (size > 0) {
            buffer[size] = '\0';
            sscanf(buffer, "%s %s %s", cmd, login, target);
        }

        if (strcmp(cmd, "LOGIN") == 0) {
            sprintf(reply, "OK %s", login);
        }
        else if (strcmp(cmd, "INVITE") == 0) {
            if (game_active) {
                sprintf(reply, "ERROR Уже есть игра");
            } 
            else if (strcmp(login, target) == 0) {
                sprintf(reply, "ERROR Нельзя пригласить себя");
            }
            else {
                strcpy(game_player1, login);
                strcpy(game_player2, target);
                strcpy(turn, login);
                game_active = 1;
                
                for (int i = 0; i < 5; i++) {
                    ships1[i][i] = 1;      
                    ships2[i][9-i] = 1;   
                }
                
                sprintf(reply, "OK Игра с %s создана", target);
            }
        }
        else if (strcmp(cmd, "SHOT") == 0) {
            int x, y;
            sscanf(buffer, "%*s %*s %d %d", &x, &y);
            
            if (!game_active) {
                sprintf(reply, "ERROR Нет игры");
            }
            else if (x < 0 || x >= 10 || y < 0 || y >= 10) {
                sprintf(reply, "ERROR Координаты 0-9");
            }
            else if (strcmp(turn, login) != 0) {
                sprintf(reply, "ERROR Ход %s", turn);
            }
            else {
                int hit = 0;
                
                if (strcmp(login, game_player1) == 0) {
                    if (ships2[x][y]) { 
                        ships2[x][y] = 0; 
                        hit = 1; 
                    }
                    strcpy(turn, game_player2);
                } 
                else {
                    if (ships1[x][y]) { 
                        ships1[x][y] = 0; 
                        hit = 1; 
                    }
                    strcpy(turn, game_player1);
                }
                
                sprintf(reply, "%s Ход: %s", hit ? "HIT" : "MISS", turn);
            }
        }
        else if (strcmp(cmd, "STATUS") == 0) {
            if (!game_active) {
                sprintf(reply, "IDLE");
            }
            else if (strcmp(login, game_player1) == 0 || 
                     strcmp(login, game_player2) == 0) {
                if (strcmp(turn, login) == 0) {
                    sprintf(reply, "YOUR_TURN");
                } else {
                    sprintf(reply, "ENEMY_TURN");
                }
            } else {
                sprintf(reply, "IDLE");
            }
        }
        else {
            sprintf(reply, "ERROR");
        }
        
        zmq_send(socket, reply, strlen(reply), 0);
    }
    
    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}