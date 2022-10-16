/*******************************************************************************************
 ** Autor: Mario Ammerschuber                                                             **
 ** Datum: Oktober 2022                                                                    **
 ** Version: 1.2.1  Pushover                                                              ** 
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
 */
String PMVersion = "1.2.1 (Pushover)";
/************************( Importieren der Bibliotheken )************************/

#include <ESP8266WiFi.h>                    // über Bibliothek installieren
#include <ESP8266WiFiGeneric.h>
#include <ArduinoOTA.h>                     // über Bibliothek installieren
#include <Timezone.h>                       
#include <TimeLib.h>                  
#include <ESP8266WebServer.h>               // Webserver - über Bibliothek installieren
#include <WiFiClient.h>                     // WLAN Client - über Bibliothek installieren
#include <ESP8266HTTPClient.h>              // HTTPClient - über Bibliothek installieren
#include "LittleFS.h"                       // Little Filesystem - über Bibliothek installieren
#include <WiFiUdp.h>

// LED's
#define GRUEN 2 // NodeMCU v3 D4 -> GPIO 2
#define BLAU 0  // NodeMCU v3 D3 -> GPIO 0
#define ROT 4   // NodeMCU v3 D2 -> GPIO 4

bool ResetNow = false; // For Sensor Reset
bool RestartNow = false;  // For Restart ESP
bool TimeRestart = false; // For Restart ESP

bool MessureLEDFlash = true; // RGB LED kurz aufblinken lassen wenn eine Messung erfolgt 

String ssid = "";     // SSID of local network 
String TempSSID = ""; // SSID of local network 
String password = "";   // Password on network 

String SecureAppToken = ""; // Pushover SecureAppToken
String UserToken = ""; // Pushover UserToken
String PushoverDevice1 = ""; // PushoverDevice
String PushoverDevice2 = ""; // PushoverDevice
String PushoverDevice3 = ""; // PushoverDevice
String PushoverMessageSound = "none"; // Pushover Message Sound
String SendMessageTime = ""; // Uhrzeit wann die Nachricht gesendet werden soll
String OSendMessageTime = ""; // Uhrzeit wann die Nachricht gesendet werden soll 

String HOSTNAME = ""; // ESP-HOSTNAME
String HN = ""; // ESP-HOSTNAME

// Sensor-Daten
const int soilMoistPin = A0; // Analog PIN
int soilMoistLevelLow = 332;    //Dieser Wert soll von euch entsprechend angepasst werden
int soilMoistLevelHigh = 856;   //Dieser Wert soll von euch entsprechend angepasst werden

String SoilSensorname = ""; // Soil-Sensorname
String soilmoisthighlevel = ""; // soil Moist High Level
String soilmoistlowlevel = ""; // soil Moist Low Level
int percentSoilMoist = 0; // Messwert in %
int soilMoist = 0; // Messwert
bool SoilCalibrate = false; // Kalibierrungsmodus
String measurementinterval = ""; // Messintervall
int measurementinterval1 = 1; // Messintervall
int soilMessureTrigger = 30; // Messintervall nach dem Start des Sensors alle 15 sekunden messen (wenn Loop 0,5 sek.)
String AlarmMessageSoilValue = "";
int AlarmMessageSoilValue1 = 30; // Default bei 30% - Alarm bei < 30% Bodenfeuchte
bool IsPushover = true; // Pushover default aktiv
String LastMessureDateTime = "k.A. / k.A."; // letzte Messung

// for the first x times
int StartMessureCount = 20; // 20 Messungen im 15 sek. Takt (siehe Variable soilMessureTrigger)
int MCount; // Hilfsvariable

// NTP Serverpool für Deutschland:
static const char ntpServerName[] = "de.pool.ntp.org";    //Finde lokale Server unter http://www.pool.ntp.org/zone/de
const int timeZone = 0;                     // 0 wenn mit <Timezone.h> gearbeitet wird !
WiFiUDP Udp;
unsigned int localPort = 1701; // lokaler Port zum Abhören von UDP-Paketen
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// - Timezone. - //
// Bearbeiten Sie diese Zeilen entsprechend Ihrer Zeitzone und Sommerzeit.
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Time (Frankfurt, Paris)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)
Timezone CE(CEST, CET);

ESP8266WebServer server(80); // Webserver auf Port 80

/********************( Definieren der Globale Variablen und Konstanten)********************/
bool WiFiConnectFail = false; // false standard
String date;

String Datum, Zeit;
String DateString;
String ip;

bool bNTPStarted = false;
bool gbIsConnecting = false;
bool await = false;
bool littleFSInit = true;

unsigned long dotTime = 0;
unsigned long hwsTime = 0;
int dx = 0;
int dy = 0;
byte del = 0;
int h, m, s, w, mo, ye, d;
uint16_t bootTime; // Startzeit Plant Monitor
String FormatedDay = "";
String FormatedMonth = "";
const char* wochenstring;
float utcOffset = 2;
long localEpoc = 0;
long localMillisAtUpdate = 0;
int waitloop = 0;
int toggle = 0;
int WiFiConnectLoop = 0;
int WiFiConnectLoop2 = 1;
int MaxWiFiConnectLoop = 30;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN
bool StartFail = false;
int StartStopServers = 0;
int APStart = 0;
int LoadValue = 0;
bool ProgramSetup = true;

/*****************************************( Setup )****************************************/
void setup()
{
  Serial.begin(115200); // Serielle Verbindung mit 115200 Baud
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen

  // Pins als Output festlegen.
  pinMode(ROT, OUTPUT);
  pinMode(BLAU, OUTPUT);
  pinMode(GRUEN, OUTPUT);

  Serial.println("Start Pflanzen-Monitor ...");
  Serial.println("");
  Serial.println("");

  // Check LED's funktion
  Serial.println("Check RGB LED ...");
  MyWaitLoop(1500); // kurze Pause
  SwitchLEDLight(false,false,false); // alle aus
  for (int i=0; i < 6; i++){BlinkLED(ROT);} // rot
  for (int i=0; i < 6; i++){BlinkLED(BLAU);} // blau
  for (int i=0; i < 6; i++){BlinkLED(GRUEN);} // grün
  SwitchLEDLight(false,false,false); // alle aus
  Serial.println("");

   LoadAdjustments(); // Plant Monitor Einstellungen laden ....

     // ***First Start***
     // Little-Filesystem formatieren
     // LittleFS.format(); // alle Dateien  danch wieder deaktivieren und nochmal hochladen
  
  MyWaitLoop(1000);

  if ((String)ssid == "") {
    Serial.println("");
    Serial.println("***** First Start *****"); // erster Start des Sensors
    Serial.println("");
    }

   // Set WiFi and OTA/DNS Hostname
   HOSTNAME = cleanText(SoilSensorname); // HOSTNAME = Sensorname
   HN = HOSTNAME; // for LowerCase Hostname
   HN.toLowerCase(); // LowerCase Hostname
   // WiFi
   WiFi.hostname(HOSTNAME);
   Serial.println("WiFi-Hostname: "+HOSTNAME);
   
   // ***********************************************************************************************
   // Arduino OTA/DNS
   ArduinoOTA.setHostname(HOSTNAME.c_str());
   ArduinoOTA.setPassword("74656"); // default Passwort for OTA
   ArduinoOTA.onEnd([]() {Serial.println("\nEnd");});
   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
   Serial.printf("Progress: %u%%\r", (progress / (total / 100)));});
   ArduinoOTA.onError([](ota_error_t error) {(void)error;ESP.restart();}); // Restart bei OTA Fehler
   // ***********************************************************************************************
  
  if ((String)ssid != ""){
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WiFi SSID: ");
  Serial.println(ssid);
  wificonnect(true); // WLAN Verbindung herstellen
  } else {WiFiConnectFail = true;} // beim ersten Start des Sensors die WLAN-Anmeldung umgehen ! - Access Point gleich starten
  
  if (WiFiConnectFail == false){ 
  // vergebene IP-Adresse
  ip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3]; // Ermittelte IP-Adresse
  Serial.println("IP-Adresse:  "+ip); // Anzeige IP-Adresse
  MyWaitLoop(10); // kurze Pause

  // Webserver - Ausgabe wenn Link nicht gefunden wurde
  server.onNotFound([](){
  server.send(404, "text/plain", "Link wurde nicht gefunden!");  
  });

  // Webserver - Startseite Sensorwert
  server.on("/", handleRoot);
  // Einstellungen anzeigen
  server.on("/getadjustments", GetAdjustments); 
  // Einstellungen löschen - Reset
  server.on("/reset", ResetSettings); 
  server.on("/goreset", ResetYes); 
  server.on("/scalibrate", SensorCalibrate); 
  server.on("/soilhighlevel", SensorMoistHighLevel); 
  server.on("/soillowlevel", SensorMoistLowLevel);
  server.on("/esprestart", ESPRestart);
  server.on("/mledflashein", MessureLEDFlashEIN);
  server.on("/mledflashaus", MessureLEDFlashAUS);
  // Pushover Testmessage
  server.on("/potmdevice1", TMPushoverDevice1);
  server.on("/potmdevice2", TMPushoverDevice2);
  server.on("/potmdevice3", TMPushoverDevice3);

  Serial.println("UDP-Server starten ...");
  WiFiUDPStart();  // UPD Server starten

    // Arduino OTA/DNS starten
    ArduinoOTA.begin();
    Serial.println("Arduino OTA/DNS-Server starten ...");

    Serial.println("Web-Server starten ...");
    WebServerStart();

    // Get all information of your LittleFS
    if (littleFSInit == true)
    {
    Serial.println(F("Little Filesystem Init - done."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    Serial.println("File sistem info.");
    Serial.print("Total space:      ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
    Serial.print("Total space used: ");
    Serial.print(fs_info.usedBytes);
    Serial.println("byte");
    Serial.print("Block size:       ");
    Serial.print(fs_info.blockSize);
    Serial.println("byte");
    Serial.print("Page size:        ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
    Serial.print("Max open files:   ");
    Serial.println(fs_info.maxOpenFiles);
    Serial.print("Max path length:  ");
    Serial.println(fs_info.maxPathLength);
    Serial.println();
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        Serial.print(dir.fileName());
        Serial.print(" - ");
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            Serial.println(f.size());
            f.close();
        }else{
            Serial.println("0");
        }   
    }
   Serial.println("_______________________________");
   Serial.println("");
   }
   else
     {
     Serial.println("Little Filesystem Init - Fehler.");      
     }

  if (!IsPushover) {Serial.println("Der Pushover-Dienst ist deaktiviert");
  SwitchLEDLight(true,true,false); // LED Rot/Blau
  MyWaitLoop(1000);
  SwitchLEDLight(false,false,false);} // alle LED's aus
  else {
  Serial.println("Der Pushover-Dienst ist aktiviert");       
  SwitchLEDLight(false,false,true); // LED Grün
  MyWaitLoop(1000);
  SwitchLEDLight(false,false,false);} // alle LED's aus

  Serial.println("");
  Serial.println("Startzeit: "+FormatedDateTime());
  Serial.println("");
  Serial.println("");

  /*Serial.println("geladene Parameter");
  Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded-Pushover] SecureAppToken: " + SecureAppToken); // Only for Debuging 
  Serial.println("[Loaded-Pushover] UserToken: " + UserToken); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverDeviceName1: " + PushoverDevice1); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverDeviceName2: " + PushoverDevice2); // Only for Debuging
  Serial.println("[Loaded-Pushover] PushoverDeviceName3: " + PushoverDevice3); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverMessageSound: " + PushoverMessageSound); // Only for Debuging 
  Serial.println("[Loaded-Pushover] SendMessageTime: " + SendMessageTime); // Only for Debuging 
  Serial.println("[Loaded-soilMoist] AlarmMessageSoilValue: " + AlarmMessageSoilValue); // Only for Debuging
  Serial.println("[Loaded-soilMoist] Sensor-Name: " + SoilSensorname); // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelHigh: " + soilmoisthighlevel);  // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelLow: " + soilmoistlowlevel);  // Only for Debuging
  Serial.println("[Loaded-soilMoist] Messurement Interval: " + measurementinterval);  // Only for Debuging*/
  }
  else
  { 
  /*Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded-Pushover] SecureAppToken: " + (String)WMsecureapptoken); // Only for Debuging 
  Serial.println("[Loaded-Pushover] UserToken: " + (String)WMusertoken); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverDeviceName1: " + (String)WMpushoverdevice1); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverDeviceName2: " + (String)WMpushoverdevice2); // Only for Debuging
  Serial.println("[Loaded-Pushover] PushoverDeviceName3: " + (String)WMpushoverdevice3); // Only for Debuging 
  Serial.println("[Loaded-Pushover] PushoverMessageSound: " + (String)WMpushovermsound); // Only for Debuging 
  Serial.println("[Loaded-Pushover] SendMessageTime: " + (String)WMsendmessagetime); // Only for Debuging 
  Serial.println("[Loaded-soilMoist] AlarmMessageSoilValue: " + (String)WMalarmsoilvalue); // Only for Debuging
  Serial.println("[Loaded-soilMoist] Sensor-Name: " + (String)WMsoilsname); // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelHigh: " + (String)WMSMLH);  // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelLow: " + (String)WMSMLL);  // Only for Debuging
  Serial.println("[Loaded-soilMoist] Messurement Interval: " + (String)WMMessureInt);  // Only for Debuging*/  
   
  if ((String)ssid != ""){ 
  Serial.println("WLAN-Verbindung beim Start des Sensors fehlgeschlagen!"); 
  SwitchLEDLight(true,false,false); // LED Rot
  MyWaitLoop(1000);
  SwitchLEDLight(false,false,false); // alle LED's aus 
  MyWaitLoop(100); // kurze Pause
  WiFiConnectLoop2 += 1;} 
  else {
  Serial.println("Starte Access Point ... [Plant-Monitor]"); 
  Serial.println("");
  SwitchLEDLight(false,true,false); // Blau
  // #####################################################
  CaptivePortal(); // Start Captive Portal (WiFi-Manager)
  // #####################################################
  }}
  
  bootTime = millis(); //save millis after setup
  ProgramSetup = false; // Setup beendet 
  }

/*************************************(Hauptprogramm)**************************************/

void loop()
{
  if (ssid == ""){server.handleClient();} else { 
  // Check ob Internetverbindung noch besteht
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) 
  {
  wificonnect(false); // WLAN-Verbindung herstellen
  if (WiFiConnectFail == true)
  { // Wenn WiFi Connection fail
  WiFiConnectLoop2 += 1; 
  if (WiFiConnectLoop2 > 25) {
  WiFiUDPStop(); // UDP Server stoppen  
  // Nach 25 durchläufen Access Point starten
  WiFiConnectLoop2 = 1;
  
  // Change Status LED's
  SwitchLEDLight(true,true,true); // Alle An
  MyWaitLoop(1500);
  SwitchLEDLight(false,false,false); // Alle Aus
  // Little-Filesystem formatieren
  ResetPlantSensor(); // Reset Sensor
  Serial.println("");
  Serial.println("Plant-Monitor wird neu gestartet");
  ESP.restart();}}} 
  
  // nur wenn WLAN-Verbindung besteht
  if (!WiFiConnectFail){
  // if (SoilCalibrate == true) {Serial.println("SoilCalibrate = true");} else {Serial.println("SoilCalibrate = false");} // Only for Debuging
  if (StartFail || TimeRestart) {Serial.println("Sensor wird neu gestartet..."); ESP.restart();} // Sensor neu starten    
  WiFiConnectLoop2 = 1; // Zurücksetzen   
  HandleOTAandWebServer();  // OTA und Webserver Abfrage
  getTimeLocal(); // aktuelle Uhrzeit holen
  // Send Message to Pushover Service when percentSoilMoist < X% - Only one Time sending per Day
  if (IsPushover == true && percentSoilMoist < AlarmMessageSoilValue1 && (String(h)+":"+String(m)+":"+String(s)) == SendMessageTime+":0" && !SoilCalibrate) {
  if (PushoverDevice1 != "kA" && PushoverDevice1 != ""){  
  SendPushoverMessage("Ich brauche Wasser !",PushoverDevice1,PushoverMessageSound);} // Message to Device 1
  if (PushoverDevice2 != "kA" && PushoverDevice2 != ""){  
  SendPushoverMessage("Ich brauche Wasser !",PushoverDevice2,PushoverMessageSound);} // Message to Device 2
  if (PushoverDevice3 != "kA" && PushoverDevice3 != ""){  
  SendPushoverMessage("Ich brauche Wasser !",PushoverDevice3,PushoverMessageSound);}} // Message to Device 3
  // #################################################################################################################
  // ScheduledRestart("4:15"); // täglicher Neustart (04:15 Uhr)
  // #################################################################################################################
  // weiter nur wenn nicht gerade Kalibriert wird   
  if (!SoilCalibrate){   
  LoadValue += 1;
  if (LoadValue == 1 || LoadValue == soilMessureTrigger) // Messurement Interval
  {
  StartMessureTimer(); // Funktion für die ersten Messungen nach dem Sensorstart
  GetSoilMoistValue(25); // Messwert des Sensors auslesen
  LoadValue = 1;   // Zurücksetzen
  }
  }
  } else {BlinkLED(ROT);} // rot 
MyWaitLoop(500); // 0,5 sekunden  
// Serial.println(runtime()); // Only for Debuging
  }
} // Loop - Ende
