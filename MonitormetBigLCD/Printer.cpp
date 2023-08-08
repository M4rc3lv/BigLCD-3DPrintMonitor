#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> // Library van Benoit BLANCHON
#include "Printer.h"
#include <UrlEncode.h>
#include "Config.h"

Printer::Printer(String Naam, String IPAdres, String APIKey) 
:_JobJson(400),_PrinterJson(600)
 { 
 _Naam=Naam;
 _IPAdres=IPAdres;
 _APIKey=APIKey; 
 IsMonitoring=IsImageUploaded=false;
}

void Printer::Refresh() { 
 WiFiClient Wifi;
 HTTPClient http;  
 http.begin(Wifi,_IPAdres+String("/api/job")); 
 http.addHeader("X-Api-Key", _APIKey);        
 int httpCode = http.GET();
 if(httpCode > 0) {     
  String payload = http.getString();  
  deserializeJson(_JobJson, payload);
  Serial.print("JOB:");Serial.println(payload);
 } 
 http.end();
 http.begin(Wifi,_IPAdres+String("/api/printer")); 
 http.addHeader("X-Api-Key", _APIKey);        
 httpCode = http.GET();
 if(httpCode > 0) {     
  String payload = http.getString();  
  deserializeJson(_PrinterJson, payload);
  Serial.print(String("Printer ")+String(_Naam)+String(": "));
  Serial.println(payload);
 } 
 http.end();
}

String Printer::Status() {
 return _JobJson["state"].as<String>();
}

int Printer::ProcentComplete() {
 return (int)round(_JobJson["progress"]["completion"].as<float>()*100.0);
}

bool Printer::IsPrinting() {
 return Status().indexOf("Printing")>=0;
}

int Printer::Temperatuur() {
 float t =_PrinterJson["temperature"]["tool0"]["actual"].as<float>();
 Serial.println(t);
 return (int)round(t);
}

String Printer::Filenaam(int maxlen/*=0*/) {
 String f=_JobJson["job"]["file"]["name"].as<String>(); 
 if(f.length()>0 && f!="null")
  _LastFileName=f;

 f=_LastFileName; 
 int dotIndex=f.lastIndexOf('.');
 if(dotIndex >= 0)
  f = f.substring(0, dotIndex); 
 if(maxlen>0 && f.length()>maxlen)
  f=f.substring(0,maxlen);
 return f;
}

void Printer::SendPrintReadyEmail() {
 // sendprintcompletemail.php?printernaam=MK3&bestandsnaam=bestandje&imagenaam=Plaatje
 WiFiClient Wifi;  
 HTTPClient http;
 String url=String(MY_WEBSITE)+
  String("sendprintcompletemail.php?printernaam=")+urlEncode(_Naam)+
  String("&bestandsnaam=")+urlEncode(Filenaam(100))+
  String("&imagenaam=")+urlEncode(_Naam)+urlEncode(".png");
 Serial.println(url); 
 http.begin(Wifi,url); 
 int httpCode = http.GET();
 if(httpCode>0) {
  Serial.print("E-mail verstuurd!");  
 }
 else Serial.printf("Code:%d\1\n",httpCode); 
 http.end(); 
}

void Printer::GetImageFromPrinter() {
 String PicFileName=_JobJson["job"]["file"]["path"].as<String>(); // /usb/TRECHT~3.GCO ==>  thumb/l/usb/TRECHT~2.GCO
 Serial.printf("Imgfile: %s\r\n",PicFileName.c_str());
 WiFiClient Wifi;  
 HTTPClient http;
 http.begin(Wifi,_IPAdres+"/thumb/l"+PicFileName);   
 http.addHeader("X-Api-Key", _APIKey);  
 int httpCode = http.GET();
 if(httpCode>0) {
  _Image=http.getString(); 
  Serial.printf("Inlezen gelukt, lengte=%ld\r\n",_Image.length());  
 }
 http.end(); 
}

void Printer::UploadImgToServer() {
 Serial.printf("Verstuur naar mijn server %d\r\n",_Image.length());
 WiFiClient c;      
 HTTPClient http2;
 http2.begin(c,String(MY_WEBSITE)+String("naam=")+urlEncode(_Naam+".png"));  
 http2.addHeader("Content-Type","application/octet-stream");
 Serial.println("0");
 //http2.POST((uint8_t*)pic.c_str(),pic.length());
 http2.sendRequest("PUT",(uint8_t*)_Image.c_str(),_Image.length());
 Serial.println("1");
 http2.end();
 
 _Image="";
}
