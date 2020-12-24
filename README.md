
# pmon - Process Monitor


Just a small program to measure execution time to be used when we forget to "time" our execution.

## Linux Only

For Linux systems, tested on Ubuntu 20.04

## Example

```
$ ps aux | grep man
cabeca     30352  0.0  0.0  10080  4288 pts/3    S+   22:57   0:00 man ioctl_list
$ ./pmon -p 30352 -t
Process run for 0 days, 0:2:32
$ 
```

## How it works

On Linux systems there is a special directory, /proc/, well described in the man page proc (5):

> The proc filesystem is a pseudo-filesystem wich provides an interface to kernel data structures.

One of the process special files in /proc is the file /proc/[pid]/stat:

> Status information about the process. This is used by ps(1). It is defined in the kernel source file fs/proc/array.c

This file have several fields, the field number 22 is the start time of the process, expressed in clock ticks.

Another special file is the file /proc/uptime, wich contains the system uptime also in seconds.

Read this files, apply a little bit of math, and done.

