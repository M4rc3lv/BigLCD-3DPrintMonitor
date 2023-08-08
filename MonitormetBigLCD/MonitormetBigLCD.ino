#include "Config.h"
#include "LiquidCrystal_I2C.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h> 
#include "Printer.h"

ESP8266WiFiMulti D1Mini;
LiquidCrystal_I2C LCD(0x27, 20,4);
/* Aansluiten I2C LCD
GND GND
VCC 5V
SDA D2
SCL D1 */

const char *Printers[]={
 PRINTER1,
 PRINTER2,
 PRINTER3,
 PRINTER4
};

Printer *p[NUMPRINTERS];
void Cls(const char *txt=NULL);

void setup() {
 Serial.begin(115200);
 LCD.begin(5,4); // I2C LCD aangesloten op D1 (SCL) and D2 (SDA)
 LCD.init();
 LCD.backlight();

 Cls("Wifi starten...");
 WiFi.mode(WIFI_STA);
 D1Mini.addAP(WIFI_SSID, WIFI_PASSWORD); 
 Cls("Wifi OK!"); 
 delay(500); 

 for(int i=0; i<NUMPRINTERS; i++) {
  p[i]=new Printer(Printers[i*3], Printers[i*3+1], Printers[i*3+2]);
 }
}

void loop() {
 static long BacklightTimer=millis();
 if(D1Mini.run()== WL_CONNECTED){  
  int nAantalPrinten=0;
  for(int i=0; i<NUMPRINTERS; i++) {
   p[i]->Refresh();
   ClrLine(i);
   LCD.setCursor(0,i);
   if(p[i]->IsPrinting()) {  
    nAantalPrinten++;
    LCD.backlight();
    p[i]->IsMonitoring=true;  
    int procent=p[i]->ProcentComplete();
    String sProcent(procent);
    if(procent<100) sProcent+="%"; else sProcent="100";
    LCD.printf("%3d%c %-3s %-11s", p[i]->Temperatuur(), 223/*Gradensymbooltje*/, sProcent.c_str(), p[i]->Filenaam(11).c_str());     

    if(!p[i]->IsImageUploaded) {
     p[i]->GetImageFromPrinter();
     p[i]->UploadImgToServer();
     p[i]->IsImageUploaded=true;
    }
   }
   else {
    if(p[i]->IsMonitoring) {
     p[i]->SendPrintReadyEmail();
     LCD.printf("Klaar: %s",p[i]->Filenaam(13).c_str());
     p[i]->IsMonitoring=false;
    }    
    else if(p[i]->Status().indexOf("Paused")==0) LCD.print("Gepauzeerd          ");
    else {
     String f=p[i]->Filenaam(13);
     if(f!="null" && f.length()>0)
      LCD.printf("Klaar: %s",f.c_str());
     else
      LCD.print("                    ");
    }
    p[i]->IsImageUploaded=false;
   }
  }
  // Als geen enkele printer iets doet, zet dan het LCD na 3 minuten uit
  if(nAantalPrinten<=0 && millis()-BacklightTimer>180000)
   LCD.noBacklight();
  else {
   if(nAantalPrinten>0) BacklightTimer=millis();
   LCD.backlight();
  }
 }
 delay(10000);
}

void ClrLine(int Line) {
 LCD.setCursor(0,Line);
 LCD.print("                    ");
}

void Cls(const char *txt /*=NULL*/) {
 LCD.clear();
 LCD.setCursor(0,0);
 if(txt) LCD.print(txt);
}
