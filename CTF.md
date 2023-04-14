
# ssh to 178.128.24.75
## remember how to handle keys
- Remember that you must have created a key pair and stuck the public key in ${HOME}/.ssh/authorizedkeys
- Look up how to get ssh to use an alternate private key
- port ends in 69, 2269? idunno so nmap
## uh, I don't actually know the port
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
Is suposidly less noisy, just sends SYN and calls it good if SYNACK? not really sure why less noisy
...and at the time of this writing I'm still waiting for a result
## actually logging in
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
Okay, that took like an hour, I'm so 1337, In fact I'm so 1337 that I had to go look up how to spell Leet (sarchasm)

## 1. What is the absloute path of the file with the permission set: setguid, user: execute, group: write, world: read
Okay is there some variant of find that does perms, I cant remember so man. BTW that sounds slow if we're on spinning rust so does Locate keep perms, gotta go look that up too.
Gotta start somewhere so lest go with ```find / -perm /g+s```, hope it's an SSD (read: not spinning rust)
```/var/local/r0k4/walter.sobchak```
via
```find / -type f -exec ls -l {} \; | grep -E "^...x.wSr"```
I know. Hardly eligant and I had to wade throught a bunch of "Permission denied"

## An asside my ```cd``` is borked and I don't like it
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
I also borked around with sort, and join but couldn't remember how to make those chooch
There was a time when the device files did not have to be in the /dev (technically devfs) but I think that time is passed; nevertheless, I'll do a find on the whole FS
```find . -type c ! -group tty -exec ls -l {} \; 2> /dev/null | wc -l```
okay, well, I gather they still can be outside devfs, I found 91 with this (less silly) method.

## 3. Send a SIGINT to the running program called interview. Useful information will then appear in a log file.

```

