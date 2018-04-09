<!DOCTYPE html>
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
    <meta charset="utf-8">
    <title>HDS Scanner (Experimental)</title>
	<script src="../script/jquery.min.js"></script>
</head>
<body style="background-color:black;color:white;">
<h2>HDS Protocol IP Scanner (Experimental)</h2>
This tool can only scan for NodeMCU / ESP8266 with HDS Protocol Enabled.<br>
To test this script, please make sure there is content under ip_address/uuid with allow origin *<br>
Your network IP is: <p id="list">Loading</p>
<p id="debug"></p>
<script>
var HDSIPList = [];
var startedConnections = 0;
$(document).ready(function(){
	GrapIP();
	setTimeout(function() {
		StartWebWorker();
    }, 2000);
});

function StartWebWorker(){
var ip = $("#list").html();
var webWorkers = [];
if (ip.includes("ifconfig")){
	$("#debug").append("This browser is not supported.<br>");
	return;
}
if (typeof(Worker) !== "undefined") {
    $("#debug").append("Web Worker Exists. IP Scanning Started.<br>");
	//The browser support everything and ready to start scanning
	var ipg = ip.split(".");
	var header = ipg[0] + "." + ipg[1] + "." + ipg[2] + "."; //a.b.c.
	
	for (var i=1; i < 255;i++){
		startedConnections++;
		GetWorkingOrNot(header + i)
	}
	WaitAllFinish();
	
} else {
    $("#debug").html("Error. Web Worker not supported.");
} 

}

function WaitAllFinish(){
	$("html, body").animate({ scrollTop: $(document).height() }, 200);
	if (startedConnections <= 1){
		$("#debug").append("<br>DONE<br><br>");
		GenerateTable();
	}else{
		setTimeout(WaitAllFinish, 1000);
	}
	
}


function GetWorkingOrNot(ip){
	$.ajax({url: "http://" + ip + "/uuid",
			type: "HEAD",
			timeout:5000,
			statusCode: {
				200: function (response) {
					$("#debug").append("[OK]" +ip + " -> OCL:" + startedConnections + "<br>");
					//$("#debug").append("<a href='http://" +ip + "/' target='_blank'>Click here to redirect</a><br>");
					HDSIPList.push(ip);
					startedConnections--;
				},
				400: function (response) {
					$("#debug").append("[NOT FIND]" +ip + " -> OCL:" + startedConnections + "<br>");
					startedConnections--;
				},
				0: function (response) {
					$("#debug").append("[NOT REPLY]" +ip + " -> OCL:" + startedConnections + "<br>");
					startedConnections--;
				}              
			}
		});
}

function GrapIP(){
	// NOTE: window.RTCPeerConnection is "not a constructor" in FF22/23
	var RTCPeerConnection = /*window.RTCPeerConnection ||*/ window.webkitRTCPeerConnection || window.mozRTCPeerConnection;

	if (RTCPeerConnection) (function () {
		var rtc = new RTCPeerConnection({iceServers:[]});
		if (1 || window.mozRTCPeerConnection) {      // FF [and now Chrome!] needs a channel/stream to proceed
			rtc.createDataChannel('', {reliable:false});
		};
		
		rtc.onicecandidate = function (evt) {
			// convert the candidate to SDP so we can run it through our general parser
			// see https://twitter.com/lancestout/status/525796175425720320 for details
			if (evt.candidate) grepSDP("a="+evt.candidate.candidate);
		};
		rtc.createOffer(function (offerDesc) {
			grepSDP(offerDesc.sdp);
			rtc.setLocalDescription(offerDesc);
		}, function (e) { console.warn("offer failed", e); });
		
		
		var addrs = Object.create(null);
		addrs["0.0.0.0"] = false;
		function updateDisplay(newAddr) {
			if (newAddr in addrs) return;
			else addrs[newAddr] = true;
			var displayAddrs = Object.keys(addrs).filter(function (k) { return addrs[k]; });
			document.getElementById('list').textContent = displayAddrs.join(" or perhaps ") || "n/a";
		}
		
		function grepSDP(sdp) {
			var hosts = [];
			sdp.split('\r\n').forEach(function (line) { // c.f. http://tools.ietf.org/html/rfc4566#page-39
				if (~line.indexOf("a=candidate")) {     // http://tools.ietf.org/html/rfc4566#section-5.13
					var parts = line.split(' '),        // http://tools.ietf.org/html/rfc5245#section-15.1
						addr = parts[4],
						type = parts[7];
					if (type === 'host') updateDisplay(addr);
				} else if (~line.indexOf("c=")) {       // http://tools.ietf.org/html/rfc4566#section-5.7
					var parts = line.split(' '),
						addr = parts[2];
					updateDisplay(addr);
					
				}
			});
		}
	})(); else {
		document.getElementById('list').innerHTML = "<code>ifconfig | grep inet | grep -v inet6 | cut -d\" \" -f2 | tail -n1</code>";
		document.getElementById('list').nextSibling.textContent = "In Chrome and Firefox your IP should display automatically, by the power of WebRTCskull.";
		
		//Callback to next function
		
	}
	
	
	
}

function GenerateTable(){
		//Request the HDS Host to generate the ip table for all in range HDS unit
		console.log(HDSIPList);
		$.post( "HDS_iptable.php", {ipTable: HDSIPList})
		.done(function( data ) {
			window.location = "index.php";
		});


	}
</script>

</body></html>