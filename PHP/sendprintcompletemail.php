<?php
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

require 'mailer/Exception.php';
require 'mailer/PHPMailer.php';
require 'mailer/SMTP.php';

// Get-parameters: printernaam, bestandsnaam, imagenaam
// sendprintcompletemail.php?printernaam=MK3&bestandsnaam=bestandje&imagenaam=Plaatje
$printernaam=$_GET["printernaam"];
$bestandsnaam=$_GET["bestandsnaam"];
$imagenaam=$_GET["imagenaam"];

echo(var_export($_GET));

function Meel($Onderwerp, $Aan, $HTML) {
 global $printernaam;
 global $bestandsnaam;
 global $imagenaam;
 $mail = new PHPMailer(true);
 try {
  $mail->SMTPAuth   = true;
  $mail->SMTPSecure = PHPMailer::ENCRYPTION_SMTPS;
  $mail->Port       = 465;
  $mail->setFrom('3D-printer@jouwweb.site', '3D Printer');
  $mail->addAddress($Aan);

  // Inline afbeelding van het geprintte object
  $mail->AddEmbeddedImage($imagenaam, 'preview3dobject');
  $mail->isHTML(true);
  $mail->Subject = $Onderwerp;
  $mail->Body    = $HTML;
  $mail->send();

  echo "OK";
 }
 catch (Exception $e) {
  echo "Message could not be sent. Mailer Error: {$mail->ErrorInfo}";
 }
}

date_default_timezone_set('Europe/Amsterdam');
$currentTime = date('H:i');

$body = "
 Hoi Marcel,<br><br>
 Je print-job van bestand <b>$bestandsnaam</b> op printer <b>$printernaam</b> is voltooid om $currentTime uur.<br>
 <img style='margin-top:8px;margin-bottom:8px' src=cid:preview3dobject><br>
 Groetjes,<br>Je 3D-printer.
";
Meel("Printjob $printernaam voltooid","jouwemail@gmail.com",$body);
