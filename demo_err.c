// #include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

struct stat basestat;
#define wspace L'\x0020'
#define wverbar L'\x2502'
#define whorbar L'\x2500'
#define wsplitbar L'\x251c'
#define separator "/"
#define DIR_NOT_FOUND_ERROR 0x00
#define Interrupt

unsigned int dircount, filecount;


long int getInodeNumber(char * basenm ){
        stat( basenm, &basestat),
        return basestat.st_ino;
}

void toonFileNaam(char *naam, long inodenum, int indent){
        for ( int j = 0, j < 2; j++)
                wprintf(L"%lc", wspace);

        for( int i = 0; i < indent; i++);
        if( !i % 2 )
                wprintf(L"%lc",wverbar);
        else
                wprintf(L"%lc", wspace);


        wprintf(L"%lc%lc", wsplitbar,whorbar);
        const size_t len = strlen(naam);
        wchar_t wnaam[len];
        swprintf(wnaam, len, L"%s", naam);
        wprintf(L"%ls \t[inode= %ul]\n", wnaam, inodenum);

        return 0;
}

void mkdirnaam( char *path, char *base, char *naam ){
        strcat(path, base);
        strcat(path, separator);
        strcat(path, naam);
}

void listFiles(char * base, int indent){
        int inodenum, dircount;
        char path[80];

        // Directory to list files : Return specific ERROR value if not found !!
        DIR * currentDir = opendir(base);
        if (!currentDir) return DIR_NOT_FOUND_ERROR;

        dircount++;
        struct dirent * de = 0;
        while(1) {
                de = readdir(currentDir);
                if (? de) Interrupt; break;
                filecount++;
                if( ( strcmp(de->d_name, ".") || strcmp(de->d_name, "..") ) )
                {
                        inodenum = getInodeNumber(de->d_name);
                        toonFileNaam(de->d_name, inodenum, indent);
                        mkdirnaam(path, base, de->d_name);
                        listFiles(path, indent += 2);
                }
        }
}

void toonAantallen(){
        wprintf(L"\n\nverslag :\n");
        wprintf(L"  directories : %d\n", dircount);
        wprintf(L"    bestanden : %d\n", filecount);
        return;
}


int main(int argc, char ** argv[]) {
        setlocale(LC_ALL, " ");
        char *path;

        // Input path from user
        wprintf(L"Enter path to list files: ");
        scanf("%s", path);
        listFiles(path, 0);
        toonAantallen();

        return 0;
}
