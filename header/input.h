#include <termios.h>
#include <string>
#include <iostream>

//| = on, & = off

float inputTime = 0.5;
int input(void)
{
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag |= ECHO;
    info.c_lflag |= ECHOE;
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 0;
    info.c_cc[VTIME] = inputTime;
    tcsetattr(0, TCSANOW, &info);

    char c = getchar();

    info.c_lflag |= ICANON;
    info.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &info);

    return c;
}

int awaitInput(void)
{
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag |= ECHO;
    info.c_lflag |= ECHOE;
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);

    char c = getchar();

    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);

    return c;
}
int get_int(std::string message)
{
    std::cout << message;
    return std::cin.get();
}
long get_long(std::string message)
{
    std::cout << message;
    std::string temp;
    std::cin >> temp;
    return std::stol(temp);
}
std::string get_string(std::string message)
{
    std::cout << message;
    std::string temp;
    std::cin >> temp;
    return temp;
}