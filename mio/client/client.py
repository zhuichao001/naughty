#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 9033))

req = "hello, select example."
s.send(req.encode())
rsp = s.recv(1024).decode()
print "recv:", rsp
s.close()
