#include <time.h>

#define length( array ) ( sizeof( array ) / sizeof( array[0] ) )

typedef struct
{
    float x;
    float y;
} __attribute__((__packed__))
POINT;

void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void itoa(char* output, int num)
{
    sprintf(output, "%d", num);
}