/*
 * This is a fork of Volos' excellent repo for the T-Display S3 and BMP080 sensor.
 * https://github.com/VolosR/TemperatureTDisplay
 * I have modified it to use the Adafruit SHT40 sensor.
 * I have also added the Adafruit ADS9960 light sensor: http://www.adafruit.com/products/3595
 * The T-Display-S3 can be found here: https://www.lilygo.cc/products/t-display-s3
 *
 * Resolution: 170x320 full-color TFT on an 8-Bit Parallel Interface driven by a ST7789V chip.
 * To read from that sensor, I am using the Adafruit SHT4x library: https://github.com/adafruit/Adafruit_SHT4X
 * I added logging meant to be used with the Arduino serial plotter.
 * This logging occurs in every iteration of loop(), so it will clutter the serial output.
 * Because of that clutter, I have that logging controlled with a pre-processor #ifdef.
 */
#include "image2.h"
#include "TFT_eSPI.h"           // https://github.com/Bodmer/TFT_eSPI
#include "Adafruit_SHT4x.h"     // https://github.com/adafruit/Adafruit_SHT4X
#include "Adafruit_APDS9960.h"  // https://github.com/adafruit/Adafruit_APDS9960
#include "ArrayManager.h"       // https://github.com/AdamJHowell/ArrayManager

#define PLOTTER

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_APDS9960 apds;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite( &tft );
TFT_eSprite spr = TFT_eSprite( &tft );   //sprite for seconds
TFT_eSprite spr2 = TFT_eSprite( &tft );  //sprite for calendar
FloatArrayManager tempArray( 5 );
FloatArrayManager humidityArray( 5 );


const int iW = 480;
const int iH = 244;
const int w = 320;
const int h = 170;
const int xt = 8;
const int yt = 8;
int m = w;
int x = 0;
int y = 30;
int changeX = 1;
int changeY = 1;
unsigned short imageS[54400] = { 0 };


void setup() 
{
  Serial.begin( 115200 );
  Serial.println( F( "T-Display S3 with SHT40" ) );
  tft.init();
  tft.setRotation( 1 );
  tft.setSwapBytes( true );
  sprite.createSprite( 320, 170 );
  sprite.setSwapBytes( true );

  spr.createSprite( 80, 40 );
  spr.fillSprite( TFT_GREEN );
  spr2.createSprite( 86, 160 );
  spr2.fillSprite( TFT_GREEN );
  spr2.setTextDatum( 4 );
  spr.setTextColor( TFT_WHITE, TFT_GREEN );
  spr2.setTextColor( TFT_WHITE, TFT_GREEN );
  spr.setFreeFont( &Orbitron_Light_32 );
  spr2.setFreeFont( &Orbitron_Light_24 );

  Wire.begin( 43, 44 );

  if( !sht4.begin() )
  {
    Serial.println( "Couldn't find SHT4x" );
    while( 1 )
      delay( 1 );
  }
  Serial.println( "Found SHT4x sensor" );
  Serial.print( "Serial number 0x" );
  Serial.println( sht4.readSerial(), HEX );

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision( SHT4X_HIGH_PRECISION );
  switch( sht4.getPrecision() )
  {
     case SHT4X_HIGH_PRECISION: 
       Serial.println( "High precision" );
       break;
     case SHT4X_MED_PRECISION: 
       Serial.println( "Med precision" );
       break;
     case SHT4X_LOW_PRECISION: 
       Serial.println( "Low precision" );
       break;
  }

  // You can have 6 different heater settings higher heat and longer times uses more power and reads will take longer too!
  sht4.setHeater( SHT4X_NO_HEATER );
  switch( sht4.getHeater() )
  {
     case SHT4X_NO_HEATER: 
       Serial.println( "No heater" );
       break;
     case SHT4X_HIGH_HEATER_1S: 
       Serial.println( "High heat for 1 second" );
       break;
     case SHT4X_HIGH_HEATER_100MS: 
       Serial.println( "High heat for 0.1 second" );
       break;
     case SHT4X_MED_HEATER_1S: 
       Serial.println( "Medium heat for 1 second" );
       break;
     case SHT4X_MED_HEATER_100MS: 
       Serial.println( "Medium heat for 0.1 second" );
       break;
     case SHT4X_LOW_HEATER_1S: 
       Serial.println( "Low heat for 1 second" );
       break;
     case SHT4X_LOW_HEATER_100MS: 
       Serial.println( "Low heat for 0.1 second" );
       break;
  }
}

void pollSensor()
{
  sensors_event_t humidity;
  sensors_event_t temp;

  uint32_t timestamp = millis();
  sht4.getEvent( &humidity, &temp );// populate temp and humidity objects with fresh data
  timestamp = millis() - timestamp;

  float tempF = temp.temperature * 1.8 + 32;
  float relativeHumidity = humidity.relative_humidity;

  tempArray.insertValue( tempF );
  humidityArray.insertValue( relativeHumidity );

#ifdef PLOTTER
  Serial.printf( "tempF:%.1f Humidity:%.1f readDurationMs:%d\n", tempF, relativeHumidity, timestamp );
#endif // PLOTTER
}


void loop() 
{
  pollSensor();

  spr2.fillSprite( TFT_BLACK );
  spr2.setFreeFont( &Orbitron_Light_24 );
  int pos = x + iW * y;
  int start = pos;
  m = w + pos;
  for ( int i = 0; i < 54400; i++ ) 
  {
    if ( start % m == 0 ) 
    {
      start = start + ( iW - w );
      m = m + iW;
    }
    imageS[i] = picture[start];
    start++;
  }
  x = x + changeX;
  if ( x == iW - w - 1 || x < 0 )
    changeX = changeX * -1;

  y = y + changeY;
  if ( y == iH - h - 1 || y < 1 )
    changeY = changeY * -1;

  sprite.pushImage( 0, 0, 320, 170, imageS );

  spr2.fillRoundRect( 0, 0, 86, 20, 3, TFT_WHITE );
  spr2.pushImage( 0, 120, 86, 40, logo );

  spr2.drawRoundRect( 0, 26, 86, 44, 3, TFT_WHITE );
  spr2.drawRoundRect( 0, 92, 86, 24, 3, TFT_WHITE );

  spr2.setTextColor( TFT_BLACK, TFT_WHITE );
  spr2.drawString( "Temp", 40, 10, 2 );

  spr2.drawCircle( 76, 36, 3, TFT_WHITE );
  spr2.setTextColor( TFT_WHITE, TFT_BLACK );

  String temperature = String( tempArray.getAverage() );
  spr2.drawString( temperature.substring( 0, 4 ), 36, 40 );
  spr2.setTextFont( 0 );
  spr2.drawString( "Fahrenheit", 26, 62 );

  spr2.drawString( "Humidity", 40, 80, 2 );

  String humidityString = String( humidityArray.getAverage(), 1 );
  // snprintf( humidityBuffer.c_str(), 10, "%.1f", humidityArray.getAverage() );
  spr2.drawString( humidityString + " % rH", 44, 104, 2 );
  spr2.pushToSprite( &sprite, xt, yt, TFT_BLACK );
  sprite.pushSprite( 0, 0 );
}
