#!/usr/bin/python3

import cgi
import os
import sys
import cgitb
from ft_cookies.cookie_man import make_sid, get_sid
from ft_cookies.cookie_man import read_dict, write_dict
from ft_cookies.cookie_man import cookie_expire


cgitb.enable()
form = cgi.FieldStorage()

request_method = os.environ.get("REQUEST_METHOD").upper()

if request_method == "GET":
	#### Read cookie database
	cdict = read_dict()
	
	#### Obtain session ID and counter
	set_cookie = False
	if os.environ.get("COOKIE") is None:
		sid = make_sid(16)
		while sid in cdict:
			sid = make_sid(16)
		cdict[sid] = 1
		set_cookie = True
	else:
		sid = get_sid(os.environ.get("COOKIE"))
		if sid not in cdict:
			sid = make_sid(16)
			while sid in cdict:
				sid = make_sid(16)
			cdict[sid] = 1
			set_cookie = True
		else:
			cdict[sid] += 1
	write_dict(cdict)
	
	#### Create response HTTP header
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/html")
	if set_cookie:
		print("Set-Cookie: sid=" + sid + "; Expires=" + cookie_expire())
	print("\r\n")

	#### Create response HTTP body
	with open("cookie_site/data/html/head", mode="r") as file:
		file_content = file.read()
		file.close()
	print(file_content)
	print("<span class='ctr'>" + str(cdict[sid]) + "</span>")
	with open("cookie_site/data/html/tail", mode="r") as file:
		file_content = file.read()
		file.close()
	print(file_content)
	print("\r\n")
