<html>
<meta name="apple-mobile-web-app-capable" content="yes" />
<meta name="viewport" content="user-scalable=no, width=device-width, initial-scale=0.6, maximum-scale=0.6"/>
<html>
<head>
<script type='text/javascript' charset='utf-8'>
    // Hides mobile browser's address bar when page is done loading.
      window.addEventListener('load', function(e) {
        setTimeout(function() { window.scrollTo(0, 1); }, 1);
      }, false);
</script>
<title>Home Dynamic</title>
<link rel="stylesheet" href="../script/tocas/tocas.css">
<script src="../script/tocas/tocas.js"></script>
<script src="../script/jquery.min.js"></script>
</head>
<body>
	<div class="ts basic menu">
		<a href="index.php" class="active item">Home Dynamic</a>
		<a href="hds_scan.php" class="item">Scan</a>
		<a class="item">Controls</a>
		<a class="item">System</a>
	</div>
	<!-- End of Header -->
	
	<div id="mainmenu" class="ts container"></div>
	
<script>
var ipTable = "<?php if(file_exists("iptable.txt")){echo file_get_contents("iptable.txt");}?>";
var deviceIPs = ipTable.slice(0, -1).split(",");
var UUID = "";
var webworkers = [];
var loadedDriverList = [];
$(document).ready(function(){
	var i = 0;
	for (i = 0; i < deviceIPs.length;i++){
		$("#mainmenu").append('<div id="header_' + i + '" class="ts horizontal divider">Loading</div>');
		$("#mainmenu").append("<duv id='panel_" + i +"' class='ts container'></div>");
		$("#mainmenu").append("<duv id='control_" + i +"' class='ts container'></div>");
	}
	setTimeout(GetAllInfo, 500);
	console.log(deviceIPs);
});


function GetAllInfo(){
	var i=0;
	for (i = 0; i < deviceIPs.length;i++){
		GetDeviceInfo(deviceIPs[i],i);
	}
}

function GetDeviceInfo(ip,id){
	var thisIP = ip;
	var thisNumber = id;
	$.ajax({
		url: "http://" + thisIP + "/info", 
		async: "false",
		timeout: 10000,
		error: function(){
			$('#panel_' + thisNumber ).html("This device has been offline or its IP has been changed.");
			$('#panel_' + thisNumber ).append("<br>Please click <a href='hds_scan.php'>here</a> to update the IP table by performing an IP scan.");
		},
		success: function(result){
			$('#panel_' + thisNumber ).html(" " + result);
			$('#panel_' + thisNumber ).append("<br>" + thisIP);
			FindDriver(result,id,thisIP);
			$('#control_' + thisNumber ).attr("ip",thisIP);
		}
	});
	$.ajax({
		url: "http://" + thisIP + "/uuid", 
		async: "false",
		timeout: 10000,
		error: function(){
			$('#header_' + thisNumber ).html("Unreachable");
		},
		success: function(result){
		$('#header_' + thisNumber ).html(result);
	}});
}

function FindDriver(info,id,ip){
	var DeviceType = info.split("_");
	$.ajax( "loadDriver.php?tp=" + DeviceType[1],
	{
		type: "GET",
		success:function(data) {
			if (data.includes("NOT FOUND") == false){
				$("#control_" + id).addClass(replaceAll(DeviceType[1],".","_"));
				AppendScript("driver/" + DeviceType[1]);
			}else{
				$("#control_" + id).addClass("Unknown_Device");
				$("#control_" + id).append("Driver not found.<br>");
				$.get( "http://" + $("#control_" + id).attr("ip") + "/", function( data ) {
				  $("#control_" + id).append('<details class="ts accordion"><summary><i class="dropdown icon"></i>Module Information Tag</summary><div class="content">' + data + '</div></details>');
				});
							
			
			}
		  
		}
	});
}

function replaceAll(data,search, replacement) {
    return data.split(search).join(replacement);
}

function AppendScript(url){
	var script = document.createElement( 'script' );
	script.type = 'text/javascript';
	script.src = url;
	$("body").append( script );
}

function DriverLoaded(needle){
	return (loadedDriverList.indexOf(needle) > -1);
}

function DriverFinishedLoading(driverName){
	loadedDriverList.push(driverName);
}

</script>
</body>

</html>