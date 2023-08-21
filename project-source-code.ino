#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

// network credentials 
const char* ssid = "";
const char* pass = "";
int status = WL_IDLE_STATUS;

// database server details
const char* serverIp    = "192.168.0.14";
const int   serverPort  = 80;

// server endpoints (php files)
const char* getUserEndpoint    = "/get-users.php";
const char* postUSerEndpoint   = "/create-user.php";
const char* postAlertEndpoint  = "/create-alert.php"

// Initialize wifi client
WiFiClient client;

// lcd screen instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RFID instance
#define RFID_PIN_RST A0
#define RFID_PIN_SDA A2
MFRC522 rfid(RFID_PIN_SDA, RFID_PIN_RST);
MFRC522::MIFARE_Key MFRC522key;

// keypad instance
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {6,7,8,9}; // row pins
byte colPins[COLS] = {2,3,4,5}; // column pins
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const char RELATION_CHAR_FAMILY     = "A";
const char RELATION_CHAR_CARERS     = "B";
const char RELATION_CHAR_FRIENDS    = "C";
const char RELATION_CHAR_NEIGHBOURS = "D";

// test data
typedef struct {
    int user_id;
    char* user_name;
    char* user_rfid_uid;
    char* user_pin;
    int relation_id;
    bool active;
} userRecord;
userRecord testUserRecord = {NULL, "Person 1", "AA DD 52 90", "B1234", 4, true};
bool authorised = false;

typedef struct {
    int alert_id;
    int user_id;
    bool confirmed;
} alertRecord;
alertRecord testAlert = {NULL, 2, false};

void setup() {
    Serial.begin(9600);
    while (!Serial);

    //initialize LCD
    lcd.init();
    lcd.backlight();

    //initialize RFID module
    SPI.begin();
    rfid.PCD_Init();
    for (byte i=0; i<6; i++) {
        MFRC522key.keyByte[i] = 0xFF;
    }

    enable_WiFi();
    connect_WiFi();
    printWifiStatus();
}

void loop() {

    // scans for RFID tag
    if (!rfid.PICC_IsNewCardPresent() && !rfid.PICC_ReadCardSerial()) {
        return;
    }

    // convert UID bytes to string
    String uidStr = "";
    byte letter;
    for (byte i=0; i < rfid.uid.size; i++) {
        uidStr.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        uidStr.concat(String(rfid.uid.uidByte[i], HEX));
    } 
    uidStr.toUpperCase();
    Serial.print("UID: ");
    Serial.println(uidStr);

    // get user record from database
    userRecord currentUser = getUserRecord(uidStr);
    if (currentUser) {
        Serial.println(currentUser);

        //pin input
        int pinTimeout = 3;
        while (pinTimeout > 0) {
            char* inputPin = getUserPin();

            if (inputPin == currentUser.pin) {
                Serial.println("PIN OK");
                resetLcd();
                lcd.print("PIN OK");
                createAlert(currentUser);
                resetLcd();
                lcd.setCursor(0,1);
                lcd.print("ALERT SENT");
            } else {
                resetLcd();
                lcd.print("INCORRECT PIN")
                pinTimeout -= 1;
                delay(1000);
            }

            if (pinTimeout == 0) {
                resetLcd();
                lcd.print("TOO MANY");
                lcd.setCursor(0,1);
                lcd.print("WRONG ATTEMPTS")
                delay(5000);
            }   
        }
    } else {
        Serial.println("Invalid RFID");
        resetLcd();
        lcd.println("Invalid RFID")
        return;
    }
}

userRecord getUserRecord(String user_rfid_uid) {
    // connect to databse
    if (client.connect(serverIp, serverPort)) {
        Seria.prinln("connected to database");

        // form HTTP GET request
        client.println("GET " + getUserEndpoint + "UID=" + user_rfid_uid + " HTTP/1.1");
        client.prinln("Host: " + String(serverIp));
        client.println("Connection: close");
        client.println();

        while (client.connected()) {
            if (client.available()) {
                char c client.read();
                Serial.println(c);

                // process server response
                // extract JSON 

            }
        }
        return currentUser;
    }
}

void createAlert(userRecord currentUser) {
    StaticJsonDocument<200> alertJson;
    alertJson["user_id"] = currentUser.user_id;
    
    const size_t bufferSize = JSON_OBJECT_SIZE(16);
    char requestBody[bufferSize];
    serializeJson(alertJson, requestBody, bufferSize);
    Serial.println(requestBody);

    Serial.println("connecting to server...");
    if (client.connect(serverIp, serverPort)) {
        Serial.println("connected");

        // Send POST request
        client.print("POST ");
        client.print(postAlertEndpoint);
        client.println(" HTTP/1.1");
        client.println("HOSTl " + String(serverIp));
        client.println("Connection: close");
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(strlen(requestBody));
        client.println();
        client.println(requestBody);    

        // Read and print response
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            Serial.println(line);
        }
    } else {
        Serial.println("connection failed");
    }

    client.stop();
    Serial.println("disconnected from server");
}

String getUserPin() {
    resetLcd();
    lcd.print("Enter PIN:");
    lcd.setCursor(0,1);

    String pinInput = "";
    // pin is 4 digits 
    while (pinInput.length() < 5) {
        //gets key from keypad
        char keypadPinNum = keypad.getKey();
        if (keypadPinNum != NO_KEY) {
            Serial.print(keypadPinNum);
            lcd.print("*");
            pinInput += keypadPinNum;
        }
    }
    Serial.println();
    return pinInput;   
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void enable_WiFi() {
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < "1.0.0") {
        Serial.println("Please upgrade the firmware");
    }
}

void connect_WiFi() {
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
}

void resetLcd() {
    //clear lcd display
    lcd.clear();
    lcd.setCursor(0,0);
}
