# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cookie_man.py                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/09 12:58:04 by yetay             #+#    #+#              #
#    Updated: 2023/12/09 14:42:48 by yetay            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import random
import string
import json


def make_sid(N):
	"""Returns a randomly generated alphanumeric string"""
	return "".join(random.choices(string.ascii_letters + string.digits, k=N))


def read_dict():
	"""Returns a dictionary loaded from data file"""
	cdict = None
	with open("cookie_site/data/ctr.dict", mode="r") as file:
		cdict = file.read()
		if len(cdict) == 0:
			cdict = {}
		else:
			cdict = json.loads(cdict)
		file.close()
	return cdict


def write_dict(cdict):
	"""Write a dictionary to data file"""
	with open("cookie_site/data/ctr.dict", mode="w") as file:
		file.write(json.dumps(cdict))
		file.close()
	return True
