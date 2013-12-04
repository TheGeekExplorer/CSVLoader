<?php

    include_once('csvLoader.inc');
    
    /**
     * Run the class with a test file
     */
    $myCSV = new csvLoader;
    $d = $myCSV->uploadFile($_FILES);

    var_dump($d);