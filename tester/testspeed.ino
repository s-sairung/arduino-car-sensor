#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

#define OLED_RESET      -1
#define OLED_ADDRESS    0x3C

// const int fuelPin = A1;
const unsigned int vssPin = 2;

volatile unsigned long counter = 0;
volatile unsigned long distance_pulse_count = 0;

volatile double traveled_distance = 0; // km
volatile unsigned int current_speed = 0; // km per hour

volatile static unsigned long pulse_distance = 0.00009810883055; // km per pulse

// Splash screen
const unsigned char bootScreenBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0xff, 0xfe, 0x0f, 0xff, 0xc1, 0xe0, 0x00, 0x1f, 0xff, 0x83, 0xe0, 0x1e, 0x3f, 0xff, 0xf0, 
  0x07, 0xff, 0xfe, 0x1f, 0xff, 0xe1, 0xe0, 0x00, 0x3f, 0xff, 0xc3, 0xf0, 0x1e, 0x3f, 0xff, 0xf0, 
  0x0f, 0xff, 0xfe, 0x3f, 0xff, 0xf1, 0xe0, 0x00, 0x7f, 0xff, 0xe3, 0xf8, 0x1e, 0x3f, 0xff, 0xf0, 
  0x0f, 0x80, 0x00, 0x3e, 0x01, 0xf1, 0xe0, 0x00, 0x7c, 0x03, 0xe3, 0xfc, 0x1e, 0x00, 0x78, 0x00, 
  0x0f, 0x00, 0x00, 0x3c, 0x00, 0xf1, 0xe0, 0x00, 0x78, 0x01, 0xe3, 0xfe, 0x1e, 0x00, 0x78, 0x00, 
  0x0f, 0x0f, 0xfe, 0x3c, 0x01, 0xf1, 0xe0, 0x00, 0x78, 0x03, 0xe3, 0xdf, 0x1e, 0x00, 0x78, 0x00, 
  0x0f, 0x0f, 0xfe, 0x3c, 0xff, 0xf1, 0xe0, 0x00, 0x79, 0xff, 0xe3, 0xcf, 0x9e, 0x00, 0x78, 0x00, 
  0x0f, 0x0f, 0xfe, 0x3c, 0xff, 0xf1, 0xe0, 0x00, 0x79, 0xff, 0xe3, 0xc7, 0xde, 0x00, 0x78, 0x00, 
  0x0f, 0x00, 0x1e, 0x3c, 0xff, 0xf1, 0xe0, 0x00, 0x79, 0xff, 0xe3, 0xc3, 0xfe, 0x00, 0x78, 0x00, 
  0x0f, 0x00, 0x1e, 0x3c, 0x00, 0xf1, 0xe0, 0x00, 0x78, 0x01, 0xe3, 0xc1, 0xfe, 0x00, 0x78, 0x00, 
  0x0f, 0x80, 0x1e, 0x3c, 0x00, 0xf1, 0xe0, 0x00, 0x78, 0x01, 0xe3, 0xc0, 0xfe, 0x00, 0x78, 0x00, 
  0x0f, 0xff, 0xfe, 0x3c, 0x00, 0xf1, 0xff, 0xfc, 0x78, 0x01, 0xe3, 0xc0, 0x7e, 0x00, 0x78, 0x00, 
  0x07, 0xff, 0xfe, 0x3c, 0x00, 0xf1, 0xff, 0xfc, 0x78, 0x01, 0xe3, 0xc0, 0x3e, 0x00, 0x78, 0x00, 
  0x03, 0xff, 0xfe, 0x3c, 0x00, 0xf1, 0xff, 0xfc, 0x78, 0x01, 0xe3, 0xc0, 0x1e, 0x00, 0x78, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

    Serial.println("Initializing...");

    // Initialize OLED Screen and show boot screen
    Serial.begin(9600);
    if (!OLED.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("SSD1306 (OLED) allocation failed.");
        for(;;);
    }
    OLED.clearDisplay();
    OLED.drawBitmap(0, 0, bootScreenBitmap, 128, 64, WHITE);
    OLED.display();
    
    noInterrupts();

    /* Setup Timers
    - Set Timer/Counter 1 Control Register A (normal mode)
    - Set Timer/Counter 1 Control Register B (prescaler value = 64)
    - Set Timer/Counter Interrupt Mask Register (enable Timer1's overflow)
    - Set Timer/Counter 1 (set Timer's initial value = 3036)
    */
    TCCR1A = 0;
    TCCR1B |= ((1 << CS10) | (1 << CS11));
    TIMSK1 |= (1 << TOIE1); 
    TCNT1 = 3036;

    // Initialize VSS Pin
    pinMode(vssPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(vssPin), distanceCount, RISING);

    interrupts();

    Serial.println("Initialize successful.");

    delay(10000);

}

void loop() {

    // displayValues(OLED);
    if (millis() % 500 == 0)
        vssDebug(OLED);

}

ISR(TIMER1_OVF_vect) {
    counter++;
    // 1 second = 4 counters
    if (counter > 3) {
        currentSpeed();
        distance_pulse_count = 0; // km per second
        counter = 0;
    }
    TCNT1 = 3036;
}

void displayValues(Adafruit_SSD1306 screen) {

    screen.clearDisplay();
    screen.setTextColor(WHITE, BLACK);

    screen.setCursor(0, 1);
    screen.setTextSize(2); // height: 14 pixels
    screen.print("SPEED");

    screen.setCursor(0, 19);
    screen.setTextSize(6); // height: 42 pixels
    screen.print(current_speed);

    screen.setCursor(72, 47); 
    screen.setTextSize(2);
    screen.print("km/L");

    screen.display();

}

void distanceCount() {
    distance_pulse_count++;
    traveled_distance += current_speed;
    // Serial.println("distance_pulse_count++");
}

void currentSpeed() {
    current_speed = pulse_distance * distance_pulse_count * 3600;
}

void vssDebug(Adafruit_SSD1306 screen) {

    screen.clearDisplay();
    screen.setTextColor(WHITE, BLACK);

    screen.setCursor(0, 1);
    screen.setTextSize(2); // height: 14 pixels
    screen.print("VSS DEBUG");

    screen.setCursor(0, 19);
    screen.setTextSize(2); // height: 14 pixels
    screen.print(distance_pulse_count);

    screen.display();

}