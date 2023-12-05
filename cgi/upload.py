#!/usr/bin/python

import cgi
import os
# import cgitb
# cgitb.enable()

form = cgi.FieldStorage()

request_method = os.environ.get("REQUEST_METHOD").upper()
route = "." + os.environ.get("ROUTE").upper()

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
		with open(route, 'r') as file:
			file_contents = file.read()
		print(file_contents)

	else:
		print("File {}' not found.".format(route))

