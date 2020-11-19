#! /usr/bin/env python3
from scapy.all import *

pkts = []

load_a = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
load_b = "bbbbbbbbbbbbbbbbbbbbbbbb"
load_c = "cccccccc"

frag_a = Ether()/IP(src="192.168.1.1",dst="100.10.1.1",id=123,flags="MF",frag=0,\
    ttl=255)/UDP(sport=33,dport=55,len=72,chksum=0)/load_a
pkts.append(frag_a)

frag_b = Ether()/IP(src="192.168.1.1",dst="100.10.1.1",id=123,flags="MF",frag=6,\
    ttl=255,proto=17)/load_b
pkts.append(frag_b)

frag_c = Ether()/IP(src="192.168.1.1",dst="100.10.1.1",id=123,frag=9,ttl=255,
    proto=17)/load_c
pkts.append(frag_c)

wrpcap("frags.pcap", pkts)
