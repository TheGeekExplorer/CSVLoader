<?php

class CSVFormat
{
    private $CSVData        = array();
    private $formTitle      = "";
    private $formCategories = array();
    private $formColumns    = 0;
    private $formProducts   = array();
    private $html           = "";
    
    public function formatCSV($a)
    {
        // Run through the multidimensional array and
        // output formatted HTML table.
        $this->CSVData        = $a;
        $this->formTitle      = $a['formTitle'];
        $this->formCategories = $a['formCategories'];
        $this->formColumns    = count($a['formCategories']);
        $this->formProducts   = $this->retrieveProducts($a);
        $this->html           = $this->formatCategory();
        
        // If Query = debug=1 then show debug information
        $this->checkForDebugMode();
        
        return $this->html;
    }
    
    private function retrieveProducts($a)
    {
        return array_slice($a, 2);
    }

    private function checkForDebugMode()
    {
        if($_POST['debug'] == 1)
        {
            header("Content-type: text/html");
            var_dump('CSV Data:   ' . count($this->CSVData));
            var_dump('Title:      ' . $this->formTitle);
            var_dump('Categories: ' . $this->formCategories);
            var_dump('Columns:    ' . $this->formColumns);
            var_dump('Products:   ' . count($this->formProducts));
            var_dump('HTML:       ' . count($this->html));
        }
    }

    private function formatCategory()
    {        
        $h .=  '<table cellpadding="2">' . "\n";
        $h .= "<tr><td class='formTitle'>" . $this->formTitle . "</td></tr>\n\n";
        $h .= "<tr>\n";  foreach($this->formCategories as $cat) { $h .= "<td class='formCategory'>$cat</td>\n"; }  $h .= "</tr>\n\n";
            
        // Get the categories from the array and their product arrays...      
        foreach($this->formProducts as $c => $p)
        {
            // If element is empty then skip
            if(empty($c)) { continue; }
                
            $h .= "<tr><td class='categoryTitle'>" . str_replace('_', ' ', $c) . "</td></tr>\n\n";
            
            // Build the products
            foreach($p as $i)
            {            
                $h .= "<tr>\n";
            
                foreach($i as $d)
                {
                    $h .= "<td>$d</td>\n";
                }
                
                $h .= "</tr>\n\n";
            }
        }
        
        $h .= "</table>\n\n";

        return $h;
    }

}