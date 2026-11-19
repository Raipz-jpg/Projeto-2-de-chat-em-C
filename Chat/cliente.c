#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define PORTA 8080
#define TAM_BUFFER 512

SOCKET sock_servidor;
int executando = 1;

DWORD WINAPI receber_mensagens(LPVOID arg) {
    char buffer[TAM_BUFFER];
        int bytes_lidos;

            while (executando && (bytes_lidos = recv(sock_servidor, buffer, TAM_BUFFER - 1, 0)) > 0) {
                    buffer[bytes_lidos] = '\0';
                            printf("%s", buffer);
                                    fflush(stdout);
                                        }

                                            if (executando) {
                                                    printf("\n[Sistema] Conexao encerrada pelo servidor.\n");
                                                            executando = 0;
                                                                }
                                                                    return 0;
                                                                    }

                                                                    int main() {
                                                                        WSADATA wsa;
                                                                            char nickname[32];
                                                                                struct sockaddr_in endereco_srv;
                                                                                    HANDLE hThread;
                                                                                        char texto[400];
                                                                                            char mensagem_formatada[TAM_BUFFER];

                                                                                                if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
                                                                                                        printf("Erro ao iniciar Winsock.\n");
                                                                                                                return 1;
                                                                                                                    }

                                                                                                                        printf("Digite seu nickname: ");
                                                                                                                            fgets(nickname, sizeof(nickname), stdin);
                                                                                                                                nickname[strcspn(nickname, "\n")] = '\0';

                                                                                                                                    sock_servidor = socket(AF_INET, SOCK_STREAM, 0);
                                                                                                                                        if (sock_servidor == INVALID_SOCKET) {
                                                                                                                                                printf("Falha ao criar socket.\n");
                                                                                                                                                        WSACleanup();
                                                                                                                                                                return 1;
                                                                                                                                                                    }

                                                                                                                                                                        endereco_srv.sin_family = AF_INET;
                                                                                                                                                                            endereco_srv.sin_port = htons(PORTA);
                                                                                                                                                                                endereco_srv.sin_addr.s_addr = inet_addr("127.0.0.1");

                                                                                                                                                                                    if (connect(sock_servidor, (struct sockaddr *)&endereco_srv, sizeof(endereco_srv)) == SOCKET_ERROR) {
                                                                                                                                                                                            printf("Nao foi possivel conectar ao servidor.\n");
                                                                                                                                                                                                    closesocket(sock_servidor);
                                                                                                                                                                                                            WSACleanup();
                                                                                                                                                                                                                    return 1;
                                                                                                                                                                                                                        }

                                                                                                                                                                                                                            printf("Conectado! Digite suas mensagens (ou /sair para encerrar):\n");

                                                                                                                                                                                                                                hThread = CreateThread(NULL, 0, receber_mensagens, NULL, 0, NULL);
                                                                                                                                                                                                                                    if (hThread != NULL) {
                                                                                                                                                                                                                                            CloseHandle(hThread);
                                                                                                                                                                                                                                                }

                                                                                                                                                                                                                                                    while (executando) {
                                                                                                                                                                                                                                                            if (fgets(texto, sizeof(texto), stdin) == NULL) break;

                                                                                                                                                                                                                                                                    if (strncmp(texto, "/sair", 5) == 0) {
                                                                                                                                                                                                                                                                                executando = 0;
                                                                                                                                                                                                                                                                                            snprintf(mensagem_formatada, sizeof(mensagem_formatada), "[%s saiu do chat]\n", nickname);
                                                                                                                                                                                                                                                                                                        send(sock_servidor, mensagem_formatada, (int)strlen(mensagem_formatada), 0);
                                                                                                                                                                                                                                                                                                                    break;
                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                    snprintf(mensagem_formatada, sizeof(mensagem_formatada), "%s: %s", nickname, texto);
                                                                                                                                                                                                                                                                                                                                            send(sock_servidor, mensagem_formatada, (int)strlen(mensagem_formatada), 0);
                                                                                                                                                                                                                                                                                                                                                }

                                                                                                                                                                                                                                                                                                                                                    closesocket(sock_servidor);
                                                                                                                                                                                                                                                                                                                                                        WSACleanup();
                                                                                                                                                                                                                                                                                                                                                            printf("Chat encerrado.\n");

                                                                                                                                                                                                                                                                                                                                                                return 0;
                                                                                                                                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                                                                                                                                                