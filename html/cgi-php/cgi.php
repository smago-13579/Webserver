#! /usr/bin/php
<?php
echo "Status: 200 OK\r\n";
echo "Content-Type: text/html\r\n\r\n";
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" href="../style.css">
	<title>Document</title>
</head>
<body>
	<header class="header">
		<div class="header_head">
			<p class="header_text header_text_1">Project</p>
			<p class="header_text header_text_2">webserv</p>
		</div>
		<div class="header_body">
			<a class="header_btext header_body_link" href="ind_on">Index</a>
			<a class="header_btext header_body_link" href="../home.html">Home</a>
			<a class="header_text header_body_link" href="../cgi-bin">Game</a>
			<a class="header_text header_body_link" href="../cgi-php">cgi-php</a>
		</div>
		<div class="header_footer">
			<p class="header_text header_text_1">made by</p>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/smago">smago</a>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/kbatwoma">kbatwoma</a>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/ngonzo">ngonzo</a>
		</div>
	</header>
	<form action="cgi-php/send.php" method="post">
		<input type="text" name="fio" placeholder="Укажите ФИО" required>
		<input type="text" name="email" placeholder="Укажите e-mail" required>
		<input type="submit" value="Отправить">
	</form>
</body>
</html>