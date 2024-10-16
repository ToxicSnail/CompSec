#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <time.h>
#include <errno.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int signo) {
    // Обработчик сигналов не делает ничего, кроме завершения цикла pselect
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int port = atoi(argv[1]);

    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to the specified port
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) == -1) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }

    // Register signal handler for SIGHUP
    signal(SIGHUP, handle_signal);

    printf("Server is running on port %d\n", port);

    struct timespec timeout = {5, 0}; // 5 seconds
    sigset_t blockedMask, origMask;

    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, SIGHUP);

    // Заблокировать сигнал SIGHUP
    if (sigprocmask(SIG_BLOCK, &blockedMask, &origMask) == -1) {
        perror("sigprocmask error");
        return 1;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(server_sock, &readfds);
    int max_fd = server_sock;

    while (keep_running) {
        // Копируем набор дескрипторов, так как pselect изменяет его
        fd_set temp_fds = readfds;

        // Wait for activity on sockets using pselect
        int ready_fds;
        do {
            ready_fds = pselect(max_fd + 1, &temp_fds, NULL, NULL, &timeout, &origMask);
        } while (ready_fds == -1 && errno == EINTR);

        if (ready_fds == -1) {
            perror("pselect error");
            break;
        } else if (ready_fds == 0) {
            printf("No activity on sockets\n");
        } else {
            // Проверка данных на соксах клиентов
            for (int i = server_sock + 1; i <= max_fd; i++) {
                if (FD_ISSET(i, &temp_fds)) {
                    char buffer[1024];
                    ssize_t bytes_read = recv(i, buffer, sizeof(buffer), 0);

                    if (bytes_read == -1) {
                        perror("Recv failed");
                    } else if (bytes_read == 0) {
                        // Соединение закрыто клиентом
                        printf("Connection closed by client\n");
                        close(i);
                        FD_CLR(i, &readfds);
                    } else {
                        printf("Received %zd bytes from client\n", bytes_read);
                        // Обработка данных, если необходимо
                    }
                }
            }

            // Проверка входящих соединений
            if (FD_ISSET(server_sock, &temp_fds)) {
                if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
                    perror("Accept failed");
                } else {
                    // Логгирование нового соединения
                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
                    printf("Accepted connection from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

                    // Добавление сокса клиента в множество
                    FD_SET(client_sock, &readfds);

                    // Обновление max_fd при необходимости
                    if (client_sock > max_fd) {
                        max_fd = client_sock;
                    }
                }
            }
        }
    }

    // Закрытие всех соксов клиентов
    for (int i = server_sock + 1; i <= max_fd; i++) {
        if (FD_ISSET(i, &readfds)) {
            close(i);
        }
    }

    // Закрытие сокса сервера
    close(server_sock);
    printf("Server is shutting down\n");
    return 0;
}
