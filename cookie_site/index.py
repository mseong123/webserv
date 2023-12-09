#!/usr/bin/python3

import cgi
import os
import sys
import cgitb
from ft_cookies.cookie_man import make_sid


cgitb.enable()
form = cgi.FieldStorage()

request_method = os.environ.get("REQUEST_METHOD").upper()

if request_method == "GET":
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/html")
	print("\r\n")
	with open("cookie_site/data/html/head", mode="r") as file:
		file_content = file.read()
		file.close()
	print(file_content)
	ctr = str(1)
	print("<span class='ctr'>" + ctr + "</span>")
	with open("cookie_site/data/html/tail", mode="r") as file:
		file_content = file.read()
		file.close()
	print(file_content)
	print(make_sid(16))
	print("\r\n")
