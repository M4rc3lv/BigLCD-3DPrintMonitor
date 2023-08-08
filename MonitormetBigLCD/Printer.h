#ifndef __PRINTER_H
#define __PRINTER_H

class Printer {
 public: 
  Printer(String Naam, String IPAdres, String APIKey);
  bool IsMonitoring,IsImageUploaded;
  
  void Refresh();     
  String Status(); 
  bool IsPrinting();
  int ProcentComplete();
  int Temperatuur();
  String Filenaam(int maxlen/*=0*/);
  void PostImage();

  void SendPrintReadyEmail();

  void GetImageFromPrinter();  
  void UploadImgToServer();
   
 private:
  String _Naam,_IPAdres,_APIKey;
  DynamicJsonDocument _JobJson, _PrinterJson;
  String _CurStatus,_LastFileName;
  String _Image;
};

#endif
