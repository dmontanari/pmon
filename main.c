
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


char stat_file_exist(const char *filename)
{
    struct stat stt;
    int         ret;
    
    // TODO: Check all possible return errors
    ret = stat(filename, &stt);
    
    return ( ret == 0 );
    
}

void monitore(char *pid)
{

    char filename[255];

    // TODO: Protect from buffer overflow
    sprintf(filename, "/proc/%s/stat", pid);

    if ( !stat_file_exist(filename))
    {
        printf("Impossible to open %s : %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // File exist, try open
    int fd = open(filename, O_RDONLY);
    if ( -1 == fd )
    {
        printf("Impossible to open file %s : %s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // file is open, read field #22 (process starttime)
    char bfr[512];

    memset(bfr, 0, sizeof(bfr));

    while ( stat_file_exist(filename) ) 
    {
        sleep(1);
    }

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


