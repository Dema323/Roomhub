// Playing a digital WAV recording repeatadly using the XTronical DAC Audio library
// prints out to the serial monitor numbers counting up showing that the sound plays
// independently of the main loop
// See www.xtronical.com for write ups on sound
// You will obviously need the appropriate hardware such as ESP32, audio amp and speaker, see
// http://www.xtronical.com/basics/audio/dacs-on-esp32/ for the simple build and specifically
//
// for the write ip on digitised speach/ sounds

#define BLYNK_PRINT Serial
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "SoundData.h";
#include "XT_DAC_Audio.h";

const uint16_t PixelCount = 24; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 27;  // make sure to set this to the correct pin, ignored for Esp8266

char auth[] = "xxxxx";

char ssid[] = "xxxxx";
char pass[] = "xxxxx";

NeoPixelBus<NeoGrbwFeature, NeoEsp32BitBang800KbpsMethod> strip(PixelCount, PixelPin);

XT_Wav_Class ForceWithYou(rawData);     // create an object of type XT_Wav_Class that is used by
                                      // the dac audio class (below), passing wav data as parameter.

XT_DAC_Audio_Class DacAudio(25,0);    // Create the main player class object.
                                      // Use GPIO 25, one of the 2 DAC pins and timer 0

void setup() {
WiFi.mode(WIFI_STA);
Blynk.begin(auth, ssid, pass);
while (Blynk.connect() == false) {}
ArduinoOTA.begin();
  strip.Begin();
}

BLYNK_WRITE(V2) // Play Ai notification
{
 if (param.asInt()){
  DacAudio.PlayWav(&ForceWithYou); // Plays wav file
 }
 }

 BLYNK_WRITE(V3) // Play light indication
  {
    if (param.asInt()) {
      for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 28; i++) {
          strip.SetPixelColor(i, RgbwColor(255, 70, 0, 0));
          strip.SetPixelColor(i - 4, RgbwColor(0, 0, 0, 0));
          delay(50);
          strip.Show();
        }
    }
  }
}

BLYNK_WRITE(V6) //Turn Roomhub light on
{
  if(param.asInt()) {
          strip.SetPixelColor(PixelCount,RgbwColor(0,255,0,0));
          strip.Show();
  }
}


void loop() {
 Blynk.run();
 ArduinoOTA.handle();
 DacAudio.FillBuffer(); // Buffers wav file

}
