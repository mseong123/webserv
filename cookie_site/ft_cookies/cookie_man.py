# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cookie_man.py                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/09 12:58:04 by yetay             #+#    #+#              #
#    Updated: 2023/12/09 14:08:41 by yetay            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import random
import string


def make_sid(N):
	"""Returns a randomly generated alphanumeric string"""
	return "".join(random.choices(string.ascii_letters + string.digits, k=N))
