<?php
 	 date_default_timezone_set('Europe/London');
    include("connect.php");
    $result = $DB->getResults("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC");
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
      
   </head>
<body>
   <h1>Temperature / Humidity sensor readings</h1>
    <a href="/">Live data</a>
    <p>You can pan (right click and hold) and zoom (mouse-wheel) with your mouse</p>
<?php
   
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
		 { yaxis: { max: 100, zoomRange: false, panRange: false },
		   xaxis: { mode: 'time',  timeformat: '%d.%m.%Y %H:%M:%S',zoomRange: [null, new Date()], panRange: [null, new Date()] },
		 	series: { lines: { show: true }, points: { show: false } },
		  	legend: { show: true },
			zoom: {
				interactive: true
			},
			pan: {
				interactive: true,
				cursor: "move"
			}
		  /*	
		  zoom: {
				interactive: true
				trigger: "dblclick" // or "click" for single click
				amount: 1.5         // 2 = 200% (zoom in), 0.5 = 50% (zoom out)
			} 
		  
			pan: {
				interactive: true
				cursor: "move"      // CSS mouse cursor value used when dragging, e.g. "pointer"
				frameRate: 20
			}
 	/*
	xaxis, yaxis, x2axis, y2axis: {
		zoomRange: null  // or [ number, number ] (min range, max range) or false
		panRange: null   // or [ number, number ] (min, max) or false
	}
	*/   
		  	 
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
