main = '''
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" href="../html/style.css">
	<title>Document</title>
</head>
<body>
	<header class="header">
		<div class="header_head">
			<p class="header_text header_text_1">Project</p>
			<p class="header_text header_text_2">webserv</p>
		</div>
		<div class="header_body">
			<a class="header_btext header_body_link" href="../html/index.html">Home</a>
			<a class="header_text header_body_link" href="../html/index.html">tictactoe</a>
		</div>
		<div class="header_footer">
			<p class="header_text header_text_1">made by</p>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/smago">smago</a>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/kbatwoma">kbatwoma</a>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/ngonzo">ngonzo</a>
		</div>
	</header>
	<form class="window" action="/cgi-bin/cgi_main.py">
		<div class="window_row">
			<button name="action" value="button1"><p class="simvol">{button1}</p></button>
			<button name="action" value="button2"><p class="simvol">{button2}</p></button>
			<button name="action" value="button3"><p class="simvol">{button3}</p></button>
		</div>
		<div class="window_row">
			<button name="action" value="button4"><p class="simvol">{button4}</p></button>
			<button name="action" value="button5"><p class="simvol">{button5}</p></button>
			<button name="action" value="button6"><p class="simvol">{button6}</p></button>
		</div>
		<div class="window_row">
			<button name="action" value="button7"><p class="simvol">{button7}</p></button>
			<button name="action" value="button8"><p class="simvol">{button8}</p></button>
			<button name="action" value="button9"><p class="simvol">{button9}</p></button>
		</div>
		<div class="window_row">
			<button class="button_restart" name="action" value="delete">RESTART</button>
		</div>
	</form>
</body>
</html>
'''
