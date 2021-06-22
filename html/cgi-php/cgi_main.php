#! /usr/bin/php
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
			<a class="header_btext header_body_link" href="/">Index</a>
			<a class="header_btext header_body_link" href="home.html">Home</a>
			<a class="header_text header_body_link" href="cgi-bin">Game</a>
			<a class="header_text header_body_link" href="cgi-php">cgi-php</a>
		</div>
		<div class="header_footer">
			<p class="header_text header_text_1">made by</p>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/smago">smago</a>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/kbatwoma">kbatwoma</a>
			<a class="header_text header_text_2" href="https://profile.intra.42.fr/users/ngonzo">ngonzo</a>
		</div>
	</header>
	<body>
		<div class="jeka_php">
			<?php
				parse_str($_ENV["QUERY_STRING"], $result);
				if($result) {
				echo "Login : " . $result['Login'] . "<br>";
				echo "Nickname : " . $result['Nickname'] . "<br>";
				echo "Data : " . $result['Data'] . "<br>";
				}
				else {
					echo '<form action="cgi-php?login=Anonimus&Nickname=Mortas&Data=Ta-tata-ta" method="get">';
					echo 'Input Login:';
					echo '<input name="Login"required type="text"><br>';
					echo 'Input Nickname:';
					echo '<input name="Nickname" required type="text"><br>';
					echo 'Input Data:';
					echo '<input name="Data" type="text"><br>';
					echo '<button><h1>Send</h1></button>';
					echo '</form>';
				}
				$f = fopen('html/cgi-php/users_base', 'a+');
				if($result) {
					fwrite($f, ";Login=" . $result['Login'] . " && ");
					fwrite($f, "Nickname=" . $result['Nickname'] . " && ");
					fwrite($f, "Data=" . $result['Data'] . ";\r\n");
				}
				fclose($f);
			?>
		</div>
	</body>
</html>