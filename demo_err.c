#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

#define wspace L'\x0020'
#define wverbar L'\x2502'
#define whorbar L'\x2500'
#define wsplitbar L'\x251C'
#define separator "/"
#define DIR_NOT_FOUND_ERROR 0x00
#define Interrupt

unsigned int dircount = 0, filecount = 0;

long int getInodeNumber(char *basenm)
{
    struct stat mstat = {0};
    stat(basenm, &mstat);
    return mstat.st_ino;
}

int toonFileNaam(char *naam, long inodenum, int indent)
{
    for (int j = 0; j < 2; j++)
        wprintf(L"%lc", wspace);

    for (int i = 0; i < indent; i++)
        if (!i % 2)
            wprintf(L"%lc", wverbar);
        else
            wprintf(L"%lc", wspace);

    wprintf(L"%lc%lc", wsplitbar, whorbar);
    size_t len = strlen(naam);
    wchar_t wnaam[len + 1];
    memset(wnaam, 0, len);
    swprintf(wnaam, len + 1, L"%s", naam);
    wprintf(L"%ls \t[inode=%lu]\n", wnaam, inodenum);

    return 0;
}

void mkdirnaam(char *path, char *base, char *naam)
{
    strcat(path, base);
    strcat(path, separator);
    strcat(path, naam);
    path[strlen(base) + strlen(separator) + strlen(naam)] = '\0';
}

int listFiles(char *base, int indent)
{
    DIR *currentDir = opendir(base);
    if (NULL == currentDir)
        return DIR_NOT_FOUND_ERROR;
    dircount++;
    struct dirent *de = 0;
    while (NULL != (de = readdir(currentDir)))
    {
        if (!(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0))
        {
            if (DT_DIR != de->d_type)
                filecount++;
            char path[80] = {};
            mkdirnaam(path, base, de->d_name);
            int inodenum = getInodeNumber(path);
            toonFileNaam(path, inodenum, indent);
            int err = listFiles(path, indent + 2);
            if (0 != err)
                return err;
        }
    }
    closedir(currentDir);
    return 0;
}

void toonAantallen()
{
    wprintf(L"\n\nverslag :\n");
    wprintf(L"  directories : %d\n", dircount);
    wprintf(L"    bestanden : %d\n", filecount);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, " ");
    char path[80] = {};
    // Input path from user
    wprintf(L"Enter path to list files: ");
    scanf("%s", path);
    int err = listFiles(path, 0);
    toonAantallen();
    return err;
}
