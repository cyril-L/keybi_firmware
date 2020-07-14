#include "keybi/hid_keyboard.h"
#include "keybi/qmk/keycode.h"

#include "CCIDHID_usb_conf.h"
#include "CCIDHID_usb_desc.h"

#include "usb_core.h"
#include "usb_pwr.h"
#include "usb_mem.h"
#include "usb_regs.h"

const uint8_t Keybi_Keyboard_ReportDescriptor[KEYBI_KEYBOARD_SIZ_REPORT_DESC] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)

    0xa1, 0x01, // COLLECTION (Application)

    // Modifier byte
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)
    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)

    // Reserved byte
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs) (0x81, 0x01 constant in HID class definition example)

    // LED report
    0x95, 0x05, // REPORT_COUNT (5)
    0x75, 0x01, // REPORT_SIZE (1)
    0x05, 0x08, // USAGE_PAGE (LEDs)
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    0x29, 0x05, // USAGE_MAXIMUM (Kana)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)

    // LED report padding
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x03, // REPORT_SIZE (3)
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)

    // Key arrays (6 bytes)
    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x65, // LOGICAL_MAXIMUM (101)
    0x05, 0x07, // USAGE_PAGE (Key codes)
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)

    0xc0   // END_COLLECTION
};

static __IO uint8_t prev_xfer_complete = 1;

void Keybi_Keyboard_SendReport(uint8_t * report) {

    if (bDeviceState == CONFIGURED)
    {
        while (!prev_xfer_complete);

        prev_xfer_complete = 0;
        /* Use the memory interface function to write to the selected endpoint */
        UserToPMABufferCopy(report, CCID_ENDP5_TXADDR, KEYBI_KEYBOARD_SIZ_REPORT);

        /* Update the data length in the control register */
        SetEPTxCount(ENDP5, KEYBI_KEYBOARD_SIZ_REPORT);
        SetEPTxStatus(ENDP5, EP_TX_VALID);
    }
}

// called in usb_endp.c

void Keybi_Keyboard_SendReportCompleted(void) {
    prev_xfer_complete = 1;
}

// called in CCIDHID_usb_prop.c

ONE_DESCRIPTOR Keybi_Keyboard_Report_Descriptor = {
    (uint8_t *) Keybi_Keyboard_ReportDescriptor,
    KEYBI_KEYBOARD_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR Keybi_Keyboard_Hid_Descriptor = {
    (uint8_t *) CCID_ConfigDescriptor + KEYBI_KEYBOARD_OFF_HID_DESC,
    KEYBI_KEYBOARD_SIZ_HID_DESC
};

uint8_t* Keybi_Keyboard_GetReportDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Keybi_Keyboard_Report_Descriptor);
}

uint8_t* Keybi_Keyboard_GetHIDDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Keybi_Keyboard_Hid_Descriptor);
}

int Keybi_Keyboard_QueueEvent(keybi_keyboard_event_queue_t * queue, keybi_keyboard_event_t event) {
	if (queue->size >= queue->capacity) {
		return -1;
	}
	unsigned tail = (queue->head + queue->size) % queue->capacity;
	queue->events[tail] = event;
    queue->size++;
	return 0;
}

int Keybi_Keyboard_QueueToReport(keybi_keyboard_event_queue_t * queue, uint8_t * report) {
    if (queue->size == 0) {
        return 0;
    }
    keybi_keyboard_event_t * event = &queue->events[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    if (IS_KEY(event->keycode)) {
        if (event->pressed) {
            for (int i = 2; i < 8; ++i) {
                if (report[i] == 0) {
                    report[i] = event->keycode;
                    break;
                }
            }
        } else {
            for (int i = 2; i < 8; ++i) {
                if (report[i] == event->keycode) {
                    report[i] = 0;
                    break;
                }
            }
        }
    } else if (IS_MOD(event->keycode)) {
    	if (event->pressed) {
            report[0] |= MOD_BIT(event->keycode);
    	} else {
            report[0] &= ~MOD_BIT(event->keycode);
        }
    }
    return 1;
}
