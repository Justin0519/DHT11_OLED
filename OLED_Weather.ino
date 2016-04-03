/*-----Library-----*/
#include <DHT.h>
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

/*-----DHT Setup-----*/
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/*-----Char setup-----*/
char str[10];

void drawTest()
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 30, "Loading...");
}

void setup()
{
/*-----Serial Monitor-----*/
  Serial.begin(9600);
  Serial.println("Loading...");

  dht.begin();
  u8g.firstPage();
  
/*-----do while迴圈-----*/
/*
do{動作}
while(結束條件)
*/
  do
  {
    drawTest();
  } while(u8g.nextPage());
}

void loop()
{
  delay(1000);  
  //wait a second for measure temp.

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);  //"true" means Fahrenheit

  //Check sensor works!
  if(isnan(h)||isnan(t)||isnan(f))
  {
    Serial.println("Please push the RESET button");
    return;
  }

  //heat index
  float hi = dht.computeHeatIndex(f,h);

  //turn heat index from FahrenHeit to Centigrade
  float hi2 = (hi-32)*5/9;

  /*-----Print in Monitor-----*/
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  
  Serial.print("Heat index: ");
  Serial.print(hi2);
  Serial.println(" *C");

  /*-----print in OLED-----*/
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_helvB08);
    
    u8g.drawStr( 0, 15, "Humidity:");
    u8g.drawStr( 80, 15, dtostrf(h, 5, 2, str));
    u8g.drawStr( 120, 15, "%");
    
    u8g.drawStr( 0, 35, "Temperature:");
    u8g.drawStr( 80, 35, dtostrf(t, 5, 2, str));
    u8g.drawStr( 120, 35, "\260C");
    
    u8g.drawStr( 80, 45, dtostrf(f, 5, 2, str));
    u8g.drawStr( 120, 45, "\260F");
    
    u8g.drawStr( 0, 60, "Heat index:");
    u8g.drawStr( 80, 60, dtostrf(hi2, 5, 2, str));
    u8g.drawStr( 120, 60, "\260C");
    
  } while( u8g.nextPage() );
}
