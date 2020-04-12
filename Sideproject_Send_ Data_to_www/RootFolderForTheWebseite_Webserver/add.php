<?php
include("connect.php");
$DB = new HandelDB;
date_default_timezone_set("Europe/Paris"); 
$date = new DateTime();
   
if (isset($_POST["secret"])) {
	if($_POST["secret"]) == "1234567890") // Replace with your secret
    $DB->pushUpdate($date->format('Y-m-d H:i:s'),$_POST["temp1"], $_POST["hum1"]);
}
   
header("Location: index.php");
