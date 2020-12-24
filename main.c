
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


unsigned long get_proc_filesize(const char *filename)
{
    struct stat stt;
    int         ret;
    
    // TODO: Check all possible return errors
    ret = stat(filename, &stt);
    
    if ( 0 != ret )
    {
        return -1;
    }

    // stat.st_size return 0 bytes for files in /proc
    // Read byte by byte and count
    // File exist, try open
    int fd = open(filename, O_RDONLY);
    if ( -1 == fd )
    {
        fprintf(stderr, "Impossible to open file %s : %s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // file is open, read field #22 (process starttime)
    char aux;
    int count = 0;

    while ( read(fd, &aux, 1) == 1)
        count++;

    close(fd);


    return count;
    
}


char *read_procfile(const char *filename)
{

    unsigned long file_size = get_proc_filesize(filename);

    if (file_size == 0)
        return 0;

    char *bfr = (char *)malloc(file_size+1);
    memset(bfr, 0, file_size+1);
    if (bfr == NULL)
    {
        fprintf(stderr, "Memory allocation error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int fd = open(filename, O_RDONLY);
    if ( fd < 0 )
    {
        printf("Impossible to open file %s : %s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    read(fd, bfr, file_size);

    return bfr;
}


void monitore(char *pid)
{

    char            filename[255];
    unsigned long   stat_size = 0;
    int             ret = 0;

    sprintf(filename, "/proc/%s/stat", pid);
    char *bfr = read_procfile(filename);

    // Jump 21 fields
    char *aux = bfr;
    int i = 0;
    do
    {
        if ( *aux == ' ')
            i++;
        aux++;
    } while ( i<21 && aux < (bfr+1024) );

    unsigned long start_time = atoi(aux);

    printf("Program started at %lu\n", start_time / sysconf(_SC_CLK_TCK));

    char *up_bfr = read_procfile("/proc/uptime");
    printf("uptime: %s\n", up_bfr);

    /*
seconds = seconds_since_boot - buf->start_time / sysconf(_SC_CLK_TCK);
  if(seconds) pcpu = (used_jiffies * 1000ULL / Hertz) / seconds;


    */


    if ( -1 == ret )
    {
        fprintf(stderr, "Impossible to read stat file : %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ( get_proc_filesize(filename) >= 0 )
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


