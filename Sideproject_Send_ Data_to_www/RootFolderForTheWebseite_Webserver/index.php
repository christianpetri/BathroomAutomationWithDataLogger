<?php
 	 date_default_timezone_set('Europe/London');
    include("connect.php");
    $result = $DB->getResults("SELECT * FROM `tempLog` WHERE `timeStamp` >= (now() - INTERVAL 1 DAY) ORDER BY `timeStamp` DESC LIMIT 2000");
   //echo print_r($result);
?>

<html lang="en">
   <head>
      <title>Sensor Data</title>
       <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
       <script src="/js/jquery.flot.js"></script>
		 <script type="text/javascript" src="/js/jquery.flot.time.js"></script>    
       <script src="/js/jquery.flot.navigate.js"></script>
       <style type="text/css">
           html{
               background-color:black;
           }
           body{
               padding: 20px;
               max-width: 1000px;
               margin:auto;
               background-color: white;
           }
            
            table.result {
                border-collapse: collapse;
            }

            table.result th {
                border: 1px solid black;
                padding: 5px;
            }
           table.result td{
                border: 1px solid black;
                padding: 5px;
            }
       
       </style>
		<script type="text/javascript">
			var timeout;
			$(document).ready(function () {
			  startReload(); //NEW PART
			  $("#siteloader").html('<object data="https:/sensor.christianpetri.ch"/>');
			});

			function stopReload() {
			  clearTimeout(timeout);
				$("#onORoff").empty();
			 	$("#onORoff").append("OFF");
			}
			function startReload() {
			  timeout = setTimeout(function() { window.location.reload(); }, 5000); // 5 seconds, e.g.
				$("#onORoff").empty();
			 	$("#onORoff").append("ON");
				
			}

		</script> 
   </head>
<body> 
      <h1>Temperature / Humidity sensor readings (last 24 hours)</h1>
      <a href="/result.php">See all logged data</a>
	<div>The website automatically updates  every 5 seconds	<button onclick="stopReload();">Stop it</button> <button onclick="startReload();">Start it</button> Refresh: <span style="color:red;t" id="onORoff"> ON</span> </div>
<?php
   //<meta http-equiv="refresh" content="5" >
$value= array();
foreach ($result as $key => $val) {
     // $timestamp =  strtotime($result[$key]['timeStamp']);
    $value[$key] = '[' . htmlspecialchars( strtotime($val['timeStamp'])*1000). ',' . htmlspecialchars($val['humidity']). ']';
    
    $returnResults .= $value[$key];
  if($key<count($result)-1) {
         $returnResults .= ",";
   }
  
}
foreach ($result as $key => $val) {

    $valueTemp[$key] = '[' . 
		 htmlspecialchars(strtotime($val['timeStamp'])*1000).
		 ',' . 
		 htmlspecialchars($val['temperature']). ']';
		//$val['ID']
    $returnTempResults .= $valueTemp[$key];
    if($key<count($result)-1) {
        $returnTempResults .= ",";
    }

}
//echo print_r($value);
    
    //echo $returnResults;
?>   
<script>
$( document ).ready(function() {
$.plot($("#placeholder"), 
		 [
			{label: "Humidity", data: [  <?php echo $returnResults; ?> ]},
		  	{ label: "Temperature", data: [  <?php echo $returnTempResults; ?> ] }
		 ], 
		 { yaxis: { max: 100 },
		   xaxis: { mode: 'time',  timeformat: '%d.%m.%Y %H:%M' },
		 	series: { lines: { show: true }, points: { show: false } },
		  	legend: { show: true },
		  	 
		 });     
});
</script>	
<div id="placeholder" style="width:1000px;height:300px"></div>    
    </br>
<?php
    
     $result = $DB->getResults("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC LIMIT 10");
    $html="";
    $html = '<table class="result">';
    // header row
    $html .= '<tr>'; 
            $html .='<th> timeStamp </th> <th> temperature </th> <th> humidity </th>'; 
    $html .='</tr>';

    // data rows
foreach ($result as $key => $value) {	
    $html .= '<tr>';
    $html .= '<td>' . htmlspecialchars($result[$key]['timeStamp']). '</td>';
    $html .= '<td>' . htmlspecialchars($result[$key]['temperature']). '</td>';
    $html .= '<td>' . htmlspecialchars($result[$key]['humidity']). '</td>';
    $html .= '</tr>';
}

    // finish table and return it

    $html .= '</table>';
    echo $html;

/* 
   </table>
    <form action="add.php" method="post">
        <input name="temp1" type="number">
        <input name="hum1" type="number">
        <input type="submit" value="Submit">
    
    </form>
 */
?>
</body>
</html>
