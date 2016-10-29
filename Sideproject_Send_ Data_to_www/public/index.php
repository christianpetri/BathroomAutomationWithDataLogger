<?php
    include("connect.php");
    $result = $DB->getResults("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC LIMIT 1000");
   //echo print_r($result);
?>

<html>
   <head>
      <title>Sensor Data</title>
       <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
       <script src="/js/jquery.flot.js"></script>
       <script src="js/jquery.flot.navigate.js"></script>
      <meta http-equiv="refresh" content="5" >
   </head>
<body>
   <h1>Temperature / moisture sensor readings</h1>
<?php
   //<meta http-equiv="refresh" content="5" >
$value= array();
foreach ($result as $key => $val) {
    
    $value[$key] = '[' . htmlspecialchars($val['ID']). ',' . htmlspecialchars($val['humidity']). ']';
    
    $returnResults .= $value[$key];
  if($key<count($result)-1) {
         $returnResults .= ",";
   }
  
}
foreach ($result as $key => $val) {

    $valueTemp[$key] = '[' . htmlspecialchars($val['ID']). ',' . htmlspecialchars($val['temperature']). ']';

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
    $.plot($("#placeholder"), [{label: "Humidity", data: [  <?php echo $returnResults; ?> ]},{ label: "Temperature", data: [  <?php echo $returnTempResults; ?> ] }], { yaxis: { max: 100 },series: {
        lines: { show: true },
        points: { show: false }
    } });     
});
</script>	
<div id="placeholder" style="width:1000px;height:300px"></div>    
<?php
    
     $result = $DB->getResults("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC LIMIT 10");
    $html="";
    $html = '<table border="1" cellspacing="1" cellpadding="1">';
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
