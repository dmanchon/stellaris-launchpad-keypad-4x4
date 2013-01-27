//*****************************************************************************
//
// usb_keyb_structs.h - Data structures defining the keyboard USB device.
//
// Copyright (c) 2009-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM3S9D90 Firmware Package.
//
//*****************************************************************************

#ifndef __USB_KEYB_STRUCTS_H__
#define __USB_KEYB_STRUCTS_H__

extern unsigned long KeyboardHandler(void *pvCBData,
                                     unsigned long ulEvent,
                                     unsigned long ulMsgData,
                                     void *pvMsgData);

extern const tUSBDHIDKeyboardDevice g_sKeyboardDevice;

#endif // __USB_KEYB_STRUCTS_H__
