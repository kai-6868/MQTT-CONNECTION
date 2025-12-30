#pragma once
#include <stdint.h>

// Khởi tạo DFPlayer
bool audio_init();

// Điều khiển cơ bản
void audio_set_volume(uint8_t volume);   // 0–30
void audio_play(uint16_t track);          // play track number
void audio_stop();
