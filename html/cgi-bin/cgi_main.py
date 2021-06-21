#!/usr/local/bin/python3

import cgi
import cgi_content as cc

try:
	f = open("html/cgi-bin/for_game.tmp")
	f.close()
except FileNotFoundError:
	f = open("html/cgi-bin/for_game.tmp", "w")
	f.write("         X")
	f.close()

f = open("html/cgi-bin/for_game.tmp")
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
delete = "Restart"
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

f = open("html/cgi-bin/for_game.tmp", "w")
f.write(str)
f.close()

print('Status: 200 OK\r\n')
print('Content-type: text/html\r\n\r\n')
if	str[0] == str[1] == str[2] and str[0] != " "\
	or str[3] == str[4] == str[5] and str[3] != " "\
	or str[6] == str[7] == str[8] and str[6] != " "\
	or str[0] == str[3] == str[6] and str[0] != " "\
	or str[1] == str[4] == str[7] and str[1] != " "\
	or str[2] == str[5] == str[8] and str[2] != " "\
	or str[0] == str[4] == str[8] and str[0] != " "\
	or str[2] == str[4] == str[6] and str[2] != " ":
	if(str[9] == "O"):
		print(cc.main.format(button1=button1, button2=button2, button3=button3,
			button4=button4, button5=button5, button6=button6,
			button7=button7, button8=button8, button9=button9,
			delete="New game", end=cc.end_x))
	else:
		print(cc.main.format(button1=button1, button2=button2, button3=button3,
		button4=button4, button5=button5, button6=button6,
		button7=button7, button8=button8, button9=button9,
		delete="New game", end=cc.end_o))
 
else:
	print(cc.main.format(button1=button1, button2=button2, button3=button3,
		button4=button4, button5=button5, button6=button6,
		button7=button7, button8=button8, button9=button9,
		delete="New game", end=""))