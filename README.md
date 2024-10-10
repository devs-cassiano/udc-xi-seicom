
# Keylogger e Servidor TCP

Este projeto consiste em um simples keylogger que captura as teclas pressionadas no sistema e envia para um servidor via conexão TCP. O servidor, por sua vez, recebe as teclas e as exibe no terminal.

## Descrição

- **Cliente (Keylogger)**: Captura as teclas digitadas no Windows e as envia ao servidor.
- **Servidor**: Recebe as teclas capturadas e as exibe no terminal, rodando no Linux (Parrot OS, por exemplo).

### Funcionalidades:
- Captura de teclas: Apenas letras, números e a tecla de espaço são capturadas.
- Detecção de teclas especiais como **Shift** e **Caps Lock** para garantir a captura correta de letras maiúsculas e minúsculas.
- Envio em tempo real das teclas pressionadas para o servidor via TCP.

## Requisitos

### Cliente (Windows)
- Windows
- MinGW instalado para compilar o cliente.
- Biblioteca `ws2_32` para sockets.

### Servidor (Linux - Parrot OS)
- Compilador `g++` instalado.
- Funções padrão POSIX para manipulação de sockets.

## Instruções de Compilação

### Cliente (Windows)

1. Instale o MinGW (se ainda não estiver instalado).
2. Para compilar o código do cliente (`keylogger_client.cpp`):

```bash
x86_64-w64-mingw32-g++ keylogger_client.cpp -o keylogger_client.exe -lws2_32
```

### Servidor (Linux - Parrot OS)

1. Para compilar o código do servidor (`keylogger_server.cpp`) no Parrot OS:

```bash
g++ keylogger_server.cpp -o keylogger_server
```

## Instruções de Execução

### Executar o Servidor no Parrot OS (Linux)

1. Compile o servidor seguindo as instruções acima.
2. Execute o servidor no terminal:

```bash
./keylogger_server
```

O servidor ficará aguardando conexões de clientes na porta **5555**.

### Executar o Cliente no Windows

1. Compile o cliente conforme as instruções.
2. Execute o cliente no Windows:

```bash
keylogger_client.exe
```

O cliente tentará se conectar ao servidor no IP e porta configurados no código. Certifique-se de que o IP do servidor no código do cliente seja o correto.

### Personalizando o Código

- **Configuração do IP do Servidor**: No código do cliente, você pode alterar o IP para o IP da máquina onde o servidor está sendo executado.
  ```cpp
  #define SERVER_IP "127.0.0.1"  // Endereço IP do servidor
  ```

- **Porta do Servidor**: Por padrão, o servidor escuta na porta **5555**, mas isso pode ser alterado no código do cliente e do servidor para outra porta, se necessário.

## Aviso Legal

Este projeto é apenas para fins educacionais. **O uso de keyloggers fora de um ambiente controlado, sem permissão explícita dos usuários, é ilegal**. Certifique-se de obter permissão e cumprir todas as leis aplicáveis ao executar ou modificar este código.
