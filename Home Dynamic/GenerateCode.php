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
<link rel="stylesheet" href="script/simplemde.min.css">
<script src="script/simplemde.min.js"></script>
</head>
<body>
	<div class="ts large link attached inverted primary menu">
        <div class="ts narrow container">
            <a class="header item"><i class="home icon"></i>Home Dynamic</a>
            <a class="item">Panel</a>
            <a class="item">Tools</a>
            <a class="right item">Setting</a>
        </div>
    </div>
	<div class="ts narrow container">
	<div class="ts inverted info message">
    <div class="header"><i class="terminal icon"></i>Hi there!</div>
    <p>This is the programming interface for the ESP8266 Wifi module. Only edit the code on the left directly when you know what you are doing. <br>
	<i class="caution sign icon"></i>Wrong code might lead to software or hardware failure on your IoT devices. Use this system for your own risk.</p>
	</div>
	</div>
    <div class="ts narrow container grid">
		<!-- Left Section Editor -->
        <div class="twelve wide column">
            <textarea id="mde"><?php
			$file = file_get_contents('script\esp_template\esp_template.ino');
			echo $file;
			?>
            </textarea>
        </div>


		<!-- Right Section Editor -->		
        <div class="four wide column">

            <div class="ts top attached borderless menu">
                <div class="header item">Script Generator</div>
                <div class="right item">
                    <i class="setting icon"></i>
                </div>
            </div>


            <!-- Simple Script Variables -->
            <div class="ts bottom attached segment">
                <div class="ts form">
                    <div class="field">
                        <label>Wifi SSID</label>
                        <input id="ssid" type="text">
                    </div>
                    <div class="field">
                        <label>Wifi Password</label>
                        <input id="wifiPassword" type="text">
                    </div>
					<div class="field">
                        <label>Device Host Name</label>
                        <input id="hostName" type="text">
                    </div>
					<div class="field">
                        <label>ESP Username</label>
                        <input id="username" type="text">
                    </div>
					<div class="field">
                        <label>ESP Password</label>
                        <input id="espPassword" type="text">
                    </div>.
					<div class="field">
                        <label>Display Name</label>
                        <input id="disName" type="text">
                    </div>
					<div class="field">
					<details class="ts accordion">
						<summary>
							<i class="dropdown icon"></i>Advanced Setting
						</summary>
						<div class="content">
							<label>On Path</label>
							<input id="onPath" type="text" placeholder="This cannot be left empty!" value="On:switch|on">
							<br>
							<label>Off Path</label>
							<input id="offPath" type="text" placeholder="This cannot be left empty!" value="Off:switch|off">
							<br>
						</div>
					</details>
					</div>
                </div>
            </div>

            <!-- Buttons -->
			<button class="ts right floated fluid icon labeled button" onClick="GenerateScript();">
                <i class="tasks icon"></i>
                Generate
            </button>
            <button class="ts right floated fluid icon labeled button">
                <i class="download icon"></i>
                Download
            </button>
			
        </div>
    </div>
    <script>
    var simplemde = new SimpleMDE({ element: document.getElementById("mde"), spellChecker: false, status: false });
    
	function GenerateScript(){
		var ssid = $('#ssid').val();
		var wifipw = $('#wifiPassword').val();
		var username = $('#username').val();
		var espPassword = $('#espPassword').val();
		var hostName = $('#hostName').val();
		console.log("Updating Script");
		var template = $('#mde').val();
		var code = template.replace('%SSID%',ssid).replace('%WIFIPASSWORD%',wifipw).replace('%WEB_USERNAME%',username).replace('%WEB_PASSWORD%',espPassword).replace('%HOSTNAME%',hostName);
		console.log(code);
		simplemde.value(code);
	}
	
	</script>
</body>
</html>