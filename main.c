
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


char get_statfile_size(const char *filename)
{
    struct stat stt;
    int         ret;
    
    // TODO: Check all possible return errors
    ret = stat(filename, &stt);
    
    if ( 0 != ret )
    {
        return -1;
    }

    return stt.st_size;
    
}

void monitore(char *pid)
{

    char    filename[255];
    int     stat_size = 0;
    int     ret = 0;

    // TODO: Protect from buffer overflow
    sprintf(filename, "/proc/%s/stat", pid);

    stat_size = get_statfile_size(filename);
    if ( stat_size < 0)
    {
        fprintf(stderr, "Impossible to open %s : %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // File exist, try open
    int fd = open(filename, O_RDONLY);
    if ( -1 == fd )
    {
        fprintf(stderr, "Impossible to open file %s : %s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // file is open, read field #22 (process starttime)
    char *aux = NULL;

    // stat stt.st_size return 0 bytes for /proc files...
    char bfr[1024];
    memset(bfr, 0, 1024);
    ret = read(fd, bfr, 1024);
    close(fd);

    printf("-----------------------\n%s\n----------------------\n", bfr);

    // Jump 21 fields
    aux = bfr;
    int i = 0;
    do
    {
        if ( *aux == ' ')
            i++;
        aux++;
    } while ( i<21 && aux < (bfr+1024) );

    unsigned long start_time = atoi(aux);

    printf("We have %s (%lu)\n", aux, start_time);

    exit(EXIT_SUCCESS);

    if ( -1 == ret )
    {
        fprintf(stderr, "Impossible to read stat file : %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ( get_statfile_size(filename) >= 0 )
    {
        sleep(1);
    }

    printf("Process finished or die.\n");

    // Process die, do the math and print time

}



int main(int argc, char **argv) {

    int     opt;
    char    *pid = NULL;
    char    mon_time = 0;

    while ( (opt = getopt(argc, argv, "p:t")) != -1 )
    {
        switch (opt)
        {
        case 'p':
            // Process ID to monitore
            pid = optarg;
            break;
        
        case 't':
            // Monitore execution time
            mon_time = 1;
            break;
        
        default:
            break;
        }
    }

    if ( NULL == pid || !mon_time )
    {
        fprintf(stderr, "Use %s -p pid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    monitore(pid);

    return 0;
}


