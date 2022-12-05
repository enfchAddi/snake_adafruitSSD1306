#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define hoch 1
#define runter 2
#define links 3
#define rechts 4
#define WEISS SSD1306_WHITE

int richtung{ rechts };
int x{ 64 };
int y{ 16 };
int laenge{ 3 };

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  DDRD = 0;
  DDRB = 0;
  pinMode(A6, 1);
  display.clearDisplay();
  display.display();
}

void hochf() {
  y--;
}

void runterf() {

  y++;
}

void linksf() {

  x -= 2;
}

void rechtsf() {

  x += 2;
}
int randomnumber{random()};

void randomupdate(int a = 1, int b = 100) {
  randomnumber = random(a, b);
}

int food_x{};
int food_y{};
bool foodgen{ true };

void generateFood() {
  if (foodgen) {
    randomupdate(1, 127);
    food_x = randomnumber;
    randomupdate(1, 31);
    food_y = randomnumber;
    foodgen = false;
    if (food_x == 1) {
      food_x = 2;
    }
    if (food_y == 1) {
      food_y = 2;
    }
    if (food_x % 2 != 0) {
      food_x--;
    }
    if (food_y % 2 != 0) {
      food_y--;
    }
  }

  display.drawPixel(food_x, food_y, WEISS);
}



void foodCheck() {
  if (x == food_x && y == food_y) {
    display.drawPixel(food_x, food_y, SSD1306_BLACK);
    foodgen = true;
    laenge++;
  }
}

void drawBorder() {
  display.drawFastHLine(0, 0, 128, WEISS);
  display.drawFastHLine(0, 31, 128, WEISS);
  display.drawFastVLine(0, 0, 32, WEISS);
  display.drawFastVLine(127, 0, 32, WEISS);
}

int koerper[100][2]{};  //limit max 100

void addBody() {
  for (int i{ laenge }; i >= 1; i--) {
    
    koerper[i][0] = koerper[i - 1][0];
    koerper[i][1] = koerper[i - 1][1];
  }
  koerper[0][0] = x;
  koerper[0][1] = y;
}

void drawSnake() {
  for (int i{ 0 }; i <= laenge; i++) {
    display.drawPixel(koerper[i][0], koerper[i][1], WEISS);  //GEHT NICHT, KEINE INFO IN KOERPER AN WELCHER X UND Y ER ISTdfsdf
  }
}

void tot()
{
  display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.println("Dein Score: ");
    display.println(laenge - 3);

    display.display();
    while (true) { ; }
}

void loop() {
  randomSeed(analogRead(A6));
  display.clearDisplay();
  drawBorder();
  randomnumber = random();
  generateFood();
  foodCheck();
  addBody();
  drawSnake();

  switch (richtung) {
    case hoch:
      hochf();
      break;
    case runter:
      runterf();
      break;
    case links:
      linksf();
      break;
    case rechts:
      rechtsf();
      break;
  }

  display.display();
  if (digitalRead(14) && richtung != hoch) {
    richtung = runter;
  }
  if (digitalRead(15) && richtung != links) {
    richtung = rechts;
  }
  if (digitalRead(16) && richtung != rechts) {
    richtung = links;
  }
  if (digitalRead(17) && richtung != runter) {
    richtung = hoch;
  }

  display.display();
  delay(20);

  if (x < 1 || y < 1 || x > 127 || y > 31) {
    tot();
  }
  for(int i{1}; i <= laenge; i++)
  {
    if(koerper[0][0] == koerper[i][0] && koerper[0][1] == koerper[i][1])
    {
      tot();
    }
  }

}