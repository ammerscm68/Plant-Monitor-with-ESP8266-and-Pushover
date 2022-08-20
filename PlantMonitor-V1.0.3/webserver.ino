// Webserver Startseite
void handleRoot() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
if (SoilCalibrate == true) 
{SoilCalibrate = false; 
GetSoilMoistValue(10); // Nach dem Kalibrieren aktuellen Messwert holen  
Serial.println("Sensorkalibrierung beendet ...");}  
String RefreshTime = "";
if (!ResetNow) {RefreshTime = "15";} else {RefreshTime = "3";} // Je nach Status
// Startmodus anzeigen
String MInterval,SMMessureCount;
if (StartMessureCount > 0) {
MInterval = "Messintervall: alle "+(String)(soilMessureTrigger/2)+" Sekunden (Startmodus)";
if (MCount < 0) {SMMessureCount = "Anzahl der Messungen im Startmodus: k.A.";} else {
SMMessureCount = "Anzahl der Messungen im Startmodus: &nbsp;noch "+ (String)(MCount-1);}
} else {
MInterval = "Messintervall: alle "+measurementinterval+" Minuten";
SMMessureCount = "<br />";} // Nach Startmodus eine Leezeile
// Farbe des Messwertes je nach Wert
String ValueColor;
if (percentSoilMoist < 15) {
ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;&nbsp;</td><td id='Rvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";  
} else {
if (percentSoilMoist >= 15 && percentSoilMoist <= 60) {ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;&nbsp;</td><td id='Yvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";} else {  
ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;&nbsp;</td><td id='Gvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";}
}

 yield();
 MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
 server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }\
  h3 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }\
  #Rvalue { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FC1E17; background-color: #000000; font-size: 60px; }\
  #Yvalue { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FFC300; background-color: #000000; font-size: 60px; }\
  #Gvalue { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>Letzte Messung: "+LastMessureDateTime+"</h2>\
  <h3>"+SMMessureCount+"</h3>\
  <a><br /></a>\
  <table>"+ValueColor+"</table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/getadjustments\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue; font-size: 14px\">Einstellungen Anzeigen</button></a>\
  <a href =\"/scalibrate\"><button class=\"button\" style=\"margin-left: 50px;height: 48px; width: 180px;color: green; font-size: 14px\">Sensor Kalibieren</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a><br /></a>\
  <h3>"+MInterval+"</h3>\
  </body>\
  </html>\
  ");
MyWaitLoop(150); // kurze Pause sonst Absturz bei Browser refresh 
if (littleFSInit == true && ResetNow == true) {
ResetNow = false;  
ResetPlantSensor();} // WLAN Zugangsdaten werden gelöscht    
if (RestartNow == true) {
RestartNow = false;  
Serial.println("Sensor wird neu gestartet");
ESP.restart();} 
}

/********************************************************************************************************************************************************/

void HTMLAnswer(String WebFunction) {
String HTMLString, HTMLAnswerString;  
if (WebFunction == "SensorMoistHighLevel") {HTMLAnswerString = "Der Wert "+soilmoisthighlevel+" wurde für den 100% trockenen Sensor übernommen und gespeichert.";} 
if (WebFunction == "SensorMoistLowLevel") {HTMLAnswerString = "Der Wert "+soilmoistlowlevel+" wurde für den 100% nassen Sensor übernommen und gespeichert.";} 
if (WebFunction == "MessureLEDFlashEIN1") {HTMLAnswerString = "Die RGB-LED blinkt kurz bei der Messung";} 
if (WebFunction == "MessureLEDFlashEIN2") {HTMLAnswerString = "Die RGB-LED blinkt kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!";} 
if (WebFunction == "MessureLEDFlashAUS1") {HTMLAnswerString = "Die RGB-LED blinkt nicht kurz bei der Messung";} 
if (WebFunction == "MessureLEDFlashAUS2") {HTMLAnswerString = "Die RGB-LED blinkt nicht kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!";} 
if (WebFunction == "TMPushoverDevice1") {HTMLAnswerString = "Die Pushover Testnachricht wurde an "+PushoverDevice1+" gesendet ...";} 
if (WebFunction == "TMPushoverDevice2") {HTMLAnswerString = "Die Pushover Testnachricht wurde an "+PushoverDevice2+" gesendet ...";} 
if (WebFunction == "TMPushoverDevice3") {HTMLAnswerString = "Die Pushover Testnachricht wurde an "+PushoverDevice3+" gesendet ...";} 
if (WebFunction == "ESPRestart") {HTMLAnswerString = "Der Sensor wird neu gestartet wenn Sie zur Startseite zurückkehren.";} 
if (WebFunction == "ResetYes") {HTMLAnswerString = "Der Access Point (Captive Portal) wird gestartet wenn Sie zur Startseite zurückkehren.";}
HTMLString = "<!DOCTYPE html> <html>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 18px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h2>"+HTMLAnswerString+"</h2>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 180px;color: black; font-size: 14px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
}

/********************************************************************************************************************************************************/

void SensorCalibrate() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
String RefreshTime = "3"; // Site Refresh Time in Sekunden
SoilCalibrate = true;
 yield();
 MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
 GetSoilMoistValue(10); // Messwert Auslesen
 server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 24px; margin-left: 5px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }\
  h4 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #FCCA00;}\
  h5 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #FC7100;}\
  h6 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
  #value { width: 190px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #2ECCFA; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>"+SoilSensorname+" Sensorkalibierung</h2>\
  <a><br /></a>\
  <table><tr><td style=\"height: 10px;\">Aktueller Sensorwert:&nbsp;&nbsp;</td>\
  <td id='value'><span>"+String(soilMoist)+"</span></td></tr></table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/soilhighlevel\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: green; font-size: 14px\">Wert für 100% Trocken Übernehmen</button></a>\
  <a href =\"/soillowlevel\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 180px;color: green; font-size: 14px\">Wert für 100% Nass Übernehmen</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: black; font-size: 14px\">zurück zur Startseite</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a><br /></a>\
  <h3>Messintervall: alle 3 Sekunden</h3>\
  <a><br /></a>\
  <h6>Hilfe:</h6>\
  <h4>für den Wert von '100% trocken' den höchsten Wert übernehmen.</h4>\
  <h4>für den Wert von '100% nass' den niedrigsten Wert übernehmen. (Sensor in ein Glas Wasser tauchen)</h4>\
  <h5>Lassen Sie den Sensor mehrere Messintervalle durchlaufen bevor Sie den Wert übernehmen !</h5>\
  </body>\
  </html>\
  ");
MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh   
}

/********************************************************************************************************************************************************/

void SensorMoistHighLevel(){
soilMoistLevelHigh = soilMoist; 
soilmoisthighlevel = (String)soilMoistLevelHigh;
SetupSave("soilmoist-highlevel.txt", soilmoisthighlevel); // soil Moist High Level
Serial.println("Sensor-Messwert "+soilmoisthighlevel+" für HighLevel (Trocken) übernommen und gespeichert"); 
HTMLAnswer("SensorMoistHighLevel");
}

/********************************************************************************************************************************************************/

void SensorMoistLowLevel(){ 
soilMoistLevelLow = soilMoist;  
soilmoistlowlevel = (String)soilMoistLevelLow;  
SetupSave("soilmoist-lowlevel.txt", soilmoistlowlevel); // soil Moist Low Level    
Serial.println("Sensor-Messwert "+soilmoistlowlevel+" für LowLevel (Nass) übernommen und gespeichert");
HTMLAnswer("SensorMoistLowLevel");
}

/********************************************************************************************************************************************************/

void GetAdjustments()
{
String GASecureAppToken, GAUserToken, GAPushoverDevice1, GAPushoverDevice2, GAPushoverDevice3, GAPushoverMessageSound, POSendMessageTime, POStatus;
String TMDevice1,TMDevice2,TMDevice3;
 
if (SecureAppToken != "kA" && SecureAppToken != "") {GASecureAppToken = SecureAppToken;} else {GASecureAppToken = "k.A.";}
if (UserToken != "kA" && UserToken != "") {GAUserToken = UserToken;} else {GAUserToken = "k.A.";}
if (PushoverDevice1 != "kA" && PushoverDevice1 != "") {
GAPushoverDevice1 = PushoverDevice1;
TMDevice1 = "href =\"/potmdevice1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Gerät 1</button>";
} else {
GAPushoverDevice1 = "k.A.";
TMDevice1 = "href =\"/potmdevice1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>Gerät 1</button>";
}
if (PushoverDevice2 != "kA" && PushoverDevice2 != "") {
GAPushoverDevice2 = PushoverDevice2;
TMDevice2 = "href =\"/potmdevice2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Gerät 2</button>";
} else {
GAPushoverDevice2 = "k.A.";
TMDevice2 = "href =\"/potmdevice2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>Gerät 2</button>";
}
if (PushoverDevice3 != "kA" && PushoverDevice3 != "") {
GAPushoverDevice3 = PushoverDevice3;
TMDevice3 = "href =\"/potmdevice3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Gerät 3</button>";
} else {
GAPushoverDevice3 = "k.A.";
TMDevice3 = "href =\"/potmdevice3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>Gerät 3</button>";
}
if (PushoverMessageSound != "kA" && PushoverMessageSound != "") {GAPushoverMessageSound = PushoverMessageSound;} else {GAPushoverMessageSound = "k.A.";}
if (SendMessageTime != "kA" && SendMessageTime != "") {POSendMessageTime = OSendMessageTime;} else {POSendMessageTime = "k.A.";}

String LEDMessureFlash1, LEDMessureFlash2;
if (MessureLEDFlash == true) {LEDMessureFlash1 = "EIN*";} else {LEDMessureFlash1 = "EIN";} 
if (!MessureLEDFlash) {LEDMessureFlash2 = "AUS*";} else {LEDMessureFlash2 = "AUS";} 
if (!IsPushover) {POStatus = "Der Pushover-Dienst ist deaktiviert !";} else {POStatus ="<br />";}
server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 42px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; }\
  h4 { font-size: 24px; margin-top: 0px; margin-bottom: 0px; }\
  h5 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
  h6 { font-size: 18px; margin-top: 0px; margin-bottom: 0px; color: lime;}\
  h7 { font-size: 14px; margin-top: 10px; margin-bottom: 10px; color: #EB4034;}\
  h8 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: white;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h4>Einstellungen aus dem Captive Portal.</h4>\
     <a><br /></a>\
     <h3>WLAN-SSID:  &nbsp;&nbsp;&nbsp;"+(String)ssid+"</h3>\
     <h5>IP-Adresse: &nbsp;&nbsp;&nbsp;"+(String)ip+"</h5>\
     <h3><br /></h3>\
     <h3>Pushover-SecureAppToken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GASecureAppToken+"</h3>\
     <h3>Pushover-UserToken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAUserToken+"</h3>\
     <h3>Pushover-Gerätename1:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAPushoverDevice1+"</h3>\
     <h3>Pushover-Gerätename2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAPushoverDevice2+"</h3>\
     <h3>Pushover-Gerätename3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAPushoverDevice3+"</h3>\
     <h3>Pushover-Nachrichtensound:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAPushoverMessageSound+"</h3>\
     <h3>Pushover-Nachricht wird gesendet:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+POSendMessageTime+" Uhr</h3>\
     <h3><br /></h3>\
     <h3>Sensorname:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+SoilSensorname+"</h3>\
     <h3>Sensor-Wert für 100% Trocken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+soilmoisthighlevel+"</h3>\
     <h3>Sensor-Wert für 100% Nass:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+soilmoistlowlevel+"</h3>\
     <h3>Messintervall:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;alle "+measurementinterval+" Minuten</h3>\
     <h3><br /></h3>\
     <h3>Alarm bei Messwert unter &nbsp;&nbsp;"+AlarmMessageSoilValue+"%</h3>\
     <h7>"+POStatus+"</h7>\
     <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
     <h6>Auswahl - Pushover Testnachricht an Gerät senden:</h6>\
     <a "+TMDevice1+"</a>\
     <a "+TMDevice2+"</a>\
     <a "+TMDevice3+"</a>\
     <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
     <h6>Auswahl - LED Blinken bei Messung &nbsp; EIN / AUS:</h6>\
     <a href =\"/mledflashein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+LEDMessureFlash1+"</button></a>\
     <a href =\"/mledflashaus\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+LEDMessureFlash2+"</button></a>\
     <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
     <h3><br /></h3>\
     <h3><br /></h3>\
     <a href =\"/esprestart\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: green; font-size: 14px\">Sensor neu starten</button></a>\
     <a href =\"/reset\"><button class=\"button\" style=\"margin-left: 50px;height: 48px; width: 180px;color: red; font-size: 14px\">Zurücksetzen! (Reset)</button></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px\">zurück zur Startseite</button></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a><br /></a>\
     <h8>*) - aktuelle Auswahl</h8>\
  </body>\
    </html>");   
Serial.println("Pushover- und Sensor-Zugangsdaten aufgerufen");  
}

/********************************************************************************************************************************************************/

void MessureLEDFlashEIN() {
MessureLEDFlash = true; // Bei Messung LED Flash  
if (littleFSInit == true) {
HTMLAnswer("MessureLEDFlashEIN1");  
Serial.println("Die RGB-LED blinkt kurz bei der Messung");     
SetupSave("flashledbymeasurement.txt", "True");  
} else {
HTMLAnswer("MessureLEDFlashEIN2");  
Serial.println("Die RGB-LED blinkt kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!");}     
}

/********************************************************************************************************************************************************/

void MessureLEDFlashAUS() {
MessureLEDFlash = false; // Bei Messung LED Flash  
if (littleFSInit == true) {
HTMLAnswer("MessureLEDFlashAUS1");  
Serial.println("Die RGB-LED blinkt nicht kurz bei der Messung");     
SetupSave("flashledbymeasurement.txt", "False");  
} else {
HTMLAnswer("MessureLEDFlashAUS2");  
Serial.println("Die RGB-LED blinkt nicht kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!");} 
}

/********************************************************************************************************************************************************/

void TMPushoverDevice1() { 
Serial.println("Pushover Testnachricht an "+PushoverDevice1+" senden ...");  
SendPushoverMessage("Testnachricht für "+PushoverDevice1,PushoverDevice1,PushoverMessageSound); // Message to Device 1 
HTMLAnswer("TMPushoverDevice1");  
}

/********************************************************************************************************************************************************/

void TMPushoverDevice2() { 
Serial.println("Pushover Testnachricht an "+PushoverDevice2+" senden ...");  
SendPushoverMessage("Testnachricht für "+PushoverDevice2,PushoverDevice2,PushoverMessageSound); // Message to Device 2
HTMLAnswer("TMPushoverDevice2");  
}

/********************************************************************************************************************************************************/

void TMPushoverDevice3() { 
Serial.println("Pushover Testnachricht an "+PushoverDevice3+" senden ...");  
SendPushoverMessage("Testnachricht für "+PushoverDevice3,PushoverDevice3,PushoverMessageSound); // Message to Device 3
HTMLAnswer("TMPushoverDevice3");  
}

/********************************************************************************************************************************************************/

void ESPRestart(){
// ESP neu starten
HTMLAnswer("ESPRestart");
RestartNow = true; // Neu starten initialisieren   
Serial.println("Der Sensor wird neu gestartet");       
}

/********************************************************************************************************************************************************/

void ResetSettings()
{
server.send(200, "text/html", 
 "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-left: 10px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-left: 180px; margin-top: 0px; margin-bottom: 50px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h2>WLAN-Zugangsdaten werden gelöscht. (Die Parameter bleiben erhalten)</h2>\
    <h3>Jetzt WLAN-Zugangsdaten löschen ?</h3>\
    <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 190px;height: 28px; width: 90px; color: red\">JA</button></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 28px; width: 90px\">NEIN</button></a>\
  </body>\
    </html>");  
}

/********************************************************************************************************************************************************/

void ResetYes()
{ /*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
HTMLAnswer("ResetYes");
ResetNow = true; // Reset initialisieren      
}

/********************************************************************************************************************************************************/

String HTTPCodeText(int httpcode)
{
String TempHTTPCodeText = "k.A.";  
if (httpcode == -1) {TempHTTPCodeText = "ERROR_CONNECTION_REFUSED";}  
if (httpcode == -2) {TempHTTPCodeText = "ERROR_SEND_HEADER_FAILED";} 
if (httpcode == -3) {TempHTTPCodeText = "ERROR_SEND_PAYLOAD_FAILED";} 
if (httpcode == -4) {TempHTTPCodeText = "ERROR_NOT_CONNECTED";} 
if (httpcode == -5) {TempHTTPCodeText = "ERROR_CONNECTION_LOST";} 
if (httpcode == -6) {TempHTTPCodeText = "ERROR_NO_STREAM";} 
if (httpcode == -7) {TempHTTPCodeText = "ERROR_NO_HTTP_SERVER";} 
if (httpcode == -8) {TempHTTPCodeText = "ERROR_TOO_LESS_RAM";} 
if (httpcode == -9) {TempHTTPCodeText = "ERROR_ENCODING";} 
if (httpcode == -10) {TempHTTPCodeText = "ERROR_STREAM_WRITE";} 
if (httpcode == -11) {TempHTTPCodeText = "ERROR_READ_TIMEOUT";}

if (httpcode == 100) {TempHTTPCodeText = "Continue";}
if (httpcode == 101) {TempHTTPCodeText = "Switching Protocols";}
if (httpcode == 102) {TempHTTPCodeText = "Processing (WebDAV)";}
if (httpcode == 200) {TempHTTPCodeText = "OK";}
if (httpcode == 201) {TempHTTPCodeText = "Created";}
if (httpcode == 202) {TempHTTPCodeText = "Accepted";}
if (httpcode == 203) {TempHTTPCodeText = "Non-Authoritative Information";}
if (httpcode == 204) {TempHTTPCodeText = "No Content";}
if (httpcode == 205) {TempHTTPCodeText = "Reset Content";}
if (httpcode == 206) {TempHTTPCodeText = "Partial Content";}
if (httpcode == 207) {TempHTTPCodeText = "Multi-Status (WebDAV)";}
if (httpcode == 208) {TempHTTPCodeText = "Already Reported (WebDAV)";}
if (httpcode == 226) {TempHTTPCodeText = "IM Used";}

if (httpcode == 300) {TempHTTPCodeText = "Multiple Choices";}
if (httpcode == 301) {TempHTTPCodeText = "Moved Permanently";}
if (httpcode == 302) {TempHTTPCodeText = "Found";}
if (httpcode == 303) {TempHTTPCodeText = "See Other";}
if (httpcode == 304) {TempHTTPCodeText = "Not Modified";}
if (httpcode == 305) {TempHTTPCodeText = "Use Proxy";}
if (httpcode == 306) {TempHTTPCodeText = "(Unused)";}
if (httpcode == 307) {TempHTTPCodeText = "Temporary Redirect";}
if (httpcode == 308) {TempHTTPCodeText = "Permanent Redirect (experimental)";}

if (httpcode == 400) {TempHTTPCodeText = "Bad Request";}
if (httpcode == 401) {TempHTTPCodeText = "Unauthorized";}
if (httpcode == 402) {TempHTTPCodeText = "Payment Required";}
if (httpcode == 403) {TempHTTPCodeText = "Forbidden";}
if (httpcode == 404) {TempHTTPCodeText = "Not Found";}
if (httpcode == 405) {TempHTTPCodeText = "Method Not Allowed";}
if (httpcode == 406) {TempHTTPCodeText = "Not Acceptable";}
if (httpcode == 407) {TempHTTPCodeText = "Proxy Authentication Required";}
if (httpcode == 408) {TempHTTPCodeText = "Request Timeout";}
if (httpcode == 409) {TempHTTPCodeText = "Conflict";}
if (httpcode == 410) {TempHTTPCodeText = "Gone";}
if (httpcode == 411) {TempHTTPCodeText = "Length Required";}
if (httpcode == 412) {TempHTTPCodeText = "Precondition Failed";}
if (httpcode == 413) {TempHTTPCodeText = "Request Entity Too Large";}
if (httpcode == 414) {TempHTTPCodeText = "Request-URI Too Long";}
if (httpcode == 415) {TempHTTPCodeText = "Unsupported Media Type";}
if (httpcode == 416) {TempHTTPCodeText = "Requested Range Not Satisfiable";}
if (httpcode == 417) {TempHTTPCodeText = "Expectation Failed";}
if (httpcode == 418) {TempHTTPCodeText = "I'm a teapot (RFC 2324)";}
if (httpcode == 420) {TempHTTPCodeText = "Enhance Your Calm (Twitter)";}
if (httpcode == 422) {TempHTTPCodeText = "Unprocessable Entity (WebDAV)";}
if (httpcode == 423) {TempHTTPCodeText = "Locked (WebDAV)";}
if (httpcode == 424) {TempHTTPCodeText = "Failed Dependency (WebDAV)";}
if (httpcode == 425) {TempHTTPCodeText = "Reserved for WebDAV";}
if (httpcode == 426) {TempHTTPCodeText = "Upgrade Required";}
if (httpcode == 428) {TempHTTPCodeText = "Precondition Required";}
if (httpcode == 429) {TempHTTPCodeText = "Too Many Requests";}
if (httpcode == 431) {TempHTTPCodeText = "Request Header Fields Too Large";}
if (httpcode == 444) {TempHTTPCodeText = "No Response (Nginx)";}
if (httpcode == 449) {TempHTTPCodeText = "Retry With (Microsoft)";}
if (httpcode == 450) {TempHTTPCodeText = "Blocked by Windows Parental Controls (Microsoft)";}
if (httpcode == 451) {TempHTTPCodeText = "Unavailable For Legal Reasons";}
if (httpcode == 499) {TempHTTPCodeText = "Client Closed Request (Nginx)";}

if (httpcode == 500) {TempHTTPCodeText = "Internal Server Error";}
if (httpcode == 501) {TempHTTPCodeText = "Not Implemented";}
if (httpcode == 502) {TempHTTPCodeText = "Bad Gateway";}
if (httpcode == 503) {TempHTTPCodeText = "Service Unavailable";}
if (httpcode == 504) {TempHTTPCodeText = "Gateway Timeout";}
if (httpcode == 505) {TempHTTPCodeText = "HTTP Version Not Supported";}
if (httpcode == 506) {TempHTTPCodeText = "Variant Also Negotiates (Experimental)";}
if (httpcode == 507) {TempHTTPCodeText = "Insufficient Storage (WebDAV)";}
if (httpcode == 508) {TempHTTPCodeText = "Loop Detected (WebDAV)";}
if (httpcode == 509) {TempHTTPCodeText = "Bandwidth Limit Exceeded (Apache)";}
if (httpcode == 510) {TempHTTPCodeText = "Not Extended";}
if (httpcode == 511) {TempHTTPCodeText = "Network Authentication Required";}
if (httpcode == 598) {TempHTTPCodeText = "Network read timeout error";}
if (httpcode == 599) {TempHTTPCodeText = "Network connect timeout error";}
return String(httpcode) + " - " + TempHTTPCodeText;
}

/********************************************************************************************************************************************************/
