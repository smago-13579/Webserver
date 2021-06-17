#!/usr/local/bin/python3

import cgi
import html
import http.cookies
import os
import cgi_content as cc

try:
    f = open("info.txt")
    f.close()
except FileNotFoundError:
	f = open("info.txt", "w")
	f.write("         X")
	f.close()

f = open("info.txt")
str = f.read()
f.close()

form = cgi.FieldStorage()
action = form.getfirst("action", "")

button1 = str[0]
button2 = str[1]
button3 = str[2]
button4 = str[3]
button5 = str[4]
button6 = str[5]
button7 = str[6]
button8 = str[7]
button9 = str[8]
delete = "Начать с начало"
i = str[9]

def new_str(str, char, pointer):
	new_str = ""
	k = 0
	for i in str:
		if k == pointer:
			new_str += char
		else:
			new_str += i
		k += 1
	return new_str

def change_user(str):
	if str[9] == "X":
		str = new_str(str, "O", 9)
	elif str[9] == "O":
		str = new_str(str, "X", 9)
	return str

if action == "button1":
	if str[0] == " ":
		str = new_str(str, i, 0)
		button1 = str[0]
		str = change_user(str)
elif action == "button2":
	if str[1] == " ":
		str = new_str(str, i, 1)
		button2 = str[1]
		str = change_user(str)
elif action == "button3":
	if str[2] == " ":
		str = new_str(str, i, 2)
		button3 = str[2]
		str = change_user(str)
elif action == "button4":
	if str[3] == " ":
		str = new_str(str, i, 3)
		button4 = str[3]
		str = change_user(str)
elif action == "button5":
	if str[4] == " ":
		str = new_str(str, i, 4)
		button5 = str[4]
		str = change_user(str)
elif action == "button6":
	if str[5] == " ":
		str = new_str(str, i, 5)
		button6 = str[5]
		str = change_user(str)
elif action == "button7":
	if str[6] == " ":
		str = new_str(str, i, 6)
		button7 = str[6]
		str = change_user(str)
elif action == "button8":
	if str[7] == " ":
		str = new_str(str, i, 7)
		button8 = str[7]
		str = change_user(str)
elif action == "button9":
	if str[8] == " ":
		str = new_str(str, i, 8)
		button9 = str[8]
		str = change_user(str)
elif action == "delete":
	str = "         X"
	button1 = " "
	button2 = " "
	button3 = " "
	button4 = " "
	button5 = " "
	button6 = " "
	button7 = " "
	button8 = " "
	button9 = " "

f = open("info.txt", "w")
f.write(str)
f.close()


print('Content-type: text/html\n')
print(cc.main.format(button1=button1, button2=button2, button3=button3,
		button4=button4, button5=button5, button6=button6,
		button7=button7, button8=button8, button9=button9,
		delete="New game"))