<?php
include("connect.php");
$DB = new HandelDB;
date_default_timezone_set("Europe/Paris");
$date = new DateTime();

/* for the sendPostToWWW_Ethernet_Arduino and sendPostToWWW_WiFI_Arduino
if (isset($_POST["secret"])) {
    if ($_POST["secret"] == "1234567890") {
        $DB->pushUpdate($date->format('Y-m-d H:i:s'), $_POST["temp1"], $_POST["hum1"]);
    }
} else {
    http_response_code(400);
}
*/

// for sendPostToWWW_Wifi_Wemos_D1_SHT30
if (isset($_POST["temperature"]) && isset($_POST["humidity"]) && isset($_POST["secret"])) {
    if ($_POST["secret"] == "1234567890") {
        #echo "Temperature in Celsius: " . htmlspecialchars($_POST["temperature"]) . " Humidity in %: " . htmlspecialchars($_POST["humidity"]) . "\n";
        $DB->pushUpdate($date->format('Y-m-d H:i:s'), $_POST["temperature"], $_POST["humidity"]);
    } else {
        http_response_code(400);
    }
} else {
    http_response_code(400);
}

header("Location: index.php");
exit;