#include <iostream>
#include <string.h>
#include <fstream> 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <google/protobuf/text_format.h>

#include "protobuf.pb.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc, const char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    protocol::Mensagem mensagem;

    int op = 1, var = 1;

    while (var) {
        if (op >= 1 && op <= 4) {
            cout << "Digite o numero da opcao que quer realizar:\n";
            cout << "1 - Enviar uma mensagem\n";
            cout << "2 - Modificar um arquivo de texto\n";
            cout << "3 - Realizar uma soma\n";
            cout << "4 - Finalizar\n";
            cout << "Opcao: ";
        }
        cin >> op;
        cin.ignore(256, '\n');

        protocol::Mensagem* mensagem = new protocol::Mensagem();
        switch (op)
        {
            case 1:
            {
                // Input do usuário para as variáveis do protobuf
                mensagem->set_id(1);
                string msg;
                cout << "Digite a mensagem a ser enviada: ";
                getline(cin, msg);
                mensagem->set_msg(msg);

                cout << "Mensagem em bytes : " << mensagem << endl;

                // Serializa os dados em uma array
                int size = mensagem->ByteSizeLong();
                char* buffer = new char[size];
                mensagem->SerializeToArray(buffer, size);

                // Winsock
                WSADATA wsaData;
                SOCKET ConnectSocket = INVALID_SOCKET;
                struct addrinfo* result = NULL,
                    * ptr = NULL,
                    hints;
                int iResult;

                // Inicializa Winsock
                iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
                ZeroMemory(&hints, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;

                // Define o endereço da conexão
                iResult = getaddrinfo("localhost", "3000", &hints, &result);
                if (iResult != 0) {
                    printf("getaddrinfo failed with error: %d\n", iResult);
                    WSACleanup();
                    return 1;
                }

                // Tenta conexão com o servidor até ser sucedido
                for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
                    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                        ptr->ai_protocol);

                    // Conecta com o servidor
                    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
                    if (iResult == SOCKET_ERROR) {
                        closesocket(ConnectSocket);
                        ConnectSocket = INVALID_SOCKET;
                        continue;
                    }
                    freeaddrinfo(result);

                    // Envia os arquivos no buffer
                    iResult = send(ConnectSocket, buffer, size, 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("Erro ao enviar o pacote: %d\n", WSAGetLastError());
                        closesocket(ConnectSocket);
                        WSACleanup();
                        return 1;
                    }

                    // Recebe os arquivos enviados pelo servidor
                    int numbytes;
                    numbytes = recv(ConnectSocket, buffer, 4096, 0);
                    buffer[numbytes] = '\0';
                    string data = buffer;
                    protocol::Mensagem* mensagemRecebida = new protocol::Mensagem();
                    mensagemRecebida->ParseFromString(data);
                    cout << mensagemRecebida->msg() << "\n" << endl;

                    google::protobuf::ShutdownProtobufLibrary();
                }
                break;
            }
            case 2:
            {
                // Case para modificar um arquivo de texto
                // Input do usuário para as variáveis do protobuf
                mensagem->set_id(2);
                cout << "Digite o nome do arquivo de texto: ";
                string nome;
                getline(cin, nome);
                mensagem->set_nome(nome);
                cout << "Digite o conteudo a ser adicionado: ";
                string conteudo;
                getline(cin, conteudo);
                mensagem->set_conteudo(conteudo);

                cout << "Mensagem em bytes : " << mensagem << endl;

                //Serializa os dados em uma array
                int size = mensagem->ByteSizeLong();
                char* buffer = new char[size];
                mensagem->SerializeToArray(buffer, size);

                cout << buffer << endl;

                // Winsock
                WSADATA wsaData;
                SOCKET ConnectSocket = INVALID_SOCKET;
                struct addrinfo* result = NULL,
                    * ptr = NULL,
                    hints;
                int iResult;

                // Inicializa Winsock
                iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
                ZeroMemory(&hints, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;

                // Define o endereço da conexão
                iResult = getaddrinfo("localhost", "3000", &hints, &result);
                if (iResult != 0) {
                    printf("getaddrinfo failed with error: %d\n", iResult);
                    WSACleanup();
                    return 1;
                }

                // Tenta conexão com o servidor até ser sucedido
                for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
                    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                        ptr->ai_protocol);

                    // Conecta com o servidor
                    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
                    if (iResult == SOCKET_ERROR) {
                        closesocket(ConnectSocket);
                        ConnectSocket = INVALID_SOCKET;
                        continue;
                    }
                    freeaddrinfo(result);

                    // Envia os arquivos no buffer
                    iResult = send(ConnectSocket, buffer, size, 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("Erro ao enviar o pacote: %d\n", WSAGetLastError());
                        closesocket(ConnectSocket);
                        WSACleanup();
                        return 1;
                    }

                    // Recebe os arquivos enviados pelo servidor
                    int numbytes;
                    numbytes = recv(ConnectSocket, buffer, 4096, 0);
                    buffer[numbytes] = '\0';
                    string data = buffer;
                    protocol::Mensagem* mensagemRecebida = new protocol::Mensagem();
                    mensagemRecebida->ParseFromString(data);
                    cout << "Mensagem recebida do servidor: " << endl;
                    cout << "Titulo do arquivo: " << mensagemRecebida->nome() << endl;
                    cout << "Conteudo do arquivo: " << mensagemRecebida->conteudo() << "\n" << endl;

                    google::protobuf::ShutdownProtobufLibrary();
                }
                break;
            }
            case 3:
            {
                // Case para função
                // Input do usuário para as variáveis do protobuf
                mensagem->set_id(3);
                cout << "Digite o primeiro numero: ";
                int num1;
                cin >> num1;
                cin.ignore(256, '\n');
                mensagem->set_num1(num1);
                cout << "Digite o segundo numero: ";
                int num2;
                cin >> num2;
                cin.ignore(256, '\n');
                mensagem->set_num2(num2);

                cout << "Mensagem em bytes : " << mensagem << endl;

                // Serializa os dados em uma array
                int size = mensagem->ByteSizeLong();
                char* buffer = new char[size];
                mensagem->SerializeToArray(buffer, size);

                cout << buffer << endl;

                // Winsock
                WSADATA wsaData;
                SOCKET ConnectSocket = INVALID_SOCKET;
                struct addrinfo* result = NULL,
                    * ptr = NULL,
                    hints;

                int iResult;

                // Inicializa Winsock
                iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
                ZeroMemory(&hints, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;
                hints.ai_flags = AI_PASSIVE;

                // Define o endereço da conexão
                iResult = getaddrinfo("localhost", "3000", &hints, &result);
                if (iResult != 0) {
                    printf("getaddrinfo failed with error: %d\n", iResult);
                    WSACleanup();
                    return 1;
                }

                // Tenta conexão com o servidor até ser sucedido
                for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
                    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                        ptr->ai_protocol);

                    // Conecta com o servidor
                    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
                    if (iResult == SOCKET_ERROR) {
                        closesocket(ConnectSocket);
                        ConnectSocket = INVALID_SOCKET;
                        continue;
                    }
                    freeaddrinfo(result);

                    // Envia os arquivos no buffer
                    iResult = send(ConnectSocket, buffer, size, 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("Erro ao enviar o pacote: %d\n", WSAGetLastError());
                        closesocket(ConnectSocket);
                        WSACleanup();
                        return 1;
                    }

                    // Recebe os arquivos enviados pelo servidor
                    int numbytes;
                    numbytes = recv(ConnectSocket, buffer, 4096, 0);
                    buffer[numbytes] = '\0';
                    string data = buffer;
                    protocol::Mensagem* mensagemRecebida = new protocol::Mensagem();
                    mensagemRecebida->ParseFromString(data);
                    cout << "Mensagem recebida do servidor, soma: " << mensagemRecebida->soma() << "\n" << endl;

                    google::protobuf::ShutdownProtobufLibrary();
                }
                break;
            }
            case 4:
                var = 0;
                break;
        }
    }
    return 0;
}