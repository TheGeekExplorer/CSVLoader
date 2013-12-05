<?php

class CSVLoader
{
    const MAX_FILE_SIZE  =  200000;
    const UPLOAD_ROOT    =  './uploads/';
    private $errorLog    =  array();
    private $errorCode   =  array(
        'fileSize'       => 'The size of the file you are trying to upload exceeds the maximum set. Please revise this list.',
        'fileType'       => 'The type of file that you have uploaded is not CSV.',
        'invalidFormat'  => 'Your CSV sheet formatting is invalid.',
        'noProductRows'  => 'Your CSV has no product rows.  The format is invalid.',
        'maximumColumns' => 'You specified too many categories.  Restricted to 10 maximum.',
        'generalError'   => 'CSV File could not be opened by the system.  Please check with the website administrator.',
        'uploadNoSave'   => 'Could not save the uploaded file.  Please check with the website administrator.'
    );
    
    public function readFile($f)
    {
        return $this->loadFile($f);
    }
    
    /**
     * UPLOADING THE CSV FILE
     */
    public function uploadFile($f)
    {
        $allowedExts = array("csv");
        $temp = explode(".", $f["file"]["name"]);
        $extension = end($temp);
        
        // Checks if file is csv ext, and is under 200kb...
        if ($this->testFileParticulars($f))
        {
            if ($f["file"]["error"] > 0)
            {
                $this->castError($_FILES["file"]["error"]);
            }
            else
            {
                // Save the upload file...
                if(
                    !move_uploaded_file($f["file"]["tmp_name"], CSVLoader::UPLOAD_ROOT . $f["file"]["name"]) ||
                    $this->testFileExists($_FILES["file"]["name"]) === false)
                {
                    $this->castError('uploadNoSave');
                }

                /**
                 * If the file was successfully uploaded then continue to load,
                 * otherwise return the error log to the user.
                 */
                if(empty($this->errorLog))
                {
                    // Now load the file...
                    return $this->loadFile(CSVLoader::UPLOAD_ROOT . $f["file"]["name"]);
                }
                else
                {
                    return $this->errorLog;
                }
            }
        }
        else
        {
            $this->testFileParticulars($f);
        }    
    }

    /**
     * LOAD & TEST THE CSV FILE
     */
    private function loadFile($f)
    {
        $rowCount   = 1;
        $formFields = array();
        $safeCats   = array();
        
        $fh = fopen($f, "r");
        
        if($fh) {
            while (($line = fgets($fh)) !== false) {
            
                // Strip new lines from line
                $line = $this->stripLines($line);

                // Get the form Title:
                if($rowCount == 1) {
                    $formTitle = $this->formTitle($line);
                
                    // Do sanity checks on the row:
                    $this->testFormTitle($formTitle);

                    // Continue to next row:
                    $rowCount++; continue;
                }

                // Get the form categories:
                if($rowCount == 2) {
                    $formCategories = $this->formCategories($line);

                    // Do sanity checks on the row:
                    $this->testFormCategories($formCategories);

                    $rowCount++; continue;
                }

                // Get the product rows:
                if($rowCount > 2)
                {
                    // Push the fields array into the main fields array - multidimensional.
                    $fields = explode(",", $line);

                    // Add the row array to the category array.    Example:    $HouseholdCleaning[] = $fields
                    $safeCategory = str_replace(" ", "_", $fields[0]);
                    //$safeCategory = preg_replace("/[^A-Za-z0-9]/", '', $fields[0]);

                    // If array does not exist then create it.
                    if(!isset(${$safeCategory}))
                      {  ${$safeCategory} = array();  } 

                    // Put the row fields into category array.
                    ${$safeCategory}[] = $fields;

                    // Record the safeCategory into array for later consolidation...
                    $safeCats[$safeCategory] = 1;
                    
                    // Do some sanity checks:
                    $this->testFormProductRow($line, $fields);

                    $rowCount++; continue;
                }

            }

            // Add the categories and title to the output array...
            $formFields['formTitle']      = $formTitle;
            $formFields['formCategories'] = $formCategories;

            // Consolidate the category arrays into formFields...
            foreach($safeCats as $cat => $val)
            {
                $formFields[$cat] = ${$cat};
            }
            
        } else {
            // Error - return error message.
            $this->castError('generalError');
        }
        
        /**
         * If there are errors then return them:
         */
        if(!empty($this->errorLog))
        {
            return $this->errorLog;
        }
        else
        {
            // Else return the data array:
            return $formFields;
        }
    }
    
    private function stripLines($l)
    {
        $l = str_replace("\n\r", "", $l);
        return str_replace("\n", "", $l);
    }

    private function formTitle($l)
    {
        $formTitle = explode(",", $l);
        if(!empty($formTitle[1]))
        {
            return false;
        }
        else
        {
            return $this->stripLines($formTitle[0]);
        }
    }

    private function formCategories($l)
    {
        $formCategories = explode(",", $l);
        if(empty($formCategories[1]))
        {
            return false;
        }
        else
        {
            return $formCategories;
        }
    }

    /**
     * TEST THE INPUT DATA
     */
    private function testFileExists($f)
    {
        if(file_exists(CSVLoader::UPLOAD_ROOT . $f))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    private function testFileParticulars($f)
    {
        $fileExt = explode(".", $f["file"]["name"]);
        
        if($f["file"]["size"] > CSVLoader::MAX_FILE_SIZE)
        {
            $this->castError('fileSize');
            return false;
        }
        else if(end($fileExt) !== 'csv')
        {
            $this->castError('fileType');
            return false;
        }
        else
        {
            return true;
        }
    }
    
    private function testFormTitle($t)
    {
        if(empty($t) || $t === false)
        {
            $this->castError('invalidFormat');
            return false;
        }
        else
        {
            return true;
        }
    }
    
    private function testFormCategories($a)
    {
        if(empty($a) || $a === false || !is_array($a)) {
            $this->castError('invalidFormat');
            return false;
        }
        else if(count($a) > 10)
        {
            $this->castError('maximumColumns');
            return false;
        }
        else
        {
            return true;
        }
    }
    
    private function testFormProductRow($l,$a)
    {
        if(empty($l) || empty($a))
        {
            $this->castError('noProductRows');
            return false;
        }
        else
        {
            return true;
        }
    }
    
    /**
     * THROW NEW ERROR
     */
    private function castError($err)
    {
        if(in_array($err, $this->errorCode))
        {
            $this->errorLog[] = $this->errorCode[$err];
        }
        else
        {
            $this->errorLog[] = $err;
        }
        return true;
    }
}