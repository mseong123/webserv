# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cookie_man.py                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/09 12:58:04 by yetay             #+#    #+#              #
#    Updated: 2023/12/09 17:13:42 by yetay            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import random
import string
import os
import json
from datetime import date


def make_sid(N):
	"""Returns a randomly generated alphanumeric string"""
	return "".join(random.choices(string.ascii_letters + string.digits, k=N))


def get_sid(s):
	"""Receives a string from environment variable, returns sid"""
	s = s.split("; ")
	s = {i.split('=')[0].strip(): i.split('=')[1].strip() for i in s}
	return s["sid"]


def read_dict(datafile):
	"""Returns a dictionary loaded from data file"""
	cdict = None
	datafile = "cookie_site/data/ctr.dict"
	if os.path.exists(datafile) and os.path.isfile(datafile):
		with open(datafile, mode="r") as file:
			cdict = file.read()
			if len(cdict) == 0:
				cdict = {}
			else:
				cdict = json.loads(cdict)
			file.close()
	if cdict is None:
		cdict = {}
	return cdict


def write_dict(cdict, datafile):
	"""Write a dictionary to data file"""
	with open(datafile, mode="w") as file:
		file.write(json.dumps(cdict))
		file.write("\n")
		file.close()
	return True


def cookie_expire():
	"""Return the HTTP date format string of the next New Years Day"""
	year = date.today().strftime('%Y')
	year = int(year) + 1
	dow = date(year, 1, 1).strftime('%a')
	return dow + ", 1 Jan " + str(year) + " 00:00:00 GMT"
