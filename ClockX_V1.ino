//by Sivasankar github repository: https://github.com/sasukeop108/ClockX-V1

#include <U8g2lib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <EEPROM.h>

//BUTTON PIN ASSIGNMENTS
#define b1 12  // D6 - GPIO12 (Menu navigation) Add 10KΩ resistor to 3.3V!
#define b2 13  // D7 - GPIO13 (Select/Jump) Add 10KΩ resistor to 3.3V!
#define b3 14  // D5 - GPIO14 (Increase/Restart) Add 10KΩ resistor to 3.3V!
#define b4 0   // D3 - GPIO0 (Decrease/Back)  Add 10KΩ resistor to 3.3V!

// Buzzer Pin
#define BUZZER_PIN 5  // D1 - GPIO5

int sen = 1; // Start at Clock (1=Clock, 2=Game, 3=Alarm, 4=Settings)

// OLED Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800);

// Icons
static const unsigned char image_calendar_bits[] U8X8_PROGMEM = {0x90,0x04,0x6e,0x3b,0xff,0x7f,0xff,0x7f,0x01,0x40,0x61,0x5b,0x61,0x5b,0x01,0x40,0x6d,0x5b,0x6d,0x5b,0x01,0x40,0x6d,0x43,0x6d,0x43,0x01,0x40,0xfe,0x3f,0x00,0x00};
static const unsigned char image_clock_alarm_bits[] U8X8_PROGMEM = {0x9e,0x3c,0xcd,0x59,0xb7,0x76,0x0b,0x68,0x05,0x50,0x82,0x20,0x82,0x20,0x81,0x40,0x83,0x60,0x41,0x40,0x22,0x20,0x12,0x20,0x04,0x10,0x08,0x08,0xb4,0x16,0xc2,0x21};
static const unsigned char image_wifi_full_bits[] U8X8_PROGMEM = {0x80,0x0f,0x00,0xe0,0x3f,0x00,0x78,0xf0,0x00,0x9c,0xcf,0x01,0xee,0xbf,0x03,0xf7,0x78,0x07,0x3a,0xe7,0x02,0xdc,0xdf,0x01,0xe8,0xb8,0x00,0x70,0x77,0x00,0xa0,0x2f,0x00,0xc0,0x1d,0x00,0x80,0x0a,0x00,0x00,0x07,0x00,0x00,0x02,0x00,0x00,0x00,0x00};
static const unsigned char image_settings_bits[] U8X8_PROGMEM = {
  0x00,0x00,0x0e,0x00,0x1f,0x00,0x3b,0x80,0x71,0xc0,0xe0,0xe0,0xc0,0x70,0x80,0x38,
  0x00,0x1c,0x00,0x0e,0x00,0x07,0x80,0x03,0xc0,0x01,0xe0,0x00,0x70,0x00,0x38,0x00,
  0x1c,0x00,0x0e,0x00,0x07,0x80,0x03,0xc0,0x01,0xe0,0x00,0x70,0x00,0x38,0x00,0x1c,
  0x00,0x0e,0x00,0x07,0x80,0x03,0xc0,0x01,0xe0,0x00,0x70,0x00,0x38,0x00,0x00,0x00
};

// BUZZER FUNCTIONS
void playTone(int frequency, int duration) {
  if (frequency == 0) {
    noTone(BUZZER_PIN);
    return;
  }
  tone(BUZZER_PIN, frequency, duration);
}

void playPulsingAlarm() {
  static unsigned long lastToneTime = 0;
  static int pulseState = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastToneTime > 100) {
    switch(pulseState) {
      case 0: playTone(1000, 80); break;
      case 1: playTone(1200, 80); break;
      case 2: playTone(1400, 80); break;
      case 3: playTone(0, 0); break;
    }
    pulseState = (pulseState + 1) % 4;
    lastToneTime = currentTime;
  }
}

void playJumpSound() { playTone(800, 50); delay(10); playTone(600, 40); }
void playGameOverSound() { playTone(300, 200); delay(100); playTone(200, 300); delay(150); playTone(150, 400); }
void playButtonClick() { playTone(500, 30); }
void playAlarmSetSound() { playTone(800, 100); delay(50); playTone(1000, 100); }
void stopAllSounds() { noTone(BUZZER_PIN); }

// EEPROM
#define EEPROM_SIZE 512
#define HIGH_SCORE_ADDR 0

struct Alarm {
  byte hour;
  byte minute;
  bool enabled;
};

#define MAX_ALARMS 3
Alarm alarms[MAX_ALARMS];

// GAME VARIABLES
#define GROUND_Y 54
#define GRAVITY 0.5
#define JUMP_FORCE -9.0
#define INITIAL_SPEED 3

// Dino Sprites
static const unsigned char dino_run1[] U8X8_PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x03,0xf0,0x07,0xf0,
  0x07,0xf0,0x07,0xf0,0x07,0xf0,0x07,0xe0,0x07,0xf0,0x07,0xf0,0x07,0xf8,0x0f,0xf8,
  0x0f,0xf8,0x0f,0xf0,0x07,0xf0,0x07,0xf0,0x03,0xe0,0x00,0x00,0x00,0x00,0x00,0x00
};

static const unsigned char cactus_small[] U8X8_PROGMEM = {
  0x00,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
  0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
  0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
  0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x00,0x00
};

struct Dino {
  float x = 10;
  float y = GROUND_Y - 16;
  float velocity = 0;
  bool jumping = false;
  int frame = 0;
} dino;

struct Obstacle {
  float x;
  bool active = false;
};

Obstacle obstacles[2];
bool gameOver = false;
unsigned long score = 0;
unsigned long highScore = 0;
float gameSpeed = INITIAL_SPEED;

// ALARM STATE
int alarmMenuState = 0;
int currentAlarm = 0;
int editingField = 0;
bool alarmTriggered = false;

//WIFI 
const char* ssid = "Sivasankarjio 5g";
const char* password = "siva2012";

String WeekDays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String Months[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug","Sep", "Oct", "Nov", "Dec"};

//SETUP 
void setup() {
  Serial.begin(115200);
  
  // Initialize buttons
  pinMode(b1, INPUT);  // D6
  pinMode(b2, INPUT);  // D7
  pinMode(b3, INPUT);  // D5
  pinMode(b4, INPUT);  // D3 
  
  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_18b_tr);
  u8g2.drawStr(5, 28, "CONNECTING TO");
  u8g2.drawStr(46, 44, "WiFi");
  u8g2.sendBuffer();
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  loadHighScore();
  loadAlarms();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  timeClient.begin();
  
  // Startup sound
  playButtonClick();
  delay(200);
  playButtonClick();
}

// EEPROM FUNCTIONS
void loadHighScore() {
  EEPROM.get(HIGH_SCORE_ADDR, highScore);
  if (highScore > 100000) highScore = 0;
}

void saveHighScore() {
  EEPROM.put(HIGH_SCORE_ADDR, highScore);
  EEPROM.commit();
}

void loadAlarms() {
  for(int i = 0; i < MAX_ALARMS; i++) {
    int addr = 10 + (i * sizeof(Alarm));
    EEPROM.get(addr, alarms[i]);
  }
}

void saveAlarms() {
  for(int i = 0; i < MAX_ALARMS; i++) {
    int addr = 10 + (i * sizeof(Alarm));
    EEPROM.put(addr, alarms[i]);
  }
  EEPROM.commit();
  playAlarmSetSound();
}

// MAIN LOOP
void loop() {
  // Show main menu (NOW WITH 4 OPTIONS)
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_12b_tr);
  u8g2.drawStr(22, 10, "WiFi NTP CLOCK");
  u8g2.drawStr(52, 25, "GAME");
  u8g2.drawStr(49, 40, "ALARM");
  u8g2.drawStr(44, 55, "SETTINGS");
  
  // Highlight current selection
  if (sen == 1) u8g2.drawFrame(19, 1, 92, 16);      // Clock
  else if (sen == 2) u8g2.drawFrame(47, 18, 33, 14); // Game
  else if (sen == 3) u8g2.drawFrame(43, 33, 40, 14); // Alarm
  else if (sen == 4) u8g2.drawFrame(39, 48, 55, 14); // Settings
  
  u8g2.sendBuffer();
  
  // Button handling
  if (digitalRead(b1) == LOW) {  // D6 - Menu navigation
    delay(200);
    playButtonClick();
    sen++;
    if (sen > 4) sen = 1;
  }
  
  if (digitalRead(b2) == LOW) {  // D7 - Select
    delay(200);
    playButtonClick();
    
    if (sen == 1) {
      showClock();
    } 
    else if (sen == 2) {
      playDinoGame();
    }
    else if (sen == 3) {
      showAlarmSystem();
    }
    else if (sen == 4) {
      showSettings();
    }
  }
  
  delay(50);
}

// CLOCK FUNCTION 
void showClock() {
  unsigned long lastUpdate = 0;
  
  while (digitalRead(b1) == HIGH) {
    if(millis() - lastUpdate > 1000) {
      timeClient.update();
      time_t epochTime = timeClient.getEpochTime();
      struct tm *ptm = localtime(&epochTime);
      
      int mday = ptm->tm_mday;
      int mon = ptm->tm_mon + 1;
      int year = ptm->tm_year + 1900;
      String date = (mday < 10 ? "0" : "") + String(mday) + " " + Months[mon] + " " + String(year % 100);
      
      int hour24 = timeClient.getHours();
      int mins = timeClient.getMinutes();
      int secs = timeClient.getSeconds();
      String period = (hour24 < 12) ? "AM" : "PM";
      int h12 = (hour24 % 12 == 0) ? 12 : hour24 % 12;
      String timeDisplay = (h12 < 10 ? "0" : "") + String(h12) + ":" + (mins < 10 ? "0" : "") + String(mins);
      
      checkAlarmTrigger();
      
      u8g2.clearBuffer();
      
      // Header
      u8g2.setFont(u8g2_font_4x6_tr);
      u8g2.drawStr(1, 6, "WiFi NTP CLOCK");
      u8g2.drawLine(2, 7, 127, 7);
      
      // Time
      u8g2.setFont(u8g2_font_t0_22b_tr);
      u8g2.drawStr(28, 42, timeDisplay.c_str());
      
      // Seconds
      u8g2.setFont(u8g2_font_t0_14b_tr);
      u8g2.setCursor(85, 42);
      u8g2.print(secs < 10 ? "0" : "");
      u8g2.print(secs);
      
      // Day and AM/PM
      u8g2.setFont(u8g2_font_t0_11b_tr);
      u8g2.drawStr(88, 23, WeekDays[timeClient.getDay()].c_str());
      u8g2.drawStr(22, 22, period.c_str());
      
      // Date
      u8g2.setFont(u8g2_font_t0_11b_tr);
      u8g2.setCursor(0, 56);
      u8g2.print(date);
      
      // Icons
      u8g2.drawXBMP(108, 12, 19, 16, image_wifi_full_bits);
      u8g2.drawXBMP(3, 10, 15, 16, image_clock_alarm_bits);
      u8g2.drawXBMP(104, 45, 15, 16, image_calendar_bits);
      
      u8g2.sendBuffer();
      lastUpdate = millis();
    }
    
    if(alarmTriggered) {
      showAlarmTriggered();
    }
    
    delay(10);
  }
  delay(200);
}

void checkAlarmTrigger() {
  if(alarmTriggered) return;
  
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  
  for(int i = 0; i < MAX_ALARMS; i++) {
    if(alarms[i].enabled && 
       alarms[i].hour == currentHour && 
       alarms[i].minute == currentMinute) {
      alarmTriggered = true;
      break;
    }
  }
}

void showAlarmTriggered() {
  static bool blink = false;
  static unsigned long lastBlink = 0;
  
  u8g2.clearBuffer();
  
  if(millis() - lastBlink > 300) {
    blink = !blink;
    lastBlink = millis();
  }
  
  if(blink) {
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 128, 64);
    u8g2.setDrawColor(1);
  }
  
  u8g2.setFont(u8g2_font_timR24_tr);
  u8g2.drawStr(20, 35, "ALARM!");
  
  u8g2.setFont(u8g2_font_t0_11b_tr);
  u8g2.drawStr(40, 55, "Press b2 to stop");
  
  u8g2.sendBuffer();
  
  playPulsingAlarm();
  
  if(digitalRead(b2) == LOW) {
    alarmTriggered = false;
    stopAllSounds();
    delay(200);
  }
}

//DINO GAME
void playDinoGame() {
  resetGame();
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_16b_tr);
  u8g2.drawStr(35, 20, "DINO");
  u8g2.drawStr(25, 40, "PRESS B2");
  u8g2.drawStr(20, 55, "TO JUMP");
  u8g2.sendBuffer();
  delay(1500);
  
  unsigned long lastFrame = 0;
  unsigned long lastObstacle = millis();
  
  while(digitalRead(b1) == HIGH && !gameOver) {
    unsigned long now = millis();
    
    if(now - lastFrame >= 16) {
      updateGame();
      drawGame();
      lastFrame = now;
    }
    
    if(digitalRead(b2) == LOW) {
      if(!dino.jumping) {
        dino.velocity = JUMP_FORCE;
        dino.jumping = true;
        playJumpSound();
      }
      delay(100);
    }
    
    if(gameOver && digitalRead(b3) == LOW) {
      resetGame();
      playButtonClick();
      delay(200);
    }
    
    if(now - lastObstacle > random(1000, 2500)) {
      spawnObstacle();
      lastObstacle = now;
    }
    
    delay(1);
  }
  
  if(gameOver) {
    showGameOver();
    delay(2000);
  }
}

void updateGame() {
  if(dino.jumping) {
    dino.y += dino.velocity;
    dino.velocity += GRAVITY;
    
    if(dino.y >= GROUND_Y - 16) {
      dino.y = GROUND_Y - 16;
      dino.jumping = false;
      dino.velocity = 0;
    }
  }
  
  dino.frame = (dino.frame + 1) % 20;
  
  for(int i = 0; i < 2; i++) {
    if(obstacles[i].active) {
      obstacles[i].x -= gameSpeed;
      
      if(obstacles[i].x < -10) {
        obstacles[i].active = false;
        score += 1;
      }
    }
  }
  
  for(int i = 0; i < 2; i++) {
    if(obstacles[i].active) {
      if(dino.x + 16 > obstacles[i].x &&
         dino.x < obstacles[i].x + 8 &&
         dino.y + 16 > GROUND_Y - 16) {
        gameOver = true;
        playGameOverSound();
        if(score > highScore) {
          highScore = score;
          saveHighScore();
        }
        return;
      }
    }
  }
}

void drawGame() {
  u8g2.clearBuffer();
  
  u8g2.drawLine(0, GROUND_Y, 128, GROUND_Y);
  
  if(dino.frame < 10) {
    u8g2.drawXBMP(dino.x, dino.y, 16, 16, dino_run1);
  } else {
    u8g2.drawXBMP(dino.x, dino.y, 16, 16, dino_run1);
  }
  
  for(int i = 0; i < 2; i++) {
    if(obstacles[i].active) {
      u8g2.drawXBMP(obstacles[i].x, GROUND_Y - 16, 8, 16, cactus_small);
    }
  }
  
  char scoreStr[20];
  u8g2.setFont(u8g2_font_5x7_tr);
  sprintf(scoreStr, "SCORE: %lu", score);
  u8g2.drawStr(80, 10, scoreStr);
  
  sprintf(scoreStr, "HIGH: %lu", highScore);
  u8g2.drawStr(80, 20, scoreStr);
  
  u8g2.sendBuffer();
}

void spawnObstacle() {
  for(int i = 0; i < 2; i++) {
    if(!obstacles[i].active) {
      obstacles[i].x = 128;
      obstacles[i].active = true;
      break;
    }
  }
}

void resetGame() {
  dino.x = 10;
  dino.y = GROUND_Y - 16;
  dino.velocity = 0;
  dino.jumping = false;
  dino.frame = 0;
  
  for(int i = 0; i < 2; i++) {
    obstacles[i].active = false;
  }
  
  score = 0;
  gameSpeed = INITIAL_SPEED;
  gameOver = false;
}

void showGameOver() {
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_t0_16b_tr);
  u8g2.drawStr(30, 20, "GAME OVER");
  
  u8g2.setFont(u8g2_font_t0_12b_tr);
  char scoreStr[30];
  sprintf(scoreStr, "Score: %lu", score);
  u8g2.drawStr(40, 35, scoreStr);
  
  sprintf(scoreStr, "High: %lu", highScore);
  u8g2.drawStr(40, 50, scoreStr);
  
  u8g2.drawStr(10, 63, "b3=Restart  b1=Exit");
  
  u8g2.sendBuffer();
}

//ALARM SYSTEM
void showAlarmSystem() {
  alarmMenuState = 0;
  currentAlarm = 0;
  
  while(digitalRead(b1) == HIGH) {
    
    if(alarmMenuState == 0) {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_t0_16b_tr);
      u8g2.drawStr(40, 15, "ALARMS");
      
      u8g2.setFont(u8g2_font_6x10_tr);
      
      for(int i = 0; i < MAX_ALARMS; i++) {
        char alarmStr[20];
        sprintf(alarmStr, "%d: %02d:%02d %s", 
                i+1, 
                alarms[i].hour, 
                alarms[i].minute,
                alarms[i].enabled ? "ON" : "OFF");
        
        int yPos = 30 + (i * 12);
        
        if(i == currentAlarm) {
          u8g2.drawBox(0, yPos-10, 128, 12);
          u8g2.setDrawColor(0);
        }
        
        u8g2.drawStr(5, yPos, alarmStr);
        
        if(i == currentAlarm) {
          u8g2.setDrawColor(1);
        }
      }
      
      u8g2.drawStr(5, 58, "b2:Edit  b3:Next  b4:Prev");
      
      u8g2.sendBuffer();
      
      if(digitalRead(b2) == LOW) {
        delay(200);
        playButtonClick();
        alarmMenuState = 1;
        editingField = 0;
      }
      
      if(digitalRead(b3) == LOW) {
        delay(200);
        playButtonClick();
        currentAlarm = (currentAlarm + 1) % MAX_ALARMS;
      }
      
      if(digitalRead(b4) == LOW) {
        delay(200);
        playButtonClick();
        currentAlarm = (currentAlarm - 1 + MAX_ALARMS) % MAX_ALARMS;
      }
    }
    else {
      static bool blink = false;
      static unsigned long lastBlink = 0;
      
      if(millis() - lastBlink > 500) {
        blink = !blink;
        lastBlink = millis();
      }
      
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_t0_16b_tr);
      char title[20];
      sprintf(title, "ALARM %d", currentAlarm + 1);
      u8g2.drawStr(30, 15, title);
      
      u8g2.setFont(u8g2_font_timR24_tr);
      char timeStr[6];
      sprintf(timeStr, "%02d:%02d", alarms[currentAlarm].hour, alarms[currentAlarm].minute);
      u8g2.drawStr(26, 45, timeStr);
      
      if(blink) {
        if(editingField == 0) {
          u8g2.drawBox(26, 46, 40, 3);
        } else if(editingField == 1) {
          u8g2.drawBox(70, 46, 40, 3);
        }
      }
      
      u8g2.setFont(u8g2_font_6x10_tr);
      char statusStr[20];
      sprintf(statusStr, "Status: %s", alarms[currentAlarm].enabled ? "ENABLED" : "DISABLED");
      
      if(editingField == 2 && blink) {
        u8g2.drawBox(45, 48, 60, 10);
        u8g2.setDrawColor(0);
      }
      u8g2.drawStr(45, 58, statusStr);
      u8g2.setDrawColor(1);
      
      u8g2.setFont(u8g2_font_4x6_tr);
      u8g2.drawStr(5, 63, "b3/b4:Adjust  b2:Save");
      
      u8g2.sendBuffer();
      
      if(digitalRead(b2) == LOW) {
        delay(200);
        playButtonClick();
        saveAlarms();
        alarmMenuState = 0;
      }
      
      if(digitalRead(b3) == LOW) {
        delay(200);
        playButtonClick();
        adjustAlarmValue(1);
      }
      
      if(digitalRead(b4) == LOW) {
        delay(200);
        playButtonClick();
        adjustAlarmValue(-1);
      }
    }
    
    delay(10);
  }
  
  delay(200);
}

void adjustAlarmValue(int delta) {
  switch(editingField) {
    case 0: alarms[currentAlarm].hour = (alarms[currentAlarm].hour + delta + 24) % 24; break;
    case 1: alarms[currentAlarm].minute = (alarms[currentAlarm].minute + delta + 60) % 60; break;
    case 2: alarms[currentAlarm].enabled = !alarms[currentAlarm].enabled; break;
  }
  editingField = (editingField + 1) % 3;
}

// SETTINGS/HELP MENU 
void showSettings() {

  int settingsPage = 0;
  
  while(digitalRead(b1) == HIGH) {
    
    if(settingsPage == 0) {
      // BUTTON GUIDE PAGE - Optimized layout
      u8g2.clearBuffer();
      
      u8g2.setFont(u8g2_font_t0_16b_tr);
      u8g2.drawStr(30, 12, "BUTTON GUIDE");
      u8g2.drawLine(2, 15, 126, 15);
      
      // Use 4x6 font for everything
      u8g2.setFont(u8g2_font_4x6_tr);
      
      // Row 1
      u8g2.drawStr(5, 25, "b1(D6): Menu Nav/Exit");
      
      // Row 2
      u8g2.drawStr(5, 33, "b2(D7): Select/Jump");
      
      // Row 3
      u8g2.drawStr(5, 41, "b3(D5): Increase/Next");
      
      // Row 4
      u8g2.drawStr(5, 49, "b4(D3): Decrease/Prev");
      
      // Row 5 - Game specific
      u8g2.drawStr(5, 57, "Game: b2=Jump b3=Restart");
      
      // Footer
      u8g2.drawStr(5, 63, "b3:Next Page  b1:Exit");
      
      u8g2.sendBuffer();
      
      if(digitalRead(b3) == LOW) {
        delay(200);
        playButtonClick();
        settingsPage = 1;
      }
    }
    else if(settingsPage == 1) {
      // SYSTEM INFO PAGE - Optimized
      u8g2.clearBuffer();
      
      u8g2.setFont(u8g2_font_t0_16b_tr);
      u8g2.drawStr(40, 12, "SYSTEM INFO");
      u8g2.drawLine(2, 15, 126, 15);
      
      u8g2.setFont(u8g2_font_4x6_tr);
      
      // Row 1: WiFi status
      u8g2.drawStr(5, 25, "WiFi:");
      u8g2.drawStr(35, 25, WiFi.status() == WL_CONNECTED ? "Connected" : "Offline");
      
      // Row 2: IP address (shortened if needed)
      u8g2.drawStr(5, 33, "IP:");
      String ipStr = WiFi.localIP().toString();
      // If IP is too long, show first part
      if(ipStr.length() > 14) {
        ipStr = ipStr.substring(0, 14);
      }
      u8g2.drawStr(35, 33, ipStr.c_str());
      
      // Row 3: High score
      u8g2.drawStr(5, 41, "High Score:");
      char hsStr[12];
      sprintf(hsStr, "%lu pts", highScore);
      u8g2.drawStr(70, 41, hsStr);
      
      // Row 4: Alarms
      u8g2.drawStr(5, 49, "Alarms:");
      int alarmsSet = 0;
      for(int i = 0; i < MAX_ALARMS; i++) {
        if(alarms[i].enabled) alarmsSet++;
      }
      char alarmStr[10];
      sprintf(alarmStr, "%d of 3 set", alarmsSet);
      u8g2.drawStr(70, 49, alarmStr);
      
      // Row 5: Memory
      u8g2.drawStr(5, 57, "Free RAM:");
      char ramStr[10];
      sprintf(ramStr, "%d bytes", ESP.getFreeHeap());
      u8g2.drawStr(70, 57, ramStr);
      
      // Footer
      u8g2.drawStr(5, 63, "b4:Prev Page  b1:Exit");
      
      u8g2.sendBuffer();
      
      if(digitalRead(b4) == LOW) {
        delay(200);
        playButtonClick();
        settingsPage = 0;
      }
    }
    
    delay(10);
  }
  

  
  delay(200);
}