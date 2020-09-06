#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 9033))
print(s.recv(1024).decode())
data = "client"
while True:
	if data:
		print(s.recv(1024).decode())
	data = input("Please input your name: ")
	if not data:
		continue
	s.send(data.encode())
	print(s.recv(1024).decode())
	if data == "exit":
		break
s.close()
