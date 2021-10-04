#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <termios.h>
#include <stdint.h>
#include <string>

typedef uint8_t  BYTE;
typedef struct
{
    BYTE rgbtRed;
    BYTE rgbtGreen;
    BYTE rgbtBlue;
} __attribute__((__packed__))
RGBTRIPLE;

void printColor(std::string text, int colorCode)
{
    if (colorCode == 1) std::cout << "\033[0;31m"; // red
    else if (colorCode == 2) std::cout << "\033[0;32m"; // green
    else if (colorCode == 3) std::cout << "\033[0;33m"; // yellow
    else if (colorCode == 4) std::cout << "\033[0;34m"; // blue
    else if (colorCode == 5) std::cout << "\033[0;35m"; // magenta
    else if (colorCode == 6) std::cout << "\033[0;36m"; // cyan
    std::cout << text;
    std::cout << "\033[0m";
}

int menu(std::string opt[], int len) // |= ON, &= OFF
{
    //find longest option
    int lengths[len];
    int longest = 0;
    for (int i = 0; i < len; i++)
    {
        char readChar = 'a';
        int l = 0;
        while (readChar != '\0')
        {
            readChar = opt[i][l];
            l++;
        }
        l--;

        lengths[i] = l;
        if (l > longest) longest = l;
    }

    //edit terminal mode
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag |= ECHO;
    info.c_lflag |= ECHOE;
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);

    int sel = 0;
    while (true)
    {
        //print
        clear();
        //print box top
        for (int i = 0; i < longest + 4; i++) std::cout << "-";
        std::cout << "\n";

        //print options
        for (int i = 0; i < len; i++)
        {
            //print box left edge
            std::cout << "|";

            //is selected left
            if (i == sel) std::cout << "[";
            else std::cout << " ";

            //print left gap
            for (int j = 0; j < floor( (float)( longest - lengths[i] ) / 2.0 ); j++)
            {
                std::cout << " ";
            }

            std::cout << opt[i];

            //print right gap
            for (int j = 0; j < ceil( (float)( longest - lengths[i] ) / 2.0 ); j++)
            {
                std::cout << " ";
            }

            //is selected right
            if (i == sel) std::cout << "]";
            else std::cout << " ";

            //print box right edge
            std::cout << "|";

            //new line
            std::cout << "\n";
        }
        //print box bottom
        for (int i = 0; i < longest + 4; i++) std::cout << "-";
        std::cout << "\n";

        //get char
        char c = getchar();
        if (c == '\033') // special character
        {
            getchar();
            c = getchar();

            if (c == 'A' && sel > 0) sel--; // up arrow
            else if (c == 'B' && sel < len - 1) sel++; // down arrow
        }
        else if (c == 10) break; // enter
    }

    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);

    clear();
    return sel;
}
int menuT(std::string title, int tLen, std::string opt[], int len) // |= ON, &= OFF
{
    //find longest option
    int lengths[len];
    int longest = 0;
    for (int i = 0; i < len; i++)
    {
        char readChar = 'a';
        int l = 0;
        while (readChar != '\0')
        {
            readChar = opt[i][l];
            l++;
        }
        l--;

        lengths[i] = l;
        if (l > longest) longest = l;
    }

    if (tLen > longest) longest = tLen;

    //edit terminal mode
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag |= ECHO;
    info.c_lflag |= ECHOE;
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);

    int sel = 0;
    while (true)
    {
        //print
        clear();
        //print box top
        for (int i = 0; i < longest + 4; i++) std::cout << "-";
        std::cout << "\n";

        //printf title
        std::cout << "| ";
        for (int j = 0; j < floor( (float)( longest - tLen ) / 2.0 ); j++)
        {
            std::cout << " ";
        }
        std::cout << title;
        for (int j = 0; j < floor( (float)( longest - tLen ) / 2.0 ); j++)
        {
            std::cout << " ";
        }
        std::cout << " |\n";
        //print options
        for (int i = 0; i < len; i++)
        {
            //print box left edge
            std::cout << "|";

            //is selected left
            if (i == sel) std::cout << "[";
            else std::cout << " ";

            //print left gap
            for (int j = 0; j < floor( (float)( longest - lengths[i] ) / 2.0 ); j++)
            {
                std::cout << " ";
            }

            std::cout << opt[i];

            //print right gap
            for (int j = 0; j < ceil( (float)( longest - lengths[i] ) / 2.0 ); j++)
            {
                std::cout << " ";
            }

            //is selected right
            if (i == sel) std::cout << "]";
            else std::cout << " ";

            //print box right edge
            std::cout << "|";

            //new line
            std::cout << "\n";
        }
        //print box bottom
        for (int i = 0; i < longest + 4; i++) std::cout << "-";
        std::cout << "\n";

        //get char
        char c = getchar();
        if (c == '\033') // special character
        {
            getchar();
            c = getchar();

            if (c == 'A' && sel > 0) sel--; // up arrow
            else if (c == 'B' && sel < len - 1) sel++; // down arrow
        }
        else if (c == 10) break; // enter
    }

    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);

    clear();
    return sel;
}

int printImg(RGBTRIPLE** px, int width, int height)
{
    printColor("########## Color Output ##########\n\n", 6);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int best = 0;
            float diff = 1000;

            //red
            if (diff > (abs(px[x][y].rgbtRed - 255) + px[x][y].rgbtGreen + px[x][y].rgbtBlue) / 3)
            {
                diff = (abs(px[x][y].rgbtRed - 255) + px[x][y].rgbtGreen + px[x][y].rgbtBlue) / 3;
                best = 1;
            }
            //green
            if (diff > (px[x][y].rgbtRed + abs(px[x][y].rgbtGreen - 255) + px[x][y].rgbtBlue) / 3)
            {
                diff = (px[x][y].rgbtRed + abs(px[x][y].rgbtGreen - 255) + px[x][y].rgbtBlue) / 3;
                best = 2;
            }
            //blue
            if (diff > (px[x][y].rgbtRed + px[x][y].rgbtGreen + abs(px[x][y].rgbtBlue - 255)) / 3)
            {
                diff = (px[x][y].rgbtRed + px[x][y].rgbtGreen + abs(px[x][y].rgbtBlue - 255)) / 3;
                best = 4;
            }

            printColor("#", best);
        }
        std::cout << "\n";
    }

    return 0;
}