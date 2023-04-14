# Linux
## ssh to 178.128.24.75
### remember how to handle keys
- Remember that you must have created a key pair and stuck the public key in ${HOME}/.ssh/authorizedkeys
- Look up how to get ssh to use an alternate private key
- port ends in 69, 2269? idunno so nmap
### uh, I don't actually know the port
```
$ nmap 178.128.24.75
Starting Nmap 7.80 ( https://nmap.org ) at 2023-04-14 11:09 EDT
Nmap scan report for 178.128.24.75
Host is up (0.27s latency).
Not shown: 997 closed ports
PORT    STATE    SERVICE
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
445/tcp filtered microsoft-ds
```
awfully noisy and No joy, it only scans likely suspects
```
for i in {654..1}; do nmap -p "${i}69" 178.128.24.75; done
```
noisy and slow
```
for i in {1..654}; do nc 178.128.24.75 "${i}69"; done
```
still noisy, but faster
```
for i in {654..1}; do nc 178.128.24.75 "${i}69"; echo "${i}"; done
```
won. But let's face it I've made a lot of noise
```
nmap -sS -p- 178.128.24.75
```
Is supposedly less noisy, just sends SYN and calls it good if SYNACK? not really sure why less noisy
...and at the time of this writing I'm still waiting for a result
### actually logging in
```
tallen@lptp:~/Downloads$ ssh -p42069 -i tim.key tim@178.128.24.75
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@         WARNING: UNPROTECTED PRIVATE KEY FILE!          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Permissions 0664 for 'tim.key' are too open.
```
Um, yeah, that's a problem
```
chmod 600 tim.key 

ssh -p42069 -i tim.key tim@178.128.24.75

Welcome to Ubuntu 22.04.2 LTS (GNU/Linux 5.15.0-69-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

  System information as of Fri Apr 14 15:51:39 UTC 2023

  System load:  0.0               Users logged in:       0
  Usage of /:   22.8% of 9.51GB   IPv4 address for eth0: 178.128.24.75
  Memory usage: 40%               IPv4 address for eth0: 10.15.0.5
  Swap usage:   0%                IPv4 address for eth1: 10.130.0.2
  Processes:    92

 * Introducing Expanded Security Maintenance for Applications.
   Receive updates to over 25,000 software packages with your
   Ubuntu Pro subscription. Free for personal use.

     https://ubuntu.com/pro

Expanded Security Maintenance for Applications is not enabled.

6 updates can be applied immediately.
3 of these updates are standard security updates.
To see these additional updates run: apt list --upgradable

Enable ESM Apps to receive additional future security updates.
See https://ubuntu.com/esm or run: sudo pro status


Last login: Thu Apr 13 15:02:13 2023 from 99.10.95.250
```

## 1. What is the absolute path of the file with the permission set: setguid, user: execute, group: write, world: read
Okay is there some variant of find that does perms, I cant remember so man. BTW that sounds slow, if we're on spinning rust.   
Gotta start somewhere so lest go with ```find / -perm /g+s```
```/var/local/r0k4/walter.sobchak```
obtained via
```find / -type f -exec ls -l {} \; | grep -E "^...x.wSr"```
I know. Hardly eligant and I had to wade through a bunch of "Permission denied"

## An aside my ```cd``` is borked and I don't like it
```hash -d cd```
fail
```enable cd```
Joy ...and delete that bloody ```bash_aliases``` file


## 2. How many character devices not owned by the "tty" group are on the system?
```ls -l /dev | grep -E "^c" | cut -c19-25 | wc -l```
the tot num of char files is 152
```ls -l /dev | grep -E "^c" | cut -c19-25 | grep tty | wc -l```
the number with group tty which is 89
so 152 - 89 = 63
That was silly, I coulda dumped in a sqlite DB and done a select
I also borked around with sort, and join but couldn't remember how to make those chooch.  
There was a time when the device files did not have to be in the /dev (technically devfs) but I think that time is passed; nevertheless,
I'll do a find on the whole FS  
```find . -type c ! -group tty -exec ls -l {} \; 2> /dev/null | wc -l```  
okay, well, I gather they still can be outside devfs, I found 91 with this (less silly) method.

## 3. Send a SIGINT to the running program called interview. Useful information will then appear in a log file.
```
ps ax | grep interview
    636 ?        Ss     0:02 /usr/local/interview/interview
 169374 pts/1    S+     0:00 grep --color=auto interview
```
Yeah, I tried /proc/636/mem and then remembered ya gotta be root for that.   
```strings interview
*PASSWORD
The password to
account wi
 superusm
-eP is: Strang&In
~Alps1998
xeon_phi
well
/c~/libc-start.c
```
That's an excerpt. No I was not able to parse the password out of the string and eventually worked up the courage to kill the process (just did a ```killall```), become wilson, and subsequently become root. Is "StrangerInTheAlps1998" a book or a keyboard, I'm gonna go with book since I'm fairly Alps keyboards were long gone by 1998.

## 4. Which user on the system is allowed to execute the tcpdump command as root but no other command?
I don't think this is true... recheck later

## 5. When the system detects a specific USB device being removed from the system, what command is run?
/usr/bin/logger 'device was removed'

## 6. Look inside the filesystem in /srv/filesystem.mnt (password: gowashingtoncapitals). What is the md5 hash of the file inside the filesystem?
```
cryptsetup luksOpen filesystem.mnt grrr
mount /dev/mapper/grrr /mnt
md5sum hashme.bin 
e05b033f8f747142b5dcc7d80dd53878  hashme.bin
```
e05b033f8f747142b5dcc7d80dd53878

## 7. On which minute of the hour does root execute a script that sends a single ping to 127.0.0.19?
```
# m h  dom mon dow   command
14 * * * *  /root/bin/script1 2 127.0.0.19
28 15 3 * * /root/bin/script2 1 127.0.0.19
11 0 * * *  /root/bin/script3 1 127.0.0.91
```
script1 ignores the args and sends a single ping to 127.0.0.19   
script2 takes two args, the count and the host (or IP to ping), since the former is 1 and the latter is 127.0.0.19 it will send a single ping at 1528 on the 3rd DOM   
script3 pings 0.0.0.1 I'm betting that's not what the author intended
So every hour, at 14 past the hour, a ping is sent to 127.0.0.19 and once a month, on the third day of the month, they get a bonus ping at 3:15pm.

## 8. Do whatever you can to harden this system and reduce its attack surface. There are no right or wrong answers here; it's up to you what you consider important.
- turn off all non-essential daemons
```
root@binford:~# netstat -lpn
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 0.0.0.0:42069           0.0.0.0:*               LISTEN      681/sshd: /usr/sbin 
tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN      548/systemd-resolve 
udp        0      0 127.0.0.53:53           0.0.0.0:*                           548/systemd-resolve 
raw        0      0 0.0.0.0:6               0.0.0.0:*               7           632/droplet-agent   
raw6       0      0 :::58                   :::*                    7           529/systemd-network 
raw6       0      0 :::58                   :::*                    7           529/systemd-network 
```
This is a good place to start. The sshd is listening obviously, and I dunno what a droplet-agent is. I so wanna say some choice things about systemd but I'll confine myself to the observation that resolve is only listening on loopback and systemd-network worries me but I can't say why.  


I see no evidence that inetd is running, inetd is used for things like NFS, and sometimes tftp (there is a whole list)

```
lsof -i
COMMAND      PID            USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
systemd-r    548 systemd-resolve   13u  IPv4  18132      0t0  UDP localhost:domain 
systemd-r    548 systemd-resolve   14u  IPv4  18133      0t0  TCP localhost:domain (LISTEN)
sshd         681            root    3u  IPv4  18772      0t0  TCP *:42069 (LISTEN)
sshd      169661            root    4u  IPv4 821569      0t0  TCP binford:42069->c-76-106-42-226.hsd1.va.comcast.net:36444 (ESTABLISHED)
sshd      169714             tim    4u  IPv4 821569      0t0  TCP binford:42069->c-76-106-42-226.hsd1.va.comcast.net:36444 (ESTABLISHED)
```
The cool kids use ```lsof```

I haven't used it in a 1000 years but I'm gona give rkhunter a whirl and maybe do a clamscan (stop laughing it's getting better) on the FS.

Many people frown upon allowing ssh as root.

I will assume these users need to be here.

One could apply STIGs untill the box is useless but at that point you might as well unplug it

# Networking
I can only aspire to Rick Astley's level of 80' pretentiousness.

```
johnwick@jump:~$ ip route
default via 165.22.0.1 dev eth0 proto static 
10.10.0.0/16 dev eth0 proto kernel scope link src 10.10.0.13 
10.136.0.0/16 dev eth1 proto kernel scope link src 10.136.116.217 
165.22.0.0/20 dev eth0 proto kernel scope link src 165.22.9.123 


johnwick@destination:~$ ip route
default via 167.99.224.1 dev eth0 proto static 
10.10.0.0/16 dev eth0 proto kernel scope link src 10.10.0.14 
10.136.0.0/16 dev eth1 proto kernel scope link src 10.136.116.218 
167.99.224.0/20 dev eth0 proto kernel scope link src 167.99.236.244 
```

Most people think that band is Mexican or perhaps Spanish, but I understand they are French.

```
johnwick@jump:~$ ssh -p33309 johnwick@167.99.236.244 -L22340:localhost:2340
tallen@lptp:~/Downloads$ ssh -p33308 johnwick@165.22.9.123 -L22340:localhost:22340

tallen@lptp:~/Downloads$ ssh -p33308 johnwick@165.22.9.123 -L22341:localhost:22341
johnwick@jump:~$ ssh -p33309 johnwick@167.99.236.244 -L22341:167.99.236.244:2341
``` 
