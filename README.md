
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





