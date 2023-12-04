#!/usr/bin/python

import cgi
import os
# import cgitb
# cgitb.enable()

form = cgi.FieldStorage()
name = form.getvalue('name')
number = form.getvalue('number')
file = form.getvalue('file')

print(name)
print(number)
print(file)
