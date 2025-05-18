////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>

#ifdef F
	#undef F
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
namespace FT800Display
{
	// Chip identifier macros
	const uint32_t FT800_CHIPID					= 0x00010008UL;

	const uint8_t	FT_DISPLAY_QVGA_320x240		= 0UL;
	const uint8_t	FT_DISPLAY_WQVGA_480x272 	= 1UL;

	// Graphics display list swap macros
    const uint32_t  FT_DLSWAP_DONE          = 0;
    const uint32_t  FT_DLSWAP_LINE          = 1;
    const uint32_t  FT_DLSWAP_FRAME         = 2;

	// Register definitions
    const uint32_t  REG_ID							= 0x102400UL;
    const uint32_t  REG_FRAMES						= 0x102404UL;
    const uint32_t  REG_CLOCK						= 0x102408UL;
    const uint32_t  REG_FREQUENCY					= 0x10240CUL;	
    const uint32_t  REG_SCREENSHOT_EN				= 0x102410UL;		
    const uint32_t  REG_SCREENSHOT_Y				= 0x102414UL;		
    const uint32_t  REG_SCREENSHOT_START 			= 0x102418UL;			
    const uint32_t  REG_CPURESET 					= 0x10241CUL;	
    const uint32_t  REG_TAP_CRC 					= 0x102420UL;	
    const uint32_t  REG_TAP_MASK 					= 0x102424UL;	
    const uint32_t  REG_HCYCLE 						= 0x102428UL;
    const uint32_t  REG_HOFFSET 					= 0x10242CUL;	
    const uint32_t  REG_HSIZE 						= 0x102430UL;
    const uint32_t  REG_HSYNC0 						= 0x102434UL;
    const uint32_t  REG_HSYNC1 						= 0x102438UL;
    const uint32_t  REG_VCYCLE 						= 0x10243CUL;
    const uint32_t  REG_VOFFSET 					= 0x102440UL;	
    const uint32_t  REG_VSIZE 						= 0x102444UL;
    const uint32_t  REG_VSYNC0 						= 0x102448UL;
    const uint32_t  REG_VSYNC1 						= 0x10244CUL;
    const uint32_t  REG_DLSWAP 						= 0x102450UL;
    const uint32_t  REG_ROTATE 						= 0x102454UL;
    const uint32_t  REG_OUTBITS 					= 0x102458UL;	
    const uint32_t  REG_DITHER 						= 0x10245CUL;
    const uint32_t  REG_SWIZZLE 					= 0x102460UL;	
    const uint32_t  REG_CSPREAD 					= 0x102464UL;	
    const uint32_t  REG_PCLK_POL 					= 0x102468UL;	
    const uint32_t  REG_PCLK 						= 0x10246CUL;
    const uint32_t  REG_TAG_X 						= 0x102470UL;
    const uint32_t  REG_TAG_Y 						= 0x102474UL;
    const uint32_t  REG_TAG 						= 0x102478UL;
    const uint32_t  REG_VOL_PB 						= 0x10247CUL;
    const uint32_t  REG_VOL_SOUND 					= 0x102480UL;	
    const uint32_t  REG_SOUND 						= 0x102484UL;
    const uint32_t  REG_PLAY 						= 0x102488UL;
    const uint32_t  REG_GPIO_DIR 					= 0x10248CUL;	
	const uint32_t	REG_GPIO						= 0x102490UL;
    const uint32_t  REG_INT_FLAGS           		= 0x102498UL;
    const uint32_t  REG_INT_EN              		= 0x10249CUL;
    const uint32_t  REG_INT_MASK            		= 0x1024A0UL;
    const uint32_t  REG_PLAYBACK_START      		= 0x1024A4UL;
    const uint32_t  REG_PLAYBACK_LENGTH     		= 0x1024A8UL;
    const uint32_t  REG_PLAYBACK_READPTR    		= 0x1024ACUL;
    const uint32_t  REG_PLAYBACK_FREQ       		= 0x1024B0UL;
    const uint32_t  REG_PLAYBACK_FORMAT     		= 0x1024B4UL;
    const uint32_t  REG_PLAYBACK_LOOP       		= 0x1024B8UL;
    const uint32_t  REG_PLAYBACK_PLAY       		= 0x1024BCUL;
    const uint32_t  REG_PWM_HZ              		= 0x1024C0UL;
    const uint32_t  REG_PWM_DUTY            		= 0x1024C4UL;
    const uint32_t  REG_MACRO_0             		= 0x1024C8UL;
    const uint32_t  REG_MACRO_1             		= 0x1024CCUL;
    const uint32_t  REG_SCREENSHOT_BUSY				= 0x1024D8UL;
    const uint32_t  REG_CMD_READ            		= 0x1024E4UL;
    const uint32_t  REG_CMD_WRITE           		= 0x1024E8UL;
    const uint32_t  REG_CMD_DL              		= 0x1024ECUL;
    const uint32_t  REG_TOUCH_MODE          		= 0x1024F0UL;
    const uint32_t  REG_TOUCH_ADC_MODE      		= 0x1024F4UL;
    const uint32_t  REG_TOUCH_CHARGE        		= 0x1024F8UL;
    const uint32_t  REG_TOUCH_SETTLE        		= 0x1024FCUL;
    const uint32_t  REG_TOUCH_OVERSAMPLE    		= 0x102500UL;
    const uint32_t  REG_TOUCH_RZTHRESH      		= 0x102504UL;
    const uint32_t  REG_TOUCH_RAW_XY        		= 0x102508UL;
    const uint32_t  REG_TOUCH_RZ            		= 0x10250CUL;
    const uint32_t  REG_TOUCH_SCREEN_XY     		= 0x102510UL;
    const uint32_t  REG_TOUCH_TAG_XY        		= 0x102514UL;
    const uint32_t  REG_TOUCH_TAG           		= 0x102518UL;
    const uint32_t  REG_TOUCH_TRANSFORM_A   		= 0x10251CUL;
    const uint32_t  REG_TOUCH_TRANSFORM_B   		= 0x102520UL;
    const uint32_t  REG_TOUCH_TRANSFORM_C   		= 0x102524UL;
    const uint32_t  REG_TOUCH_TRANSFORM_D   		= 0x102528UL;
    const uint32_t  REG_TOUCH_TRANSFORM_E   		= 0x10252CUL;
    const uint32_t  REG_TOUCH_TRANSFORM_F   		= 0x102530UL;
    const uint32_t  REG_SCREENSHOT_READ				= 0x102554UL;
    const uint32_t  REG_TRIM						= 0x10256CUL;
    const uint32_t  REG_TOUCH_DIRECT_XY 			= 0x102574UL;
    const uint32_t  REG_TOUCH_DIRECT_Z1Z2 			= 0x102578UL;
    const uint32_t  REG_TRACKER						= 0x109000UL;

	const uint8_t C_ENABLE_DISPLAY_PIN			= 7;
	const uint8_t C_ENABLE_AUDIO_PIN			= 1;

	// Memory definitions
	const uint32_t FT_RAM_G						= 0x000000UL;
	const uint32_t FT_ROM_CHIPID				= 0x0C0000UL;	
	const uint32_t FT_ROM_FONT					= 0x0BB23CUL;
	const uint32_t FT_ROM_FONT_ADDR				= 0x0FFFFCUL;		
	const uint32_t FT_RAM_DL					= 0x100000UL;
	const uint32_t FT_RAM_PAL					= 0x102000UL;
	const uint32_t FT_RAM_CMD					= 0x108000UL;
	const uint32_t FT_RAM_SCREENSHOT			= 0x1C2000UL;		

	// Host command macros
	const uint8_t FT_ACTIVE						= 0x00;			// Place FT800 in active state
	const uint8_t FT_STANDBY					= 0x41;			// Place FT800 in Standby (clk running)
	const uint8_t FT_SLEEP						= 0x42;			// Place FT800 in Sleep (clk off)
	const uint8_t FT_PWRDOWN					= 0x50;			// Place FT800 in Power Down (core off)
	const uint8_t FT_CLKEXT						= 0x44;			// Select external clock source
	const uint8_t FT_CLKINT						= 0x48;			// Select internal clock source
	const uint8_t FT_CLK48M						= 0x62;			// Select 48MHz PLL output
	const uint8_t FT_CLK36M						= 0x61;			// Select 36MHz PLL output
	const uint8_t FT_CORERST					= 0x68;			// Reset core - all registers default and processors reset

	// Widget command macros
	const uint8_t FT_OPT_MONO              = 1;
	const uint8_t FT_OPT_NODL              = 2;
	const uint32_t FT_OPT_FLAT             = 256;
	const uint32_t FT_OPT_CENTERX          = 512;
	const uint32_t FT_OPT_CENTERY          = 1024;
	const uint32_t FT_OPT_CENTER           = (FT_OPT_CENTERX | FT_OPT_CENTERY);
	const uint32_t FT_OPT_NOBACK           = 4096;
	const uint32_t FT_OPT_NOTICKS          = 8192;
	const uint32_t FT_OPT_NOHM             = 16384;
	const uint32_t FT_OPT_NOPOINTER        = 16384;
	const uint32_t FT_OPT_NOSECS           = 32768;
	const uint32_t FT_OPT_NOHANDS          = 49152;
	const uint32_t FT_OPT_RIGHTX           = 2048;
	const uint32_t FT_OPT_SIGNED           = 256;

	// Memory buffer sizes
	const uint32_t FT_RAM_G_SIZE					= 256*1024L;
	const uint32_t FT_CMDFIFO_SIZE					= 4*1024L;
	const uint32_t FT_RAM_DL_SIZE					= 8*1024L;
	const uint32_t FT_RAM_PAL_SIZE					= 1*1024L;

	const uint32_t FT_SPI_CLK_FREQ_MIN			= 8 * 1000000;
	const uint32_t FT_SPI_CLK_FREQ_MAX			= 21 * 1000000;

	//coprocessor error macros
	const uint32_t FT_COPRO_ERROR					= 0xfffUL;

	// Graphics primitives macros
	const uint8_t FT_BITMAPS              = 1;
	const uint8_t FT_POINTS               = 2;
	const uint8_t FT_LINES                = 3;
	const uint8_t FT_LINE_STRIP           = 4;
	const uint8_t FT_EDGE_STRIP_R         = 5;
	const uint8_t FT_EDGE_STRIP_L         = 6;
	const uint8_t FT_EDGE_STRIP_A         = 7;
	const uint8_t FT_EDGE_STRIP_B         = 8;
	const uint8_t FT_RECTS                = 9;

	// Coprocessor related commands
	const uint32_t CMD_APPEND           			= 0xFFFFFF1EUL;
	const uint32_t CMD_BGCOLOR          			= 0xFFFFFF09UL;
	const uint32_t CMD_BITMAP_TRANSFORM 			= 0xFFFFFF21UL;
	const uint32_t CMD_BUTTON           			= 0xFFFFFF0DUL;
	const uint32_t CMD_CALIBRATE        			= 0xFFFFFF15UL;
	const uint32_t CMD_CLOCK            			= 0xFFFFFF14UL;
	const uint32_t CMD_COLDSTART        			= 0xFFFFFF32UL;
	const uint32_t CMD_CRC              			= 0xFFFFFF03UL;
	const uint32_t CMD_DIAL             			= 0xFFFFFF2DUL;
	const uint32_t CMD_DLSTART          			= 0xFFFFFF00UL;
	const uint32_t CMD_EXECUTE          			= 0xFFFFFF07UL;
	const uint32_t CMD_FGCOLOR          			= 0xFFFFFF0AUL;
	const uint32_t CMD_GAUGE            			= 0xFFFFFF13UL;
	const uint32_t CMD_GETMATRIX        			= 0xFFFFFF33UL;
	const uint32_t CMD_GETPOINT         			= 0xFFFFFF08UL;
	const uint32_t CMD_GETPROPS         			= 0xFFFFFF25UL;
	const uint32_t CMD_GETPTR           			= 0xFFFFFF23UL;
	const uint32_t CMD_GRADCOLOR        			= 0xFFFFFF34UL;
	const uint32_t CMD_GRADIENT         			= 0xFFFFFF0BUL;
	const uint32_t CMD_HAMMERAUX        			= 0xFFFFFF04UL;
	const uint32_t CMD_IDCT             			= 0xFFFFFF06UL;
	const uint32_t CMD_INFLATE          			= 0xFFFFFF22UL;
	const uint32_t CMD_INTERRUPT        			= 0xFFFFFF02UL;
	const uint32_t CMD_KEYS             			= 0xFFFFFF0EUL;
	const uint32_t CMD_LOADIDENTITY     			= 0xFFFFFF26UL;
	const uint32_t CMD_LOADIMAGE        			= 0xFFFFFF24UL;
	const uint32_t CMD_LOGO             			= 0xFFFFFF31UL;
	const uint32_t CMD_MARCH            			= 0xFFFFFF05UL;
	const uint32_t CMD_MEMCPY           			= 0xFFFFFF1DUL;
	const uint32_t CMD_MEMCRC           			= 0xFFFFFF18UL;
	const uint32_t CMD_MEMSET           			= 0xFFFFFF1BUL;
	const uint32_t CMD_MEMWRITE         			= 0xFFFFFF1AUL;
	const uint32_t CMD_MEMZERO          			= 0xFFFFFF1CUL;
	const uint32_t CMD_NUMBER           			= 0xFFFFFF2EUL;
	const uint32_t CMD_PROGRESS         			= 0xFFFFFF0FUL;
	const uint32_t CMD_REGREAD          			= 0xFFFFFF19UL;
	const uint32_t CMD_ROTATE           			= 0xFFFFFF29UL;
	const uint32_t CMD_SCALE            			= 0xFFFFFF28UL;
	const uint32_t CMD_SCREENSAVER      			= 0xFFFFFF2FUL;
	const uint32_t CMD_SCROLLBAR        			= 0xFFFFFF11UL;
	const uint32_t CMD_SETFONT          			= 0xFFFFFF2BUL;
	const uint32_t CMD_SETMATRIX        			= 0xFFFFFF2AUL;
	const uint32_t CMD_SKETCH           			= 0xFFFFFF30UL;
	const uint32_t CMD_SLIDER           			= 0xFFFFFF10UL;
	const uint32_t CMD_SNAPSHOT         			= 0xFFFFFF1FUL;
	const uint32_t CMD_SPINNER          			= 0xFFFFFF16UL;
	const uint32_t CMD_STOP             			= 0xFFFFFF17UL;
	const uint32_t CMD_SWAP             			= 0xFFFFFF01UL;
	const uint32_t CMD_TEXT             			= 0xFFFFFF0CUL;
	const uint32_t CMD_TOGGLE           			= 0xFFFFFF12UL;
	const uint32_t CMD_TOUCH_TRANSFORM  			= 0xFFFFFF20UL;
	const uint32_t CMD_TRACK            			= 0xFFFFFF2CUL;
	const uint32_t CMD_TRANSLATE        			= 0xFFFFFF27UL;

	// Macros for sound play and stop
	const uint8_t FT_SOUND_PLAY					= 1;
	const uint8_t FT_AUDIO_PLAY					= 1;

/* Macros related to inbuilt font */
	const uint32_t FT_NUMCHAR_PERFONT 		= 128L;		//number of font characters per bitmap handle
	const uint32_t FT_FONT_TABLE_SIZE 		= 148L;		//size of the font table - utilized for loopup by the graphics engine
	const uint32_t FT_FONT_TABLE_POINTER	= 0xFFFFCUL;	//pointer to the inbuilt font tables starting from bitmap handle 16

/* Audio sample type macros */
	const uint8_t FT_LINEAR_SAMPLES      = 0;	//8bit signed samples
	const uint8_t FT_ULAW_SAMPLES        = 1;	//8bit ulaw samples
	const uint8_t FT_ADPCM_SAMPLES       = 2;	//4bit ima adpcm samples

/* Synthesized sound macros */
	const uint8_t FT_SILENCE              = 0x00;

	const uint8_t FT_SQUAREWAVE           = 0x01;
	const uint8_t FT_SINEWAVE             = 0x02;
	const uint8_t FT_SAWTOOTH             = 0x03;
	const uint8_t FT_TRIANGLE             = 0x04;

	const uint8_t FT_BEEPING              = 0x05;
	const uint8_t FT_ALARM                = 0x06;
	const uint8_t FT_WARBLE               = 0x07;
	const uint8_t FT_CAROUSEL             = 0x08;

//	const uint8_t FT_PIPS(n)              = 0x0F + (n);

	const uint8_t FT_HARP                 = 0x40;
	const uint8_t FT_XYLOPHONE            = 0x41;
	const uint8_t FT_TUBA                 = 0x42;
	const uint8_t FT_GLOCKENSPIEL         = 0x43;
	const uint8_t FT_ORGAN                = 0x44;
	const uint8_t FT_TRUMPET              = 0x45;
	const uint8_t FT_PIANO                = 0x46;
	const uint8_t FT_CHIMES               = 0x47;
	const uint8_t FT_MUSICBOX             = 0x48;
	const uint8_t FT_BELL                 = 0x49;

	const uint8_t FT_CLICK                = 0x50;
	const uint8_t FT_SWITCH               = 0x51;
	const uint8_t FT_COWBELL              = 0x52;
	const uint8_t FT_NOTCH                = 0x53;
	const uint8_t FT_HIHAT                = 0x54;
	const uint8_t FT_KICKDRUM             = 0x55;
	const uint8_t FT_POP                  = 0x56;
	const uint8_t FT_CLACK                = 0x57;
	const uint8_t FT_CHACK                = 0x58;

	const uint8_t FT_MUTE                 = 0x60;
	const uint8_t FT_UNMUTE               = 0x61;

	/* Synthesized sound frequencies, midi note macros */
	const uint8_t FT_MIDI_A0            	= 21;
	const uint8_t FT_MIDI_A_0           	= 22;
	const uint8_t FT_MIDI_B0            	= 23;
	const uint8_t FT_MIDI_C1            	= 24;
	const uint8_t FT_MIDI_C_1           	= 25;
	const uint8_t FT_MIDI_D1            	= 26;
	const uint8_t FT_MIDI_D_1           	= 27;
	const uint8_t FT_MIDI_E1            	= 28;
	const uint8_t FT_MIDI_F1            	= 29;
	const uint8_t FT_MIDI_F_1           	= 30;
	const uint8_t FT_MIDI_G1            	= 31;
	const uint8_t FT_MIDI_G_1           	= 32;
	const uint8_t FT_MIDI_A1            	= 33;
	const uint8_t FT_MIDI_A_1           	= 34;
	const uint8_t FT_MIDI_B1            	= 35;
	const uint8_t FT_MIDI_C2            	= 36;
	const uint8_t FT_MIDI_C_2           	= 37;
	const uint8_t FT_MIDI_D2            	= 38;
	const uint8_t FT_MIDI_D_2           	= 39;
	const uint8_t FT_MIDI_E2            	= 40;
	const uint8_t FT_MIDI_F2            	= 41;
	const uint8_t FT_MIDI_F_2           	= 42;
	const uint8_t FT_MIDI_G2            	= 43;
	const uint8_t FT_MIDI_G_2           	= 44;
	const uint8_t FT_MIDI_A2            	= 45;
	const uint8_t FT_MIDI_A_2           	= 46;
	const uint8_t FT_MIDI_B2            	= 47;
	const uint8_t FT_MIDI_C3            	= 48;
	const uint8_t FT_MIDI_C_3           	= 49;
	const uint8_t FT_MIDI_D3            	= 50;
	const uint8_t FT_MIDI_D_3           	= 51;
	const uint8_t FT_MIDI_E3            	= 52;
	const uint8_t FT_MIDI_F3            	= 53;
	const uint8_t FT_MIDI_F_3           	= 54;
	const uint8_t FT_MIDI_G3            	= 55;
	const uint8_t FT_MIDI_G_3           	= 56;
	const uint8_t FT_MIDI_A3            	= 57;
	const uint8_t FT_MIDI_A_3           	= 58;
	const uint8_t FT_MIDI_B3            	= 59;
	const uint8_t FT_MIDI_C4            	= 60;
	const uint8_t FT_MIDI_C_4           	= 61;
	const uint8_t FT_MIDI_D4            	= 62;
	const uint8_t FT_MIDI_D_4           	= 63;
	const uint8_t FT_MIDI_E4            	= 64;
	const uint8_t FT_MIDI_F4            	= 65;
	const uint8_t FT_MIDI_F_4           	= 66;
	const uint8_t FT_MIDI_G4            	= 67;
	const uint8_t FT_MIDI_G_4           	= 68;
	const uint8_t FT_MIDI_A4            	= 69;
	const uint8_t FT_MIDI_A_4           	= 70;
	const uint8_t FT_MIDI_B4            	= 71;
	const uint8_t FT_MIDI_C5            	= 72;
	const uint8_t FT_MIDI_C_5           	= 73;
	const uint8_t FT_MIDI_D5            	= 74;
	const uint8_t FT_MIDI_D_5           	= 75;
	const uint8_t FT_MIDI_E5            	= 76;
	const uint8_t FT_MIDI_F5            	= 77;
	const uint8_t FT_MIDI_F_5           	= 78;
	const uint8_t FT_MIDI_G5            	= 79;
	const uint8_t FT_MIDI_G_5           	= 80;
	const uint8_t FT_MIDI_A5            	= 81;
	const uint8_t FT_MIDI_A_5           	= 82;
	const uint8_t FT_MIDI_B5            	= 83;
	const uint8_t FT_MIDI_C6            	= 84;
	const uint8_t FT_MIDI_C_6           	= 85;
	const uint8_t FT_MIDI_D6            	= 86;
	const uint8_t FT_MIDI_D_6           	= 87;
	const uint8_t FT_MIDI_E6            	= 88;
	const uint8_t FT_MIDI_F6            	= 89;
	const uint8_t FT_MIDI_F_6           	= 90;
	const uint8_t FT_MIDI_G6            	= 91;
	const uint8_t FT_MIDI_G_6           	= 92;
	const uint8_t FT_MIDI_A6            	= 93;
	const uint8_t FT_MIDI_A_6           	= 94;
	const uint8_t FT_MIDI_B6            	= 95;
	const uint8_t FT_MIDI_C7            	= 96;
	const uint8_t FT_MIDI_C_7           	= 97;
	const uint8_t FT_MIDI_D7            	= 98;
	const uint8_t FT_MIDI_D_7           	= 99;
	const uint8_t FT_MIDI_E7            	= 100;
	const uint8_t FT_MIDI_F7            	= 101;
	const uint8_t FT_MIDI_F_7           	= 102;
	const uint8_t FT_MIDI_G7            	= 103;
	const uint8_t FT_MIDI_G_7           	= 104;
	const uint8_t FT_MIDI_A7            	= 105;
	const uint8_t FT_MIDI_A_7           	= 106;
	const uint8_t FT_MIDI_B7            	= 107;
	const uint8_t FT_MIDI_C8            	= 108;

	// Status enums for graphics engine
	enum FT_GEStatus
	{	
		FT_GE_OK = 0,	
		FT_GE_BUSY = 1,
		FT_GE_FINISHED = 2,
	
		/* Graphics related error enums */
		FT_GE_ERROR_INVALID_PRIMITIVE = 20,
		FT_GE_ERROR_INVALID_BITMAP_FORMAT = 21,
		FT_GE_ERROR_INVALID_BITMAP_HANDLENUM = 22,
		FT_GE_ERROR_VERTEX_OUTOFRANGE = 23,

		/* Coprocessor related enums */
		FT_GE_ERROR = 50,						//undefined error
		FT_GE_ERROR_JPEG = 51,					//erranious jpeg data
		FT_GE_ERROR_DEFLATE = 52,				//erranious deflated data
		FT_GE_ERROR_DISPLAYLIST_OVERFLOW = 53,	//DL buffer overflow
		FT_GE_ERROR_INVALID_WIDGET_PARAMS = 54,	//invalid input parameters - out of bound
	
		/* Display parameters error */
		FT_GE_ERROR_DISPLAYPARAMS = 100,//error in the display parameters
	};

	struct sTagXY
	{
		int16_t y;		//y coordinate of touch object
		int16_t x;		//x coordinate of touch object
		uint16_t tag;	//TAG value of the object
	};

	struct sTrackTag
	{
		uint16_t tag;	//TAG value of the object
		uint16_t track;	//track value of the object	
	};

	const uint8_t CFontSizes[] =
	{
		8,
		8,
		16,
		16,
		13,
		17,
		20,
		22,
		29,
		38,
		16,
		20,
		25,
		28,
		36,
		49
	};
}
//---------------------------------------------------------------------------
	class DisplayFT800_WQVGA_480x272
	{
	public:
		template <typename T> static void ApplyInit( T *AInstance )
		{
			// FT_VM800P43_50
			AInstance->WriteFast16( FT800Display::REG_VSYNC0, 	0L ); //FT800Display::FT_DISPLAY_VSYNC0_WQVGA );
			AInstance->WriteFast16( FT800Display::REG_VSYNC1, 	10L ); //FT800Display::FT_DISPLAY_VSYNC1_WQVGA );
			AInstance->WriteFast16( FT800Display::REG_VOFFSET, 	12L ); //FT800Display::FT_DISPLAY_VOFFSET_WQVGA);
			AInstance->WriteFast16( FT800Display::REG_VCYCLE, 	292L ); //FT800Display::FT_DISPLAY_VCYCLE_WQVGA );
			AInstance->WriteFast16( FT800Display::REG_HSYNC0, 	0L ); //FT800Display::FT_DISPLAY_HSYNC0_WQVGA );
			AInstance->WriteFast16( FT800Display::REG_HSYNC1, 	41L ); //FT800Display::FT_DISPLAY_HSYNC1_WQVGA );
			AInstance->WriteFast16( FT800Display::REG_HOFFSET, 	43L ); //FT800Display::FT_DISPLAY_HOFFSET_WQVGA);
			AInstance->WriteFast16( FT800Display::REG_HCYCLE, 	548L ); //FT800Display::FT_DISPLAY_HCYCLE_WQVGA );
			AInstance->WriteFast16( FT800Display::REG_HSIZE,	480L ); //FT800Display::FT_DISPLAY_HSIZE_WQVGA 	);
			AInstance->WriteFast16( FT800Display::REG_VSIZE, 	272L ); //FT800Display::FT_DISPLAY_VSIZE_WQVGA 	);		 		
			AInstance->WriteFast16( FT800Display::REG_PCLK_POL,	1L ); //FT800Display::FT_DISPLAY_PCLKPOL_WQVGA);
			AInstance->WriteFast16( FT800Display::REG_SWIZZLE, 	0L ); //FT800Display::FT_DISPLAY_SWIZZLE_WQVGA);
			// after configuring display parameters, configure pclk
			AInstance->WriteFast16( FT800Display::REG_PCLK,		5L ); //FT800Display::FT_DISPLAY_PCLK_WQVGA	);	
		}

	};
//---------------------------------------------------------------------------
	class DisplayFT800_QVGA_320x240
	{
	public:
		template <typename T> static void ApplyInit( T *AInstance )
		{
			// FT_VM800P43_50
			AInstance->WriteFast16( FT800Display::REG_VSYNC0, 	0L ); //FT800Display::FT_DISPLAY_VSYNC0_QVGA );
			AInstance->WriteFast16( FT800Display::REG_VSYNC1, 	2L ); //FT800Display::FT_DISPLAY_VSYNC1_QVGA );
			AInstance->WriteFast16( FT800Display::REG_VOFFSET, 	13L ); //FT800Display::FT_DISPLAY_VOFFSET_QVGA);
			AInstance->WriteFast16( FT800Display::REG_VCYCLE, 	263L ); //FT800Display::FT_DISPLAY_VCYCLE_QVGA );
			AInstance->WriteFast16( FT800Display::REG_HSYNC0, 	0L ); //FT800Display::FT_DISPLAY_HSYNC0_QVGA );
			AInstance->WriteFast16( FT800Display::REG_HSYNC1, 	10L ); //FT800Display::FT_DISPLAY_HSYNC1_QVGA );
			AInstance->WriteFast16( FT800Display::REG_HOFFSET, 	70L ); //FT800Display::FT_DISPLAY_HOFFSET_QVGA);
			AInstance->WriteFast16( FT800Display::REG_HCYCLE, 	408L ); //FT800Display::FT_DISPLAY_HCYCLE_QVGA );
			AInstance->WriteFast16( FT800Display::REG_HSIZE,	320L ); //FT800Display::FT_DISPLAY_HSIZE_QVGA 	);
			AInstance->WriteFast16( FT800Display::REG_VSIZE, 	240L ); //FT800Display::FT_DISPLAY_VSIZE_QVGA 	);		 		
			AInstance->WriteFast16( FT800Display::REG_PCLK_POL,	0L ); //FT800Display::FT_DISPLAY_PCLKPOL_QVGA);
			AInstance->WriteFast16( FT800Display::REG_SWIZZLE, 	2L ); //FT800Display::FT_DISPLAY_SWIZZLE_QVGA);
			// after configuring display parameters, configure pclk
			AInstance->WriteFast16( FT800Display::REG_PCLK,		8L ); //FT800Display::FT_DISPLAY_PCLK_WQVGA	);	
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_Brightness,
		typename T_BusyOutputPin,
		typename T_ChipSelectOutputPin,
		typename T_Elements_Render,
		typename T_ExternalClock,
		typename T_FModified,
		typename T_FTrnsFlag,
		typename T_PowerDownOutputPin,
		typename T_DISPLAY_TYPE
	> class DisplayFT800 :
		public T_Brightness,
		public T_BusyOutputPin,
		public T_ChipSelectOutputPin,
		public T_ExternalClock,
		public T_FModified,
		public T_FTrnsFlag,
		public T_PowerDownOutputPin
	{
	public:
		typedef DisplayFT800<
				T_SPI, C_SPI,
				T_Brightness,
				T_BusyOutputPin,
				T_ChipSelectOutputPin,
				T_Elements_Render,
				T_ExternalClock,
				T_FModified,
				T_FTrnsFlag,
				T_PowerDownOutputPin,
				T_DISPLAY_TYPE
			> T_DISPLAY;
		
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( PowerDownOutputPin )
		_V_PIN_( BusyOutputPin ) 

	public:
		_V_PROP_( ExternalClock )

	protected:
		_V_PROP_( FTrnsFlag )
		_V_PROP_( FModified )
		uint8_t	FLastTag = 0;
		uint16_t FCmdFifoWp = 0;
		uint16_t FFreeSpace = FT800Display::FT_CMDFIFO_SIZE - 4;

	public:
		inline void SetBrightness( float AOldValue, float AValue )
		{
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_PWM_DUTY, AValue * 128 + 0.5 );
		}

	public:
		inline T_DISPLAY &GetDisplay()
		{
			return *this;
		}

		inline void Modified()
		{
			FModified() = true;
		}

	public:
		template <uint32_t C_SPEED> inline void BeginTransaction()
		{
			C_SPI.beginTransaction(SPISettings( C_SPEED, MSBFIRST, SPI_MODE0));
//			C_SPI.beginTransaction(SPISettings( FT800Display::FT_SPI_CLK_FREQ_MIN, MSBFIRST, SPI_MODE0));
		}

		// APIs related to memory read & write/transport
		template <uint32_t C_SPEED> void StartRead( uint32_t Addr )
		{
			//make sure the union array indexes are modified for big endian usecase
			TAsBytes<uint32_t>	AValue = Addr;
			T_ChipSelectOutputPin::SetPinValueLow();

			BeginTransaction<C_SPEED>();
			C_SPI.transfer(AValue[2]);
			C_SPI.transfer(AValue[1]);
			C_SPI.transfer(AValue[0]);
			C_SPI.transfer(0x00);//dummy byte for read

/*
			union
			{
				uint32_t UUint32;
				uint8_t  A[4];
			};
			UUint32 = Addr;

			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer(A[2]);
			C_SPI.transfer(A[1]);
			C_SPI.transfer(A[0]);
			C_SPI.transfer(0x00);//dummy byte for read
*/
		}

		template <uint32_t C_SPEED> void StartWrite(uint32_t Addr)
		{
			TAsBytes<uint32_t>	AValue = Addr;
			T_ChipSelectOutputPin::SetPinValueLow();

			BeginTransaction<C_SPEED>();
			C_SPI.transfer(AValue[2] | 0x80);
			C_SPI.transfer(AValue[1]);
			C_SPI.transfer(AValue[0]);
		}
	
		//Read a byte from Addr location
		template <uint32_t C_SPEED> uint8_t Read(uint32_t Addr)
		{
			StartRead<C_SPEED>(Addr);
			uint8_t ReadByte = C_SPI.transfer(0x00);
			EndTransfer();
			return (ReadByte);
		}
	
		//read a short from Addr location
		template <uint32_t C_SPEED> uint16_t Read16(uint32_t Addr)
		{
			///little endian read
			StartRead<C_SPEED>(Addr);

			TAsBytes<uint16_t>	AValue;
			AValue[0] = C_SPI.transfer(0x00);
			AValue[1] = C_SPI.transfer(0x00);		

			EndTransfer();
			return (AValue);
		}

		//read a word from Addr location
		template <uint32_t C_SPEED> uint32_t Read32(uint32_t Addr)
		{
			//little endian read
			StartRead<C_SPEED>(Addr);		

			TAsBytes<uint32_t>	AValue;
			AValue[0] = C_SPI.transfer(0x00);
			AValue[1] = C_SPI.transfer(0x00);		
			AValue[2] = C_SPI.transfer(0x00);
			AValue[3] = C_SPI.transfer(0x00);		

			EndTransfer();
			return (AValue);
		}

		//read N bytes from Addr location
		template <uint32_t C_SPEED> void Read(uint32_t Addr, uint8_t *Src, uint32_t NBytes)
		{
			StartRead<C_SPEED>(Addr);
			for( uint32_t i=0;i<NBytes;i++)
				*Src++ = C_SPI.transfer(0x00);

			EndTransfer();
		}
	
		//write a byte to Addr location
		template <uint32_t C_SPEED> void Write(uint32_t Addr, uint8_t Value8)
		{	
			StartWrite<C_SPEED>(Addr);
			C_SPI.transfer(Value8);
			EndTransfer();
		}

		void WriteFast16(uint32_t Addr, uint16_t Value16)
		{
			Write16<FT800Display::FT_SPI_CLK_FREQ_MIN>( Addr, Value16 );
		}

		//write a short to Addr location
		template <uint32_t C_SPEED> void Write16(uint32_t Addr, uint16_t Value16)
		{
			TAsBytes<uint16_t>	AValue = Value16;
			///little endian read
			StartWrite<C_SPEED>(Addr);
			C_SPI.transfer(AValue[0]);
			C_SPI.transfer(AValue[1]);		
			EndTransfer();
		}	

		//write a word to Addr location
		template <uint32_t C_SPEED> void Write32(uint32_t Addr, uint32_t Value32)
		{
			TAsBytes<uint32_t>	AValue = Value32;
			///little endian read
			StartWrite<C_SPEED>(Addr);
			C_SPI.transfer(AValue[0]);
			C_SPI.transfer(AValue[1]);		
			C_SPI.transfer(AValue[2]);
			C_SPI.transfer(AValue[3]);		
			EndTransfer();
		}	
	
		template <uint32_t C_SPEED> void Write(uint32_t Addr, uint8_t *Src, uint32_t NBytes)
		{
			StartWrite<C_SPEED>(Addr);
			for( uint32_t i=0;i<NBytes;i++)
				C_SPI.transfer(*Src++);

			EndTransfer();
		}
	
/*
		void Writefromflash(uint32_t Addr, prog_uchar *Src, uint32_t NBytes)
		{
			uint32_t i;
			StartWrite(Addr);
			for(i=0;i<NBytes;i++)
			{
				C_SPI.transfer(pgm_read_byte_near(Src));
				Src++;
			}		
			EndTransfer();
		}
*/	
		//apis useful for performance via spi - write only functionality

		//assert CSpin and send write command
		void StartTransfer(uint32_t Addr)
		{
			TAsBytes<uint32_t>	AValue = Addr;
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(AValue[2] | 0x80);
			C_SPI.transfer(AValue[1]);
			C_SPI.transfer(AValue[0]);
		}

		//de assert CSpin
		void EndTransfer(void)
		{
			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		//transfer a single byte
		void Transfer(uint8_t Value8)
		{
			C_SPI.transfer(Value8);
		}

		//transfer 2 bytes
		void Transfer16(uint16_t Value16)
		{
			TAsBytes<uint16_t>	AValue = Value16;
			AValue[0] = C_SPI.transfer(AValue[0]);
			AValue[1] = C_SPI.transfer(AValue[1]);		
		}

		//transfer 4 bytes
		void Transfer32(uint32_t Value32)
		{
			TAsBytes<uint32_t>	AValue = Value32;		
			C_SPI.transfer(AValue[0]);
			C_SPI.transfer(AValue[1]);
			C_SPI.transfer(AValue[2]);
			C_SPI.transfer(AValue[3]);		
		}

		//transfer N bytes
		void Transfer(uint8_t *Buff,uint32_t NBytes)
		{
			for( uint32_t i=0; i < NBytes; i++ )
				C_SPI.transfer(*Buff++);
			
		}

	protected:
		void DisplayOn( bool AValue )
		{
			// switch on the display, 1 means enable and 0 means disable
			if( AValue )
				Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_GPIO, (1 << FT800Display::C_ENABLE_DISPLAY_PIN ) | Read<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_GPIO ));

			else
				Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_GPIO, (~(1 << FT800Display::C_ENABLE_DISPLAY_PIN )) & Read<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_GPIO ));
		}

		void AudioOn( bool AValue )
		{
			// switch on the audio , 1 means enable and 0 means disable
			if( AValue )
				Write<FT800Display::FT_SPI_CLK_FREQ_MAX>(FT800Display::REG_GPIO, (1 << FT800Display::C_ENABLE_AUDIO_PIN) | Read<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_GPIO ));

			else
				Write<FT800Display::FT_SPI_CLK_FREQ_MAX>(FT800Display::REG_GPIO, (~(1 << FT800Display::C_ENABLE_AUDIO_PIN)) & Read<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_GPIO ));
		}

		void Reset(void)
		{

			// Reset of whole graphics controller
			PDN_Cycle();
			// send active command to enable SPI, followed by download of default DL into graphics engine, followed by SPI settings wrt internal clock requirements during bootup stage
			ActiveInternalClock();
		}

		void PDN_Cycle(void)
		{
			/* do a power cycle by toggling power down pin followed by active command followed by display list initialization sequence */
			T_PowerDownOutputPin::SetPinValueHigh();
			delay(20);
			T_PowerDownOutputPin::SetPinValueLow();
			delay(20);
			T_PowerDownOutputPin::SetPinValueHigh();
			delay(20);
		}

		void ActiveInternalClock(void)
		{
			uint8_t FT_DLCODE_BOOTUP[] =
			{
			  0,0,0,2,  //GPU instruction CLEAR_COLOR_RGB - black color
			  7,0,0,38, //GPU instruction CLEAR
			  0,0,0,0,  //GPU instruction DISPLAY
			};
			// change the SPI clock to <11MHz

/*
		void HostCommand(uint32_t HostCommand)
		{
			// construct host command and send to graphics controller
			uint32_t Addr = HostCommand << 16;
			Read(Addr);//ideally sending 3 bytes is sufficient
			delay(20);//worst scenario
		}    
*/

//			ChangeClock( FT800Display::FT_SPI_CLK_FREQ_MIN );

			// FT_ACTIVE with low frequency!
/*
			TAsBytes<uint32_t>	AValue = FT800Display::FT_ACTIVE;
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction(SPISettings( FT800Display::FT_SPI_CLK_FREQ_MIN, MSBFIRST, SPI_MODE0));

			C_SPI.transfer(AValue[2]);
			C_SPI.transfer(AValue[1]);
			C_SPI.transfer(AValue[0]);
			C_SPI.transfer(0x00);//dummy byte for read

			uint8_t ReadByte = C_SPI.transfer(0x00);
			EndTransfer();
*/
			HostCommand<FT800Display::FT_SPI_CLK_FREQ_MIN>( FT800Display::FT_ACTIVE );//wake up the processor from sleep state
			delay(20);

			// download the first display list
			Write<FT800Display::FT_SPI_CLK_FREQ_MIN>( FT800Display::FT_RAM_DL, FT_DLCODE_BOOTUP, sizeof( FT_DLCODE_BOOTUP ) );
			// perform first swap command
			Write<FT800Display::FT_SPI_CLK_FREQ_MIN>( FT800Display::REG_DLSWAP, FT800Display::FT_DLSWAP_FRAME );
			// make the cmd read write pointers to 0

//			Serial.println( "TEST" );
			FCmdFifoWp = 0;
			FFreeSpace = FT800Display::FT_CMDFIFO_SIZE - 4;	
		}

/*
		void ChangeClock(uint32_t ClockValue)
		{
			/ For due it is straight forward, for others only a switch
	#if defined(__SAM3X8E__)
			SPI.setClockDivider((uint32_t)(84*1000000/ClockValue));//hardcoding for due board
	#else	
			// Other Arduino boards
			if(ClockValue < 1000000)
				SPI.setClockDivider(SPI_CLOCK_DIV32);//for galileo this value should be fine
			else
				SPI.setClockDivider(SPI_CLOCK_DIV2);//for galileo this value should be fine
	#endif			
		}
*/
		template <uint32_t C_SPEED> void HostCommand(uint32_t HostCommand)
		{
			// construct host command and send to graphics controller
			uint32_t Addr = HostCommand << 16;
			Read<C_SPEED>(Addr);//ideally sending 3 bytes is sufficient
			delay(20);//worst scenario
		}    

		void DisplayConfigExternalClock(void)
		{
			T_DISPLAY_TYPE::ApplyInit( this );
		}

		//assert CSpin and send write command
		void StartTransferCmd()
		{
			//start write transaction
			StartWrite<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::FT_RAM_CMD + FCmdFifoWp);
			FTrnsFlag() = true;
		}    

		void EndTransferCmd(void)
		{	
			EndTransfer();
			//update the write pointer of fifo
			Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_CMD_WRITE, FCmdFifoWp );
			FTrnsFlag() = false;
		}    

		void WriteCmd(uint32_t Cmd)
		{
		#if 0	
			if( ! FTrnsFlag() )
				StartTransferCmd();

			TransferCmd(Cmd);
			if( ! FTrnsFlag() )
				EndTransferCmd();
		#else
			if( ! FTrnsFlag() )
			{
				StartTransferCmd();
				TransferCmd(Cmd);
				EndTransferCmd();
			}
			else
				TransferCmd(Cmd);

		#endif
		}

		void WriteCmd(uint8_t *Src,uint32_t NBytes)
		{
			if( ! FTrnsFlag() )
			{
				StartTransferCmd();
				TransferCmd( Src, NBytes );
				EndTransferCmd();//here transflag is made to 0
			}
			else
				TransferCmd(Src,NBytes);
		
		}   

		FT800Display::FT_GEStatus ChkGetFreeSpace(uint16_t NBytes)
		{	
			//return busy if no space
			if( FFreeSpace < NBytes )
			{
				if( FTrnsFlag() )
				{
					EndTransferCmd();
					FTrnsFlag() = true;//coz EndTransferCmd will make it 0
				}

				//update the write pointer
				Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_CMD_WRITE, FCmdFifoWp );
				while( FFreeSpace < NBytes )
				{
					uint16_t rdptr = Read16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_CMD_READ );
					if(rdptr == FT800Display::FT_COPRO_ERROR)
						return FT800Display::FT_GE_ERROR;

					//update the freespace by reading the register
					FFreeSpace = (( FCmdFifoWp - rdptr)&0xffc);
					FFreeSpace = ( FT800Display::FT_CMDFIFO_SIZE - 4) - FFreeSpace;
				}		

				if( FTrnsFlag() )
					StartTransferCmd();

			}

			return FT800Display::FT_GE_OK;
		}

		void UpdateFreeSpace()
		{	
			if( FTrnsFlag() )
			{
				EndTransferCmd();
				//update the write pointer
				Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_CMD_WRITE, FCmdFifoWp );
				StartTransferCmd();
			}
			else
				//update the write pointer
				Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_CMD_WRITE, FCmdFifoWp );

		}    

		bool TransferCmd(uint32_t Cmd)
		{
			if(FFreeSpace <4)
				//blocking call till freespace is available
				if( FT800Display::FT_GE_ERROR == ChkGetFreeSpace(4))
					return false;

			Transfer32(Cmd);
			FCmdFifoWp = (FCmdFifoWp + 4)&0xfff;
			FFreeSpace -= 4;
	
			return true;
		}    

		bool TransferCmd(uint8_t *Src,uint32_t NBytes)
		{
			uint32_t i,Count;
			//align the NBytes to multiple of 4
			NBytes = (NBytes + 3)&(~3);
			//transfer the whole buffer into command buffer
			while(NBytes)
			{
				Count = NBytes;
				if(Count > FFreeSpace)
				{
					//first update the free space
					UpdateFreeSpace();
					//then transfer the data
					Count = min(FFreeSpace,Count);
					for(i = 0;i<Count;i++)	
						Transfer(*Src++);

					FCmdFifoWp = (FCmdFifoWp + Count)&0xfff;
					FFreeSpace -= Count;
					//get the free space
					NBytes -= Count;
					Count = min( NBytes, FT800Display::FT_CMDFIFO_SIZE / 2 );//atleast wait for half the buffer completion
					if( FT800Display::FT_GE_ERROR == ChkGetFreeSpace(Count))
						return false;

				}

				else
				{
					//transfer of data to command buffer
					for(i = 0;i<Count;i++)	
						Transfer(*Src++);

					FCmdFifoWp = (FCmdFifoWp + Count)&0xfff;
					FFreeSpace -= Count;
					NBytes -= Count;
				}		
			}
			return true;
		}    

	public:
		// APIs related to graphics engine
		inline void AlphaFunc(uint8_t Func, uint8_t Ref) 
		{
			WriteCmd((9UL << 24) | ((Func & 7L) << 8) | ((Ref & 0xFFL) << 0));
		}

		inline void Begin(uint8_t Prim) 
		{
		  WriteCmd(( 31UL << 24) | Prim );
		}

		inline void BitmapHandle(uint8_t Handle)
		{
		  WriteCmd((5UL << 24) | Handle );
		}

		void BitmapLayout(uint8_t Format, uint16_t Linestride, uint16_t Height) 
		{
		  // WriteCmd((7UL << 24) | ((format & 0x1FL) << 19) | ((linestride & 0x3FFL) << 9) | ((height & 0x1FFL) << 0));

			TAsBytes<uint32_t> AValue;

			AValue[0] = Height;
			AValue[1] = (1 & (Height >> 8)) | (Linestride << 1);
			AValue[2] = (7 & (Linestride >> 7)) | (Format << 3);
			AValue[3] = 7;

			WriteCmd( AValue );
		}

		void BitmapSize(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height) 
		{
			uint8_t fxy = (filter << 2) | (wrapx << 1) | (wrapy);
		  // WriteCmd((8UL << 24) | ((uint32_t)fxy << 18) | ((width & 0x1FFL) << 9) | ((height & 0x1FFL) << 0));

			TAsBytes<uint32_t> AValue;

			AValue[0] = height;
			AValue[1] = (1 & (height >> 8)) | (width << 1);
			AValue[2] = (3 & (width >> 7)) | (fxy << 2);
			AValue[3] = 8;
			WriteCmd( AValue );
		}

		inline void BitmapSource(uint32_t Addr) 
		{
			WriteCmd((1UL << 24) | ((Addr & 0xFFFFFL) << 0));
		}

		inline void BitmapTransformA(int32_t A) 
		{
			WriteCmd((21UL << 24) | ((A & 0x1FFFFL) << 0));
		}

		inline void BitmapTransformB(int32_t B) 
		{
			WriteCmd((22UL << 24) | ((B & 0x1FFFFL) << 0));
		}

		inline void BitmapTransformC(int32_t C) 
		{
			WriteCmd((23UL << 24) | ((C & 0xFFFFFFL) << 0));
		}

		inline void BitmapTransformD(int32_t D) 
		{
			WriteCmd((24UL << 24) | ((D & 0x1FFFFL) << 0));
		}

		inline void BitmapTransformE(int32_t E) 
		{
			WriteCmd((25UL << 24) | ((E & 0x1FFFFL) << 0));
		}

		inline void BitmapTransformF(int32_t F) 
		{
			WriteCmd((26UL << 24) | ((F & 0xFFFFFFL) << 0));
		}

		inline void BlendFunc(uint8_t Src, uint8_t Dst) 
		{
			WriteCmd((11UL << 24) | ((Src & 7L) << 3) | ((Dst & 7L) << 0));
		}

		inline void Call(uint16_t Dest) 
		{
			WriteCmd((29UL << 24) | ((Dest & 0xFFFFL) << 0));
		}

		inline void Cell(uint8_t Cell) 
		{
			WriteCmd((6UL << 24) | ((Cell & 0x7FL) << 0));
		}

		inline void ClearColorA(uint8_t Alpha) 
		{
			WriteCmd((15UL << 24) | ((Alpha & 0xFFL) << 0));
		}

		inline void ClearColorRGB(uint8_t red, uint8_t green, uint8_t blue) 
		{
			WriteCmd((2UL << 24) | ((red & 0xFFL) << 16) | ((green & 0xFFL) << 8) | ((blue & 0xFFL) << 0));
		}

		inline void ClearColorRGB(uint32_t rgb) 
		{
			WriteCmd((2UL << 24) | (rgb & 0xFFFFFFL));
		}

		inline void Clear( bool c, bool s, bool t) 
		{
			uint8_t m = ( c ? 0b100 : 0 ) |
						( s ? 0b010 : 0 ) |
						( t ? 0b001 : 0 );
//			uint8_t m = (c << 2) | (s << 1) | t;
			WriteCmd((38UL << 24) | m);
		}

/*
		inline void Clear(void) 
		{
			WriteCmd((38UL << 24) | 7);
		}
*/
		inline void ClearStencil(uint8_t s) 
		{
			WriteCmd((17UL << 24) | ((s & 0xFFL) << 0));
		}

		inline void ClearTag(uint8_t s) 
		{
			WriteCmd((18UL << 24) | ((s & 0xFFL) << 0));
		}

		inline void ColorA(uint8_t Alpha) 
		{
			WriteCmd((16UL << 24) | ((Alpha & 0xFFL) << 0));
		}

		inline void ColorMask(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
		{
			WriteCmd((32UL << 24) | ((r & 1L) << 3) | ((g & 1L) << 2) | ((b & 1L) << 1) | ((a & 1L) << 0));
		}

		void ColorRGB(uint8_t red, uint8_t green, uint8_t blue) 
		{
			// WriteCmd((4UL << 24) | ((red & 0xFFL) << 16) | ((green & 0xFFL) << 8) | ((blue & 0xFFL) << 0)) );
			TAsBytes<uint32_t> AValue;

			AValue[0] = blue;
			AValue[1] = green;
			AValue[2] = red;
			AValue[3] = 4;
			WriteCmd( AValue );
		}

		inline void Display(void) 
		{
			WriteCmd( 0UL << 24 );
		}

		inline void End(void) 
		{
			WriteCmd( 33UL << 24 );
		}

		inline void Jump( uint16_t Dest )
		{
			WriteCmd( (30UL << 24) | ((Dest & 0x7FFL) << 0) );
		}

		inline void LineWidth(uint16_t Width) 
		{
			WriteCmd((14UL << 24) | ((Width & 0xFFFL) << 0));
		}

		inline void Macro(uint8_t m) 
		{
			WriteCmd((37UL << 24) | ((m & 1L) << 0));
		}

		inline void PointSize(uint16_t Size) 
		{
			WriteCmd((13UL << 24) | ((Size & 0x1FFFL) << 0));
		}

		inline void RestoreContext(void) 
		{
			WriteCmd( 35UL << 24 );
		}

		inline void Return(void) 
		{
			WriteCmd( 36UL << 24 );
		}

		inline void SaveContext(void) 
		{
			WriteCmd( 34UL << 24 );
		}

		inline void ScissorSize(uint16_t Width, uint16_t Height) 
		{
			WriteCmd((28UL << 24) | ((Width & 0x3FFL) << 10) | ((Height & 0x3FFL) << 0));
		}

		inline void ScissorXY(uint16_t x, uint16_t y)
		{
			WriteCmd((27UL << 24) | ((x & 0x1FFL) << 9) | ((y & 0x1FFL) << 0));
		}

		inline void StencilFunc(uint8_t Func, uint8_t Ref, uint8_t Mask) 
		{
			WriteCmd((10UL << 24) | ((Func & 7L) << 16) | ((Ref & 0xFFL) << 8) | ((Mask & 0xFFL) << 0));
		}

		inline void StencilMask(uint8_t Mask) 
		{
			WriteCmd((19UL << 24) | ((Mask & 0xFFL) << 0));
		}

		inline void StencilOp(uint8_t Sfail, uint8_t Spass) 
		{
			WriteCmd((12UL << 24) | ((Sfail & 7L) << 3) | ((Spass & 7L) << 0));
		}   

		inline void TagMask(uint8_t Mask) 
		{
			WriteCmd((20UL << 24) | ((Mask & 1L) << 0));
		}

		inline void Tag(uint8_t s) 
		{
			WriteCmd((3UL << 24) | ((s & 0xFFL) << 0));
		}

		inline void Vertex2f( int16_t x, int16_t y ) 
		{
			// x = int(16 * x);
			// y = int(16 * y);
			WriteCmd((1UL << 30) | ((x & 0x7FFFL) << 15) | ((y & 0x7FFFL) << 0));
		}

		void Vertex2ii(uint16_t x, uint16_t y, uint8_t Handle, uint8_t Cell) 
		{
			//return (WriteCmd((2UL << 30) | ((x & 0x1FFL) << 21) | ((y & 0x1FFL) << 12) | ((handle & 0x1FL) << 7) | ((cell & 0x7FL) << 0)) ));
			/* Generates invalid graphics instruction due to shift operation of -ve coefficient */
			x &= 0x1FFL;//error handling of -ve coefficients
			y &= 0x1FFL;

			TAsBytes<uint32_t> AValue;

			AValue[0] = Cell | ((Handle & 1) << 7);
			AValue[1] = (Handle >> 1) | (y << 4);
			AValue[2] = (y >> 4) | (x << 5);
			AValue[3] = (2 << 6) | (x >> 3);

			WriteCmd( AValue );
		}

	public:
		// graphics helper apis
		inline void ColorRGB(uint32_t rgb) 
		{
			WriteCmd((4UL << 24) | (rgb & 0xFFFFFFL));
		}

		void ColorARGB(uint32_t argb) 
		{
			WriteCmd((4UL << 24) | (argb & 0xFFFFFFL));
			WriteCmd((16UL << 24) | ((argb>>24) & 0xFFL) );
  		}

	public:
		FT800Display::FT_GEStatus CheckLogo(void) //special api to check logo completion
		{
			uint16_t ReadCmdPtr = Read16(FT800Display::REG_CMD_READ) ;
			if((ReadCmdPtr == Read16(FT800Display::REG_CMD_WRITE)) && (ReadCmdPtr == 0))
			{
				FCmdFifoWp = 0;
				return FT800Display::FT_GE_FINISHED;
			}

			else if( FT800Display::FT_COPRO_ERROR == ReadCmdPtr )
				return FT800Display::FT_GE_ERROR;

			else
				return FT800Display::FT_GE_BUSY;
	
		}

	public:
		//apis to render all the commands to hardware
		void Flush(void)
		{
			if( FTrnsFlag() )
				EndTransferCmd();

			Write16( FT800Display::REG_CMD_WRITE, FCmdFifoWp );
		}    

		//flushes out all the commands to FT_GC and waits for the completion of execution
		void Finish(void)
		{
	
			if( FTrnsFlag() )
				EndTransferCmd();

			Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_CMD_WRITE, FCmdFifoWp );
	
			uint16_t ReadPrt;
			while((ReadPrt = Read16<FT800Display::FT_SPI_CLK_FREQ_MAX>(FT800Display::REG_CMD_READ)) != FCmdFifoWp )
				if( FT800Display::FT_COPRO_ERROR == ReadPrt )
					return;
			
		}

		//checks fifo and returns the status	
		FT800Display::FT_GEStatus CheckFinish(void)
		{
			uint16_t ReadPrt = Read16<FT800Display::FT_SPI_CLK_FREQ_MAX>(FT800Display::REG_CMD_READ);
	

			if ( FT800Display::FT_COPRO_ERROR == ReadPrt)
				return FT800Display::FT_GE_ERROR;

			if(ReadPrt != FCmdFifoWp)
				return FT800Display::FT_GE_BUSY; //success case return finished

			return FT800Display::FT_GE_FINISHED;
		}

		//error from graphics controller library
		uint32_t GetError(void)
		{
			if( FT800Display::FT_COPRO_ERROR == Read16<FT800Display::FT_SPI_CLK_FREQ_MAX>(FT800Display::REG_CMD_READ))
				return FT800Display::FT_GE_ERROR;
			
			return FT800Display::FT_GE_OK;	
		}

	public:
		bool IsReady()
		{
			return ( CheckFinish() == FT800Display::FT_GE_FINISHED );
		}

	public:
		// APIs related to coprocessor commands, widgets etc
		inline void Cmd_Logo(void)
		{
			WriteCmd( FT800Display::CMD_LOGO );
		}    

		void Cmd_Append(uint32_t Ptr, uint32_t Num)
		{
			WriteCmd( FT800Display::CMD_APPEND );
			WriteCmd(Ptr);
			WriteCmd(Num);
		}    

		void Cmd_BGColor(uint32_t c)
		{
			WriteCmd( FT800Display::CMD_BGCOLOR );
			WriteCmd(c);
		}    

		// make sure the string is in ram instead of program memory. it is assumed that *s is valid string and if null then it should contain \0
		void Cmd_Button(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t Font, uint16_t Options, const char *s)
		{
			WriteCmd( FT800Display::CMD_BUTTON );
			WriteCmd(((y & 0xFFFFL) <<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
			WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);//make sure last byte is added into the
		}    

		// Check the result of command calibrate by cmd_GetResult
		void Cmd_Calibrate(uint32_t Result)
		{
			WriteCmd( FT800Display::CMD_CALIBRATE );
			WriteCmd(Result);//write extra word for result
		}    

		void Cmd_Clock(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms)
		{
			WriteCmd( FT800Display::CMD_CLOCK );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (r & 0xFFFFL));
			WriteCmd(((m & 0xFFFFL)<<16) | (h & 0xFFFFL));
			WriteCmd(((ms & 0xFFFFL)<<16) | (s & 0xFFFFL));
		}

		inline void Cmd_ColdStart(void)
		{
			//should we even change command read and write pointers
			WriteCmd( FT800Display::CMD_COLDSTART );
		}

		void Cmd_Dial(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t Val)
		{
			WriteCmd( FT800Display::CMD_DIAL );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (r & 0xFFFFL));
			WriteCmd(Val);
		}    

		inline void Cmd_DLStart(void)
		{
			WriteCmd( FT800Display::CMD_DLSTART );
		}    

		void Cmd_FGColor(uint32_t c)
		{
			WriteCmd( FT800Display::CMD_FGCOLOR );
			WriteCmd(c);
		}    

		void Cmd_Gauge(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t Major, uint16_t Minor, uint16_t Val, uint16_t Range)
		{
			WriteCmd( FT800Display::CMD_GAUGE );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (r & 0xFFFFL));
			WriteCmd(((Minor & 0xFFFFL)<<16) | (Major & 0xFFFFL));
			WriteCmd(((Range & 0xFFFFL)<<16) | (Val & 0xFFFFL));
		}  

		// Results are available from getresults api
		void Cmd_GetMatrix(void)
		{
			WriteCmd( FT800Display::CMD_GETMATRIX );
			WriteCmd(0);
			WriteCmd(0);
			WriteCmd(0);
			WriteCmd(0);
			WriteCmd(0);
			WriteCmd(0);
		}    

		// perform this api and wait for the completion by finish and use getresults api  for the result
		void Cmd_GetProps(uint32_t &Ptr, uint32_t &w, uint32_t &h)
		{
			WriteCmd( FT800Display::CMD_GETPROPS );
			WriteCmd(0);
			WriteCmd(0);
			WriteCmd(0);
		}    

		void Cmd_GetPtr(uint32_t Result)
		{
			WriteCmd( FT800Display::CMD_GETPTR );
			WriteCmd(Result);
		}

		void Cmd_GradColor(uint32_t c)
		{
			WriteCmd( FT800Display::CMD_GRADCOLOR );
			WriteCmd(c);
		}

		void Cmd_Gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1)
		{
			WriteCmd( FT800Display::CMD_GRADIENT );
			WriteCmd(((y0 & 0xFFFFL)<<16)|(x0 & 0xFFFFL));
			WriteCmd(rgb0);
			WriteCmd(((y1 & 0xFFFFL)<<16)|(x1 & 0xFFFFL));
			WriteCmd(rgb1);
		}    

		void Cmd_Inflate(uint32_t Ptr)
		{
			WriteCmd( FT800Display::CMD_INFLATE );
			WriteCmd(Ptr);
		}    

		void Cmd_Interrupt(uint32_t ms)
		{
			WriteCmd( FT800Display::CMD_INTERRUPT );
			WriteCmd(ms);
		}    

		void Cmd_Keys(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t Font, uint16_t Options, const char *s)
		{
			WriteCmd( FT800Display::CMD_KEYS );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
			WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);	
		}

		inline void Cmd_LoadIdentity(void)
		{
			WriteCmd( FT800Display::CMD_LOADIDENTITY );
		}

		void Cmd_LoadImage(uint32_t Ptr, int32_t Options)
		{
			WriteCmd( FT800Display::CMD_LOADIMAGE );
			WriteCmd(Ptr);
			WriteCmd(Options);
		}

		void Cmd_Memcpy(uint32_t Dest, uint32_t Src, uint32_t Num)
		{
			WriteCmd( FT800Display::CMD_MEMCPY );
			WriteCmd(Dest);
			WriteCmd(Src);
			WriteCmd(Num);
		}

		void Cmd_Memset(uint32_t Ptr, uint8_t Value, uint32_t Num)
		{
			WriteCmd( FT800Display::CMD_MEMSET );
			WriteCmd(Ptr);
			WriteCmd(Value);	
			WriteCmd(Num);
		}

		void Cmd_Memcrc(uint32_t Ptr, uint32_t Num,uint32_t &Result)
		{
			WriteCmd( FT800Display::CMD_MEMCRC );
			WriteCmd(Ptr);
			WriteCmd(Num);
			WriteCmd(Result);
		}

		void Cmd_Memwrite(uint32_t Ptr, uint32_t Num)
		{
			WriteCmd( FT800Display::CMD_MEMWRITE );
			WriteCmd(Ptr);
			WriteCmd(Num);
		}

		void Cmd_Memzero(uint32_t Ptr, uint32_t Num)
		{
			WriteCmd( FT800Display::CMD_MEMZERO );
			WriteCmd(Ptr);
			WriteCmd(Num);
		}

		void Cmd_Number(int16_t x, int16_t y, uint8_t Font, uint16_t Options, uint32_t n)
		{
			WriteCmd( FT800Display::CMD_NUMBER );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
			WriteCmd(n);
		}

		void Cmd_Progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t Options, uint16_t Val, uint16_t Range)
		{
			WriteCmd( FT800Display::CMD_PROGRESS );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(((Val & 0xFFFFL)<<16) | (Options & 0xFFFFL));
			WriteCmd(Range);
		}

		void Cmd_RegRead(uint32_t Ptr,uint32_t Result)
		{
			WriteCmd( FT800Display::CMD_REGREAD );
			WriteCmd(Ptr);
			WriteCmd(Result);
		}

		void Cmd_Rotate(int32_t a)
		{
			WriteCmd( FT800Display::CMD_ROTATE );
			WriteCmd(a);
		}

		void Cmd_Scale(int32_t sx, int32_t sy)
		{
			WriteCmd( FT800Display::CMD_SCALE );
			WriteCmd(sx);
			WriteCmd(sy);
		}

		inline void Cmd_ScreenSaver(void)
		{
			WriteCmd( FT800Display::CMD_SCREENSAVER );
		}    

		void Cmd_Scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t Options, uint16_t Val, uint16_t Size, uint16_t Range)
		{
			WriteCmd( FT800Display::CMD_SCROLLBAR );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(((Val & 0xFFFFL)<<16) | (Options & 0xFFFFL));
			WriteCmd(((Range & 0xFFFFL)<<16) | (Size & 0xFFFFL));
		}

		void Cmd_SetFont(uint8_t Font, uint32_t Ptr)
		{
			WriteCmd( FT800Display::CMD_SETFONT );
			WriteCmd(Font);
			WriteCmd(Ptr);
		}

		inline void Cmd_SetMatrix(void)
		{
			WriteCmd( FT800Display::CMD_SETMATRIX );	
		}    

		void Cmd_Sketch(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t Ptr, uint16_t Format)
		{
			WriteCmd( FT800Display::CMD_SKETCH );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(Ptr);
			WriteCmd(Format);
		}

		void Cmd_Slider(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t Options, uint16_t Val, uint16_t Range)
		{
			WriteCmd( FT800Display::CMD_SLIDER );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(((Val & 0xFFFFL)<<16) | (Options & 0xFFFFL));
			WriteCmd(Range);
		}

		void Cmd_Snapshot(uint32_t OutputAddr)
		{
			WriteCmd( FT800Display::CMD_SNAPSHOT );
			WriteCmd(OutputAddr);
		}    

		void Cmd_Spinner(int16_t x, int16_t y, uint8_t Style, uint8_t Scale)
		{
			WriteCmd( FT800Display::CMD_SPINNER );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Scale & 0xFFFFL)<<16) | (Style & 0xFFFFL));
		}

		inline void Cmd_Stop(void)
		{
			WriteCmd( FT800Display::CMD_STOP );
		}

		inline void Cmd_Swap(void)
		{
			WriteCmd( FT800Display::CMD_SWAP );
		}    

		void Cmd_Text(int16_t x, int16_t y, uint8_t Font, uint16_t Options, const char *s)
		{
			WriteCmd( FT800Display::CMD_TEXT );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
			WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);
		}

		void Cmd_Toggle(int16_t x, int16_t y, int16_t w, uint8_t Font, uint16_t Options, uint16_t State, const char *s)
		{
			WriteCmd( FT800Display::CMD_TOGGLE );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((Font & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(((State & 0xFFFFL)<<16) | (Options & 0xFFFFL));
			WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);
		}    

		void Cmd_Track(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t Tag)
		{
			WriteCmd( FT800Display::CMD_TRACK );
			WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
			WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
			WriteCmd(Tag);
		}

		void Cmd_Translate(int32_t tx, int32_t ty)
		{
			WriteCmd( FT800Display::CMD_TRANSLATE );
			WriteCmd(tx);
			WriteCmd(ty);
		}    

	public:
		// Apis related to audio engine
		void PlaySound(uint8_t Volume,uint16_t SoundNote)
		{
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_VOL_SOUND, Volume );//change the volume of synthesized sound, 0 means off, 255 means max on
			Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_SOUND, SoundNote );
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_PLAY, FT800Display::FT_SOUND_PLAY );
		}

		//higher byte is the note and lower byte is the sound
		void PlaySound(uint16_t SoundNote)
		{
			Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_SOUND, SoundNote );
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_PLAY, FT800Display::FT_SOUND_PLAY );
		}

		//volume will not be modified
		void StopSound(void)
		{
			Write16<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_SOUND, FT800Display::FT_SILENCE );//configure silence
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_PLAY, FT800Display::FT_SOUND_PLAY );//play the silence
		}

	public:
		// Apis related to touch engine
		//one of 0ff/oneshot/frame/continuous. default being continuous
		inline void SetTouchMode( uint8_t TMode )
		{
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_MODE, TMode );
		}

		//api to set coordinates for host specific tag query
		void SetHostTagXY(uint16_t xoffset,uint16_t yoffset)
		{
			uint8_t A[6];
			//little endian specific
			A[0] = xoffset & 0xFF;
			A[1] = xoffset >> 8;
			A[4] = yoffset & 0xFF;
			A[5] = yoffset >> 8;
			Write<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TAG_X, A, 6 );
		}

		//api to get TAG from FT_GC for coordinates set by  SetHostTagXY() api - host needs to wait for at least 1 frame to get these query values
		inline uint8_t GetHostTagXY(void)
		{
			return Read<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TAG );
		}

		//get the touched object tag and repective xy coordinates
		inline void GetTagXY( FT800Display::sTagXY &sTagxy)
		{
			Read<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TAG_XY, (uint8_t *)&sTagxy, 6 );
		}

		//get the track value and the tag value
		inline void GetTrackTag( FT800Display::sTrackTag &sTracktag )
		{
			uint32_t *ptr = (uint32_t *)&sTracktag;
			*ptr = Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TRACKER );
		}

	public:
		inline void SystemInit(void)
		{
			T_PowerDownOutputPin::SetPinValueHigh();

			// handle due, yue, galileo and other hardware
/*
#if defined(__SAM3X8E__)
			C_SPI.setClockDivider(4);	//hardcoding for due board
#else	
			// Other Arduino boards
			C_SPI.setClockDivider(SPI_CLOCK_DIV2);	//for galileo this value should be fine
#endif	
*/

//			C_SPI.setBitOrder(MSBFIRST);
//			C_SPI.setDataMode(SPI_MODE0);

			// Bootup of graphics controller
			Reset();

			// Set the display configurations followed by external clock set, spi clock change wrt FT80x
			DisplayConfigExternalClock();
	
			if( ExternalClock() )
				HostCommand<FT800Display::FT_SPI_CLK_FREQ_MIN>( FT800Display::FT_CLKEXT );	// Set to use external clock

			else				
				HostCommand<FT800Display::FT_SPI_CLK_FREQ_MIN>( FT800Display::FT_CLKINT ); // Set to use internal clock source
	
	
			// change the clock to maximum SPI operating frequency
//			ChangeClock( FT800Display::FT_SPI_CLK_FREQ_MAX);	//change the clock to normal operating frequency - harcoded wrt due	

			delay(20);	//for safer side

			T_ChipSelectOutputPin::SetPinValueHigh();
//			uint32_t chipid = Read32( FT800Display::FT_ROM_CHIPID );
//			Serial.println( chipid, HEX );

			DisplayOn( true );
			AudioOn( true );

		}

		inline void SystemStart()
		{
			if( ! FModified() )
				return;

			if( ! IsReady() )
				return;

			FModified() = false; // Set before calling the chain!
			uint32_t ATrackRegisterVal = Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TRACKER );
			FT800Display::sTagXY sTagxy;
			GetTagXY( sTagxy );
			FLastTag = sTagxy.tag;
			T_Elements_Render::Call( ATrackRegisterVal, &sTagxy, true );
			T_Elements_Render::Call( 0, nullptr, true );

		}

		inline void SystemLoopBegin()
		{
//			uint32_t ATest = Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_F );
//			Serial.println( ATest, HEX );
			uint32_t ATrackRegisterVal = Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TRACKER );
			FT800Display::sTagXY sTagxy;
			GetTagXY( sTagxy );
			if( ( ATrackRegisterVal & 0xFF ) && ( ( ATrackRegisterVal & 0xFF ) != 0xFF ) ||
				( FLastTag != sTagxy.tag )
				)
			{
				FLastTag = sTagxy.tag;
				T_Elements_Render::Call( ATrackRegisterVal, &sTagxy, false );
			}

			T_BusyOutputPin::SetPinValue( ! IsReady() );
		}

		inline void SystemLoopEnd()
		{
			SystemStart();
		}

	public:
		inline DisplayFT800()
		{
			ExternalClock() = true;
			FTrnsFlag() = false;
			FModified() = true;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled
	> class DisplayFT800AnimateLogo :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( Enabled() )
				if( ! ATagXY )
					C_OWNER.Cmd_Logo();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.IsReady() )
				return;

			Render( false, 0, nullptr, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AOutputPin,
		typename T_BOutputPin,
		typename T_COutputPin,
		typename T_DOutputPin,
		typename T_EOutputPin,
		typename T_Enabled,
		typename T_FOutputPin
	> class DisplayFT800GetTouchTransform :
		public T_AOutputPin,
		public T_BOutputPin,
		public T_COutputPin,
		public T_DOutputPin,
		public T_EOutputPin,
		public T_Enabled,
		public T_FOutputPin
	{
	public:
		_V_PIN_( AOutputPin )
		_V_PIN_( BOutputPin )
		_V_PIN_( COutputPin )
		_V_PIN_( DOutputPin )
		_V_PIN_( EOutputPin )
		_V_PIN_( FOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( Enabled() )
				if( ! ATagXY )
				{
					T_AOutputPin::SetPinValue( C_OWNER.template Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_A ) );
					T_BOutputPin::SetPinValue( C_OWNER.template Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_B ) );
					T_COutputPin::SetPinValue( C_OWNER.template Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_C ) );
					T_DOutputPin::SetPinValue( C_OWNER.template Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_D ) );
					T_EOutputPin::SetPinValue( C_OWNER.template Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_E ) );
					T_FOutputPin::SetPinValue( C_OWNER.template Read32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_F ) );
				}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.IsReady() )
				return;

			Render( false, 0, nullptr, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_A,
		typename T_B,
		typename T_C,
		typename T_D,
		typename T_E,
		typename T_Enabled,
		typename T_F
	> class DisplayFT800SetTouchTransform :
		public T_A,
		public T_B,
		public T_C,
		public T_D,
		public T_E,
		public T_Enabled,
		public T_F
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( A )
		_V_PROP_( B )
		_V_PROP_( C )
		_V_PROP_( D )
		_V_PROP_( E )
		_V_PROP_( F )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( Enabled() )
				if( ! ATagXY )
				{
					C_OWNER.template Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_A, A() );
					C_OWNER.template Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_B, B() );
					C_OWNER.template Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_C, C() );
					C_OWNER.template Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_D, D() );
					C_OWNER.template Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_E, E() );
					C_OWNER.template Write32<FT800Display::FT_SPI_CLK_FREQ_MAX>( FT800Display::REG_TOUCH_TRANSFORM_F, F() );
				}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.IsReady() )
				return;

			Render( false, 0, nullptr, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled
	> class DisplayFT800StopAnimation :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( Enabled() )
				C_OWNER.Cmd_Stop();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.IsReady() )
				return;

			Render( false, 0, nullptr, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_Render,
		typename T_Enabled
	> class DisplayFT800Scene :
		public T_Enabled
	{
	public:
		typedef typename T_OWNER::T_DISPLAY	T_DISPLAY;

	public:
		_V_PROP_( Enabled )

//	public:
//		void ( *Elements_CallChain )( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY ) = nullptr;

	public:
		inline typename T_OWNER::T_DISPLAY &GetDisplay()
		{
			return C_OWNER.GetDisplay();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ! ATagXY )
				C_OWNER.GetDisplay().Cmd_DLStart();

			T_Elements_Render::Call( ATagValue, ATagXY, AFromStart );

			if( ! ATagXY )
			{
				C_OWNER.GetDisplay().Display();
				C_OWNER.GetDisplay().Cmd_Swap();
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_Render,
		typename T_Enabled
	> class DisplayFT800SubScene :
		public T_Enabled
	{
	public:
		typedef typename T_OWNER::T_DISPLAY	T_DISPLAY;

	public:
		_V_PROP_( Enabled )

	public:
//		void ( *Elements_CallChain )( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY ) = nullptr;

	public:
		inline typename T_OWNER::T_DISPLAY &GetDisplay()
		{
			return C_OWNER.GetDisplay();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			T_Elements_Render::Call( ATagValue, ATagXY, AFromStart );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_Render,
		typename T_Enabled
	> class DisplayFT800SaveContext :
		public T_Elements_Render,
		public T_Enabled
	{
	public:
		typedef typename T_OWNER::T_DISPLAY	T_DISPLAY;

	public:
		_V_PROP_( Enabled )

//	public:
//		void ( *Elements_CallChain )( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY ) = nullptr;

	public:
		inline typename T_OWNER::T_DISPLAY &GetDisplay()
		{
			return C_OWNER.GetDisplay();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			C_OWNER.GetDisplay().SaveContext();
			T_Elements_Render::Call( ATagValue, ATagXY, AFromStart );
			C_OWNER.GetDisplay().RestoreContext();

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Major,
		typename T_Minor
	> class TArduinoDisplayFT800SceneElementGaugeTicks :
		public T_Enabled,
		public T_Major,
		public T_Minor
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Minor )
		_V_PROP_( Major )
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_HasBackground,
		typename T_HasHand,
		typename T_In3D,
		typename T_InitialValue,
		typename T_Radius,
		typename T_Ticks,
		typename T_X,
		typename T_Y
	> class DisplayFT800Gauge :
		public T_Enabled,
		public T_HasBackground,
		public T_HasHand,
		public T_In3D,
		public T_InitialValue,
		public T_Radius,
		public T_Ticks,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Radius )
		_V_PROP_( InitialValue )
		_V_PROP_( In3D )
		_V_PROP_( HasHand )
		_V_PROP_( HasBackground )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( Ticks )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
			{
				return;
			}

//			Serial.println( "Render" );
//			Serial.println( InitialValue );
			uint16_t Options = 0;
			if( ! In3D() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( ! HasHand() )
				Options |= FT800Display::FT_OPT_NOPOINTER;

			if( ! HasBackground() )
				Options |= FT800Display::FT_OPT_NOBACK;
			
			if( ! Ticks().Enabled() )
				Options |= FT800Display::FT_OPT_NOTICKS;

			C_OWNER.GetDisplay().Cmd_Gauge( X(), Y(), Radius(), Options, Ticks().Major(), Ticks().Minor(), InitialValue().GetValue() * 0xFFFF, 0xFFFF );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue == InitialValue().GetValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.GetDisplay().Modified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_HasBackground,
		typename T_In3D,
		typename T_OutputPin,
		typename T_Radius,
		typename T_Tag,
		typename T_Track,
		typename T_Value,
		typename T_X,
		typename T_Y
	> class DisplayFT800Dial : 
		public T_Enabled,
		public T_HasBackground,
		public T_In3D,
		public T_OutputPin,
		public T_Radius,
		public T_Tag,
		public T_Track,
		public T_Value,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Radius )
		_V_PROP_( Tag )
		_V_PROP_( Value )
		_V_PROP_( Track )
		_V_PROP_( In3D )
		_V_PROP_( HasBackground )

	public:
		_V_PROP_( Enabled )

	public:
		void SetValue( float AValue )
		{
			if( AValue == Value().GetValue() )
				return;

			Value() = AValue;
			UpdateValue();
		}

		inline void UpdateValue()
		{
			T_OutputPin::SetPinValue( Value() );
			C_OWNER.GetDisplay().Modified();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ATagXY )
			{
				if( Track().GetValue() && ( Tag().GetValue() > 0 ))
				{
					if( ( ATagValue & 0xFF ) == Tag().GetValue() )
					{
						SetValue( float( ATagValue >> 16 ) / 0xFFFF );
						return;
					}
						
					if( AFromStart )
						T_OutputPin::SetPinValue( Value().GetValue() );
				}

				return;
			}

			uint16_t Options = 0;
			if( ! In3D().GetValue() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( Tag() )
				C_OWNER.GetDisplay().Tag( Tag() );

			C_OWNER.GetDisplay().Cmd_Dial( X(), Y(), Radius(), Options, Value().GetValue() * 0xFFFF + 0.5 );
			if( Track() )
				if( Tag() )
					C_OWNER.GetDisplay().Cmd_Track( X(), Y(), 1, 1, Tag() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Height,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Crop :
		public T_Enabled,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		_V_PROP_( Enabled )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().ScissorXY( X(), Y() );
			C_OWNER.GetDisplay().ScissorSize( Width(), Height() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Height,
		typename T_In3D,
		typename T_InitialValue,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800ProgressBar :
		public T_Enabled,
		public T_Height,
		public T_In3D,
		public T_InitialValue,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( InitialValue )
		_V_PROP_( In3D )
//		_V_PROP_( HasBackground )

	public:
		_V_PROP_( Enabled )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			uint16_t Options = 0;
			if( ! In3D() )
				Options |= FT800Display::FT_OPT_FLAT;

			C_OWNER.GetDisplay().Cmd_Progress( X(), Y(), Width(), Height(), Options, InitialValue().GetValue() * 0xFFFF + 0.5, 0xFFFF );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue == InitialValue().GetValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.GetDisplay().Modified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Height,
		typename T_In3D,
		typename T_OutputPin,
		typename T_Tag,
		typename T_Track,
		typename T_Value,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Slider :
		public T_Enabled,
		public T_Height,
		public T_In3D,
		public T_OutputPin,
		public T_Tag,
		public T_Track,
		public T_Value,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( Tag )
		_V_PROP_( Value )
		_V_PROP_( Track )
		_V_PROP_( In3D )
//		_V_PROP_( HasBackground )

	public:
		_V_PROP_( Enabled )

	public:
		void SetValue( float AValue )
		{
			if( AValue == Value().GetValue() )
				return;

			Value() = AValue;
			UpdateValue();
		}

		inline void UpdateValue()
		{
			T_OutputPin::SetPinValue( Value() );
			C_OWNER.GetDisplay().Modified();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ATagXY )
			{
				if( Track().GetValue() && ( Tag().GetValue() > 0 ) )
				{
					if( ( ATagValue & 0xFF ) == Tag().GetValue() )
					{
						SetValue( float( ATagValue >> 16 ) / 0xFFFF );
						return;
					}
						
					if( AFromStart )
						T_OutputPin::SetPinValue( Value().GetValue() );
				}

				return;
			}

			uint16_t Options = 0;
			if( ! In3D().GetValue() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( Tag() )
				C_OWNER.GetDisplay().Tag( Tag() );

			C_OWNER.GetDisplay().Cmd_Slider( X(), Y(), Width(), Height(), Options, Value().GetValue() * 0xFFFF + 0.5, 0xFFFF );
			if( Track() )
				if( Tag() )
					C_OWNER.GetDisplay().Cmd_Track( X(), Y(), Width(), Height(), Tag() );

		}

/*
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			UpdatedOutput( AValue );
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Height,
		typename T_In3D,
		typename T_OutputPin,
		typename T_Size,
		typename T_Tag,
		typename T_Track,
		typename T_Value,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Scrollbar :
		public T_Enabled,
		public T_Height,
		public T_In3D,
		public T_OutputPin,
		public T_Size,
		public T_Tag,
		public T_Track,
		public T_Value,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( Size )
		_V_PROP_( Tag )
		_V_PROP_( Value )
		_V_PROP_( Track )
		_V_PROP_( In3D )
//		_V_PROP_( HasBackground )

	public:
		_V_PROP_( Enabled )

	public:
		void SetValue( float AValue )
		{
			if( AValue == Value().GetValue() )
				return;

			Value() = AValue;
			UpdateValue();
		}

		inline void UpdateValue()
		{
			T_OutputPin::SetPinValue( Value() );
			C_OWNER.GetDisplay().Modified();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ATagXY )
			{
				if( Track().GetValue() && ( Tag().GetValue() > 0 ) )
				{
					if( ( ATagValue & 0xFF ) == Tag().GetValue() )
					{
						SetValue( float( ATagValue >> 16 ) / 0xFFFF );
						return;
					}
						
					if( AFromStart )
						T_OutputPin::SetPinValue( Value() );
				}

				return;
			}

			uint16_t Options = 0;
			if( ! In3D().GetValue() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( Tag() )
				C_OWNER.GetDisplay().Tag( Tag() );

			C_OWNER.GetDisplay().Cmd_Scrollbar( X(), Y(), Width(), Height(), Options, Value().GetValue() * ( 0xFFFF - ( Size().GetValue() * 0xFFFF ) ) + 0.5, Size().GetValue() * 0xFFFF, 0xFFFF );
			if( Track() )
				if( Tag() )
					C_OWNER.GetDisplay().Cmd_Track( X(), Y(), Width(), Height(), Tag() );

		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_Enabled,
		typename T_Off,
		typename T_On
	> class TArduinoDisplayFT800SceneElementToggleText :
//		public T_Enabled,
		public T_Off,
		public T_On
	{
	public:
		_V_PROP_( On )
		_V_PROP_( Off )
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Font,
		typename T_In3D,
		typename T_OutputPin,
		typename T_Tag,
		typename T_Text,
		typename T_Track,
		typename T_Value,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Toggle :
		public T_Enabled,
		public T_Font,
		public T_In3D,
		public T_OutputPin,
		public T_Tag,
		public T_Text,
		public T_Track,
		public T_Value,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Tag )
		_V_PROP_( Value )
		_V_PROP_( Track )
		_V_PROP_( In3D )
		_V_PROP_( Font )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Text )

	public:
		void SetValue( bool AOldValue, bool AValue )
		{
			if( AValue == AOldValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( Value() );
			C_OWNER.GetDisplay().Modified();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
			{
				if( Track().GetValue() && ( Tag().GetValue() > 0 ) )
				{
					if( ( ATagValue & 0xFF ) == Tag().GetValue() )
					{
						SetValue( Value(), ( ATagValue >> 16 ) > 0x7FFF );
						return;
					}
						
					if( AFromStart )
						T_OutputPin::SetPinValue( Value() );
				}

				return;
			}

			uint16_t Options = 0;
			if( ! In3D() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( Tag() )
				C_OWNER.GetDisplay().Tag( Tag() );

			C_OWNER.GetDisplay().Cmd_Toggle( X(), Y(), Width(), Font(), Options, Value().GetValue() ? 0xFFFF : 0, ( Text().Off().GetValue() + "\xFF" + Text().On().GetValue() ).c_str() );
			if( Track() )
				if( Tag() )
				{
					uint8_t AHeight;
					if( Font() < 16 )
						AHeight = 16; // Temporary not supporting custom fonts

					else
						AHeight = FT800Display::CFontSizes[ Font() - 16 ];

					C_OWNER.GetDisplay().Cmd_Track( X(), Y(), Width(), AHeight, Tag() );
				}

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Font,
		typename T_Height,
		typename T_In3D,
		typename T_OutputPin,
		typename T_Tag,
		typename T_Text,
		typename T_Track,
		typename T_Value,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Button :
		public T_Enabled,
		public T_Font,
		public T_Height,
		public T_In3D,
		public T_OutputPin,
		public T_Tag,
		public T_Text,
		public T_Track,
		public T_Value,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Tag )
		_V_PROP_( Value )
		_V_PROP_( Track )
		_V_PROP_( In3D )
		_V_PROP_( Font )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( Text )

	public:
		void SetValue( bool AOldValue, bool AValue )
		{
//			if( ! OpenWire::GetIsStarted() )
//				if( AValue == Value().GetValue() )
//					return;

			Value() = AValue;
			T_OutputPin::SetPinValue( Value() );
			C_OWNER.GetDisplay().Modified();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
			{
				if( Track().GetValue() && ( Tag().GetValue() > 0 ) )
					SetValue( 0, ATagXY->tag == Tag().GetValue() );

				return;
			}

			uint16_t Options = 0;
			if( Value().GetValue() || ( !In3D().GetValue() ) )
				Options |= FT800Display::FT_OPT_FLAT;

			if( Tag() )
				C_OWNER.GetDisplay().Tag( Tag() );

			C_OWNER.GetDisplay().Cmd_Button( X(), Y(), Width(), Height(), Font(), Options, Text().c_str() );
			if( Track() )
				if( Tag() )
					C_OWNER.GetDisplay().Cmd_Track( X(), Y(), Width(), Height(), Tag() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Scale,
		typename T_Style,
		typename T_X,
		typename T_Y
	> class DisplayFT800Spinner :
		public T_Enabled,
		public T_Scale,
		public T_Style,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( X )
		_V_PROP_( Y )

		_V_PROP_( Style )
		_V_PROP_( Scale )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Cmd_Spinner( X(), Y(), Style(), Scale() );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_HoursMinutes,
		typename T_Seconds
	> class TArduinoDisplayFT800SceneElementClockHands :
		public T_HoursMinutes,
		public T_Seconds
	{
	public:
		_V_PROP_( Seconds )
		_V_PROP_( HoursMinutes )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Hands,
		typename T_HasBackground,
		typename T_In3D,
		typename T_InitialValue,
		typename T_Radius,
		typename T_TicksEnabled,
		typename T_X,
		typename T_Y
	> class DisplayFT800Clock :
		public T_Enabled,
		public T_Hands,
		public T_HasBackground,
		public T_In3D,
		public T_InitialValue,
		public T_Radius,
		public T_TicksEnabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( TicksEnabled )
		_V_PROP_( In3D )
		_V_PROP_( HasBackground )
		_V_PROP_( Hands )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Radius )
		_V_PROP_( InitialValue )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			uint16_t AHour;
			uint16_t AMin;
			uint16_t ASec;
			uint16_t AMSec;

			InitialValue().GetValue().DecodeTime( AHour, AMin, ASec, AMSec );

			uint16_t Options = 0;
			if( ! In3D() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( ! Hands().Seconds() )
				Options |= FT800Display::FT_OPT_NOSECS;

			if( ! Hands().HoursMinutes() )
				Options |= FT800Display::FT_OPT_NOHM;

			if( ! HasBackground() )
				Options |= FT800Display::FT_OPT_NOBACK;
			
			if( ! TicksEnabled() )
				Options |= FT800Display::FT_OPT_NOTICKS;

			C_OWNER.GetDisplay().Cmd_Clock( X(), Y(), Radius(), Options, AHour, AMin, ASec, AMSec );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDateTime AValue = *(Mitov::TDateTime*)_Data;
			if( AValue == InitialValue().GetValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.GetDisplay().Modified();
		}

	};
//---------------------------------------------------------------------------
	namespace TArduinoDisplayFT800SceneElementHorizontalAlignment
	{
		enum TArduinoDisplayFT800SceneElementHorizontalAlignment { Left, Center, Right };
	}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Font,
		typename T_HorizontalAlignment,
		typename T_InitialValue,
		typename T_VerticalCentered,
		typename T_X,
		typename T_Y
	> class DisplayFT800Text :
		public T_Enabled,
		public T_Font,
		public T_HorizontalAlignment,
		public T_InitialValue,
		public T_VerticalCentered,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Font )
		_V_PROP_( HorizontalAlignment )
		_V_PROP_( VerticalCentered )
		_V_PROP_( InitialValue )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			uint16_t Options = 0;

			if( VerticalCentered() )
				Options |= FT800Display::FT_OPT_CENTERY;

			switch( HorizontalAlignment() )
			{
				case TArduinoDisplayFT800SceneElementHorizontalAlignment::Center:
					Options |= FT800Display::FT_OPT_CENTERX;
					break;

				case TArduinoDisplayFT800SceneElementHorizontalAlignment::Right:
					Options |= FT800Display::FT_OPT_RIGHTX;
					break;

			}

			C_OWNER.GetDisplay().Cmd_Text( X(), Y(), Font(), Options, InitialValue().c_str() );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AData = (char *)_Data;
			if( InitialValue().GetValue() == AData )
				return;

			InitialValue() = AData;
			C_OWNER.GetDisplay().Modified();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_OPTIONS,
		typename T_Enabled,
		typename T_Font,
		typename T_HorizontalAlignment,
		typename T_InitialValue,
		typename T_DATA,
		typename T_VerticalCentered,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Number :
		public T_Enabled,
		public T_Font,
		public T_HorizontalAlignment,
		public T_InitialValue,
		public T_VerticalCentered,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Font )
		_V_PROP_( Width )
		_V_PROP_( HorizontalAlignment )
		_V_PROP_( VerticalCentered )
		_V_PROP_( InitialValue )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			uint16_t Options = C_OPTIONS | Width().GetValue();

			if( VerticalCentered() )
				Options |= FT800Display::FT_OPT_CENTERY;

			switch( HorizontalAlignment() )
			{
				case TArduinoDisplayFT800SceneElementHorizontalAlignment::Center:
					Options |= FT800Display::FT_OPT_CENTERX;
					break;

				case TArduinoDisplayFT800SceneElementHorizontalAlignment::Right:
					Options |= FT800Display::FT_OPT_RIGHTX;
					break;

			}

			T_DATA AInitialValue = InitialValue().GetValue();

			C_OWNER.GetDisplay().Cmd_Number( X(), Y(), Font(), Options, *(uint32_t *)&AInitialValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_DATA AData = *(T_DATA *)_Data;
			if( InitialValue().GetValue() == AData )
				return;

			InitialValue() = AData;
			C_OWNER.GetDisplay().Modified();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Characters,
		typename T_Enabled,
		typename T_Font,
		typename T_Height,
		typename T_HorizontalAlignment,
		typename T_In3D,
		typename T_OutputPin,
		typename T_Track,
		typename T_Value,
		typename T_VerticalCentered,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayFT800Keys :
		public T_Characters,
		public T_Enabled,
		public T_Font,
		public T_Height,
		public T_HorizontalAlignment,
		public T_In3D,
		public T_OutputPin,
		public T_Track,
		public T_Value,
		public T_VerticalCentered,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( In3D )
		_V_PROP_( HorizontalAlignment )
		_V_PROP_( VerticalCentered )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Font )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( Track )
		_V_PROP_( Value )
		_V_PROP_( Characters )

	public:
		void SetValue( char AValue )
		{
			if( AValue == Value() )
				return;

			Value() = AValue;
			if( Value() )
				T_OutputPin::SetPinValue( Value() );

			C_OWNER.GetDisplay().Modified();
		}

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
			{
				if( Track() )
				{
					if( ATagXY->tag == 0 )
					{
						SetValue( 0 );
						return;
					}

					else
					{
						if( Characters().GetValue().indexOf( ATagXY->tag ) >= 0 )
						{
							SetValue( ATagXY->tag );
							return;
						}

					}

				if( AFromStart )
					T_OutputPin::SetPinValue( Value() );

				}

//				Serial.println( "TEST" );
//				Serial.println( ATagXY->tag );
/*
				if( Track && ( Tag > 0 ) )
				{
					if( ( ATagValue & 0xFF ) == Tag )
					{
						uint8_t ALength = Characters.length();
						if( ALength )
						{
							uint8_t APosition = ( ATagValue >> 16 ) % ALength;
							Serial.println( APosition );
						}
//						SetValue( ( ATagValue >> 16 ) > 0x7FFF );
						return;
					}
						
				}
*/
			}

			uint16_t Options = Value();

			if( ! In3D() )
				Options |= FT800Display::FT_OPT_FLAT;

			if( VerticalCentered() )
				Options |= FT800Display::FT_OPT_CENTERY;

			switch( HorizontalAlignment() )
			{
				case TArduinoDisplayFT800SceneElementHorizontalAlignment::Center:
					Options |= FT800Display::FT_OPT_CENTERX;
					break;

				case TArduinoDisplayFT800SceneElementHorizontalAlignment::Right:
					Options |= FT800Display::FT_OPT_RIGHTX;
					break;

			}

//			if( Tag )
//				C_OWNER.GetDisplay().Tag( Tag );

			C_OWNER.GetDisplay().Cmd_Keys( X(), Y(), Width(), Height(), Font(), Options, Characters().c_str() );
//			if( Track )
//				if( Tag )
//					C_OWNER.GetDisplay().Cmd_Track( X, Y, Width, Height, Tag );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled
	> class DisplayFT800ClearScreen :
		public T_Color,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Color )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().ClearColorRGB( Color().GetReference().Red, Color().GetReference().Green, Color().GetReference().Blue );
			C_OWNER.GetDisplay().ClearColorA( Color().GetReference().Alpha );
			C_OWNER.GetDisplay().Clear( true, true, true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Calibrated,
		typename T_Enabled
	> class DisplayFT800Calibrate :
		public T_Calibrated,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( Calibrated )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( Calibrated() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Cmd_Calibrate( 0 );
			C_OWNER.GetDisplay().Modified();
			Calibrated() = true;
		}

	public:
		inline DisplayFT800Calibrate()
		{
			Calibrated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled
	> class DisplayFT800BackgroundColor :
		public T_Color,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Color )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Cmd_BGColor( Color().GetValue().AsRGBWord() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled
	> class DisplayFT800ForegroundColor :
		public T_Color,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Color )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Cmd_FGColor( Color().GetValue().AsRGBWord() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled
	> class DisplayFT800Color :
		public T_Color,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Color )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().ColorA( Color().GetReference().Alpha );
			C_OWNER.GetDisplay().ColorRGB( Color().GetReference().Red, Color().GetReference().Green, Color().GetReference().Blue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Color,
		typename T_X,
		typename T_Y
	> class TArduinoDisplayFT800SceneElementGradientSingleColor :
		public T_Color,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color1,
		typename T_Color2,
		typename T_Enabled
	> class DisplayFT800ColorGradient :
		public T_Color1,
		public T_Color2,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Color1 )
		_V_PROP_( Color2 )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Cmd_Gradient( Color1().X(), Color1().Y(), Color1().Color().GetValue(), Color2().X(), Color2().Y(), Color2().Color().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Value
	> class DisplayFT800Tag :
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Tag( Value().GetValue() );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		uint8_t C_TYPE,
		typename T_X,
		typename T_Y
	> class DisplayFT800DrawPoints :
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		FloatPoint2D *_Points;
		uint32_t _Count;

	public:
		inline operator FloatPoint2D *() { return _Points; }

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Begin( C_TYPE );
			for( int i = 0; i < _Count; ++i )
				C_OWNER.GetDisplay().Vertex2f( ( X().GetValue() + _Points[ i ].X ) * 16, ( Y().GetValue() + _Points[ i ].Y ) * 16 );

			C_OWNER.GetDisplay().End();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		uint8_t C_TYPE,
		typename T_X,
		typename T_Y
	> class DisplayFT800DrawPoints_Const :
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		const /*PROGMEM*/ FloatPoint2D *_Points;
		uint32_t _Count;

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Begin( C_TYPE );
			for( int i = 0; i < _Count; ++i )
				C_OWNER.GetDisplay().Vertex2f( ( X().GetValue() + pgm_read_float( ((float * )_Points) + i * 2 )) * 16, ( Y().GetValue() + pgm_read_float( ((float * )_Points) + i * 2 + 1 ) ) * 16 );
//				C_OWNER.GetDisplay().Vertex2f( ( X + _Points[ i ].X ) * 16, ( Y + _Points[ i ].Y ) * 16 );

			C_OWNER.GetDisplay().End();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		uint8_t C_TYPE,
		typename T_X,
		typename T_Y
	> class DisplayFT800DrawLines :
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		FloatRect2D *_Elements;
		uint32_t _Count;

	public:
		inline operator FloatRect2D *() { return _Elements; }

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Begin( C_TYPE );
			for( int i = 0; i < _Count; ++i )
			{
				C_OWNER.GetDisplay().Vertex2f( ( X().GetValue() + _Elements[ i ].X ) * 16, ( Y().GetValue() + _Elements[ i ].Y ) * 16 );
				C_OWNER.GetDisplay().Vertex2f( ( X().GetValue() + _Elements[ i ].X + _Elements[ i ].Width ) * 16, ( Y().GetValue() + _Elements[ i ].Y + _Elements[ i ].Height ) * 16 );
			}

			C_OWNER.GetDisplay().End();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		uint8_t C_TYPE,
		typename T_X,
		typename T_Y
	> class DisplayFT800DrawLines_Const :
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		const /*PROGMEM*/ FloatRect2D *_Elements;
		uint32_t _Count;

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().Begin( C_TYPE );
			for( int i = 0; i < _Count; ++i )
			{
				C_OWNER.GetDisplay().Vertex2f( ( X().GetValue() + pgm_read_float( ((float * )_Elements) + i * 4 )) * 16, ( Y().GetValue() + pgm_read_float( ((float * )_Elements) + i * 4 + 1 ) ) * 16 );
				C_OWNER.GetDisplay().Vertex2f( ( X().GetValue() + pgm_read_float( ((float * )_Elements)) + pgm_read_float( ((float * )_Elements) + i * 4 + 2 )) * 16, ( Y().GetValue() + pgm_read_float( ((float * )_Elements) + i * 4 + 1 ) + pgm_read_float( ((float * )_Elements) + i * 4 + 3 ) ) * 16 );
			}

			C_OWNER.GetDisplay().End();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Value
	> class DisplayFT800PointSize :
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().PointSize( Value().GetValue() * 16 );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Value
	> class DisplayFT800LineWidth :
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	public:
		inline void Render( uint32_t ATagValue, Mitov::FT800Display::sTagXY *ATagXY, bool AFromStart )
		{
			if( ! Enabled() )
				return;

			if( ATagXY )
				return;

			C_OWNER.GetDisplay().LineWidth( Value().GetValue() * 16 );
		}
	};
//---------------------------------------------------------------------------
//	template <typename T_OWNER, T_OWNER &C_OWNER, typename T_X_IMPLEMENTATION, typename T_Y_IMPLEMENTATION, typename T_PRESSURE_IMPLEMENTATION> class DisplayFT800Touch : public T_X_IMPLEMENTATION, public T_Y_IMPLEMENTATION, public T_PRESSURE_IMPLEMENTATION
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_PressedOutputPin,
		typename T_PressureOutputPin,
		typename T_TagOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class DisplayFT800Touch :
		public T_PressedOutputPin,
		public T_PressureOutputPin,
		public T_TagOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )

		_V_PIN_( PressureOutputPin )
		_V_PIN_( TagOutputPin )
		_V_PIN_( PressedOutputPin )

	public:
		inline void SystemStart()
		{
			FT800Display::sTagXY sTagxy;
			C_OWNER.GetDisplay().GetTagXY( sTagxy );

			bool APressed = ( sTagxy.x >= 0 );
			T_PressedOutputPin::SetPinValue( APressed );
			if( APressed )
			{
				T_XOutputPin::SetPinValue( sTagxy.x );
				T_YOutputPin::SetPinValue( sTagxy.y );
			}

			else
			{
				T_XOutputPin::SetPinValue( 0 );
				T_YOutputPin::SetPinValue( 0 );
			}

		}

		inline void SystemLoopEnd()
		{
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif