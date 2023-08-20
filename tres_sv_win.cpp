#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstring>

int PORT = 5050;
const std::string SERVER_IP = "25.53.80.246";

std::vector<SOCKET> clients;

std::vector<std::vector<int>> tabla;

int in = 0;

void print_line(std::vector<int> header)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "+";
        std::cout << "-";
        for (unsigned int x = 0; x < 1; x++)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+";
    std::cout << std::endl;
}

void print_line(std::vector<int> header, std::vector<int> max)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "+";
        std::cout << "-";
        for (int x = 0; x < max[i]; x++)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+";
    std::cout << std::endl;
}

void print_words(std::vector<int> header)
{
    char print_value = ' ';
    for (unsigned int i = 0; i < header.size(); i++)
    {
        switch (header[i])
        {
        case 0:
            print_value = ' ';
            break;
        
        case 1:
            print_value = 'X';
            break;
        
        case 2:
            print_value = 'O';

        default:
            break;
        }
        std::cout << "|" << " " << print_value << " ";
    }
    std::cout << "|";
    std::cout << std::endl;
}

void print_words(std::vector<int> header, std::vector<int> max)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "|" << " " << header[i];
        for (int x = 0; x < (max[i] - (2 - 1)); x++)
            std::cout << " ";
    }
    std::cout << "|";
    std::cout << std::endl;
}

void print_tabla(std::vector<std::vector<int>> tabla)
{
    print_line(tabla[0]);
    for (unsigned int y = 0; y < tabla.size(); y++)
    {
        print_words(tabla[y]);
        print_line(tabla[y]);
    }
}


int check_win(std::vector<std::vector<int>> tabla)
{
    for(int i = 0; i < 3; i++)
    {
        if (tabla[i][0] == tabla[i][1] && tabla[i][1] == tabla[i][2] && tabla[i][0] > 0)
        {
            return tabla[i][0];
        }
    }

    for(int i = 0; i < 3; i++)
    {
        if (tabla[0][i] == tabla[1][i] && tabla[1][i] == tabla[2][i] && tabla[0][i] > 0)
        {
            return tabla[0][i];
        }
    }

    if (tabla[0][0] == tabla[1][1] && tabla[1][1] == tabla[2][2] && tabla[0][0] > 0)
        return tabla[0][0];
    
    if (tabla[0][2] == tabla[1][1] && tabla[1][1] == tabla[2][0] && tabla[0][2] > 0)
        return tabla[0][2];
    return 0;
}

void eval(int *x, char *buf)
{
    switch (buf[0])
    {
    case 'a':
        *x = 0;
        break;
    
    case 'b':
        *x = 1;
        break;
    
    case 'c':
        *x = 2;
        break;
    
    default:
        break;
    }
}

void manage_game(std::vector<SOCKET> players)
{
    //generar tabla
    for(int i = 0; i < 3; i++)
    {
        tabla.push_back({0, 0, 0});
    }
    send(players[0], "0", 1, 0);
    send(players[1], "0", 1, 0);
    char *buf = (char *)calloc(3, sizeof(char));
    int x = 0;
    int y = 0;
    int in1 = 0;
    int in2 = 1;
    SOCKET p1;
    SOCKET p2;

    send(players[0], "1", 1, 0);
    send(players[1], "2", 1, 0);

    while (true)
    {
        print_tabla(tabla);
        p1 = players[in1];
        p2 = players[in2];
        send(p1, "2", 1, 0);
        send(p2, "6", 1, 0);
        recv(p1, buf, 2, 0);

        eval(&x, buf);
        y = buf[1] - 48;
        tabla[y][x] = in1 + 1;
        send(p2, buf, 2, 0);
        int wcondition = check_win(tabla);
        if ( wcondition > 0)
        {
            if (wcondition == 1)
            {
                send(p1, "3", 1, 0);
                send(p2, "4", 1, 0);
            }
            
            if (wcondition == 2)
            {
                send(p1, "4", 1, 0);
                send(p2, "3", 1, 0);
            }
            break;
        }
        if (in1 == 0)
        {
            in1 = 1;
            in2 = 0;
        }
        else if(in1 == 1)
        {
            in1 = 0;
            in2 = 1;
        }
    }
}

void manage_server(SOCKET socket)
{
    if (clients.size() % 2 != 0)
    {
        send(socket, "1", 1, 0);
    }
    else
    {
        //std::cout<< "S" << std::endl;
        in += 2;
        std::vector<SOCKET> players = {clients[in-2], clients[in-1]};
        std::thread game(manage_game, players);
        game.detach();
    }
}


int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == 0)
    {
        std::cout << "Se fallo al crear socket" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP.c_str(), &address.sin_addr);
    address.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        std::cout << "Bind ha fallado" << std::endl;
    }
    std::cout << "Escuchando conexiones" << "en "<< PORT << std::endl;
    while (true)
    {
        std::cout << "AAAA" << std::endl;
        listen(sock, 5);
        int new_socket = accept(sock, (struct sockaddr*)&address, &addrlen);
        std::cout << new_socket << std::endl;
        clients.push_back(new_socket);
        std::thread man_sv(manage_server, new_socket);
        man_sv.detach();
    }

}