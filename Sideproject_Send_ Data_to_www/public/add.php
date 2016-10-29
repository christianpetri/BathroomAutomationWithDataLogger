<?php
include("connect.php");
date_default_timezone_set("Europe/Paris");
$date = new DateTime();
   
if (isset($_POST["temp1"])) {
    $DB->pushUpdate($date->format('Y-m-d H:i:s'), $_POST["temp1"], $_POST["hum1"]);
}
   
header("Location: index.php");
