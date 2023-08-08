#ifndef __CONFIG_H
#define __CONFIG_H

#define WIFI_SSID "Wifi SSID"
#define WIFI_PASSWORD "Wifi Wachtwoord"

// Lijst van 3D-Printers
// Naam, IP-adres, API key (wachtwoord van maker-user)
#define PRINTER1 "MK3.9", "http://192.168.0.10", "APIKEY"
#define PRINTER2 "MK4","http://192.168.0.49:80","APIKEY"
#define PRINTER3 "MK4-2","http://192.168.0.50:80","APIKEY"
#define PRINTER4 "Mini","http://192.168.0.4:80","APIKEY"

// Je website ergens online waar de PHP-bestanden staan (moet eindigen op een /)
#define MY_WEBSITE "http://jouw.website.nl/"

// Hieronder niets meer wijzigen
#define NUMPRINTERS (sizeof(Printers) / sizeof(Printers[0])/3)

#endif
