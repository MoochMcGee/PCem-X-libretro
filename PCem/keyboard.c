#include "ibm.h"
#include "plat-keyboard.h"
#include "keyboard.h"

int keybsendcallback = 0;

typedef struct
{
        int scancodes_make[8];
        int scancodes_break[8];        
} scancode;

/*272 = 256 + 16 fake interim scancodes for disambiguation purposes.*/
static scancode scancode_set1[272] =
{
        { {-1},       {-1} },       { {0x01, -1}, {0x81, -1} }, { {0x02, -1}, {0x82, -1} }, { {0x03, -1}, {0x83, -1} },
        { {0x04, -1}, {0x84, -1} }, { {0x05, -1}, {0x85, -1} }, { {0x06, -1}, {0x86, -1} }, { {0x07, -1}, {0x87, -1} },
        { {0x08, -1}, {0x88, -1} }, { {0x09, -1}, {0x89, -1} }, { {0x0a, -1}, {0x8a, -1} }, { {0x0b, -1}, {0x8b, -1} },
        { {0x0c, -1}, {0x8c, -1} }, { {0x0d, -1}, {0x8d, -1} }, { {0x0e, -1}, {0x8e, -1} }, { {0x0f, -1}, {0x8f, -1} },
        { {0x10, -1}, {0x90, -1} }, { {0x11, -1}, {0x91, -1} }, { {0x12, -1}, {0x92, -1} }, { {0x13, -1}, {0x93, -1} },
        { {0x14, -1}, {0x94, -1} }, { {0x15, -1}, {0x95, -1} }, { {0x16, -1}, {0x96, -1} }, { {0x17, -1}, {0x97, -1} },
        { {0x18, -1}, {0x98, -1} }, { {0x19, -1}, {0x99, -1} }, { {0x1a, -1}, {0x9a, -1} }, { {0x1b, -1}, {0x9b, -1} },
        { {0x1c, -1}, {0x9c, -1} }, { {0x1d, -1}, {0x9d, -1} }, { {0x1e, -1}, {0x9e, -1} }, { {0x1f, -1}, {0x9f, -1} },
        { {0x20, -1}, {0xa0, -1} }, { {0x21, -1}, {0xa1, -1} }, { {0x22, -1}, {0xa2, -1} }, { {0x23, -1}, {0xa3, -1} },
        { {0x24, -1}, {0xa4, -1} }, { {0x25, -1}, {0xa5, -1} }, { {0x26, -1}, {0xa6, -1} }, { {0x27, -1}, {0xa7, -1} },
        { {0x28, -1}, {0xa8, -1} }, { {0x29, -1}, {0xa9, -1} }, { {0x2a, -1}, {0xaa, -1} }, { {0x2b, -1}, {0xab, -1} },
        { {0x2c, -1}, {0xac, -1} }, { {0x2d, -1}, {0xad, -1} }, { {0x2e, -1}, {0xae, -1} }, { {0x2f, -1}, {0xaf, -1} },
        { {0x30, -1}, {0xb0, -1} }, { {0x31, -1}, {0xb1, -1} }, { {0x32, -1}, {0xb2, -1} }, { {0x33, -1}, {0xb3, -1} },
        { {0x34, -1}, {0xb4, -1} }, { {0x35, -1}, {0xb5, -1} }, { {0x36, -1}, {0xb6, -1} }, { {0x37, -1}, {0xb7, -1} },
        { {0x38, -1}, {0xb8, -1} }, { {0x39, -1}, {0xb9, -1} }, { {0x3a, -1}, {0xba, -1} }, { {0x3b, -1}, {0xbb, -1} },
        { {0x3c, -1}, {0xbc, -1} }, { {0x3d, -1}, {0xbd, -1} }, { {0x3e, -1}, {0xbe, -1} }, { {0x3f, -1}, {0xbf, -1} },
        { {0x40, -1}, {0xc0, -1} }, { {0x41, -1}, {0xc1, -1} }, { {0x42, -1}, {0xc2, -1} }, { {0x43, -1}, {0xc3, -1} },
        { {0x44, -1}, {0xc4, -1} }, { {0x45, -1}, {0xc5, -1} }, { {0x46, -1}, {0xc6, -1} }, { {0x47, -1}, {0xc7, -1} },
        { {0x48, -1}, {0xc8, -1} }, { {0x49, -1}, {0xc9, -1} }, { {0x4a, -1}, {0xca, -1} }, { {0x4b, -1}, {0xcb, -1} },
        { {0x4c, -1}, {0xcc, -1} }, { {0x4d, -1}, {0xcd, -1} }, { {0x4e, -1}, {0xce, -1} }, { {0x4f, -1}, {0xcf, -1} },
        { {0x50, -1}, {0xd0, -1} }, { {0x51, -1}, {0xd1, -1} }, { {0x52, -1}, {0xd2, -1} }, { {0x53, -1}, {0xd3, -1} },
        { {0x54, -1}, {0xd4, -1} }, { {0x55, -1}, {0xd5, -1} }, { {0x56, -1}, {0xd6, -1} }, { {0x57, -1}, {0xd7, -1} },
        { {0x58, -1}, {0xd8, -1} }, { {0x59, -1}, {0xd9, -1} }, { {0x5a, -1}, {0xda, -1} }, { {0x5b, -1}, {0xdb, -1} },
        { {0x5c, -1}, {0xdc, -1} }, { {0x5d, -1}, {0xdd, -1} }, { {0x5e, -1}, {0xde, -1} }, { {0x5f, -1}, {0xdf, -1} },
        { {0x60, -1}, {0xe0, -1} }, { {0x61, -1}, {0xe1, -1} }, { {0x62, -1}, {0xe2, -1} }, { {0x63, -1}, {0xe3, -1} },
        { {0x64, -1}, {0xe4, -1} }, { {0x65, -1}, {0xe5, -1} }, { {0x66, -1}, {0xe6, -1} }, { {0x67, -1}, {0xe7, -1} },
        { {0x68, -1}, {0xe8, -1} }, { {0x69, -1}, {0xe9, -1} }, { {0x6a, -1}, {0xea, -1} }, { {0x6b, -1}, {0xeb, -1} },
        { {0x6c, -1}, {0xec, -1} }, { {0x6d, -1}, {0xed, -1} }, { {0x6e, -1}, {0xee, -1} }, { {0x6f, -1}, {0xef, -1} },
        { {0x70, -1}, {0xf0, -1} }, { {0x71, -1}, {0xf1, -1} }, { {0x72, -1}, {0xf2, -1} }, { {0x73, -1}, {0xf3, -1} },
        { {0x74, -1}, {0xf4, -1} }, { {0x75, -1}, {0xf5, -1} }, { {0x76, -1}, {0xf6, -1} }, { {0x77, -1}, {0xf7, -1} },
        { {0x78, -1}, {0xf8, -1} }, { {0x79, -1}, {0xf9, -1} }, { {0x7a, -1}, {0xfa, -1} }, { {0x7b, -1}, {0xfb, -1} },
        { {0x7c, -1}, {0xfc, -1} }, { {0x7d, -1}, {0xfd, -1} }, { {0x7e, -1}, {0xfe, -1} }, { {0x7f, -1}, {0xff, -1} },

        { {0x80, -1},       {-1} },             { {0x81, -1},       {-1} },             { {0x82, -1},       {-1} },             { {0xe0, 0x03, -1}, {0xe0, 0x83, -1} }, /*80*/
        { {0xe0, 0x04, -1}, {0xe0, 0x84, -1} }, { {0x85, -1},       {-1} },             { {0x86, -1},       {-1} },             { {0x87, -1},       {-1} },             /*84*/
        { {0xe0, 0x08, -1}, {0xe0, 0x88, -1} }, { {0xe0, 0x09, -1}, {0xe0, 0x89, -1} }, { {0xe0, 0x0a, -1}, {0xe0, 0x8a, -1} }, { {0xe0, 0x0b, -1}, {0xe0, 0x8b, -1} }, /*88*/
        { {0xe0, 0x0c, -1}, {0xe0, 0x8c, -1} }, { {-1},             {-1} },             { {0xe0, 0x0e, -1}, {0xe0, 0x8e, -1} }, { {0xe0, 0x0f, -1}, {0xe0, 0x8f, -1} }, /*8c*/
        { {0xe0, 0x10, -1}, {0xe0, 0x90, -1} }, { {0xe0, 0x11, -1}, {0xe0, 0x91, -1} }, { {0xe0, 0x12, -1}, {0xe0, 0x92, -1} }, { {0xe0, 0x13, -1}, {0xe0, 0x93, -1} }, /*90*/
        { {0xe0, 0x14, -1}, {0xe0, 0x94, -1} }, { {0xe0, 0x15, -1}, {0xe0, 0x95, -1} }, { {0xe0, 0x16, -1}, {0xe0, 0x96, -1} }, { {0xe0, 0x17, -1}, {0xe0, 0x97, -1} }, /*94*/
        { {0xe0, 0x18, -1}, {0xe0, 0x98, -1} }, { {0xe0, 0x19, -1}, {0xe0, 0x99, -1} }, { {0xe0, 0x1a, -1}, {0xe0, 0x9a, -1} }, { {0xe0, 0x1b, -1}, {0xe0, 0x9b, -1} }, /*98*/
        { {0xe0, 0x1c, -1}, {0xe0, 0x9c, -1} }, { {0xe0, 0x1d, -1}, {0xe0, 0x9d, -1} }, { {0xe0, 0x1e, -1}, {0xe0, 0x9e, -1} }, { {0xe0, 0x1f, -1}, {0xe0, 0x9f, -1} }, /*9c*/
        { {0xe0, 0x20, -1}, {0xe0, 0xa0, -1} }, { {0xe0, 0x21, -1}, {0xe0, 0xa1, -1} }, { {0xe0, 0x22, -1}, {0xe0, 0xa2, -1} }, { {0xe0, 0x23, -1}, {0xe0, 0xa3, -1} }, /*a0*/
        { {0xe0, 0x24, -1}, {0xe0, 0xa4, -1} }, { {0xe0, 0x25, -1}, {0xe0, 0xa5, -1} }, { {0xe0, 0x26, -1}, {0xe0, 0xa6, -1} }, { {-1},             {-1} },             /*a4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {0xe0, 0x2a, -1}, {0xe0, 0xaa, -1} }, { {-1},             {-1} },             /*a8*/
        { {0xe0, 0x2c, -1}, {0xe0, 0xac, -1} }, { {0xe0, 0x2d, -1}, {0xe0, 0xad, -1} }, { {0xe0, 0x2e, -1}, {0xe0, 0xae, -1} }, { {0xe0, 0x2f, -1}, {0xe0, 0xaf, -1} }, /*ac*/
        { {0xe0, 0x30, -1}, {0xe0, 0xb0, -1} }, { {0xe0, 0x31, -1}, {0xe0, 0xb1, -1} }, { {0xe0, 0x32, -1}, {0xe0, 0xb2, -1} }, { {-1},             {-1} },             /*b0*/
        { {0xe0, 0x34, -1}, {0xe0, 0xb4, -1} }, { {0xe0, 0x35, -1}, {0xe0, 0xb5, -1} }, { {0xe0, 0x36, -1}, {0xe0, 0xb6, -1} }, { {0xe0, 0x37, -1}, {0xe0, 0xb7, -1} }, /*b4*/
        { {0xe0, 0x38, -1}, {0xe0, 0xb8, -1} }, { {-1},             {-1} },             { {0xe0, 0x3a, -1}, {0xe0, 0xba, -1} }, { {0xe0, 0x3b, -1}, {0xe0, 0xbb, -1} }, /*b8*/
        { {0xe0, 0x3c, -1}, {0xe0, 0xbc, -1} }, { {0xe0, 0x3d, -1}, {0xe0, 0xbd, -1} }, { {0xe0, 0x3e, -1}, {0xe0, 0xbe, -1} }, { {0xe0, 0x3f, -1}, {0xe0, 0xbf, -1} }, /*bc*/
        { {0xe0, 0x40, -1}, {0xe0, 0xc0, -1} }, { {0xe0, 0x41, -1}, {0xe0, 0xc1, -1} }, { {0xe0, 0x42, -1}, {0xe0, 0xc2, -1} }, { {0xe0, 0x43, -1}, {0xe0, 0xc3, -1} }, /*c0*/
        { {0xe0, 0x44, -1}, {0xe0, 0xc4, -1} }, { {-1},             {-1} },             { {0xe0, 0x46, -1}, {0xe0, 0xc6, -1} }, { {0xe0, 0x47, -1}, {0xe0, 0xc7, -1} }, /*c4*/
        { {0xe0, 0x48, -1}, {0xe0, 0xc8, -1} }, { {0xe0, 0x49, -1}, {0xe0, 0xc9, -1} }, { {-1},             {-1} },             { {0xe0, 0x4b, -1}, {0xe0, 0xcb, -1} }, /*c8*/
        { {0xe0, 0x4c, -1}, {0xe0, 0xcc, -1} }, { {0xe0, 0x4d, -1}, {0xe0, 0xcd, -1} }, { {0xe0, 0x4e, -1}, {0xe0, 0xce, -1} }, { {0xe0, 0x4f, -1}, {0xe0, 0xcf, -1} }, /*cc*/
        { {0xe0, 0x50, -1}, {0xe0, 0xd0, -1} }, { {0xe0, 0x51, -1}, {0xe0, 0xd1, -1} }, { {0xe0, 0x52, -1}, {0xe0, 0xd2, -1} }, { {0xe0, 0x53, -1}, {0xe0, 0xd3, -1} }, /*d0*/
        { {0xd4, -1},       {-1} },             { {0xe0, 0x55, -1}, {0xe0, 0xd5, -1} }, { {-1},             {-1} },             { {0xe0, 0x57, -1}, {0xe0, 0xd7, -1} }, /*d4*/
        { {0xe0, 0x58, -1}, {0xe0, 0xd8, -1} }, { {0xe0, 0x59, -1}, {0xe0, 0xd9, -1} }, { {0xe0, 0x5a, -1}, {0xe0, 0xaa, -1} }, { {0xe0, 0x5b, -1}, {0xe0, 0xdb, -1} }, /*d8*/
        { {0xe0, 0x5c, -1}, {0xe0, 0xdc, -1} }, { {0xe0, 0x5d, -1}, {0xe0, 0xdd, -1} }, { {0xe0, 0x5e, -1}, {0xe0, 0xee, -1} }, { {0xe0, 0x5f, -1}, {0xe0, 0xdf, -1} }, /*dc*/
        { {-1},             {-1} },             { {0xe0, 0x61, -1}, {0xe0, 0xe1, -1} }, { {0xe0, 0x62, -1}, {0xe0, 0xe2, -1} }, { {0xe0, 0x63, -1}, {0xe0, 0xe3, -1} }, /*e0*/
        { {0xe0, 0x64, -1}, {0xe0, 0xe4, -1} }, { {0xe0, 0x65, -1}, {0xe0, 0xe5, -1} }, { {0xe0, 0x66, -1}, {0xe0, 0xe6, -1} }, { {0xe0, 0x67, -1}, {0xe0, 0xe7, -1} }, /*e4*/
        { {0xe0, 0x68, -1}, {0xe0, 0xe8, -1} }, { {0xe0, 0x69, -1}, {0xe0, 0xe9, -1} }, { {0xe0, 0x6a, -1}, {0xe0, 0xea, -1} }, { {0xe0, 0x6b, -1}, {0xe0, 0xeb, -1} }, /*e8*/
        { {0xe0, 0x6c, -1}, {0xe0, 0xec, -1} }, { {0xe0, 0x6d, -1}, {0xe0, 0xed, -1} }, { {0xe0, 0x6e, -1}, {0xe0, 0xee, -1} }, { {-1},             {-1} },             /*ec*/
        { {0xe0, 0x70, -1}, {0xe0, 0xf0, -1} }, { {0xf1, -1},       {-1} },             { {0xf2, -1},       {-1} },             { {0xe0, 0x73, -1}, {0xe0, 0xf3, -1} }, /*f0*/
        { {0xe0, 0x74, -1}, {0xe0, 0xf4, -1} }, { {0xe0, 0x75, -1}, {0xe0, 0xf5, -1} }, { {-1},             {-1} },             { {0xe0, 0x77, -1}, {0xe0, 0xf7, -1} }, /*f4*/
        { {0xe0, 0x78, -1}, {0xe0, 0xf8, -1} }, { {0xe0, 0x79, -1}, {0xe0, 0xf9, -1} }, { {0xe0, 0x7a, -1}, {0xe0, 0xfa, -1} }, { {0xe0, 0x7b, -1}, {0xe0, 0xfb, -1} }, /*f8*/
        { {0xe0, 0x7c, -1}, {0xe0, 0xfc, -1} }, { {0xe0, 0x7d, -1}, {0xe0, 0xfd, -1} }, { {0xe0, 0x7e, -1}, {0xe0, 0xfe, -1} }, { {0xe1, 0x1d, 0x45, 0xe1, 0x9d, 0xc5, -1},             {-1} },  /*fc*/
        { {-1},             {-1} },             { {0xe0, 0x01, -1}, {0xe0, 0x81, -1} }, { {0xe0, 0x02, -1}, {0xe0, 0x82, -1} }, { {-1},             {-1} },             /*100*/
        { {-1},             {-1} },             { {0xe0, 0x05, -1}, {0xe0, 0x85, -1} }, { {0xe0, 0x06, -1}, {0xe0, 0x86, -1} }, { {0xe0, 0x07, -1}, {0xe0, 0x87, -1} }, /*104*/
        { {0xe0, 0x71, -1}, {0xe0, 0xf1, -1} }, { {0xe0, 0x72, -1}, {0xe0, 0xf2, -1} }, { {0xe0, 0x7f, -1}, {0xe0, 0xff, -1} }, { {0xe0, 0xe1, -1}, {-1} },             /*108*/
        { {0xe0, 0xee, -1}, {-1} },             { {0xe0, 0xf1, -1}, {-1} },             { {0xe0, 0xfe, -1}, {-1} },             { {0xe0, 0xff, -1}, {-1} }              /*10c*/
};

/*XT keyboard has no escape scancodes, and no scancodes beyond 53*/
static scancode scancode_xt[272] =
{
        { {-1},       {-1} },       { {0x01, -1}, {0x81, -1} }, { {0x02, -1}, {0x82, -1} }, { {0x03, -1}, {0x83, -1} },
        { {0x04, -1}, {0x84, -1} }, { {0x05, -1}, {0x85, -1} }, { {0x06, -1}, {0x86, -1} }, { {0x07, -1}, {0x87, -1} },
        { {0x08, -1}, {0x88, -1} }, { {0x09, -1}, {0x89, -1} }, { {0x0a, -1}, {0x8a, -1} }, { {0x0b, -1}, {0x8b, -1} },
        { {0x0c, -1}, {0x8c, -1} }, { {0x0d, -1}, {0x8d, -1} }, { {0x0e, -1}, {0x8e, -1} }, { {0x0f, -1}, {0x8f, -1} },
        { {0x10, -1}, {0x90, -1} }, { {0x11, -1}, {0x91, -1} }, { {0x12, -1}, {0x92, -1} }, { {0x13, -1}, {0x93, -1} },
        { {0x14, -1}, {0x94, -1} }, { {0x15, -1}, {0x95, -1} }, { {0x16, -1}, {0x96, -1} }, { {0x17, -1}, {0x97, -1} },
        { {0x18, -1}, {0x98, -1} }, { {0x19, -1}, {0x99, -1} }, { {0x1a, -1}, {0x9a, -1} }, { {0x1b, -1}, {0x9b, -1} },
        { {0x1c, -1}, {0x9c, -1} }, { {0x1d, -1}, {0x9d, -1} }, { {0x1e, -1}, {0x9e, -1} }, { {0x1f, -1}, {0x9f, -1} },
        { {0x20, -1}, {0xa0, -1} }, { {0x21, -1}, {0xa1, -1} }, { {0x22, -1}, {0xa2, -1} }, { {0x23, -1}, {0xa3, -1} },
        { {0x24, -1}, {0xa4, -1} }, { {0x25, -1}, {0xa5, -1} }, { {0x26, -1}, {0xa6, -1} }, { {0x27, -1}, {0xa7, -1} },
        { {0x28, -1}, {0xa8, -1} }, { {0x29, -1}, {0xa9, -1} }, { {0x2a, -1}, {0xaa, -1} }, { {0x2b, -1}, {0xab, -1} },
        { {0x2c, -1}, {0xac, -1} }, { {0x2d, -1}, {0xad, -1} }, { {0x2e, -1}, {0xae, -1} }, { {0x2f, -1}, {0xaf, -1} },
        { {0x30, -1}, {0xb0, -1} }, { {0x31, -1}, {0xb1, -1} }, { {0x32, -1}, {0xb2, -1} }, { {0x33, -1}, {0xb3, -1} },
        { {0x34, -1}, {0xb4, -1} }, { {0x35, -1}, {0xb5, -1} }, { {0x36, -1}, {0xb6, -1} }, { {0x37, -1}, {0xb7, -1} },
        { {0x38, -1}, {0xb8, -1} }, { {0x39, -1}, {0xb9, -1} }, { {0x3a, -1}, {0xba, -1} }, { {0x3b, -1}, {0xbb, -1} },
        { {0x3c, -1}, {0xbc, -1} }, { {0x3d, -1}, {0xbd, -1} }, { {0x3e, -1}, {0xbe, -1} }, { {0x3f, -1}, {0xbf, -1} },
        { {0x40, -1}, {0xc0, -1} }, { {0x41, -1}, {0xc1, -1} }, { {0x42, -1}, {0xc2, -1} }, { {0x43, -1}, {0xc3, -1} },
        { {0x44, -1}, {0xc4, -1} }, { {0x45, -1}, {0xc5, -1} }, { {0x46, -1}, {0xc6, -1} }, { {0x47, -1}, {0xc7, -1} },
        { {0x48, -1}, {0xc8, -1} }, { {0x49, -1}, {0xc9, -1} }, { {0x4a, -1}, {0xca, -1} }, { {0x4b, -1}, {0xcb, -1} },
        { {0x4c, -1}, {0xcc, -1} }, { {0x4d, -1}, {0xcd, -1} }, { {0x4e, -1}, {0xce, -1} }, { {0x4f, -1}, {0xcf, -1} },
        { {0x50, -1}, {0xd0, -1} }, { {0x51, -1}, {0xd1, -1} }, { {0x52, -1}, {0xd2, -1} }, { {0x53, -1}, {0xd3, -1} },
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*54*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*58*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*5c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*60*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*64*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*68*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*6c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*70*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*74*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*78*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*7c*/

        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*80*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*84*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*88*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*8c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*90*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*94*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*98*/
        { {0x1c, -1}, {0x9c, -1} },             { {0x1d, -1}, {0x9d, -1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*9c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*a0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*a4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {0x2a, -1}, {0xaa, -1} },             { {-1},             {-1} },             /*a8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*ac*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*b0*/
        { {-1},             {-1} },             { {0x35, -1}, {0xb5, -1} },             { {0x36, -1}, {0xb6, -1} },             { {0x37, -1}, {0xb7, -1} }, /*b4*/
        { {0x38, -1}, {0xb8, -1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*b8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*bc*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*c0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {0x46, -1}, {0xc6, -1} },             { {0x47, -1}, {0xc7, -1} }, /*c4*/
        { {0x48, -1}, {0xc8, -1} },             { {0x49, -1}, {0xc9, -1} },             { {-1},             {-1} },             { {0x4b, -1}, {0xcb, -1} }, /*c8*/
        { {-1},             {-1} },             { {0x4d, -1}, {0xcd, -1} },             { {-1},             {-1} },             { {0x4f, -1}, {0xcf, -1} }, /*cc*/
        { {0x50, -1}, {0xd0, -1} },             { {0x51, -1}, {0xd1, -1} },             { {0x52, -1}, {0xd2, -1} },             { {0x53, -1}, {0xd3, -1} }, /*d0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*d4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*d8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*dc*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*e0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*e4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*e8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*ec*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*f0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*f4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*f8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*fc*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*100*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*104*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*108*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*10c*/
};

/*Tandy keyboard has slightly different scancodes to XT*/
static scancode scancode_tandy[272] =
{
        { {-1},       {-1} },       { {0x01, -1}, {0x81, -1} }, { {0x02, -1}, {0x82, -1} }, { {0x03, -1}, {0x83, -1} },
        { {0x04, -1}, {0x84, -1} }, { {0x05, -1}, {0x85, -1} }, { {0x06, -1}, {0x86, -1} }, { {0x07, -1}, {0x87, -1} },
        { {0x08, -1}, {0x88, -1} }, { {0x09, -1}, {0x89, -1} }, { {0x0a, -1}, {0x8a, -1} }, { {0x0b, -1}, {0x8b, -1} },
        { {0x0c, -1}, {0x8c, -1} }, { {0x0d, -1}, {0x8d, -1} }, { {0x0e, -1}, {0x8e, -1} }, { {0x0f, -1}, {0x8f, -1} },
        { {0x10, -1}, {0x90, -1} }, { {0x11, -1}, {0x91, -1} }, { {0x12, -1}, {0x92, -1} }, { {0x13, -1}, {0x93, -1} },
        { {0x14, -1}, {0x94, -1} }, { {0x15, -1}, {0x95, -1} }, { {0x16, -1}, {0x96, -1} }, { {0x17, -1}, {0x97, -1} },
        { {0x18, -1}, {0x98, -1} }, { {0x19, -1}, {0x99, -1} }, { {0x1a, -1}, {0x9a, -1} }, { {0x1b, -1}, {0x9b, -1} },
        { {0x1c, -1}, {0x9c, -1} }, { {0x1d, -1}, {0x9d, -1} }, { {0x1e, -1}, {0x9e, -1} }, { {0x1f, -1}, {0x9f, -1} },
        { {0x20, -1}, {0xa0, -1} }, { {0x21, -1}, {0xa1, -1} }, { {0x22, -1}, {0xa2, -1} }, { {0x23, -1}, {0xa3, -1} },
        { {0x24, -1}, {0xa4, -1} }, { {0x25, -1}, {0xa5, -1} }, { {0x26, -1}, {0xa6, -1} }, { {0x27, -1}, {0xa7, -1} },
        { {0x28, -1}, {0xa8, -1} }, { {0x29, -1}, {0xa9, -1} }, { {0x2a, -1}, {0xaa, -1} }, { {0x47, -1}, {0xc7, -1} },
        { {0x2c, -1}, {0xac, -1} }, { {0x2d, -1}, {0xad, -1} }, { {0x2e, -1}, {0xae, -1} }, { {0x2f, -1}, {0xaf, -1} },
        { {0x30, -1}, {0xb0, -1} }, { {0x31, -1}, {0xb1, -1} }, { {0x32, -1}, {0xb2, -1} }, { {0x33, -1}, {0xb3, -1} },
        { {0x34, -1}, {0xb4, -1} }, { {0x35, -1}, {0xb5, -1} }, { {0x36, -1}, {0xb6, -1} }, { {0x37, -1}, {0xb7, -1} },
        { {0x38, -1}, {0xb8, -1} }, { {0x39, -1}, {0xb9, -1} }, { {0x3a, -1}, {0xba, -1} }, { {0x3b, -1}, {0xbb, -1} },
        { {0x3c, -1}, {0xbc, -1} }, { {0x3d, -1}, {0xbd, -1} }, { {0x3e, -1}, {0xbe, -1} }, { {0x3f, -1}, {0xbf, -1} },
        { {0x40, -1}, {0xc0, -1} }, { {0x41, -1}, {0xc1, -1} }, { {0x42, -1}, {0xc2, -1} }, { {0x43, -1}, {0xc3, -1} },
        { {0x44, -1}, {0xc4, -1} }, { {0x45, -1}, {0xc5, -1} }, { {0x46, -1}, {0xc6, -1} }, { {0x47, -1}, {0xc7, -1} },
        { {0x48, -1}, {0xc8, -1} }, { {0x49, -1}, {0xc9, -1} }, { {0x4a, -1}, {0xca, -1} }, { {0x4b, -1}, {0xcb, -1} },
        { {0x4c, -1}, {0xcc, -1} }, { {0x4d, -1}, {0xcd, -1} }, { {0x4e, -1}, {0xce, -1} }, { {0x4f, -1}, {0xcf, -1} },
        { {0x50, -1}, {0xd0, -1} }, { {0x51, -1}, {0xd1, -1} }, { {0x52, -1}, {0xd2, -1} }, { {0x56, -1}, {0xd6, -1} },
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*54*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*58*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*5c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*60*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*64*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*68*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*6c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*70*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*74*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*78*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*7c*/

        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*80*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*84*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*88*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*8c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*90*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*94*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*98*/
        { {0x57, -1}, {0xd7, -1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*9c*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*a0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*a4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {0x2a, -1}, {0xaa, -1} },             { {-1},             {-1} },             /*a8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*ac*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*b0*/
        { {-1},             {-1} },             { {0x35, -1}, {0xb5, -1} },             { {0x36, -1}, {0xb6, -1} },             { {0x37, -1}, {0xb7, -1} }, /*b4*/
        { {0x38, -1}, {0xb8, -1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*b8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*bc*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*c0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {0x46, -1}, {0xc6, -1} },             { {0x47, -1}, {0xc7, -1} }, /*c4*/
        { {0x29, -1}, {0xa9, -1} },             { {0x49, -1}, {0xc9, -1} },             { {-1},             {-1} },             { {0x2b, -1}, {0xab, -1} }, /*c8*/
        { {-1},             {-1} },             { {0x4e, -1}, {0xce, -1} },             { {-1},             {-1} },             { {0x4f, -1}, {0xcf, -1} }, /*cc*/
        { {0x4a, -1}, {0xca, -1} },             { {0x51, -1}, {0xd1, -1} },             { {0x52, -1}, {0xd2, -1} },             { {0x53, -1}, {0xd3, -1} }, /*d0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*d4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*d8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*dc*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*e0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*e4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*e8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*ec*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*f0*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*f4*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*f8*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*fc*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*100*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*104*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*108*/
        { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             { {-1},             {-1} },             /*10c*/
};
static int oldkey[272];
static int keydelay[272];

void (*keyboard_send)(uint8_t val);
void (*keyboard_poll)();
int keyboard_scan = 1;

void keyboard_process()
{
        int c;
        int d;
        scancode *scancodes = (AT) ? scancode_set1 : scancode_xt;
        if (!keyboard_scan) return;
        if (TANDY) scancodes = scancode_tandy;
                
        for (c = 0; c < 272; c++)
        {
                if (pcem_key[c])
                        keydelay[c]++;
                else
                        keydelay[c] = 0;
        }
        
        for (c = 0; c < 272; c++)
        {
                if (pcem_key[c] != oldkey[c])
                {
                        oldkey[c] = pcem_key[c];
                        if ( pcem_key[c] && scancodes[c].scancodes_make[0]  == -1)
                           continue;
                        if (!pcem_key[c] && scancodes[c].scancodes_break[0] == -1)
                           continue;
//                        pclog("Key %02X start\n", c);
                        d = 0;
                        if (pcem_key[c])
                        {
                                while (scancodes[c].scancodes_make[d] != -1)
                                      keyboard_send(scancodes[c].scancodes_make[d++]);
                        }
                        else
                        {
                                while (scancodes[c].scancodes_break[d] != -1)
                                      keyboard_send(scancodes[c].scancodes_break[d++]);
                        }
                }
        }

        for (c = 0; c < 272; c++)
        {
                if (keydelay[c] >= 30)
                {
                        keydelay[c] -= 10;
                        if (scancode_set1[c].scancodes_make[0] == -1)
                           continue;
                           
                        d = 0;

                        while (scancode_set1[c].scancodes_make[d] != -1)
                              keyboard_send(scancode_set1[c].scancodes_make[d++]);
                }
        }
}
