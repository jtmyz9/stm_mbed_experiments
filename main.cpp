/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "mbed.h"
#include "USBSerial.h"

using namespace std::chrono;

// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

// USB Serial Wrapper
USBSerial connection;
// Basic thread to send USB data
Thread usb_thread;

void usb_tx_thread(){
    connection.connect();
    char hello[100];
    static uint32_t time_since_boot = 0;

    while (true) {
        time_since_boot = duration_cast<std::chrono::milliseconds>(Kernel::Clock::now().time_since_epoch()).count();
        int size = sniprintf(hello, 100, "Hello: Time since boot: %ld \n", time_since_boot);

        connection.send((uint8_t*)hello, size);
        ThisThread::sleep_for(BLINKING_RATE);
    }
    
}

int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);

    usb_thread.start(usb_tx_thread);

    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
