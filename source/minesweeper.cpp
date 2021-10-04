#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>

#include "misc.h"
#include "input.h"
#include "graphics.h"
#include "config.h"

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

void start();
void init();
void draw();
void moveCur();
void openMenu();
void pauseMenu();
void saveBoard();
void loadBoard();
void win();
void multiExpose(int, int);
int numColor(int);

int tiles[MAX_WIDTH][MAX_HEIGHT];
bool hidden[MAX_WIDTH][MAX_HEIGHT]; // -1: mine, 0: blank, 1->: number
bool flagged[MAX_WIDTH][MAX_HEIGHT];
bool checked[MAX_WIDTH][MAX_HEIGHT];

int BOARD_WIDTH = 9;
int BOARD_HEIGHT = 9;
int MINES = 10;
long SEED = 0;

int curX = 0;
int curY = 0;

int main(int argc, std::string argv[])
{
    configCreate("settings.cfg", 3);
    configOpen("settings.cfg");

    BOARD_WIDTH = configGet(1);
    BOARD_HEIGHT = configGet(2);
    MINES = configGet(3);

    openMenu();
}

void start()
{
    init();
    while (true)
    {
        draw();
        moveCur();
    }
}

void init()
{
    curX = 0;
    curY = 0;

    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        hidden[x][y] = true;
        flagged[x][y] = false;
        checked[x][y] = false;
    }


    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        tiles[x][y] = 0;
    }

    for (int i = 0; i < MINES; i++)
    {
        tiles[(int)round((rand() / (float)RAND_MAX) * BOARD_WIDTH)][(int)round((rand() / (float)RAND_MAX) * BOARD_HEIGHT)] = -1;
    }

    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        if (tiles[x][y] != -1)
        {
            int num = 0;

            if (x == BOARD_WIDTH - 1)
            {
                if (y == BOARD_HEIGHT - 1)
                {
                    if (tiles[x][y - 1] == -1) num++;
                    if (tiles[x - 1][y - 1] == -1) num++;
                    if (tiles[x - 1][y] == -1) num++;
                }
                else if (y == 0)
                {
                    if (tiles[x][y + 1] == -1) num++;
                    if (tiles[x - 1][y + 1] == -1) num++;
                    if (tiles[x - 1][y] == -1) num++;
                }
                else
                {
                    if (tiles[x - 1][y + 1] == -1) num++;
                    if (tiles[x - 1][y] == -1) num++;
                    if (tiles[x - 1][y - 1] == -1) num++;
                    if (tiles[x][y + 1] == -1) num++;
                    if (tiles[x][y - 1] == -1) num++;
                }
            }
            else if (x == 0)
            {
                if (y == BOARD_HEIGHT - 1)
                {
                    if (tiles[x][y - 1] == -1) num++;
                    if (tiles[x + 1][y - 1] == -1) num++;
                    if (tiles[x + 1][y] == -1) num++;
                }
                else if (y == 0)
                {
                    if (tiles[x][y + 1] == -1) num++;
                    if (tiles[x + 1][y + 1] == -1) num++;
                    if (tiles[x + 1][y] == -1) num++;
                }
                else
                {
                    if (tiles[x + 1][y + 1] == -1) num++;
                    if (tiles[x + 1][y] == -1) num++;
                    if (tiles[x + 1][y - 1] == -1) num++;
                    if (tiles[x][y + 1] == -1) num++;
                    if (tiles[x][y - 1] == -1) num++;
                }
            }
            else if (y == BOARD_WIDTH - 1)
            {
                if (tiles[x + 1][y - 1] == -1) num++;
                if (tiles[x][y - 1] == -1) num++;
                if (tiles[x - 1][y - 1] == -1) num++;
                if (tiles[x + 1][y] == -1) num++;
                if (tiles[x - 1][y] == -1) num++;
            }
            else if (y == 0)
            {
                if (tiles[x + 1][y + 1] == -1) num++;
                if (tiles[x][y + 1] == -1) num++;
                if (tiles[x - 1][y + 1] == -1) num++;
                if (tiles[x + 1][y] == -1) num++;
                if (tiles[x - 1][y] == -1) num++;
            }
            else
            {
                if (tiles[x - 1][y - 1] == -1) num++;
                if (tiles[x][y - 1] == -1) num++;
                if (tiles[x + 1][y - 1] == -1) num++;

                if (tiles[x - 1][y] == -1) num++;
                if (tiles[x + 1][y] == -1) num++;

                if (tiles[x - 1][y + 1] == -1) num++;
                if (tiles[x][y + 1] == -1) num++;
                if (tiles[x + 1][y + 1] == -1) num++;
            }

            tiles[x][y] = num;
        }
    }
}

void draw()
{
    clear();

    for (int i = 0; i < BOARD_WIDTH + 2; i++) std::cout << "###";
    std::cout << "\n";

    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        std::cout << "## ";
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (x == curX && y == curY)
            {
                printColor("(", 4);

                if (flagged[x][y])
                {
                    std::cout << "f";
                }
                else if (hidden[x][y])
                {
                    std::cout << "#";
                }
                else
                {
                    if (tiles[x][y] == -1) printColor("x", 1);
                    else if (tiles[x][y] == 0) printf(" ");
                    else
                    {
                        char temp[256];
                        sprintf(temp, "%i", tiles[x][y]);
                        printColor(temp, numColor(tiles[x][y]));
                    }
                }

                printColor(")", 4);
            }
            else
            {
                if (flagged[x][y])
                {
                    printf("[f]");
                }
                else if (hidden[x][y])
                {
                    printf("[#]");
                }
                else
                {
                    if (tiles[x][y] == -1) printColor("[x]", 1);
                    else if (tiles[x][y] == 0) printf("[ ]");
                    else
                    {
                        char temp[256];
                        sprintf(temp, "[%i]", tiles[x][y]);
                        printColor(temp, numColor(tiles[x][y]));
                    }
                }
            }
        }
        printf(" ##\n");
    }

    for (int i = 0; i < BOARD_WIDTH + 2; i++) printf("###");
    printf("\n");
}

void moveCur()
{
    char c = awaitInput();
    if (c == '\033')
    {
        inputTime = 0.01;
        c = input();

        if (c == -1)
        {
            pauseMenu();
        }
        else
        {
            c = awaitInput();

            if (!(curX == 0)) if (c == 'D') curX--; // left
            if (!(curX == BOARD_WIDTH - 1)) if (c == 'C') curX++; // right
            if (!(curY == 0)) if (c == 'A') curY--; // up
            if (!(curY == BOARD_HEIGHT - 1)) if (c == 'B') curY++; // down
        }
    }
    else if (c == 10)
    {
        if (rand() / (float)RAND_MAX * 5 == 1)
        {
            hidden[curX][curY] = false;
            hidden[curX + 1][curY + 1] = false;
            hidden[curX + 1][curY - 1] = false;
            hidden[curX - 1][curY + 1] = false;
            hidden[curX - 1][curY - 1] = false;
        }
        else hidden[curX][curY] = false;
        if (tiles[curX][curY] == -1)
        {
            for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
            {
                hidden[x][y] = false;
            }

            draw();
                            printf("##############################################\n");
            printf("##"); printColor("  #####    ####    ####     ##  ##    ##  ", 1); printf("##\n");
            printf("##"); printColor("  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ", 1); printf("##\n");
            printf("##"); printColor("  #####   ##  ##  ##  ##  ##  ##  ##  ##  ", 1); printf("##\n");
            printf("##"); printColor("  ##  ##  ##  ##  ##  ##  ##  ##  ##      ", 1); printf("##\n");
            printf("##"); printColor("  #####    ####    ####   ##  ##  ##  ##  ", 1); printf("##\n");
                            printf("##############################################\n");
            printf("\nPress any key to continue...");
            awaitInput();
            clear();

            openMenu();

            exit(0);
        }
        else if (tiles[curX][curY] == 0)
        {
            multiExpose(curX, curY);
        }
    }
    else if (c == ' ')
    {
        if (flagged[curX][curY]) flagged[curX][curY] = false;
        else flagged[curX][curY] = true;
    }

    bool isWin = true;
    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        if (!(flagged[x][y] && tiles[x][y] == -1))
        {
            if (hidden[x][y])
            {
                isWin = false;
            }
        }
    }
    if (isWin) win();
}

void openMenu()
{
    while (true)
    {
        int x = -1;
        {
            std::string opt[] = { "NEW GAME", "LOAD GAME", "SETTINGS", "EXIT" };
            x = menuT("MAIN MENU", 9, opt, 4);
        }

        if (x == 0)
        {
            start();
        }
        else if (x == 1)
        {
            loadBoard();
            while (true)
            {
                draw();
                moveCur();
            }
        }
        else if (x == 2)
        {
            while (true)
            {
                {
                    std::string opt[] = { "GAME", "SEED", "BACK" };
                    x = menuT("SETTINGS", 8, opt, 3);
                }
                if (x == 0)
                {
                    while (true)
                    {
                        {
                            std::string opt[] = { "BEGINNER", "INTERMEDIATE", "ADVANCED", "CUSTOM", "BACK" };

                            char* g = "";
                            if (BOARD_WIDTH == 9 && BOARD_HEIGHT == 9 && MINES == 10) g = "BEGINNER";
                            else if (BOARD_WIDTH == 16 && BOARD_HEIGHT == 16 && MINES == 40) g = "INTERMEDIATE";
                            else if (BOARD_WIDTH == 24 && BOARD_HEIGHT == 24 && MINES == 99) g = "ADVANCED";
                            else g = "CUSTOM";

                            char* temp = (char*)malloc(40 * sizeof(char));
                            sprintf(temp, "CURRENT: %s", g);
                            int len = 0;
                            while (true)
                            {
                                if (temp[len] == '\0') break;
                                len++;
                            }
                            x = menuT(temp, len, opt, 5);

                            free(temp);
                        }
                        if (x == 0)
                        {
                            BOARD_WIDTH = 9;
                            BOARD_HEIGHT = 9;
                            MINES = 10;
                            configSet(1, BOARD_WIDTH);
                            configSet(2, BOARD_HEIGHT);
                            configSet(3, MINES);
                            break;
                        }
                        else if (x == 1)
                        {
                            BOARD_WIDTH = 16;
                            BOARD_HEIGHT = 16;
                            MINES = 40;
                            configSet(1, BOARD_WIDTH);
                            configSet(2, BOARD_HEIGHT);
                            configSet(3, MINES);
                            break;
                        }
                        else if (x == 2)
                        {
                            BOARD_WIDTH = 24;
                            BOARD_HEIGHT = 24;
                            MINES = 99;
                            configSet(1, BOARD_WIDTH);
                            configSet(2, BOARD_HEIGHT);
                            configSet(3, MINES);
                            break;
                        }
                        else if (x == 3)
                        {
                            clear();
                            BOARD_WIDTH = get_int("Enter width: ");
                            BOARD_HEIGHT = get_int("Enter height: ");
                            MINES = get_int("Enter mines: ");
                            configSet(1, BOARD_WIDTH);
                            configSet(2, BOARD_HEIGHT);
                            configSet(3, MINES);
                            break;
                        }
                        else if (x == 4)
                        {
                            break;
                        }
                    }
                }
                else if (x == 1)
                {
                    clear();
                    SEED = get_long("Enter seed: ");
                    srand(SEED);
                    clear();
                }
                else if (x == 2)
                {
                    break;
                }
            }

        }
        else if (x == 3)
        {
            clear();
            printf("\n\n\n\n\n\n");
            configClose();
            exit(0);
        }
    }
}

void pauseMenu()
{
    int x;
    const std::string opt[] = { "RESUME", "SAVE", "MAIN MENU" };
    x = menuT("PAUSE", 5, opt, 3);

    if (x == 1)
    {
        saveBoard();
    }
    else if (x == 2)
    {
        openMenu();
    }
}

void saveBoard()
{
    std::string pathInput = get_string("Save name: ");
    char* path = (char*)malloc(256 * sizeof(char));
    sprintf(path, "saves/%s.save", pathInput);


    FILE* ptr = fopen(path, "w");
    if (ptr == NULL)
    {
        printf("Could not open file \"%s\".", path);
        exit(1);
    }

    {
        short temp = BOARD_WIDTH;
        fwrite(&temp, sizeof(short), 1, ptr);
        temp = BOARD_HEIGHT;
        fwrite(&temp, sizeof(short), 1, ptr);
        temp = MINES;
        fwrite(&temp, sizeof(short), 1, ptr);
    }

    fwrite(&SEED, sizeof(long), 1, ptr);
    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        fwrite(&hidden[x][y], sizeof(bool), 1, ptr);
    }
    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        fwrite(&flagged[x][y], sizeof(bool), 1, ptr);
    }

    fclose(ptr);
}
void loadBoard()
{
    std::string pathInput = get_string("Save name: ");
    char* path = (char*)malloc(256 * sizeof(char));
    sprintf(path, "saves/%s.save", pathInput);

    FILE* ptr = fopen(path, "r");
    if (ptr == NULL)
    {
        printf("Could not open file \"%s\".", path);
        exit(1);
    }

    fread(&BOARD_WIDTH, sizeof(short), 1, ptr);
    fread(&BOARD_HEIGHT, sizeof(short), 1, ptr);
    fread(&MINES, sizeof(short), 1, ptr);

    fread(&SEED, sizeof(long), 1, ptr);
    srand(SEED);
    init();
    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        fread(&hidden[x][y], sizeof(bool), 1, ptr);
    }
    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++)
    {
        fread(&flagged[x][y], sizeof(bool), 1, ptr);
    }

    fclose(ptr);
}

void win()
{
    clear();
                    printf("#########################################\n");
    printf("##"); printColor("  ##   ##   ##  ######  ##   ##  ##  ", 1); printf("##\n");
    printf("##"); printColor("   ##  ##  ##     ##    ###  ##  ##  ", 1); printf("##\n");
    printf("##"); printColor("   ##  ##  ##     ##    #### ##  ##  ", 1); printf("##\n");
    printf("##"); printColor("    ########      ##    ##  ###      ", 1); printf("##\n");
    printf("##"); printColor("     ##  ##     ######  ##   ##  ##  ", 1); printf("##\n");
                    printf("#########################################\n");
    printf("\nPress any key to continue...");
    awaitInput();
    clear();

    openMenu();
}

void MERecur(int x, int y)
{
    if ( (x < 0 || y < 0 || x > BOARD_WIDTH - 1 || y > BOARD_HEIGHT - 1) || (checked[x][y]) ) return;

    checked[x][y] = true;
    hidden[x][y] = false;

    if (tiles[x][y] == 0)
    {
        MERecur(x - 1, y - 1);
        MERecur(x, y - 1);
        MERecur(x + 1, y - 1);

        MERecur(x - 1, y);
        MERecur(x + 1, y);

        MERecur(x - 1, y + 1);
        MERecur(x, y + 1);
        MERecur(x + 1, y + 1);
    }
}

void multiExpose(int _x, int _y)
{
    for (int y = 0; y < BOARD_HEIGHT; y++) for (int x = 0; x < BOARD_WIDTH; x++) checked[x][y] = false;
    MERecur(_x, _y);
}

int numColor(int x)
{
    switch (x)
    {
        case 1:
            return 4;
        case 2:
            return 2;
        case 3:
            return 1;
        case 4:
            return 5;
        default:
            return 6;
    }
}