/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
*/

/*
 * Modified by Wagner Morais on Sep 2024.
*/

/* PiFace Control and Display Driver */
#include <stdint.h>
#include <string.h>
#include "rpi-gpio.h"
#include "piface.h"
	
int cnt;	
	
/* Bit-Banging SPI Driver */
static void spi_init(void){
	RPI_GetGpio()->GPFSEL0 |= (1 << 21);				// GPIO  7,  9
	RPI_GetGpio()->GPFSEL1 |= (1 <<  0) | (1 <<  3);	// GPIO 10, 11
	RPI_GetGpio()->GPSET0 = (1 << 7);					// CE high
	RPI_GetGpio()->GPCLR0 = (1 << 11);			        // CLK low
}

static void spi_start(void){
	/* /CE low  */
	RPI_GetGpio()->GPCLR0 = (1 << 7);			        
}

static void spi_stop(void){
	/* /CE high */
	RPI_GetGpio()->GPSET0 = (1 << 7);			        
}

static void spi_byte(const uint8_t out, uint8_t *in){
	uint8_t tmpin = 0, tmpout = out;

	/* clock each bit out and read back */
	for(int i = 0; i < 8; i++){
		tmpin <<= 1;

		if(tmpout & 0x80)
			RPI_GetGpio()->GPSET0 = (1 << 10);			// set MOSI
		else
			RPI_GetGpio()->GPCLR0 = (1 << 10);

		if(RPI_GetGpio()->GPLEV0 & (1 << 9))
			tmpin |= 1;		// read MISO

		RPI_GetGpio()->GPSET0 = (1 << 11);			// set CLK
		RPI_GetGpio()->GPCLR0 = (1 << 11);			// clear CLK

		tmpout <<= 1;
	}

	*in = tmpin;
	return;
}

static uint8_t mcp_read(const uint8_t reg){
	uint8_t in;

	spi_start();
	spi_byte(0x41, &in);
	spi_byte(reg, &in);
	spi_byte(0x00, &in);
	spi_stop();

	return in;
}

static void mcp_write(const uint8_t reg, const uint8_t val){
	uint8_t in;

	spi_start();
	spi_byte(0x40, &in);
	spi_byte(reg, &in);
	spi_byte(val, &in);
	spi_stop();
}

static void mcp_init(void){
	/* Port A: (S1 .. S5, S, L, R) */
	mcp_write(MCP_IODIRA, 0xFF);
	mcp_write(MCP_GPPUA, 0xFF);
	/* Port B: (DB4 .. DB7, /EN, R/W, RS, LED) */
	mcp_write(MCP_IODIRB, 0x00);
}


static uint8_t lcd_read_busy_flag_register(){
	// Set read pin
	mcp_write(MCP_GPIOB, LCD_RW);

	// Set enable pin, keeping read pin high
	mcp_write(MCP_GPIOB, LCD_RW | LCD_EN);

	// Read bit 7 to 4
	uint8_t firstNibble = (mcp_read(MCP_GPIOB) & 0x0F);

	// Reset enable pin, keeping read pin high
	mcp_write(MCP_GPIOB, LCD_RW);

	// Set enable pin, keeping read pin high
	mcp_write(MCP_GPIOB, LCD_RW | LCD_EN);

	// Read bit 3 to 0
	uint8_t secondNibble = (mcp_read(MCP_GPIOB) & 0x0F);

	return ((firstNibble << 4) | secondNibble);
}

static void lcd_busy_wait(){
	while (lcd_read_busy_flag_register() & 0x80); 
}

static void lcd_pulse(uint8_t val){
	mcp_write(MCP_GPIOB, val | LCD_EN );
	mcp_write(MCP_GPIOB, val);
}

// static void lcd_write_cmd(uint8_t cmd){
    // lcd_busy_wait();
	// write high nibble
	// lcd_pulse( LCD_BL | (cmd >> 4) );				
	// write low nibble
    // lcd_pulse( LCD_BL | (cmd & 0x0F) );    			
    // LCD_DELAY;
// }

static void lcd_write_cmd(uint8_t cmd){
	// lcd_busy_wait();
	/* write high nibble */
	lcd_pulse( LCD_BL | (cmd >> 4)   );
	lcd_busy_wait();
	/* write low nibble */
    lcd_pulse( LCD_BL | (cmd & 0x0F) );
	LCD_DELAY;
}

static void lcd_write_data(uint8_t data){
	lcd_busy_wait();
	/* write high nibble */
	lcd_pulse( LCD_BL | LCD_RS | (data >> 4)   );
	lcd_busy_wait();
    /* write low nibble */
    lcd_pulse( LCD_BL | LCD_RS | (data & 0x0F) );
}

static void lcd_init(void){
	/* enable 4 bit mode */
	LCD_DELAY;
	lcd_pulse( 0x03 );
	LCD_DELAY;
	lcd_pulse( 0x03 );
	LCD_DELAY;
	lcd_pulse( 0x03 );
	LCD_DELAY;
	lcd_pulse( 0x02 );
	LCD_DELAY;

    /* function set; N = 1 for two rows, F = 0 for 5x8 display */
	lcd_write_cmd( 0x28 );
    /* display clear */
	lcd_write_cmd( 0x01 );
	LCD_DELAY;

    /* entry mode set; I/D = 1 for direction left to right, S = 0 for shift off */
	lcd_write_cmd( 0x06 );
    /* display on/off; D = 1 for display on, C = 1 for cursor on; B = 0 for blinking off*/
	lcd_write_cmd( 0x0E );
	// lcd_write_cmd( 0x0F );
}

__attribute__((constructor))
void piface_init(void){
	spi_init();
	mcp_init();
	lcd_init();
	cnt=0;
}

uint8_t piface_getc(void){
	return mcp_read(MCP_GPIOA);
}

/** @brief Writes a character
 */
void piface_putc(char c){
	/* write character */
	if(cnt%32==16) {
		lcd_write_cmd((0x40|0x80));
	} else {
		if(cnt%32==0){
			lcd_write_cmd(0x02);
			LCD_DELAY;	
			cnt = 0;
		} else {
			if (c=='\n') {
				if (cnt > 16) {
					lcd_write_cmd(0x02);
					LCD_DELAY;	
					cnt = 0;
				}
				else {
					lcd_write_cmd((0x40|0x80));
					cnt = 16;
				}
				return;
			}
		}
	}
	lcd_write_data(c);
	cnt++;
}


/** @brief Writes a string
 */
void piface_puts(char s[])
{
    /* write string */
	int i = 0;
	while (s[i] != '\0') {
		piface_putc(s[i]);
		i++;
	}
}

/** @brief Clears the display
 */
void piface_clear(void)
{
    /* clear display */
	cnt = 0;
	lcd_write_cmd(0b00000001);
}




/*
    The methods below are expected to be implemented and used 
	during Assignments 2, 3 and 4.
*/

/** @brief Sets the cursor on a specific row and column
 *  Please check the url: http://piface.github.io/libpifacedigital/
 *  @param uint8_t col indicates the columns to position the cursor
 *  @param uint8_t row indicates row 0 (first) or 1 (second)
 *  Pre-condition: 0<=seg<=3
 */
void piface_set_cursor(uint8_t col, uint8_t row)
{
	//to be implemented
	uint8_t cmd = 0x00;
	if (row == 0) {
        cmd = 0x00 + col; 
    } else if (row == 1) {
        cmd = 0x40 + col;
    }
	lcd_write_cmd(0x80|cmd);
}

/** @brief Displays an integer content in a given segment in the PiFace display.
 *  Suppose that you decided to segment the display into 4 segments of 8 digits each.
 *  For example, the illustration below ...
 *      ------------------
 *      |S0:XXXXXS1:XXXXX| 
 *      |S2:XXXXXS3:XXXXX|
 *      ------------------ 
 *  shows each segment (seg:0...3) with digits each.
 *  @param int seg Is the segment, i.e., 0: top left, 1:top right, 2: bottom left and 3: bottom right.
 *  @param int num Is the integer input parameter to be displayed.
 *  Pre-condition: 0<=seg<=3
 
 *  NOTE: maybe, you want to display arbitrary content on a given segment. For example:
 *      printAtSeg(1, "S%i: %.1f", 1, 3.14);
 *  This requires the use of variadic arguments with proper string formatting.
 *  Please check the url: https://en.cppreference.com/w/c/language/variadic
 *  You can redeclare printAtSeg as:
 *     void printAtSeg(int seg, const char* fmt, ...);
 */
void print_at_seg(int seg, int num) {
	//to be implemented
	uint8_t row = 0;
	uint8_t col = 0;
	if (seg == 0){
		row = 0;
		col = 0;
		cnt = 0;
	} else if (seg == 1) {
		row = 0;
		col = 8;
		cnt = 8; 
	} else if (seg == 2) {
		row = 1;
		col = 0;
		cnt = 16;
	} else if (seg == 3) {
		row = 1;
		col = 8;
		cnt = 24;
	}
	piface_set_cursor(col, row);

	PUTTOLDC("T%d: %d\n", seg, num);
}

/** @brief Similar to print_at_seg, but displays arbitrary content on a given segment. For example:
 *      printAtSeg(1, "S%i: %.1f", 1, 3.14);
 *  This requires the use of variadic arguments with proper string formatting.
 *  Please check the url: https://en.cppreference.com/w/c/language/variadic
 *  You can redeclare printAtSeg as:
 *     void printAtSeg(int seg, const char* fmt, ...);
 */
// void printf_at_seg(int seg, const char* fmt, ...) {
//     // The implementation is optional.	
// }
