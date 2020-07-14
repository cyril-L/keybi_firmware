#include "keybi/keybi.h"
#include "keybi/hid_keyboard.h"
#include "keybi/hid_mouse.h"
#include "keybi/drivers/pmw3360.h"
#include "keybi/drivers/matrix.h"
#include "keybi/keymap.h"

void Keybi_Init(void) {

    Keybi_Pmw3360_Init();
    Keybi_Matrix_Init();
}

#pragma GCC push_options
#pragma GCC optimize ("O0")

static int8_t SignedByteClamp(int value) {
	if (value < -128) {
		return -128;
	}
	if (value > 127) {
		return 127;
	}
	return value;
}

void Keybi_MainLoop(void)
{
    static keybi_pmw3360_motion_t trackball_motion;

    static uint8_t keyboard_hid_report[8] = {0};
    static uint8_t mouse_hid_report[5] = {0};

    Keybi_Matrix_Scan(&Keybi_Keymap_EventHandler);
    while (Keybi_Keyboard_QueueToReport(&keybi_keymap_events, keyboard_hid_report)) {
    	Keybi_Keyboard_SendReport(keyboard_hid_report);
    }

    Keybi_Pmw3360_Read(&trackball_motion);
    if (trackball_motion.dx != 0 || trackball_motion.dy != 0) {
    	if (keybi_keyboard_layer != L_MOUSE) {
    		keybi_keyboard_layer = L_MOUSE;
    		keybi_mouse_is_scrolling = 0;
    	}
    	int dx = SignedByteClamp(3 * ((int)trackball_motion.dy) / 4);
    	int dy = SignedByteClamp(trackball_motion.dx / 2);

    	if (!keybi_mouse_is_scrolling) {
    		mouse_hid_report[1] = dx;
    		mouse_hid_report[2] = dy;
    		mouse_hid_report[3] = 0;
    		mouse_hid_report[4] = 0;
    	} else {
    		mouse_hid_report[1] = 0;
    		mouse_hid_report[2] = 0;
    		mouse_hid_report[3] = -dy/3;
    		mouse_hid_report[4] = dx/3;
    	}
        Keybi_Mouse_SendReport(mouse_hid_report);
    }
    if (mouse_hid_report[0] != keybi_mouse_buttons) {
    	mouse_hid_report[0] = keybi_mouse_buttons;
		mouse_hid_report[1] = 0;
		mouse_hid_report[2] = 0;
		mouse_hid_report[3] = 0;
		mouse_hid_report[4] = 0;
    	Keybi_Mouse_SendReport(mouse_hid_report);
    }
}

#pragma GCC pop_options
