#!/usr/bin/python3

import cgi
import os
import sys
import cgitb
import datetime
from ft_cookies.cookie_man import make_sid, get_sid
from ft_cookies.cookie_man import read_dict, write_dict
from ft_cookies.cookie_man import cookie_expire
from ft_cookies.html_io import get_file_txt


cgitb.enable()
form = cgi.FieldStorage()

request_method = os.environ.get("REQUEST_METHOD").upper()

cdict = read_dict("cookie_site/data/ctr.dict")

if request_method == "GET":
	cookie = os.environ.get("COOKIE")
	file_content = ""

	#### If server does not receive a cookie,
	#### or if server receives an invalid cookie
	#### serve a sign-up page;
	#### else serve a greetings page.
	if (cookie is None) or (get_sid(cookie) not in cdict):
		html_page = "cookie_site/data/html/signup.html"
		file_content = get_file_txt(html_page)
	else:
		sid = get_sid(os.environ.get("COOKIE"))
		user_age = datetime.date.today().year
		user_age -= cdict[sid]['yob']

		html_page = "cookie_site/data/html/dashboard.html"
		file_content = get_file_txt(html_page)
		file_content = file_content.replace(
				"USER_SUBMITTED_FNAME_HERE",
				cdict[sid]['fname'])
		file_content = file_content.replace(
				"USER_SUBMITTED_LNAME_HERE",
				cdict[sid]['lname'])
		file_content = file_content.replace(
				"COMPUTED_USER_AGE_HERE",
				str(user_age))

	#### Serve the selected webpage
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/html")
	print("\r")
	print(file_content)
	print("\r")
elif request_method == "POST":
	#### Create an sid to send to client as cookie
	sid = make_sid(16)
	while sid in cdict:
		sid = make_sid(16)

	#### Populate DB fields
	cdict[sid] = {}
	cdict[sid]['fname'] = str(form.getvalue('fname')).capitalize()
	cdict[sid]['lname'] = str(form.getvalue('lname')).capitalize()
	cdict[sid]['yob'] = int(form.getvalue('yob'))

	#### Write data to file
	write_dict(cdict, "cookie_site/data/ctr.dict")

	#### Serve a auto-redirect confirmation page
	file_content = get_file_txt("cookie_site/data/html/random.html")
	print("HTTP/1.1 200 OK")
	print("Set-Cookie: sid=" + sid + "; Expires=" + cookie_expire())
	print("Content-Type: text/html")
	print("\r")
	print(file_content)
	print("\r")
