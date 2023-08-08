<?php
if(isset($_GET["naam"])) {
 $rawImage = file_get_contents('php://input');
 // Schrijf naar png-bestand
 $fp = fopen($_GET["naam"], 'wb');
 fwrite($fp, $rawImage);
 fclose($fp);
}
