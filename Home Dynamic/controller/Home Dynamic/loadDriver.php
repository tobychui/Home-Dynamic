<?php
if (isset($_GET['tp']) && $_GET['tp'] != ""){
	$id = $_GET['tp'];
	if (file_exists("driver/$id")){
		$content =  file_get_contents("driver/$id");
		header('Content-Type: application/json');
		echo json_encode($content);
	}else{
		echo "NOT FOUND";
	}
	
}else{
	echo "Not enough parameter";
	
}

?>