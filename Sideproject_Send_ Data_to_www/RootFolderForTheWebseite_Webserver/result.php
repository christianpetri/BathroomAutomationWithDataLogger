<?php
date_default_timezone_set('Europe/London');
include("connect.php");
$DB = new HandelDB;
//$result = $DB->getResults("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC LIMIT 100");
if (!isset($_GET["year"])) {
    $year = (string)date("Y");
} else {
    $year = (string)htmlspecialchars($_GET["year"]);
} 
$result = $DB->getResults("SELECT * FROM `tempLog` WHERE EXTRACT(year FROM timeStamp) = {$year}");
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
        html {
            background-color: black;
        }

        body {
            padding: 20px;
            max-width: 1000px;
            margin: auto;
            background-color: white;
        }

        table.result {
            border-collapse: collapse;
        }

        table.result th {
            border: 1px solid black;
            padding: 5px;
        }

        table.result td {
            border: 1px solid black;
            padding: 5px;
        }
    </style>

</head>

<body>
    <h1>Temperature / Humidity sensor readings</h1>
    <a href="/">Live data</a>
    <p>Year: 
 
    <?php  
    $resultYear = $DB->getResults("SELECT EXTRACT(year FROM timeStamp)as year FROM `tempLog` GROUP BY EXTRACT(year FROM timeStamp) ORDER by year");

    foreach ($resultYear as $key => $value) {  
        $x=htmlspecialchars($resultYear[$key]['year']);
        echo '<a href="/result.php?year='.$x.'">'.$x.'</a> ';
    } 
    ?>
    </p>
    <p>You can pan (right click and hold) and zoom (mouse-wheel) with your mouse</p>
    <?php

    $value = array();
    foreach ($result as $key => $val) {
        // $timestamp =  strtotime($result[$key]['timeStamp']);
        $value[$key] = '[' . htmlspecialchars(strtotime($val['timeStamp']) * 1000) . ',' . htmlspecialchars($val['humidity']) . ']';

        $returnResults .= $value[$key];
        if ($key < count($result) - 1) {
            $returnResults .= ",";
        }
    }
    foreach ($result as $key => $val) {

        $valueTemp[$key] = '[' .
            htmlspecialchars(strtotime($val['timeStamp']) * 1000) .
            ',' .
            htmlspecialchars($val['temperature']) . ']';
        //$val['ID']
        $returnTempResults .= $valueTemp[$key];
        if ($key < count($result) - 1) {
            $returnTempResults .= ",";
        }
    }
    //echo print_r($value);

    //echo $returnResults;
    ?>
    <script>
        $(document).ready(function() {
            $.plot($("#placeholder"),
                [{
                        label: "Humidity",
                        data: [<?php echo $returnResults; ?>]
                    },
                    {
                        label: "Temperature",
                        data: [<?php echo $returnTempResults; ?>]
                    }
                ], {
                    yaxis: {
                        max: 100,
                        zoomRange: false,
                        panRange: false
                    },
                    xaxis: {
                        mode: 'time',
                        timeformat: '%d.%m.%Y %H:%M:%S',
                        zoomRange: [null, new Date()],
                        panRange: [null, new Date()]
                    },
                    series: {
                        lines: {
                            show: true
                        },
                        points: {
                            show: false
                        }
                    },
                    legend: {
                        show: true
                    },
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

    //$result = $DB->getResults("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC LIMIT 10");
    //$result = $DB->getResults("SELECT * FROM `tempLog` WHERE EXTRACT(year FROM timeStamp) = {$year} Limit 15");
    
    $result = $DB->getResults("SELECT date_format(timestamp, '%Y-%m') as 'year-month', TRUNCATE(avg(HUMIDITY),0) AS avgHum, TRUNCATE(avg(TEMPERATURE),0) as avgTemp FROM `tempLog` WHERE EXTRACT(year FROM timeStamp) = {$year} GROUP BY EXTRACT(YEAR_MONTH FROM timeStamp) ORDER by timeStamp;");
    //echo print_r($result);
    //print_r(array_keys($result));
    //$arrayKeys= array_keys($result);  
    $html = "";
    $html = '<table class="result">';
    // header row
    $html .= '<tr>';
    $html .= '<th> year-month </th> <th> avgTemp </th> <th> avgHum </th>';
    $html .= '</tr>';

    // data rows
    foreach ($result as $key => $value) {
        $html .= '<tr>';
        $html .= '<td>' . htmlspecialchars($result[$key]['year-month']) . '</td>';
        $html .= '<td>' . htmlspecialchars($result[$key]['avgTemp']) . '</td>';
        $html .= '<td>' . htmlspecialchars($result[$key]['avgHum']) . '</td>';
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