#include <Adafruit_SSD1306.h>
#include <CayenneMQTTESP8266.h>
#define OLED_Address 0x3C
Adafruit_SSD1306 oled(1);

// WiFi network info.
char ssid[] = "sahana";
char wifiPassword[] = "sahana123";
// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "002d0640-ba79-11e8-bf81-6b1a7e6fd7d2";
char password[] = "4b848b83d646f0fc48259473bde92af70b161e55";
char clientID[] = "7ab02130-bc32-11e8-9747-bb363659b178";

int x=0;
int lastx=0;
int lasty=0;
int LastTime=0;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
 
#define UpperThreshold 550
#define LowerThreshold 500
 
void setup() {
  oled.begin(SSD1306_SWITCHCAPVCC,OLED_Address);
  oled.clearDisplay();
  oled.setTextSize(2);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  Serial.begin(9600);
}
 
 
void loop() 
{
  if(x>127)  
  {
    oled.clearDisplay();
    x=0;
    lastx=x;
  }
 
  int value=analogRead(A0);
  oled.setTextColor(WHITE);
  int y=60-(value/16);
  oled.writeLine(lastx,lasty,x,y,WHITE);
  lasty=y;
  lastx=x;
  // calc bpm
 
  if(value>UpperThreshold)
  {
    if(BeatComplete)
    {
      BPM=millis()-LastTime;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      BeatComplete=false;
    }
    if(BPMTiming==false)
    {
      LastTime=millis();
      BPMTiming=true;
    }
  }
  if((value<LowerThreshold)&(BPMTiming))
    BeatComplete=true;
    // display bpm
  oled.writeFillRect(0,50,128,16,BLACK);
  oled.setCursor(0,50);
  oled.print(BPM);
  oled.print(" BPM");
  oled.display();
  Serial.print("BPM = ");Serial.print(BPM);
  x++;
  Cayenne.virtualWrite(0, BPM);
}


