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
			<form action="cgi-php?name=Anonimus&nickname=Mortas&data=Ta-tata-ta" method="post">
				<button><a class="jeka_php_b">Create default</a></button>
			</form>
			<?php
				parse_str($_ENV["QUERY_STRING"], $result);
				echo "Name : " . $result['name'] . "<br>";
				echo "Nickname : " . $result['nickname'] . "<br>";
				echo "Date : " . $result['data'] . "<br>";
				$f = fopen('html/users_base', 'a+');
				if($result) {
					fwrite($f, "Name=" . $result['name'] . " && ");
					fwrite($f, "Nickname=" . $result['nickname'] . " && ");
					fwrite($f, "Data=" . $result['data'] . " ;\r\n");
				}
				fclose($f);
			?>
		</div>
	</body>
</html>