#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <string>

int PORT = 5050;
std::string SERVER_IP = "25.53.80.246";

int	ft_atoi(const char* a)
{
    int	ret;
    int	sign;

    ret = 0;
    sign = 1;
    while ((*a >= 9 && *a <= 13) || *a == 32)
    {
        a++;
    }
    if (*a == '+' || *a == '-')
    {
        if (*a == '-')
            sign = -1;
        a++;
    }
    while (*a >= '0' && *a <= '9')
    {
        ret = ret * 10 + (*a - '0');
        a++;
    }
    return (sign * ret);
}


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

/*int check_win(std::vector<std::vector<int>> tabla)
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


void bot_play(std::vector<std::vector<int>> &tabla)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 2);
    std::vector<std::vector<int>> prov_tabla = tabla;
    int reps = 0;
    int x = 0;
    int y = 0;
    
    if (tabla[1][1] == 0)
        tabla[1][1] = 2;
    else
    {
        while (true)
        {
            x = distr(gen);
            y = distr(gen);
            prov_tabla[x][y] = 2;
            if (tabla[x][y] == 0)
            {
                if (check_win(prov_tabla) == 2)
                {
                    tabla[x][y] = 2;
                    break;
                }
                prov_tabla[x][y] = 1;
                if (check_win(prov_tabla) == 1)
                {
                    tabla[x][y] = 2;
                    break;
                }
                if(reps > 15)
                {
                    tabla[x][y] = 2;
                    break;
                }
            }
            reps++;
        }
    }
}*/

int check_tabla(std::string buf, std::vector<std::vector<int>> tabla)
{
    int x = 0;
    int y = 0;

    switch (buf[0])
    {
    case 'a':
        x = 0;
        break;
    
    case 'b':
        x = 1;
        break;
    
    case 'c':
        x = 2;
        break;
    
    default:
        break;
    }
    y = buf[1] - 48;

    if (tabla[y][x] != 0)
    {
        return 1;
    }
    
    return 0;
}


int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }
    SOCKET sock;
    struct sockaddr_in address;
    std::vector<std::vector<int>> tabla;
    //bool playing = true;
    int x = 0;
    int y = 0;
    char *buf = (char *)calloc(3, sizeof(char));
    std::string buff;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "ERROR" << std::endl;
    }
    std::cout << sock << std::endl;
    
    inet_pton(AF_INET, SERVER_IP.c_str(), &address.sin_addr);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    int conn = connect(sock, (struct sockaddr*)&address, sizeof(address));
    std::cout << conn << std::endl;
    recv(sock, buf, 1, 0);
    std::cout << buf << std::endl;
    if (ft_atoi(buf) == 1)
    {
        memset(buf, 0, 3);
        std::cout << "Esperando a partida..." << std::endl;
        recv(sock, buf, 1, 0);
        std::cout << buf << std::endl;
    }
    if (ft_atoi(buf) == 0)
    {
        int player = 0;
        std::cout << "Encontraste partida!" << std::endl;
        for(int i = 0; i < 3; i++)
        {
            tabla.push_back({0, 0, 0});
        }
        memset(buf, 0, 3);
        recv(sock, buf, 1, 0);
        player = ft_atoi(buf);
        memset(buf, 0, 3);
        while (true)
        {
            system("cls");
            print_tabla(tabla);
            memset(buf, 0, 3);
            recv(sock, buf, 1, 0);
            if (ft_atoi(buf) == 3 || ft_atoi(buf) == 4)
            {
                if (ft_atoi(buf) == 3)
                {
                    std::cout << "HAS GANADO!" << std::endl;
                    break;
                }
                std::cout << "HAS PERDIDO!" << std::endl;
                break;
            }
            std::cout << buf << std::endl;
            if (ft_atoi(buf) == 2)
            {
                x = 0;
                y = 0;
                memset(buf, 0, 3);
                
                while (true)
                {
                    std::cout << "Elige una posicion donde quieras poner tu marca: ";
                    std::cin >> buff;
                    if (buff[2] != 0 || (buff[1] - 48) > 2 || !(buff[0] >= 'a' && buff[0] <= 'c') || check_tabla(buff, tabla) == 1)
                    {
                        std::cout << "Input invalido" << std::endl;
                        continue;
                    }
                    break;
                }
                buf = (char *)buff.c_str();
                send(sock, buf, 2, 0);
                switch (buf[0])
                {
                case 'a':
                    x = 0;
                    break;
                
                case 'b':
                    x = 1;
                    break;
                
                case 'c':
                    x = 2;
                    break;
                
                default:
                    break;
                }
                y = buf[1] - 48;
                tabla[y][x] = player;
            }
            if(ft_atoi(buf) == 6)
            {
                memset(buf, 0, 3);
                recv(sock, buf, 2, 0);
                switch (buf[0])
                {
                case 'a':
                    x = 0;
                    break;
                
                case 'b':
                    x = 1;
                    break;
                
                case 'c':
                    x = 2;
                    break;
                
                default:
                    break;
                }
                y = buf[1] - 48;
                switch (player)
                {
                case 1:
                    tabla[y][x] = player + 1;
                    break;
                case 2:
                    tabla[y][x] = player - 1;
                    break;
                default:
                    break;
                }
                memset(buf, 0, 3);
            }
        }
    }
    std::cout << "Pulsa enter para salir...";
    std::cin >> buf;
    
    return 0;
}