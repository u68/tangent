#include "base.h"
#include "config.h"
#include "func_defs.h"
#include "io.h"
#include "thefont.h"

const byte button_to_char[64] = {
    0,        0,       0,    0,    0,    0,   0,    0,
    0,        0,       0,  '0',  ' ',  ',', '.', '\n',
SP_PLUS, SP_MINUS,   SP_NO,  ']',  ')',    0,   0,    0,
    0, SP_RIGHT,  SP_END,  '[',  '(', '\b', '#',  '%',
SP_UP,    SP_OK, SP_DOWN, '\'',  '/',  '!', '@',  '$',
    0,  SP_LEFT, SP_HOME,  ';', '\\',  '9', '6',  '3',
SP_ABC,   SP_ALT,     '*',  '-',  '=',  '8', '5',  '2',
    0,   SP_ESC,  SP_YES, '\t',  '`',  '7', '4',  '1',
};

const byte button_to_char_abc[64] = {
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0, 'V', 'W', 'X', 'Y', 'Z',
  0,   0,   0,   0, 'F',   0,   0,   0,
  0,   0,   0,   0, 'E', 'K', 'P', 'U',
  0,   0,   0,   0, 'D', 'J', 'O', 'T',
  0,   0,   0,   0, 'C', 'I', 'N', 'S',
  0,   0,   0,   0, 'B', 'H', 'M', 'R',
  0,   0,   0,   0, 'A', 'G', 'L', 'Q',
};

const byte button_to_char_alt[64] = {
  0,              0,        0,   0,   0,   0,   0,   0,
  0,              0,        0,   0,   0, '<', '>',   0,
  0,              0, SP_PASTE, '}', '^',   0,   0,   0,
  0, SP_SELECTRIGHT,        0, '{', '*',   0,   0,   0,
  0,              0,        0, '"', '?',   0,   0,   0,
  0,  SP_SELECTLEFT,        0, ':', '|',   0,   0,   0,
  0,              0,      '&', '_', '+',   0,   0,   0,
  0,              0,  SP_COPY,   0, '~',   0,   0,   0,
};

void invert_line(word line)
{
	word i = 0;
	word j = 0;
	word offset = (line<<9);
	for(i = 0; i < 24; i++)
	{
		deref(0xF037) = 0;
		deref(0xF800+i+offset) = (~deref(0xF800+i+offset));
		deref(0xF037) = 4;
		deref(0xF800+i+offset) = (~deref(0xF800+i+offset));
	}
}

void drawbitmap(const byte* ptr, word offset, byte width, byte height,byte color)
{
	word x;
	word y;
	word i = 0;
	word j = 0;
	deref(0xF037) = 0;
	if(color == 1)
	{
		deref(0xF037) = 4;
	}
	for(y = 0; y < offset; y++)
	{
		++j;
		if((j&0x1f) == 0x18)
		{
			j += 0x8;
		}
	}

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(color == 2)
			{
				deref(0xF037) = 0;
				deref(0xf800+j) = ptr[i];
				deref(0xF037) = 4;
			}
			deref(0xf800+j) = ptr[i];
			++j;
			if((j&0x1f) == 0x18)
			{
				j += 0x8;
			}
			++i;
		}
		j += 32-width;
		if((j&0x1f) == 0x18)
		{
			j += 0x8;
		}
	}
}

void render(word addr)
{
    word i = 0;
    word j = 0;
    deref(0xf037) = 0;
    for(i = 0; i < 0x0800; i++)
    {
        j++;
        if((j & 0x001F) == 0x18)
        {
            j+=8;
        }
        *((word *)(0xf800 + j)) = *((word *)(addr + i));
    }
    j = 0;
    deref(0xf037) = 4;
    for(i = 0; i < 0x0800; i++)
    {
        j++;
        if((j & 0x001F) == 0x18)
        {
            j+=8;
        }
        *((word *)(0xf800 + j)) = *((word *)(addr + i + 0x600));
    }
}

word print(const byte* str, byte x, byte y,byte color)
{
	const byte* what = str;
	word curoffset = (word)x+((word)y<<8)+((word)y<<7);
	//derefw(0xB510) = (word)(what);a
	word i = 0;
	while(*what)
	{
		//deref(0xB500+i) = *what;

		drawbitmap(image_raw+((word)(*what)<<4),curoffset,1,16,color);
		++curoffset;
		++what;
		++i;
		//++i;
	}
	return i;
}

byte menu(const byte* option_a, const byte* option_b, const byte* option_c, const byte* option_d)
{
    word i = 0;
    word j = 0;
    word x = 0;
	for(x = 0;x <2048;x++) {
		deref(0xF037) = 0;
		deref(0xF800+x) = 0;
		deref(0xF037) = 4;
		deref(0xF800+x) = 0;
	}
	print(option_a, 0, 0, 1);
    print(option_b, 0, 1, 1);
    print(option_c, 0, 2, 1);
    print(option_d, 0, 3, 1);
	invert_line(i);
	while(1)
	{
		byte pressedbutton = CheckButtons();
		if(pressedbutton == 0xff)
		{
			delay(0x100);
			continue;
		} else if(pressedbutton == SP_DOWN) {
			i++;
			if(i == 4) {
				i = 0;
			}
			invert_line(j);
		} else if(pressedbutton == SP_UP) {
			i--;
			if(i == -1) {
				i = 3;
			}
			invert_line(j);
		} else if(pressedbutton == SP_EXE || pressedbutton == SP_OK || pressedbutton == SP_YES) {
			/*for(x = 0;x <2048;x++) {
				deref(0xF037) = 0;
				deref(0xF800+x) = 0;
				deref(0xF037) = 4;
				deref(0xF800+x) = 0;
			}*/
			if(i == 0) {
				return 1;
			} else if(i == 1) {
				return 2;
			} else if(i == 2) {
				return 3;
			} else if(i == 3) {
				return 4;
			}
		}
		invert_line(i);
		j = i;

	}

	return 0;
}

byte menu_2(const byte* option_a, const byte* option_b)
{
    word i = 0;
    word j = 0;
    word x = 0;
	for(x = 0;x <2048;x++) {
		deref(0xF037) = 0;
		deref(0xF800+x) = 0;
		deref(0xF037) = 4;
		deref(0xF800+x) = 0;
	}
	print(option_a, 0, 0, 1);
    print(option_b, 0, 1, 1);
	invert_line(i);
	while(1)
	{
		byte pressedbutton = CheckButtons();
		if(pressedbutton == 0xff)
		{
			delay(0x100);
			continue;
		} else if(pressedbutton == SP_DOWN) {
			i++;
			if(i == 2) {
				i = 0;
			}
			invert_line(j);
		} else if(pressedbutton == SP_UP) {
			i--;
			if(i == -1) {
				i = 1;
			}
			invert_line(j);
		} else if(pressedbutton == SP_EXE || pressedbutton == SP_OK || pressedbutton == SP_YES) {
			/*for(x = 0;x <2048;x++) {
				deref(0xF037) = 0;
				deref(0xF800+x) = 0;
				deref(0xF037) = 4;
				deref(0xF800+x) = 0;
			}*/
			if(i == 0) {
				return 1;
			} else if(i == 1) {
				return 2;
			}
		}
		invert_line(i);
		j = i;

	}
	return 0;
}

void set_pixel(byte x, byte y, byte color)
{
	word byteIndex = (y*24) + (x >> 3);
	byte bitIndex = x & 7;
	if (x > 191)
	{
		return;
	}
	if (y > 63)
	{
		return;
	}
	if (byteIndex + vram2 > 0x9c02)
	{
		return;
	}
	deref(byteIndex + vram1) &= ~(0x80>>bitIndex);
	deref(byteIndex + vram2) &= ~(0x80>>bitIndex);
    if(color == 1)
    {
    	deref(byteIndex + vram1) |= (0x80>>bitIndex);
    }
    if(color > 1)
    {
    	deref(byteIndex + vram2) |= (0x80>>bitIndex);
    }
    if(color == 3)
    {
        deref(byteIndex + vram1) |= (0x80>>bitIndex);
    }
}

void plot_line (int x0, int y0, int x1, int y1, byte color)
{
  int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  for (;;){
    set_pixel (x0,y0,color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}

byte lastbutton = 0xff;
byte CheckButtons()
{
	byte x;
	byte y;
	byte i = 0;
	for(x = 0x80; x != 0; x = x >> 1)
	{
		deref(0xf046) = x;
		for(y = 0x80; y != 0; y = y >> 1)
		{
			if((deref(0xf040) & y) == 0)
			{
				if(i != lastbutton)
				{
					lastbutton = i;
					return i;
				}
				return 0xff;
			}
			++i;
		}
	}
	lastbutton = 0x50;
	return 0xff;
}


