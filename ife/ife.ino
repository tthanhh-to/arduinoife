// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
// initalizing the joystick pins
int VRx = A0;
int VRy = A1;
int SW = 2;
// setting positions for joystick
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
unsigned long lastMoveTime = 0;
const int moveDelay = 300;
// menu states
enum menuState {MOVIES, FLIGHT_INFO, LIGHTS};
int currentOption = 0; 
menuState currentScreen = MOVIES;
// movies
const char* movieTitles[] = {
  "Ponyo",
  "Top Gun",
  "Iron Man",
  "Star Wars",
  "Toy Story"
};
const int totalMovies = 5;
int movieIndex = 0;
// LED
int redLED = 6;
// temp
int rawTemp = analogRead(A2);
float tempC = 0;
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  pinMode(redLED, OUTPUT);
}

void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = digitalRead(SW);
  lcd.setCursor(0, 0);
  if (millis() - lastMoveTime > moveDelay){
    if (yPosition < 200){
      currentOption = (currentOption + 3) % 4;
      updateDisplay();
      lastMoveTime = millis();
    }
    if (yPosition > 800) {
      currentOption = (currentOption - 1 + 3) % 3;
      updateDisplay();
      lastMoveTime = millis();
    }
    if (digitalRead(SW) ==  LOW){
      delay(300);
      handleSelection();
    }
  }
}
void updateDisplay(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IFE Main Menu:");
  lcd.setCursor(0,1);
  switch(currentOption){
    case 0 : lcd.print("> Movie Select"); break;
    case 1 : lcd.print("> Flight Info"); break;
    case 2 : lcd.print("> Cabin Lights"); break;
  }
}
void handleSelection(){
  // flight info
  if (currentOption == 0){
    showMovieSelection();
  }
  if (currentOption == 1){
    bool inSubMenu = true;
    lcd.clear();

    while (inSubMenu){
      rawTemp = analogRead(A2);
      tempC = map(rawTemp, 0, 1023, 0, 750);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp:"); 
      lcd.print(tempC);
      lcd.print("C");
      lcd.setCursor(0,1);
      lcd.print("Press to exit");
      delay(500);
      if (digitalRead(SW)==LOW){
        delay(300);
        inSubMenu = false;
      }
    }
  }
  if (currentOption==2){
    digitalWrite(redLED, HIGH);
  }
  lcd.clear();
  lcd.print("Loading...");
  delay(1000);
  updateDisplay();

}

void showMovieSelection(){
  bool selecting = true;
  lcd.clear();
  while (selecting){
    int x = analogRead(A0);
    int y = analogRead(A1);
    if (y < 200){
      movieIndex = (movieIndex + 1) % totalMovies;
      delay(200);
    }
    if (y > 800) {
      movieIndex = (movieIndex - 1 + totalMovies) % totalMovies;
      delay(200);
    }
    lcd.setCursor(0,0);
    lcd.print("SELECT MOVIE:   ");
    lcd.setCursor(0,1);
    lcd.print(movieTitles[movieIndex]);

    if (digitalRead(SW) ==  LOW){
      playMovieAnimation(movieTitles[movieIndex]);
      selecting = false;
      delay(500);
    }
    if (x>800){
      selecting = false;
    }
  }
}
void playMovieAnimation(const char* title){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print (title);
  for (int i = 0; i < 16; i++){
    lcd.print(">");
    delay(100);
  }
  lcd.clear();
  lcd.print(title);
  lcd.setCursor(0,1);
  lcd.print ("[movie playing]");
  delay(3000);
}