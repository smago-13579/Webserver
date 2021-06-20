#!/usr/bin/php
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


	<form action="cgi.php" method="get" target='_blank'>
	<!-- указываем в value цифру нужного нам голосования, для которого уже создали файлы //-->
	<input type="Hidden" name="id" value="1">
	<p><b>Оцените сайт:</b></p>
	<input type="Radio" name="vote" value="1" checked>Отлично<br>
	<input type="Radio" name="vote" value="2">Хорошо<br>
	<input type="Radio" name="vote" value="3">Удовлет.<br>
	<input type="Radio" name="vote" value="4">Плохо<br>
	<input type="Radio" name="vote" value="5">Очень плохо<br>
	<input type="Submit" value=" Голосовать" style="margin-top:10px;">
	</form>

	<!-- указываем в href после vote.php?id= цифру нужного нам голосования, для которого уже создали файлы //-->
	<p><a href='cgi.php?id=1' target='_blank'><button>Результаты</button></a></p>

	<!-- <div>
		<form action="cgi-php" method="post">
			<h2>Привет! Как твои дела?</h2>
			<input type="text" name="ansew" placeholder="enter here" required>
			<input type="submit" value=  "send">
			<?php

			if (ansew == "good") {
				echo "<h2>Мы очень рады за тебя!</h2></br>";
			} elseif (ansew == "bad") {
				echo "<h2>Мы надеемся что когда ты проставишь нам бонусы тебе бедет веселее</h2></br>";
			} else {
				echo "<h2>Напиши плохо или хорошо!";
			}
			?><br>

		</form>
	</div> -->

	<?php
	// получаем файлы по определенному голосованию
	$id =  (int) $_GET[id]; //приводим к целому числу идентификатор голосования
	$vote = (int) $_GET[vote]; //приводим к целому числу передаваемое значение голосования

	//проверяем, существует ли такое голосование
	if (file_exists("$id.dat")) {

	$ip= $_SERVER['REMOTE_ADDR']; //получаем ip адрес
	$ip_file = file_get_contents("ip$id.dat");//читаем содержимое файла ip адресов и помещаем в строку
	$ip_abbr = explode(",", $ip_file);//получаем в массив имеющиеся ip адреса

	$data = file("$id.dat"); //читаем содержимое файла результатов и помещаем в массив

	// если это не просто просмотр результатов
	if ($vote) {

	//сравниваем ip с уже записанными
	foreach($ip_abbr as $value) 
	if ($ip == $value) {echo "<p><b><font color=red> Вы уже голосовали! </font></b></p>";
	exit;
	}
	// выводим благодарность
	echo "<p><b><font color=green> Спасибо! </font></b><br /><i>*Показаны результаты до Вашего голосования:</i><p>";
	}

	// выводим заголовок голосования - 1я строка файла
	echo "<b>$data[0]</b><p>";

	// печатаем список ответов и результатов - остальные строки
	for ($i=1;$i<count($data);$i++) {
	$votes = explode("~", $data[$i]); // значение~ответ
	echo "$votes[1]: <b>$votes[0]</b><br>"; //поменяйте местами 0 и 1 в $votes и в результатах цифры будут первыми
	}
	echo "<br>Всего проголосовало: <b>".(count($ip_abbr)-1)."</b>";

	// если это не просмотр результатов, а голосование,
	// производим необходимые действия для учета голоса
	if ($vote) {
	$f = fopen("$id.dat","w");
	flock($f,LOCK_EX);
	fputs($f, "$data[0]");
	for ($i=1;$i<count($data);$i++) {
		$votes = explode("~", $data[$i]);
		if ($i==$vote) $votes[0]++;
		fputs($f,"$votes[0]~$votes[1]");
		fflush($f);
	flock($f,LOCK_UN);
	}
	fclose($f);
	
	//и записываем ip
	$ip_adr = fopen("ip$id.dat","a++");
		flock($ip_adr,LOCK_EX);
	fputs($ip_adr, "$ip".",");
	fflush($ip_adr);
		flock($ip_adr,LOCK_UN);
	fclose($ip_adr);
	}

	} else {
	//передан id несуществующего голосования
		echo "Такого голосования не существует.";
		exit;
	}

	?>


</body>
</html>