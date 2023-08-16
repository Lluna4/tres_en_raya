#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <vector>

int PORT = 5050;
const std::string SERVER_IP = "0.0.0.0";

std::vector<int> clients;

std::vector<std::vector<int>> tabla;

int index = 0;

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

void manage_game(int p1, int p2)
{
    send(p1, "0", 1, 0);
    send(p2, "0", 1, 0);
    char *buf = (char *)calloc(3, sizeof(char));
    int x = 0;
    int y = 0;

    while (true)
    {
        send(p1, "2", 1, 0);
        recv(p1, buf, 2, 0);

        eval(&x, buf);
        y = buf[1] - 48;
        tabla[y][x] = 1;
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
    }

}

void manage_server(int socket)
{
    if (clients.size() % 2 != 0)
    {
        send(socket, "1", 1, 0);
    }
    else
    {
        index += 2;
        std::thread game(manage_game, clients[index-2], clients[index-1]);
        game.detach();
    }
}


int main()
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
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

    for(int i = 0; i < 3; i++)
    {
        tabla.push_back({0, 0, 0});
    }
    std::cout << "Escuchando conexiones" << std::endl;
    while (true)
    {
       
        listen(sock, 100);
        int new_socket = accept(sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        std::cout << new_socket << std::endl;
        clients.push_back(new_socket);
        std::thread man_sv(manage_server, new_socket);
        man_sv.detach();
    }

}