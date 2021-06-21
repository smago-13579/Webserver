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
	<div>
		<p>Привет! Как твои дела?</p>
		<?php
		parse_str($_ENV["QUERY_STRING"], $result);
		if ($result == "хорошо") {
			echo "<h2>Мы очень рады за тебя!</h2></br>";
		} elseif ($result == "плохо") {
			echo "<h2>Мы надеемся что когда ты проставишь нам бонусы тебе бедет веселее</h2></br>";
		} else {
			echo "<h2>Напиши плохо или хорошо!";
		}
		?>
	</div>
</body>
</html>