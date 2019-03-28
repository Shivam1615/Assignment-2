#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void printdir(char *dir,int depth)
{
    DIR *d;
    struct dirent *entry;
    struct stat check;
    int spaces=depth*4;

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
            printf("%*s%s/\n",spaces,"",entry->d_name);
            
            printdir(entry->d_name,depth+1);
        }
        else printf("%*s%s\n",spaces,"",entry->d_name);
    }
    chdir("..");
    closedir(d);
}

int main(int argc, char* argv[])
{
        char *currentDir, x[2]=".";
        currentDir=x;

        printdir(currentDir,0);

        return 0;
}
