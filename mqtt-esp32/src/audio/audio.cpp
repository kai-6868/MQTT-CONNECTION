#include "audio.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ===== Pin cấu hình =====
static const uint8_t PIN_MP3_TX = 26; // ESP32 TX → DFPlayer RX
static const uint8_t PIN_MP3_RX = 27; // ESP32 RX ← DFPlayer TX

// ===== UART & Player =====
static HardwareSerial dfSerial(2);   // UART2
static DFRobotDFPlayerMini player;

bool audio_init()
{
  dfSerial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  delay(500);

  if (!player.begin(dfSerial))
  {
    Serial.println("[AUDIO] DFPlayer init FAILED ❌");
    return false;
  }

  player.volume(25);   // volume mặc định
  Serial.println("[AUDIO] DFPlayer READY ✅");

  return true;
}

void audio_set_volume(uint8_t volume)
{
  volume = constrain(volume, 0, 30);
  player.volume(volume);

  Serial.print("[AUDIO] Volume = ");
  Serial.println(volume);
}

void audio_play(uint16_t track)
{
  Serial.print("[AUDIO] Play track ");
  Serial.println(track);

  player.play(track);
}

void audio_stop()
{
  Serial.println("[AUDIO] Stop");
  player.stop();
}
