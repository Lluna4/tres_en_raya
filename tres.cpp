#include <iostream>
#include <vector>
#include <random>

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
        for (unsigned int x = 0; x < max[i]; x++)
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
        for (unsigned int x = 0; x < (max[i] - (2 - 1)); x++)
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
                else if(reps > 15)
                {
                    tabla[x][y] = 2;
                    break;
                }
            }
            reps++;
        }
    }
}



int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(25, 63);
    std::vector<std::vector<int>> tabla;
    bool playing = true;
    int x = 0;
    int y = 0;
    char *buf = (char *)calloc(3, sizeof(char));

    for(int i = 0; i < 3; i++)
    {
        tabla.push_back({0, 0, 0});
    }
    while (playing == true)
    {
        //x = 0;
        //y = 0;
        system("clear");
        print_tabla(tabla);
        std::cout << "Elige una posicion donde quieras poner tu marca (tienes la x): ";
        std::cin >> buf;
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
        tabla[y][x] = 1;
        bot_play(tabla);

        int wcondition = check_win(tabla);
        if ( wcondition > 0)
        {
            system("clear");
            print_tabla(tabla);
            std::cout << "HA GANADO " << wcondition << std::endl;
            break;
        }
    }
}