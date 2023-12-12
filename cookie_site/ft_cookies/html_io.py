# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    html_io.py                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/12 15:26:55 by yetay             #+#    #+#              #
#    Updated: 2023/12/12 15:35:29 by yetay            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

def get_file_txt(path):
    """Reads a file and put the content to standard output"""
    file_content = ""
    with open(path, mode="r") as file:
	    file_content = file.read()
	    file.close
    return file_content
