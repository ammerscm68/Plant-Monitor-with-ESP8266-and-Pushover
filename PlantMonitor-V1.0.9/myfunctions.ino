// ****************************Hilfsfunktionen******************************************************
void clearscreen() { 
for(int i=0; i<10; i++) {
Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
}

// *************************************************************************************************

void LoadAdjustments(){
// *** Einstellungen laden .... ***
String TMessureLEDFlash, MDTime; // Hilfsvariable

// WEMOS D1 Little-Filesystem aktivieren
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); //waits for data

if (littleFSInit == true)
{
  Serial.println("Load Plant-Sensor Adjustments ...");
  // Zugangsdaten WiFi laden
  if (LittleFS.exists("/ssid.txt")){
  lssid = SetupLoad("ssid.txt");} else {lssid = "";}
  lssid.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/ssid-passwd.txt")){
  lpassword = SetupLoad("ssid-passwd.txt");} else {lpassword = "";}
  lpassword.trim(); // Leerzeichen vorne und Hinten entfernen
  ssid = lssid.c_str();
  password = lpassword.c_str();
  // Pushover Daten laden
  if (LittleFS.exists("/po-secureapptoken.txt")){
  SecureAppToken = SetupLoad("po-secureapptoken.txt");} else {SecureAppToken = "kA";}
  SecureAppToken.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-usertoken.txt")){
  UserToken = SetupLoad("po-usertoken.txt");} else {UserToken = "kA";}
  UserToken.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-device1.txt")){
  PushoverDevice1 = SetupLoad("po-device1.txt");} else {PushoverDevice1 = "kA";}
  PushoverDevice1.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-device2.txt")){
  PushoverDevice2 = SetupLoad("po-device2.txt");} else {PushoverDevice2 = "kA";}
  PushoverDevice2.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-device3.txt")){
  PushoverDevice3 = SetupLoad("po-device3.txt");} else {PushoverDevice3 = "kA";}
  PushoverDevice3.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-messagesound.txt")){
  PushoverMessageSound = SetupLoad("po-messagesound.txt");} else {PushoverMessageSound= "none";}
  PushoverMessageSound.trim(); // Leerzeichen vorne und Hinten entfernen
  PushoverMessageSound.toLowerCase(); // Alle Zeichen in kleine umwandeln
  if (LittleFS.exists("/po-sendmessagetime.txt")){
  SendMessageTime = SetupLoad("po-sendmessagetime.txt");} else {SendMessageTime = "kA";}
  SendMessageTime.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/soilmoist-sensorname.txt")){
  SoilSensorname = SetupLoad("soilmoist-sensorname.txt");} else {SoilSensorname = "kA";}
  SoilSensorname.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/soilmoist-highlevel.txt")){
  soilmoisthighlevel = SetupLoad("soilmoist-highlevel.txt");} else {soilmoisthighlevel = "856";}
  soilmoisthighlevel.trim(); // Leerzeichen vorne und Hinten entfernen
  if (!IsNumeric(soilmoisthighlevel)) {soilmoisthighlevel = "856";} else {
  soilMoistLevelHigh = soilmoisthighlevel.toInt();} // Wert in Integer umwandeln
  if (LittleFS.exists("/soilmoist-lowlevel.txt")){
  soilmoistlowlevel = SetupLoad("soilmoist-lowlevel.txt");} else {soilmoistlowlevel = "382";}
  soilmoistlowlevel.trim(); // Leerzeichen vorne und Hinten entfernen
  if (!IsNumeric(soilmoistlowlevel)) {soilmoistlowlevel = "382";} else {
  soilMoistLevelLow = soilmoistlowlevel.toInt();} // Wert in Integer umwandeln
  if (LittleFS.exists("/measurementinterval.txt")){
  measurementinterval = SetupLoad("measurementinterval.txt");} else {measurementinterval = "10";}
  measurementinterval.trim(); // Leerzeichen vorne und Hinten entfernen
  if (!IsNumeric(measurementinterval)) {measurementinterval = "10";} else {
  measurementinterval1 = measurementinterval.toInt();} // Wert in Integer umwandeln
  if (LittleFS.exists("/alarmsoilmeasurevalue.txt")){
  AlarmMessageSoilValue = SetupLoad("alarmsoilmeasurevalue.txt");} else {AlarmMessageSoilValue = "30";}
  AlarmMessageSoilValue.trim(); // Leerzeichen vorne und Hinten entfernen
  if (!IsNumeric(AlarmMessageSoilValue)) {AlarmMessageSoilValue1 = 30;} else {
  AlarmMessageSoilValue1 = AlarmMessageSoilValue.toInt();} // Wert in Integer umwandeln
  if (LittleFS.exists("/flashledbymeasurement.txt")){
  TMessureLEDFlash = SetupLoad("flashledbymeasurement.txt");} // RGB-LED blinken lassen bei Messung (oder auch nicht)
  if (TMessureLEDFlash == "True" || TMessureLEDFlash == "") {MessureLEDFlash = true;} else {MessureLEDFlash = false;}

  // Prüfen
  if (SoilSensorname == "kA" || SoilSensorname == "") {SoilSensorname = "Pflanze";} // wenn kein Sensorname angegeben wurde

  // Send Message Time check
  OSendMessageTime = SendMessageTime; // Originale Uhrzeit
  if (SendMessageTime.length()  > 5 || SendMessageTime.length()  < 3) {SendMessageTime = "";}
  if (SendMessageTime != "") {
  if (strstr(SendMessageTime.c_str(), ":") == NULL) {SendMessageTime = "";};
  if (SendMessageTime != "") {
  if (SendMessageTime.substring(0,1) == "0") {SendMessageTime.remove(0, 1);}
  int DP = SendMessageTime.indexOf(':')+1;
  if (SendMessageTime.substring(DP,DP+1) == "0") {SendMessageTime.remove(DP, 1);}
  if (SendMessageTime.length()  > 5 || SendMessageTime.length()  < 3) {SendMessageTime = "";}
if (SendMessageTime != "") {
MDTime = false;
int DP = SendMessageTime.indexOf(':'); 
String CheckMDHourOn = SendMessageTime.substring(0,DP); 
String CheckMDMinOn =  SendMessageTime.substring(DP+1,6);
CheckMDMinOn.trim();
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOn) {MDTime = true; break;}} 
if (!MDTime) {SendMessageTime = "";} else {   
MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOn) {MDTime = true; break;}} 
if (!MDTime) {SendMessageTime = "";}}}}}
if (SendMessageTime == "") {
if (SecureAppToken != "kA" && SecureAppToken != "" && UserToken != "kA" && UserToken != "" && PushoverDevice1 != ""){
SendMessageTime = "12:15";} else {SendMessageTime = "kA";}}
  
  // for WiFi-Manager
  if (SecureAppToken != "kA" && SecureAppToken != "" && UserToken != "kA" && UserToken != ""){
  SecureAppToken.toCharArray(WMsecureapptoken, 50); // Pushover SecureAppToken und UserToken
  UserToken.toCharArray(WMusertoken, 50); 
  if (PushoverDevice1 != "kA" && PushoverDevice1 != ""){
  PushoverDevice1.toCharArray(WMpushoverdevice1, 50);} else {IsPushover = false;} // Pushover Devicename 1
  if (PushoverDevice2 != "kA" && PushoverDevice2 != ""){
  PushoverDevice2.toCharArray(WMpushoverdevice2, 50);} // Pushover Devicename 2
  if (PushoverDevice3 != "kA" && PushoverDevice3 != ""){
  PushoverDevice3.toCharArray(WMpushoverdevice3, 50);} // Pushover Devicename 3
  } else {IsPushover = false;}  // kein Pushover Service
  PushoverMessageSound.toCharArray(WMpushovermsound, 50); // Pushover Message Sound
  if (SendMessageTime != "kA" && SendMessageTime != ""){
  SendMessageTime.toCharArray(WMsendmessagetime, 6);} // Pushover Send Message Time
  SoilSensorname.toCharArray(WMsoilsname, 30); // Sensor-Name
  soilmoisthighlevel.toCharArray(WMSMLH, 4); // soilMoistLevelHigh
  soilmoistlowlevel.toCharArray(WMSMLL, 4); // soilMoistLevelLow
  measurementinterval.toCharArray(WMMessureInt, 3); // Messintervall
  AlarmMessageSoilValue.toCharArray(WMalarmsoilvalue, 4); // Alarm by soil moisture < X percent
  
  /*Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded-Pushover] SecureAppToken: " + SecureAppToken); // Only for Debuging 
  Serial.println("[Loaded-Pushover] UserToken: " + UserToken); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverDeviceName1: " + PushoverDevice1); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverDeviceName2: " + PushoverDevice2); // Only for Debuging
  Serial.println("[Loaded-Pushover] PushoverDeviceName3: " + PushoverDevice3); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverMessageSound: " + PushoverMessageSound); // Only for Debuging 
  Serial.println("[Loaded-Pushover] SendMessageTime: " + SendMessageTime); // Only for Debuging
  Serial.println("[Loaded-soilMoist] AlarmMessageSoilValue: " + AlarmMessageSoilValue); // Only for Debuging
  Serial.println("[Loaded- soilMoist] Sensor-Name: " + SoilSensorname); // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelHigh: " + soilmoisthighlevel);  // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelLow: " + soilmoistlowlevel);  // Only for Debuging
  Serial.println("[Loaded-soilMoist] Messurement Interval: " + measurementinterval);  // Only for Debuging*/
} else
    {
     // Standardwerte setzen bei "Little FS" Fehler
    ssid = "wiFi";
    password = "xxxxx"; 
    SecureAppToken = "0987654321";
    UserToken = "1234567890";
    PushoverDevice1 = "Handy";
    PushoverDevice2 = "";
    PushoverDevice3 = "";
    PushoverMessageSound = "none";
    SoilSensorname = "Pflanze";
    soilmoisthighlevel = "856";
    soilmoistlowlevel = "382";
    measurementinterval = "10";
    SendMessageTime = "12:15";
    AlarmMessageSoilValue = "30";
    Serial.println("Load Plant-Sensor Adjustments fail !!!");
    }  
}

// ************************************************************************************************************************

void SaveAdjustments(){
String sassid, sapasswd, sasecureapptoken, sausertoken, sapushoverdevice1, sapushoverdevice2, sapushoverdevice3, sapushovermsound, sasoilname, saWMSMLH, saWMSMLL, saWMMessure,sasendmessagetime;  
String saWMalarmsoilvalue;
int saWMMessureInt, saWMalarmsoilvalueInt;
if (littleFSInit == true)
{
Serial.println(""); 
Serial.println("Save Plant-Monitor Adjustments ..."); 
sassid = (String)wifiManager.getWiFiSSID();
sassid.trim();
sapasswd = (String)wifiManager.getWiFiPass();
sapasswd.trim();
sasecureapptoken = (String)WMsecureapptoken; // Pushover SecureAppToken
sasecureapptoken.trim(); // Pushover SecureAppToken
sausertoken = (String)WMusertoken; // Pushover UserToken
sausertoken.trim(); // Pushover UserToken
sapushoverdevice1 = (String)WMpushoverdevice1;
sapushoverdevice1.trim(); // Pushover Devicename 1
sapushoverdevice2 = (String)WMpushoverdevice2;
sapushoverdevice2.trim(); // Pushover Devicename 2
sapushoverdevice3 = (String)WMpushoverdevice3;
sapushoverdevice3.trim(); // Pushover Devicename 3
sapushovermsound = (String)WMpushovermsound;
sapushovermsound.trim(); // Pushover Message Sound
sapushovermsound.toLowerCase(); // Alle Zeichen in kleine umwandeln
sasendmessagetime = (String)WMsendmessagetime;
sasendmessagetime.trim(); // Pushover Send Message Time
sasoilname = (String)WMsoilsname;
sasoilname.trim(); // SoilMoist Name
saWMSMLH = (String)WMSMLH;
saWMSMLH.trim(); // 100% Dry Value
saWMSMLL = (String)WMSMLL;
saWMSMLL.trim(); // 100% Wet Value
saWMMessure = (String)WMMessureInt;
saWMMessure.trim(); // Messure Interval
saWMalarmsoilvalue = (String)WMalarmsoilvalue;
saWMalarmsoilvalue.trim(); // Alarm by soil moisture < X percent

/*Serial.println("[Save-WIFI] SSID: " + sassid);  // Only for Debuging 
Serial.println("[Save-WIFI] PASS: " + sapasswd);   // Only for Debuging 

Serial.println("[Save-Pushover] SecureAppToken: " + sasecureapptoken);  // Only for Debuging 
Serial.println("[Save-Pushover] UserToken: " + sausertoken);  // Only for Debuging 
Serial.println("[Save-Pushover] PushoverDeviceName1: " + sapushoverdevice1);  // Only for Debuging
Serial.println("[Save-Pushover] PushoverDeviceName2: " + sapushoverdevice2);  // Only for Debuging
Serial.println("[Save-Pushover] PushoverDeviceName3: " + sapushoverdevice3);  // Only for Debuging
Serial.println("[Save-Pushover] PushoverMessageSound: " + sapushovermsound);  // Only for Debuging
Serial.println("[Save-Pushover] SendMessageTime: " + sasendmessagetime);  // Only for Debuging
Serial.println("[Save-soilMoist] AlarmMessageSoilValue: " + saWMalarmsoilvalue); // Only for Debuging
Serial.println("[Save-soilMoist] Sensor-Name: " + sasoilname);  // Only for Debuging  
Serial.println("[Save-soilMoist] soilMoistLevelHigh: " + saWMSMLH);  // Only for Debuging  
Serial.println("[Save-soilMoist] soilMoistLevelLow: " + saWMSMLL);  // Only for Debuging
Serial.println("[Save-soilMoist] Messurement Interval: " + saWMMessureInt);  // Only for Debuging*/ 

// Plausi Prüfung
if (sasecureapptoken == "") {sasecureapptoken = "kA";} // Pushover SecureAppToken
if (sausertoken == "") {sausertoken = "kA";} // Pushover UserToken
if (sapushoverdevice1 == "") {sapushoverdevice1 = "kA";} // Pushover Devicename 1
if (sapushoverdevice2 == "") {sapushoverdevice2 = "kA";} // Pushover Devicename 2
if (sapushoverdevice3 == "") {sapushoverdevice3 = "kA";} // Pushover Devicename 3
if (sapushovermsound == "") {sapushovermsound = "none";} // Pushover Message Sound
if (sapushovermsound != "pushover" && sapushovermsound != "bike" &&  sapushovermsound != "bugle" &&
    sapushovermsound != "cashregister" && sapushovermsound != "classical" && sapushovermsound != "cosmic" &&
    sapushovermsound != "falling" && sapushovermsound != "gamelan" && sapushovermsound != "incoming" &&
    sapushovermsound != "intermission" && sapushovermsound != "magic" && sapushovermsound != "mechanical" &&
    sapushovermsound != "pianobar" && sapushovermsound != "siren" && sapushovermsound != "spacealarm" &&
    sapushovermsound != "tugboat" && sapushovermsound != "alien" && sapushovermsound != "climb" &&
    sapushovermsound != "persistent" && sapushovermsound != "echo" && sapushovermsound != "updown" &&
    sapushovermsound != "none") {sapushovermsound = "none";} 
if (sasendmessagetime == "") {sasendmessagetime = "kA";}     
if (sasoilname == "") {sasoilname = "kA";} // Sensorname
if (!IsNumeric(saWMSMLH)) {saWMSMLH = "856";} // Default - Prüfen ob soilMoistLevelHigh eine Zahl ist
if (!IsNumeric(saWMSMLL)) {saWMSMLL = "382";} // Default - Prüfen obsoilMoistLevelLow eine Zahl ist
if (!IsNumeric(saWMMessure)) {saWMMessureInt = 10;} else {saWMMessureInt = saWMMessure.toInt();} // Prüfen ob Messinterval eine Zahl ist
if (!IsNumeric(saWMalarmsoilvalue)) {saWMalarmsoilvalueInt = 30;} else {saWMalarmsoilvalueInt = saWMalarmsoilvalue.toInt();} // Prüfen ob Alarm Measure eine Zahl ist

if (sassid != "" && sapasswd != "") {
   // Messintervall begrenzen
  if (saWMMessureInt < 1 && saWMMessureInt > 99) {saWMMessureInt = 10;} // Default = 10 Minuten
  // Alarm Measure Value begrenzen
  if (saWMalarmsoilvalueInt < 1 && saWMalarmsoilvalueInt > 100) {saWMalarmsoilvalueInt = 30;} // Default = 30%
  // Daten speichern
  SetupSave("ssid.txt", (String)wifiManager.getWiFiSSID()); // SSID WLAN
  SetupSave("ssid-passwd.txt", (String)wifiManager.getWiFiPass()); // Password WLAN
  //                    *** Pushover Service ***
  // Pushover SecureAppToken
  if (sasecureapptoken == "kA") {
  if (LittleFS.exists("/po-secureapptoken.txt")){LittleFS.remove("/po-secureapptoken.txt");}  
  } else {SetupSave("po-secureapptoken.txt", sasecureapptoken);} 
  // Pushover UserToken
  if (sausertoken == "kA") {
  if (LittleFS.exists("/po-usertoken.txt")){LittleFS.remove("/po-usertoken.txt");} 
  } else {SetupSave("po-usertoken.txt", sausertoken);}
  // Pushover Devicename 1
  if (sapushoverdevice1 == "kA") {
  if (LittleFS.exists("/po-device1.txt")){LittleFS.remove("/po-device1.txt");} 
  } else {SetupSave("po-device1.txt", sapushoverdevice1);}
  // Pushover Devicename 2
  if (sapushoverdevice2 == "kA") {
  if (LittleFS.exists("/po-device2.txt")){LittleFS.remove("/po-device2.txt");} 
  } else {SetupSave("po-device2.txt", sapushoverdevice2);}
  // Pushover Devicename 3
  if (sapushoverdevice3 == "kA") {
  if (LittleFS.exists("/po-device3.txt")){LittleFS.remove("/po-device3.txt");} 
  } else {SetupSave("po-device3.txt", sapushoverdevice3);}
  // Pushover Message Sound
  if (sapushovermsound == "kA") {
  if (LittleFS.exists("/po-messagesound.txt")){LittleFS.remove("/po-messagesound.txt");} 
  } else {SetupSave("po-messagesound.txt", sapushovermsound);}
  // Send Message Time
  if (sasendmessagetime == "kA") {
  if (LittleFS.exists("/po-sendmessagetime.txt")){LittleFS.remove("/po-sendmessagetime.txt");} 
  } else {SetupSave("po-sendmessagetime.txt", sasendmessagetime);}
  // Sensor-Name
  if (sasoilname == "kA") {
  if (LittleFS.exists("/soilmoist-sensorname.txt")){LittleFS.remove("/soilmoist-sensorname.txt");}  
  } else {SetupSave("soilmoist-sensorname.txt", sasoilname);}
  SetupSave("soilmoist-highlevel.txt", saWMSMLH); // soil Moist High Level
  SetupSave("soilmoist-lowlevel.txt", saWMSMLL); // soil Moist Low Level
  SetupSave("measurementinterval.txt", (String)saWMMessureInt); // Messurement Interval
  SetupSave("alarmsoilmeasurevalue.txt", (String)saWMalarmsoilvalue); // Messurement Interval
  MyWaitLoop(500); // 0,5 sek. warten 
  Serial.println("Zugangsdaten gespeichert... "); 
  for (int i=0; i <= 5; i++){BlinkLED(GRUEN);} // grün
} else
  {
// Daten unvollständig
 Serial.println("Daten unvollständig -  AccessPoint wird neu gestartet"); 
 for (int i=0; i <= 10; i++){BlinkLED(ROT);} // rot
 // Little-Filesystem formatieren
 LittleFS.format();
 wifiManager.resetSettings(); // alle Einstellungen zurücksetzen   
  }
} else {
Serial.println(""); 
Serial.println("Save Plant-Monitor Adjustments fail !!!");} 
}

// *************************************************************************************************

// Setup Einstellungen laden und speichern
String SetupLoad(String file_name) {
String result = ""; // init
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return result;
}

// *************************************************************************************************

bool SetupSave(String file_name, String contents) {  
  File this_file = LittleFS.open(file_name, "w");
  if (!this_file) { // failed to open the file, return false
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
      return false;
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return true;
}

// ************************************ HTTP-Client *************************************************

void SendPushoverMessage(String POMessage, String PODevice, String POSound){
bool FailConnect;
Serial.println("");
Serial.println("[Pushover] - Sende Nachricht: "+POMessage);
WiFiClient client;
HTTPClient http;
MyWaitLoop(100);
if (!client.connect("api.pushover.net", 80)) {FailConnect = true;} else {FailConnect = false;}
if (!FailConnect) {
String postmessage = "token="+SecureAppToken+"&user="+UserToken+"&title=Meldung von: "+SoilSensorname+"&message="+POMessage+"&device="+PODevice+"&sound="+POSound;  
http.begin(client,"http://api.pushover.net/1/messages.json");
MyWaitLoop(500); //wait
int httpCode = http.POST(postmessage);
Serial.println("[HTTP] POST... code: " + HTTPCodeText(httpCode));
if(httpCode == HTTP_CODE_OK) {
Serial.println("[Pushover] - Die Nachricht wurde erfolgreich gesendet");
if (MessureLEDFlash == true) {SendPOMessageSignal(true);}} else {
Serial.println("[Pushover] - Die Nachricht konnte nicht gesendet werden !");
Serial.println("");
if (MessureLEDFlash == true) {SendPOMessageSignal(false);}}
http.end();
client.flush();
client.stop();
} else {
client.flush();
client.stop();
Serial.println("[Pushover] - Verbindung zum Dienst fehlgeschlagen !");
Serial.println("");
if (MessureLEDFlash == true) {SendPOMessageSignal(false);}
}
}

// *************************************************************************************************

void GetSoilMoistValue(int MinValue) {
soilMoist = analogRead(soilMoistPin);
// Auswertung der Bodenfeuchtigkeit in Prozent
if (soilMoist > MinValue) {
percentSoilMoist = map(soilMoist, soilMoistLevelHigh+1, soilMoistLevelLow-1, 0, 100);  
if (percentSoilMoist < AlarmMessageSoilValue1) {
if (MessureLEDFlash == true) {SwitchLEDLight(true,false,false);}} else {
if (percentSoilMoist < AlarmMessageSoilValue1) {if (MessureLEDFlash == true) {SwitchLEDLight(true,false,false);}} 
else {if (MessureLEDFlash == true) {SwitchLEDLight(false,false,true);}}} // Flash Red or green LED
if (MessureLEDFlash == true) {MyWaitLoop(100);}
LEDsOff();} else {
percentSoilMoist = 0; // kein Messwert
if (MessureLEDFlash == true) {SwitchLEDLight(true,false,false); // Rot
MyWaitLoop(150);}
LEDsOff();}
// percentSoilMoist = map(soilMoist, 856,323 , 0, 100); // Only for Debuging
LastMessureDateTime = FormatedDateTime(); // Datum / Uhrzeit - letzte Messung 
if (!SoilCalibrate) { 
Serial.println("Sensor Analog Value: "+(String)soilMoist+" - Feuchte: "+String(percentSoilMoist)+"%  -  "+LastMessureDateTime);} else {
Serial.println("Sensor Kalibrierung - Analog Value: "+ (String)soilMoist + " - "+LastMessureDateTime);}
MyWaitLoop(150);
}

// *************************************************************************************************

void StartMessureTimer() {
if (StartMessureCount > 0) {
StartMessureCount--;
MCount = StartMessureCount +1;
Serial.println("Startmodus - Noch für "+(String)MCount+" Messungen");}
if (StartMessureCount == 0) {
StartMessureCount = -1;  
soilMessureTrigger = measurementinterval1 * 120; // Messintervall von Minuten in Sekunden umrechnen  - 120 wenn Loop 0,5 sek.
Serial.println("");
Serial.println("Startmodus beendet");
Serial.println("");
}
}

// *************************************************************************************************

// check a string to see if it is numeric and accept Decimal point and positiv negativ Value
boolean IsNumeric(String str)
{
if(str.length()<1){return false;}
bool bPoint=false;
if ((str.charAt(0) == '-' || str.charAt(0) == '+') && (str.length()>1)) {  
for(unsigned char i = 1; i < str.length(); i++)
{
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};
}
} else
{
for(unsigned char i = 0; i < str.length(); i++)
{
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};
}  
  }
return true;
}

// *************************************************************************************************

String FormatedDateTime()
{
String DateTimeString;
getTimeLocal(); // aktuelle Uhrzeit holen
// Uhrzeit
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
if (s < 10){DateTimeString += ":0"+String(s)+" Uhr / ";} else{DateTimeString += ":"+String(s)+" Uhr / ";}
// Datum
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += ".0"+String(mo)+"."+String(ye);} else {DateTimeString += "."+String(mo)+"."+String(ye);}
return DateTimeString;    
}

// ********************** Routine by freddy64 *******************************************************

String runtime() {
  static uint8_t rolloverCounter = 0;
  static uint32_t lastMillis = 0;
  uint32_t currentMillis = millis() - bootTime;
  if (currentMillis < lastMillis) {       // check millis overflow
    rolloverCounter++;
  }
  lastMillis = currentMillis;
  uint32_t secs = (0xFFFFFFFF / 1000 ) * rolloverCounter + (currentMillis / 1000);
  char buf[20];
  sprintf(buf, "%ld %s %02ld:%02ld:%02ld", secs / 86400, (secs < 86400 || secs > 172800) ? "Tage" : "Tag", secs / 3600 % 24, secs / 60 % 60, secs % 60);
  return buf;
}

// *************************************************************************************************

void ScheduledRestart(String STime) {
if  (String(h)+":"+String(m)+":"+String(s) == STime+":0") {
if (IsPushover == true && !SoilCalibrate) {
if (PushoverDevice1 != "kA" && PushoverDevice1 != ""){  
SendPushoverMessage("Der Sensor wird neu gestartet",PushoverDevice1,"none");} // Message to Device 1
if (PushoverDevice2 != "kA" && PushoverDevice2 != ""){  
SendPushoverMessage("Der Sensor wird neu gestartet",PushoverDevice2,"none");} // Message to Device 2
if (PushoverDevice3 != "kA" && PushoverDevice3 != ""){  
SendPushoverMessage("Der Sensor wird neu gestartet",PushoverDevice3,"none");}} // Message to Device 3
TimeRestart = true;
  }   
}

// *************************************************************************************************

// Arduino OTA und Webserverabfrage nur jede 0,5 sek.
void HandleOTAandWebServer()
{ 
if (millis() - hwsTime > 500) 
 { 
 hwsTime = millis();
 server.handleClient();  // Webserver Abfrage
 ArduinoOTA.handle(); // OTA Abfrage
 delay(1); // kurze Pause 
 } 
}

// *************************************************************************************************

void wificonnect(bool Start) 
{
WiFiConnectLoop = 0; // Variable zurücksetzen  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{ 
MyWaitLoop(500);
Serial.print("."); 
BlinkLEDWifiConnect();
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED){
WiFiConnectFail = true;
if (!Start) {Serial.println("WLAN-Verbindung getrennt - "+(String)WiFiConnectLoop2);} else {StartFail = true;} 
} else
{
WiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop)); // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.println("");
LoadValue = 0;   // Zurücksetzen
Serial.print("Verbunden mit IP-Adresse: "); Serial.println(WiFi.localIP());
}
}

// *************************************************************************************************

void WiFiUDPStart()
{ 
Udp.begin(localPort);
Serial.print("lokaler Port: ");
Serial.println(Udp.localPort());
Serial.println("NTP-Time - Warten auf die Synchronisation");
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
// setSyncInterval(60); // Test
Serial.println("UDP Server gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WiFiUDPStop()
{
Udp.stop();
Serial.println("UDP Server gestoppt - "+FormatedDateTime());   
}

// *************************************************************************************************

void WebServerStart()
{
server.begin();
Serial.println("Webserver gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WebServerStop()
{
server.stop();
Serial.println("Webserver gestoppt - "+FormatedDateTime()); 
}

// *************************************************************************************************

void MyWaitLoop(int wlt)
{// Pause Loop
waitloop = 0; // Variable zurücksetzen
while (waitloop < wlt) // Pause in Millisekunden
{ 
waitloop += 1; // inc waitloop 1
delay(1);
yield(); // Equivalent zu  Application.ProcessMessages; 
// Serial.println(String(waitloop));
}
}

// *************************************************************************************************

void BlinkLED(int farbe) {
 toggle = !toggle;
 if (toggle) {
 if (farbe == ROT) {
 SwitchLEDLight(true,false,false); // Rot
 }
 if (farbe == BLAU) {
 SwitchLEDLight(false,true,false); // Blau
 }
 if (farbe == GRUEN) {
 SwitchLEDLight(false,false,true); // Grün
 }
  } else 
     {
 LEDsOff(); // alle LED's aus
      } 
MyWaitLoop(500); // kurze Pause 
}

// *************************************************************************************************

void BlinkLEDWifiConnect() {
toggle = !toggle;
 if (toggle) {
 SwitchLEDLight(false,true,false); // Blau
 }
  else 
     {
  SwitchLEDLight(false,false,true); // Grün
      } 
 delay(1); // kurze Pause
}

// *************************************************************************************************

void SendPOMessageSignal(bool Status) {
 if (!Status) {
 SwitchLEDLight(false,true,false); // Blau
 MyWaitLoop(1000); // kurze Pause 
 SwitchLEDLight(true,false,false); // Rot
 MyWaitLoop(1000); // kurze Pause 
 LEDsOff(); // alle LED's aus
 }
  else 
     {
 SwitchLEDLight(false,true,false); // Blau
 MyWaitLoop(1000); // kurze Pause 
 SwitchLEDLight(false,false,true); // Grün
 MyWaitLoop(1000); // kurze Pause 
 LEDsOff(); // alle LED's aus
      }    
}

// *************************************************************************************************

void LEDsOff() {
digitalWrite(ROT, LOW); 
digitalWrite(BLAU, LOW);
digitalWrite(GRUEN, LOW);      
}

// *************************************************************************************************

void SwitchLEDLight(bool TLRed, bool TLBlue, bool TLGreen){
if (TLRed){digitalWrite(ROT, HIGH);} else {digitalWrite(ROT, LOW);}
if (TLBlue){digitalWrite(BLAU, HIGH);} else {digitalWrite(BLAU, LOW);}
if (TLGreen){digitalWrite(GRUEN, HIGH);} else {digitalWrite(GRUEN, LOW);}
}

// *************************************************************************************************

void ResetPlantSensor()
{
APStart = 0; // zurücksetzen
bool IsFileExist = false; // zurücksetzen
if (littleFSInit == true)
{
// Little-Filesystem formatieren
LittleFS.format();
wifiManager.resetSettings(); // alle Einstellungen zurücksetzen
if (SecureAppToken != "" && SecureAppToken != "kA"){
SetupSave("po-secureapptoken.txt", SecureAppToken);} // Pushover SecureAppToken
if (UserToken != "" && UserToken != "kA"){
SetupSave("po-usertoken.txt", UserToken);} // Pushover UserToken
if (PushoverDevice1 != "" && PushoverDevice1 != "kA"){
SetupSave("po-device1.txt", PushoverDevice1);} // Pushover PushoverDevice 1
if (PushoverDevice2 != "" && PushoverDevice2 != "kA"){
SetupSave("po-device2.txt", PushoverDevice2);} // Pushover PushoverDevice 2
if (PushoverDevice3 != "" && PushoverDevice3 != "kA"){
SetupSave("po-device3.txt", PushoverDevice3);} // Pushover PushoverDevice 3
if (PushoverMessageSound != "" && PushoverMessageSound != "kA"){
SetupSave("po-messagesound.txt", PushoverMessageSound);} // Pushover PushoverMessageSound
if (SendMessageTime != "" && SendMessageTime != "kA"){
SetupSave("po-sendmessagetime.txt", SendMessageTime);} // Pushover Send Message Time
SetupSave("soilmoist-sensorname.txt", SoilSensorname); // Sensor-Name
SetupSave("soilmoist-highlevel.txt", soilmoisthighlevel); // soil Moist High Level
SetupSave("soilmoist-lowlevel.txt", soilmoistlowlevel); // soil Moist Low Level
SetupSave("measurementinterval.txt", measurementinterval); // Messurement Interval
SetupSave("alarmsoilmeasurevalue.txt",  AlarmMessageSoilValue); // Alarm by soil moisture < X percent
MyWaitLoop(500); // 0,5 sek. warten 
Serial.println("Reset - Sensor wird neu gestartet");
ESP.restart();
}   
}

// *************************************************************************************************

String cleanText(String text) {
  text.replace("’", "'");
  text.replace("“", "\"");
  text.replace("”", "\"");
  text.replace("`", "'");
  text.replace("‘", "'");
  text.replace("„", "'");
  text.replace("\\\"", "'");
  text.replace("•", "-");
  text.replace("é", "e");
  text.replace("è", "e");
  text.replace("ë", "e");
  text.replace("ê", "e");
  text.replace("à", "a");
  text.replace("â", "a");
  text.replace("ù", "u");
  text.replace("ç", "c");
  text.replace("î", "i");
  text.replace("ï", "i");
  text.replace("ô", "o");
  text.replace("…", "...");
  text.replace("–", "-");
  text.replace("Â", "A");
  text.replace("À", "A");
  text.replace("æ", "ae");
  text.replace("Æ", "AE");
  text.replace("É", "E");
  text.replace("È", "E");
  text.replace("Ë", "E");
  text.replace("Ô", "O");
  text.replace("Ö", "Oe");
  text.replace("ö", "oe");
  text.replace("œ", "oe");
  text.replace("Œ", "OE");
  text.replace("Ù", "U");
  text.replace("Û", "U");
  text.replace("Ü", "Ue");
  text.replace("ü", "ue");
  text.replace("Ä", "Ae");
  text.replace("ä", "ae");
  text.replace("ß", "ss");
  text.replace("»", "'");
  text.replace("«", "'");
  return text;
}

// *************************************************************************************************
// *************************************************************************************************
