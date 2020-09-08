#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import socket

def connsend():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 9033))
    
    req = "hello, world..."
    s.send(req.encode())
    return s

def test():
    socks = []
    for i in range(0,10000):
        s = connsend()
        socks.append(s)
    for s in socks:
        rsp = s.recv(1024).decode()
        print "recv:", rsp
        s.close()

test()
