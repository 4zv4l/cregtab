# cregtab

file regex extraction to table

## usage

`usage: ./cregtab [regex] [col:width;] [filename]`

## example

```
$ ./cregtab '^([^:]+):[^:]+:([5-9][0-9]{2}|[1-9][0-9]{3,}):[^:]+:[^:]*:[^:]+:([^:]+)$' 'user:16;uid:-7;shell' /etc/passwd
user             |     uid | shell
-                |       - | -
nobody           |   65534 | /usr/sbin/nologin
azz              |    1000 | /bin/zsh
systemd-coredump |     999 | /usr/sbin/nologin
libvirt-qemu     |   64055 | /usr/sbin/nologin
lu               |    1001 | /bin/bash
```

## build

`gcc13 cregtab.c persolib/*.c -O3 -static -o cregtab`

> requires -std=c2x
