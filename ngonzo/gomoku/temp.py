#!/usr/local/bin/python3

print("Content-type: text/html")
print()
print("<h1>Hello world!</h1>")

if	str[0] == str[1] == str[2]\
	or str[3] == str[4] == str[5]\
	or str[6] == str[7] == str[8]\
	or str[0] == str[3] == str[6]\
	or str[1] == str[4] == str[7]\
	or str[2] == str[5] == str[8]\
	or str[0] == str[4] == str[8]\
	or str[2] == str[4] == str[6]:
	print("Winner is " , str[9])
else:
	print("Draw")