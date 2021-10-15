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

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {

    // Initialize OLED Screen
    Serial.begin(9600);
    if (!OLED.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("SSD1306 (OLED) allocation failed.");
        for(;;);
    }
    OLED.clearDisplay();

    
    
    
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
    pinMode(vssPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(vssPin), distanceCount, FALLING);

    interrupts();

}

void loop() {

    displayValues(OLED);

}

ISR(TIMER1_OVF_vect) {
    counter++;
    if (counter > 3) {
        counter = 0;
    }
    TCNT1 = 3036;
}

void displayValues(Adafruit_SSD1306 screen) {

    screen.clearDisplay();
    screen.setTextColor(WHITE, BLACK);

    screen.setCursor(0, 1);
    screen.setTextSize(2); // height: 14 pixels
    screen.print("VSS TEST");

    screen.setCursor(0, 19);
    screen.setTextSize(6); // height: 42 pixels
    screen.print(distance_pulse_count);

    // screen.setCursor(72, 47); 
    // screen.setTextSize(2);
    // screen.print("km/L");

    screen.display();

}

void distanceCount() {
    distance_pulse_count++;
}