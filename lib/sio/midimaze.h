#ifndef MIDIMAZE_H
#define MIDIMAZE_H
// https://github.com/FujiNetWIFI/atariwifi/blob/master/esp32/tests/midimaze/midimaze.ino

#include <driver/ledc.h>
#include "sio.h"
#include "../tcpip/fnUDP.h"

#define LEDC_TIMER_RESOLUTION  LEDC_TIMER_1_BIT

#define MIDIMAZE_PORT 5004
#define MIDIMAZE_BUFFER_SIZE 8192
#define MIDIMAZE_PACKET_TIMEOUT 5000
#define MIDI_BAUD 31250

class sioMIDIMaze : public sioDevice
{
private:
    fnUDP udpMIDI;
    uint8_t buf1[MIDIMAZE_BUFFER_SIZE];
    uint8_t i1=0;
    uint8_t buf2[MIDIMAZE_BUFFER_SIZE];
    uint8_t i2=0;

    void sio_status() override;                  // $53, 'S', Status
    void sio_process(uint32_t commanddata, uint8_t checksum) override;

public:
    bool midimazeActive = false; // If we are in midimaze mode or not
    char midimaze_host_ip[64] = "0";

    void sio_enable_midimaze();  // setup midimaze
    void sio_disable_midimaze(); // stop midimaze
    void sio_handle_midimaze();  // Handle incoming & outgoing data for midimaze
};

#endif