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

static int8_t SignedByteClamp(int value) {
	if (value < -128) {
		return -128;
	}
	if (value > 127) {
		return 127;
	}
	return value;
}
int32_t move_x = 0;
int32_t move_y = 0;
int32_t scroll_v = 0;
int32_t scroll_h = 0;

#define MOVE_X_SCALE (3.f / 4.f)
#define MOVE_Y_SCALE (1.f / 2.f)
#define SCROLL_V_SCALE (-MOVE_Y_SCALE / 30.0)
#define SCROLL_H_SCALE (MOVE_X_SCALE / 30.0)

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
    	if (!keybi_mouse_is_scrolling) {

        	move_x += trackball_motion.dy;
        	move_y += trackball_motion.dx;

        	int8_t dx = SignedByteClamp(move_x * MOVE_X_SCALE);
        	int8_t dy = SignedByteClamp(move_y * MOVE_Y_SCALE);

        	if (dx != 0 || dy != 0) {
        		mouse_hid_report[1] = dx;
        		mouse_hid_report[2] = dy;
        		mouse_hid_report[3] = 0;
        		mouse_hid_report[4] = 0;
        		Keybi_Mouse_SendReport(mouse_hid_report);
            	move_x -= dx / MOVE_X_SCALE;
            	move_y -= dy / MOVE_Y_SCALE;
        	}
    	} else {

    		scroll_h += trackball_motion.dy;
    		scroll_v += trackball_motion.dx;

        	int8_t dh = SignedByteClamp(scroll_h * SCROLL_H_SCALE);
        	int8_t dv = SignedByteClamp(scroll_v * SCROLL_V_SCALE);

        	if (dh != 0 || dv != 0) {
        		mouse_hid_report[1] = 0;
        		mouse_hid_report[2] = 0;
        		mouse_hid_report[3] = dv;
        		mouse_hid_report[4] = dh;
        		Keybi_Mouse_SendReport(mouse_hid_report);
        		scroll_h -= dh / SCROLL_H_SCALE;
        		scroll_v -= dv / SCROLL_V_SCALE;
        	}
    	}
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
