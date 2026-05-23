feffi-map is a network scanner written in C, a mini-version of Nmap. 
The goal of the project is to to reimplement, for educational purposes, some core network scanning techniques used by Nmap, such as ARP host discovery, similar to `nmap -sn` on local networks, TCP connect scan, similar to `nmap -sT`, Basic port range scanning, Basic service identification. 



# feffi-map

`feffi-map` is a small network scanner written in C.

The idea is not to clone the whole Nmap project, but to rebuild some of its core features from scratch in order to understand how network scanning actually works at a lower level.

## Current goal

The first feature I want to implement is host discovery on a local network, similar to:

```bash
nmap -sn 192.168.1.0/24
```

In the first version, this will be done through ARP requests.

Expected usage:

```bash
sudo ./feffi-map -sn -i eth0 192.168.1.0/24
```

## Planned features

- ARP-based host discovery on local networks
- TCP connect scan, similar to `nmap -sT`
- Port range scanning
- Basic service detection
- Cleaner output and error handling

## Why this project

I am building this project mainly to practice C, raw sockets, Ethernet/ARP packets, and general network programming.

## Disclaimer

This project is for educational purposes only. It should only be used on networks I own or where I have explicit permission.s