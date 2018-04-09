<?php
//Generate HDS IP Table
if (isset($_POST['ipTable']) && $_POST['ipTable'] != ""){
	$iptable = fopen("iptable.txt", "w");
	foreach ($_POST['ipTable'] as $ip){
		fwrite($iptable, $ip . ",");	
	}
	fclose($iptable);
	echo "DONE";
}else{
	echo "Missing argument.";
}

?>