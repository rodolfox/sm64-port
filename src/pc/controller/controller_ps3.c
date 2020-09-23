#ifdef TARGET_PS3

#include <stdbool.h>
#include <ultra64.h>

#include "controller_api.h"
#include <SDL.h>

#include <io/pad.h>

#define DEADZONE (0x20)

static bool init_ok;
static padInfo padinfo;
static padData paddata;

static void controller_ps3_init(void) {
    ioPadInit(7);

    init_ok = true;
}

static void controller_ps3_read(OSContPad *pad) {
    if (!init_ok) {
        return;
    }

    ioPadGetInfo(&padinfo);
    for(int i=0; i<MAX_PADS; i++){
        if(padinfo.status[i]){
            ioPadGetData(i, &paddata);
            if(paddata.BTN_START) pad->button |= START_BUTTON;
            if(paddata.BTN_L1) pad->button |= Z_TRIG;
            if(paddata.BTN_L1) pad->button |= Z_TRIG;
            if(paddata.BTN_CROSS) pad->button |= A_BUTTON;
            if(paddata.BTN_SQUARE) pad->button |= B_BUTTON;

            if (paddata.BTN_LEFT) pad->button |= L_CBUTTONS;
            if (paddata.BTN_RIGHT) pad->button |= R_CBUTTONS;
            if (paddata.BTN_UP) pad->button |= U_CBUTTONS;
            if (paddata.BTN_DOWN) pad->button |= D_CBUTTONS;

            int16_t stickH = ((uint8_t)paddata.ANA_L_H) - 0x80;
            int16_t stickV = 0x80 - ((uint8_t)paddata.ANA_L_V);
            const uint32_t magnitude_sq = (uint32_t)(stickH * stickH) + (uint32_t)(stickV * stickV);

            if (magnitude_sq > (uint32_t)(DEADZONE * DEADZONE)) {
                pad->stick_x = ((float)stickH/127)*80;
                pad->stick_y = ((float)stickV/127)*80;  
            }
        }
    }
}

struct ControllerAPI controller_ps3 = {
    controller_ps3_init,
    controller_ps3_read
};

#endif // TARGET_PS3
