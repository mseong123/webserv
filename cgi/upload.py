#!/usr/bin/python

import cgi
import os
import sys
from urllib import unquote
import cgitb
cgitb.enable()

form = cgi.FieldStorage()
request_method = os.environ.get("REQUEST_METHOD").upper()
route = unquote("." + os.environ.get("ROUTE"))

if request_method == "POST":
	if "file" in form and form["file"].filename:
		file_item = form["file"]
		current_dir = os.path.dirname(os.path.abspath(__file__))
		filename = os.path.join(current_dir, file_item.filename)
		with open(filename, "wb") as file:
			file.write(file_item.file.read())
		print("<html><body style='font-family: Arial; sans-serif;margin: 20px;'>")
		print("<div>File uploaded successfully.</div>")
		print("<div>Uploaded File: {}</div>".format(filename))
		print("</body></html>")

	else:
		print("No file uploaded.")

elif request_method == "GET":
	if os.path.exists(route):
		root, extension = os.path.splitext(route)
		file_content = ""
		with open(route, mode="rb") as file:
			file_content = file.read()
		print("HTTP/1.1 200 OK")
		if (extension == ".html" or extension == ".txt") and len(file_content) < 25000:
			if extension == ".html":
				print("Content-Type: text/html")
			else:
				print("Content-Type: text/plain")
			print("\r\n")
			print(file_content)
			print("\r\n")
		else:
			print("Content-Type: text/plain")
			print("\r\n")
			print("Error: Python CGI can only serve .html and .txt files (these files must be less than 25kb)")
			print("\r\n")
	else:
		error_message_body = "<html><head><title>404 Not Found(CGI-Python)</title></head><body><center><h1>404 Not Found(CGI-Python)</h1></center></body></html>"
		print("HTTP/1.1 404 Not Found(CGI-Python)")
		print("Content-Type: text/html")
		print("Content-Length: " + str(len(error_message_body)))
		print("\r\n")
		print(error_message_body)
		print("\r\n")
		
