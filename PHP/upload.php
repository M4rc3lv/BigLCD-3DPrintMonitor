<?php
/*
 try {
  // Sla posted image op
  file_put_contents("t.txt",var_export($_POST,true));
  file_put_contents("f.txt",var_export($_FILES,true));

  $file_tmp = $_FILES['image']['tmp_name'];
  move_uploaded_file($file_tmp, $_FILES['image']['name']);
  //$img = $_FILES['image'];
  //file_put_contents('image.png', $img);
 }
 catch(Exception $e) {
  file_put_contents("e.txt",$e->getMessage());
 }*/

if(isset($_GET["naam"])) {

 $rawImage = file_get_contents('php://input');
 // Schrijf naar png-bestand
 $fp = fopen($_GET["naam"], 'wb');
 fwrite($fp, $rawImage);
 fclose($fp);
}
