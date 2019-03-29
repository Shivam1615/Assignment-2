#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void printdir(char *dir)
{
    DIR *d;
    struct dirent *entry;
    struct stat check;

    if((d = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(d)) != NULL) {
        lstat(entry->d_name,&check);
        if(S_ISDIR(check.st_mode)) {
            
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%s/",entry->d_name);
            
            printdir(entry->d_name);
        }
        else printf("%s\n",entry->d_name);
    }
    chdir("..");
    closedir(d);
}

int main(int argc, char* argv[])
{
        char *currentDir, x[2]=".";
        currentDir=x;

        printdir(currentDir);

        return 0;
}
