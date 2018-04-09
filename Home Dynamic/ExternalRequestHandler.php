<?php
session_start();
if (isset($_SESSION['username']) && $_SESSION['username'] == "tobychui"){
	$content = file_get_contents("http://192.168.1.105/off");
	echo $content;
}else{
	echo 'Permission Denied';
}

?>