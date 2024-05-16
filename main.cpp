/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "mbed.h"
#include "USBSerial.h"


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

// USB Serial Wrapper
USBSerial connection;
// Basic thread to send USB data
Thread thread;

void usb_tx_thread(){
    ThisThread::sleep_for(5s);
    connection.disconnect();
    connection.init();
    connection.connect();
    char hello[100];
    static uint64_t now = 0;
    while (true) {
        now = Kernel::get_ms_count();
        int size = sniprintf(hello, 100, "Hello: Time since boot: %d", now);

        connection.send((uint8_t*)hello, size);
        ThisThread::sleep_for(BLINKING_RATE);
    }
    
}

int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);

    thread.start(usb_tx_thread);

    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
