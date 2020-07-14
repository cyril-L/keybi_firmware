#include "keybi/keybi.h"
#include "keybi/hid_keyboard.h"
#include "keybi/hid_mouse.h"
#include "keybi/drivers/pmw3360.h"
#include "keybi/drivers/matrix.h"
#include "keybi/keymap.h"

static uint8_t keyboard_hid_report[8] = {0};

void Keybi_Init(void) {

    Keybi_Pmw3360_Init();
    Keybi_Matrix_Init();
}

#pragma GCC push_options
#pragma GCC optimize ("O0")

void Keybi_MainLoop(void)
{
    static keybi_pmw3360_motion_t trackball_motion;

    static uint8_t mouse_report[5] = {0};

    Keybi_Matrix_Scan(&Keybi_Keymap_EventHandler);
    while (Keybi_Keyboard_QueueToReport(&keybi_keymap_events, keyboard_hid_report)) {
    	Keybi_Keyboard_SendReport(keyboard_hid_report);
    }

    Keybi_Pmw3360_Read(&trackball_motion);
    if (trackball_motion.dx != 0 || trackball_motion.dy != 0) {
        mouse_report[1] = 3 * ((int)trackball_motion.dy) / 4;
        mouse_report[2] = trackball_motion.dx / 2;
        Keybi_Mouse_SendReport(mouse_report);
    }
}

#pragma GCC pop_options
