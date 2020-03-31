#ifndef STICKS_LUT_H
#define	STICKS_LUT_H

#include <stdint.h>

#define MAP_EMPTY 0x00
#define MAP_ANGLE_HORIZ 0x01
#define MAP_ANGLE_VERT 0x02
#define MAP_SHIELD_DROP 0x04
#define MAP_C_HORIZ 0x40
#define MAP_C_VERT 0x80

#define ANGLE_HORIZ_X_POS 204
#define ANGLE_HORIZ_Y_POS 151
#define ANGLE_HORIZ_X_NEG 52
#define ANGLE_HORIZ_Y_NEG 105

#define ANGLE_VERT_X_POS 151
#define ANGLE_VERT_Y_POS 204
#define ANGLE_VERT_X_NEG 105
#define ANGLE_VERT_Y_NEG 52

#define SHIELD_DROP_X 73
#define SHIELD_DROP_Y 183

#define C_HORIZ_X 255
#define C_HORIZ_Y 0

#define C_VERT_X 0
#define C_VERT_Y 255

extern const uint8_t sticks_lut[128][128];

#endif // STICKS_LUT_H

