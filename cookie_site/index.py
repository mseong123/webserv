#!/usr/bin/python3

import cgi
import os
import sys
import cgitb
from ft_cookies.cookie_man import make_sid, get_sid
from ft_cookies.cookie_man import read_dict, write_dict
from ft_cookies.cookie_man import cookie_expire

#for funz
import datetime


cgitb.enable()
form = cgi.FieldStorage()

request_method = os.environ.get("REQUEST_METHOD").upper()

if request_method == "GET":
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/html")
	if os.environ.get("COOKIE") is None:
		print("\r")
		with open("cookie_site/data/html/signup", mode="r") as file:
			file_content = file.read()
			file.close()
		print(file_content)
		print("\r")
	else:
		cdict = read_dict("cookie_site/data/ctr.dict")
		sid = get_sid(os.environ.get("COOKIE"))
		print("\r")
		with open("cookie_site/data/html/dashboard", mode="r") as file:
			file_content = file.read()
			file.close()
		file_content = file_content.replace("fname", cdict[sid]['fname'] + " ")
		file_content = file_content.replace("lname", cdict[sid]['lname'])
		file_content = file_content.replace("yob", str(datetime.date.today().year - cdict[sid]['yob']))
		print(file_content)
		print("\r")
elif request_method == "POST":
	cdict = read_dict("cookie_site/data/ctr.dict")
	if os.environ.get("COOKIE") is None:
		sid = make_sid(16)
		while sid in cdict:
			sid = make_sid(16)
		cdict[sid] = {}
		cdict[sid]['fname'] = str(form.getvalue('fname')).capitalize()
		cdict[sid]['lname'] = str(form.getvalue('lname')).capitalize()
		cdict[sid]['yob'] = int(form.getvalue('yob'))
		print("HTTP/1.1 200 OK")
		print("Content-Type: text/html")
		print("Set-Cookie: sid=" + sid + "; Expires=" + cookie_expire())
		print("\r")
		with open("cookie_site/data/html/random.html", mode="r") as file:
			file_content = file.read()
			file.close()
		print(file_content)
		write_dict(cdict, "cookie_site/data/ctr.dict")
