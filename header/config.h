#include <unistd.h>
#include <string>

FILE* CONFIG_FILE = NULL;
std::string CONFIG_PATH;
int CONFIG_MAX;

void configCreate(const char* path, int size)
{
    if (access(path, F_OK) != -1) return;

    FILE* ptr = fopen(path, "w");
    if (ptr == NULL)
    {
        printf("Could not create file \"%s\".", path);
        exit(1);
    }

    fwrite(&size, sizeof(int), 1, ptr);

    for (int i = 0 ; i < size; i++)
    {
        int temp = 0;
        fwrite(&temp, sizeof(int), 1, ptr);
    }

    fclose(ptr);
}

void configOpen(const char* path)
{
    CONFIG_FILE = fopen(path, "r+");
    if (CONFIG_FILE == NULL)
    {
        printf("Could not open file \"%s\".", path);
        exit(1);
    }
    CONFIG_PATH = path;
    fread(&CONFIG_MAX, sizeof(int), 1, CONFIG_FILE);
}
void configClose()
{
    CONFIG_PATH = "";
    fclose(CONFIG_FILE);
    CONFIG_FILE = NULL;
    CONFIG_MAX = 0;
}

int configGet(int index)
{
    if ( index < 0 || index > CONFIG_MAX ) return -1;

    fseek(CONFIG_FILE, index * sizeof(int), SEEK_SET);
    int ret = -1;
    fread(&ret, sizeof(int), 1, CONFIG_FILE);
    return ret;
}

void configSet(int index, int value)
{
    if ( index < 0 || index > CONFIG_MAX ) return;

    fseek(CONFIG_FILE, index * sizeof(int), SEEK_SET);
    fwrite(&value, sizeof(value), 1, CONFIG_FILE);
}