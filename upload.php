<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>

		<title>My Premier Materials - (Premier 09) Leaflets, Shelf Cards, etc</title>
		<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
		<meta name="description" content=""/>
		<meta name="keywords" content="websites"/>
        
        <style type="text/css">
        body,h1,h2,h3,h4,h5,h6,div,p,span,hgroup,table,tr,td,ol,ul,li { font-family:tahoma; font-size:13px; color:#000; }
        .formTitle      { font-weight:bold; font-size:18px; color:#411143; border:0; padding-top:20px; margin:0; }
        .formCategory   { font-weight:bold; font-size:13px; color:#fff; background:#444; text-align:center; vertical-align:middle; border:0; padding-top:10px; padding-bottom:10px; margin:0; }
        .categoryTitle  { font-weight:bold; font-size:14px; color:#411143; border:0; padding-top:20px; padding-bottom:10px; margin:0; }
        td { font-size:13px; color:#333; border-bottom:1px solid #444; padding:5px; }
        tr { padding:0; margin:0; }
        </style>

    </head>
    
<?php

    include_once('csvLoader.inc');
    include_once('csvFormat.inc');
    
    /**
     * Run the class with a test file
     */
    $myCSV = new CSVLoader;
    $a = $myCSV->uploadFile($_FILES);
    
    /**
     * CSV HTML Formatter
     */
    $myFormat = new CSVFormat;
    echo $myFormat->formatCSV($a);

?>

    </body>

</html>