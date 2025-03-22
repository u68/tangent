#include "base.h"
#include "thefont.h"
#include "config.h"

void delay(ushort after_ticks)
{
    if ((FCON & 2) != 0)
        FCON &= 0xfd;
    __DI();
    Timer0Interval = after_ticks;
    Timer0Counter = 0;
    Timer0Control = 0x0101;
    InterruptPending_W0 = 0;
    StopAcceptor = 0x50;
    StopAcceptor = 0xa0;
    StopControl = 2;
    __asm("nop");
    __asm("nop");
    __EI();
}

void invert_line(const word line)
{
	word i = 0;
	word j = 0;
	word offset = (line<<9);
	for(i = 0; i < 512; i++)
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

enum BUTTON
{
	B_0 = 0xb,
	B_1 = 0x3f,
	B_2 = 0x37,
	B_3 = 0x2f,
	B_4 = 0x3e,
	B_5 = 0x36,
	B_6 = 0x2e,
	B_7 = 0x3d,
	B_8 = 0x35,
	B_9 = 0x2d,

	B_A = 0x3c,
	B_B = 0x34,
	B_C = 0x2c,
	B_D = 0x24,
	B_E = 0x1c,
	B_F = 0x14,

	B_G = 0x3d,
	B_H = 0x35,
	B_I = 0x2d,
	B_J = 0x25,
	B_K = 0x1d,

	B_L = 0x3e,
	B_M = 0x36,
	B_N = 0x2e,
	B_O = 0x26,
	B_P = 0x1e,

	B_Q = 0x3f,
	B_R = 0x37,
	B_S = 0x2f,
	B_T = 0x27,
	B_U = 0x1f,

	B_V = 0xb,
	B_W = 0xc,
	B_X = 0xd,
	B_Y = 0xe,
	B_Z = 0xf,



	BUTTON_COUNT = 0x40
};

enum SPECIAL_CHARS
{
	SP_EXE = '\n',
	SP_TAB = '\t',
	SP_SPACE = ' ',
	SP_BACKSPACE = '\b',

	SP_HOME = 0x2A,
	SP_END = 0x1A,
	SP_YES = 0x3A,
	SP_NO = 0x12,
	SP_OK = 0x21,
	SP_UP = 0x20,
	SP_DOWN = 0x22,
	SP_LEFT = 0x29,
	SP_RIGHT = 0x19,
	SP_PLUS = 0x10,
	SP_MINUS = 0x11,
	SP_ALT = 0x31,
	SP_ABC = 0x30,
	SP_ESC = 0x39,
	SP_SELECTLEFT,
	SP_SELECTRIGHT,
	SP_PASTE,
	SP_COPY,

	SPECIAL_MAX
};

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
}

void set_pixel(word x, word y, byte color) {
	word byteIndex = (y<<5) + (x >> 3);
	byte bitIndex = x & 7;
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

void plot_line (byte x0, byte y0, byte x1, byte y1, byte color)
{
  word dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  word dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
  word err = dx + dy, e2;

  for (;;){
    set_pixel(x0,y0,color);
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

void byte_to_nibble(byte cv2)
{
	byte bufa = 0;
	byte bufb = 0;
	bufa = (cv2&0xF0)>>4;
	bufb = cv2&0x0F;
	deref(0xe0e0) = (bufa + 0x30);
	deref(0xe0e1) = (bufb + 0x30);
}

void custom_break()
{
	byte error[] = "BRK TRIGGERED";
	byte message[] = "OUT OF BOUNDS";
	while(1)
	{
		print(error,1,1,2);
		print(message,1,2,1);
	}

}
void invalid_instruction()
{
	byte error[] = "BRK TRIGGERED";
	byte message[] = "INST INVALID";
	while(1)
	{
		print(error,1,1,2);
		print(message,1,2,1);
	}

}


void init_prog()
{
    byte prog_index = 0;
    word progs = derefw(PROGRAM_COUNT);
    for (prog_index = 0;prog_index < 16;prog_index++)
    {
        if((progs >> prog_index) & 1 == 1)
        {
            derefw(PROGRAM_COUNT) |= 1 << prog_index;
        }
    }
}

void kill(word PID)
{
    byte prog_index = (PID - 0x9C02) / PROGRAM_SIZE;
    derefw(PROGRAM_COUNT) &= ~(1 << prog_index);
}

void memcpy(word dest, word src, word size)
{
    word i = 0;
    for(i = 0;i < size;i++)
    {
        deref(dest+i) = deref(src+i);
    }
}

void load_from_rom(adr)
{
    byte prog_index = 0;
    word progs = derefw(PROGRAM_COUNT);
    for (prog_index = 0;prog_index < 16;prog_index++)
    {
        if((progs >> prog_index) & 1 == 1)
        {
            memcpy(prog_index*PROGRAM_SIZE+0x9C02,adr,PROGRAM_SIZE);
            init_prog();
        }
    }
}

void main()
{
    byte progs = 0;
    word PID = 0;
    deref(PROGRAM_COUNT) = 0; //program count
    //init DE
    load_from_rom(/*de adr*/);
    while (1)
    {
        for (progs = 0;progs < 16;progs++)
        {
            if((derefw(PROGRAM_COUNT) >> progs) & 1 == 1)
            {
                PID = derefw(progs*PROGRAM_SIZE+0x9C02); //program * size + prog adr + prog count + vram1/2
                exc_instruction(PID);
            }
        }
    }
}

/*
byte size registers 0-15 @ PID +4...+19
limit ram to b, code to 1kb
*/

byte read_byte(word PID, word addr)
{
    if(addr < RAM_SIZE)
    {
        retrun deref(PID + 10 + NUM_REGS + addr);
    } else {
        custom_break();
    }
}

word read_word(word PID, word addr)
{
    if(addr < RAM_SIZE)
    {
        return derefw(PID + 10 + NUM_REGS + addr);
    } else {
        custom_break();
    }
}

void write_byte(word PID, word addr, byte data)
{
    if(addr < RAM_SIZE)
    {
        deref(PID + 10 + NUM_REGS + addr) = data;
    } else {
        custom_break();
    }
}

void write_word(word PID, word addr, word data)
{
    if(addr < RAM_SIZE)
    {
        derefw(PID + 10 + NUM_REGS + addr) = data;
    } else {
        custom_break();
    }
}

void exc_instruction(word PID)
{
    word PC = PID + 2;
    word SP = PID + 4;
    word LR = PID + 6;
    word FLAGS = derefw(PID + 8);
    word regs = PID + 10;
    word ram = PID + 10 + NUM_REGS;
    word code = PID + 10 + NUM_REGS + RAM_SIZE;
    word instruction = derefw(PC)+code;
    byte opcode = (instruction & 0xFF00) >> 8;
    byte operand = instruction & 0x00FF;
    byte reg1 = (operand & 0xF0) >> 4;
    byte reg2 = operand & 0x0F;
    derefw(PC) += 2;
    switch(opcode)
    {
        //BYTE SIZE OPERATIONS
        case 0x00:
            //MOV
            deref(regs + reg1) = deref(regs + reg2);
            break;
        case 0x01:
            //ADD
            deref(regs + reg1) += deref(regs + reg2);
            break;
        case 0x02:
            //SUB
            deref(regs + reg1) -= deref(regs + reg2);
            break;
        case 0x03:
            //MUL
            deref(regs + reg1) *= deref(regs + reg2);
            break;
        case 0x04:
            //DIV
            deref(regs + reg1) /= deref(regs + reg2);
            break;
        case 0x05:
            //SLL
            deref(regs + reg1) = deref(regs + reg1) << deref(regs + reg2);
            break;
        case 0x06:
            //SRL
            deref(regs + reg1) = deref(regs + reg1) >> deref(regs + reg2);
            break;
        case 0x07:
            //L
            deref(regs + reg1) = read_byte(PID, derefw(regs + reg2));
            break;
        case 0x08:
            //ST
            write_byte(PID, derefw(regs + reg2), deref(regs + reg1));
            break;
        case 0x09:
            //AND
            deref(regs + reg1) &= deref(regs + reg2);
            break;
        case 0x0A:
            //OR
            deref(regs + reg1) |= deref(regs + reg2);
            break;
        case 0x0B:
            //XOR
            deref(regs + reg1) ^= deref(regs + reg2);
            break;
        case 0x0C:
            //INC
            deref(regs + reg1)++;
            break;
        case 0x0D:
            //DEC
            deref(regs + reg1)--;
            break;
        case 0x0E:
            //CMP
            if(deref(regs + reg1) == deref(regs + reg2))
            {
                FLAGS |= 0x01;
            }
            else
            {
                FLAGS &= 0xFE;
            }
            if(deref(regs + reg1) > deref(regs + reg2))
            {
                FLAGS |= 0x02;
            }
            else
            {
                FLAGS &= 0xFD;
            }
            if(deref(regs + reg1) < deref(regs + reg2))
            {
                FLAGS |= 0x04;
            }
            else
            {
                FLAGS &= 0xFB;
            }
            if(deref(regs + reg1) >= deref(regs + reg2))
            {
                FLAGS |= 0x08;
            }
            else
            {
                FLAGS &= 0xF7;
            }
            if(deref(regs + reg1) <= deref(regs + reg2))
            {
                FLAGS |= 0x10;
            }
            else
            {
                FLAGS &= 0xEF;
            }
            break;
        case 0x0F:
            //JMP
            derefw(PID) = derefw(regs + reg1);
            break;
        case 0x10:
            //JZ
            if(FLAGS & 0x01)
            {
                derefw(PID) = derefw(regs + reg1);
            }
            break;
        case 0x11:
            //JNZ
            if(!(FLAGS & 0x01))
            {
                derefw(PID) = derefw(regs + reg1);
            }
            break;
        case 0x12:
            //JG
            if(FLAGS & 0x02)
            {
                derefw(PID) = derefw(regs + reg1);
            }
            break;
        case 0x13:
            //JL
            if(FLAGS & 0x04)
            {
                derefw(PID) = derefw(regs + reg1);
            }
            break;
        case 0x14:
            //JGE
            if(FLAGS & 0x08)
            {
                derefw(PID) = derefw(regs + reg1);
            }
            break;
        case 0x15:
            //JLE
            if(FLAGS & 0x10)
            {
                derefw(PID) = derefw(regs + reg1);
            }
            break;
    //WORD SIZE OPERATIONS
        case 0x16:
            //MOV
            derefw(regs + reg1) = derefw(regs + reg2);
            break;
        case 0x17:
            //ADD
            derefw(regs + reg1) += derefw(regs + reg2);
            break;
        case 0x18:
            //SUB
            derefw(regs + reg1) -= derefw(regs + reg2);
            break;
        case 0x19:
            //MUL
            derefw(regs + reg1) *= derefw(regs + reg2);
            break;
        case 0x1A:
            //DIV
            derefw(regs + reg1) /= derefw(regs + reg2);
            break;
        case 0x1B:
            //SLL
            derefw(regs + reg1) = derefw(regs + reg1) << derefw(regs + reg2);
            break;
        case 0x1C:
            //SRL
            derefw(regs + reg1) = derefw(regs + reg1) >> derefw(regs + reg2);
            break;
        case 0x1D:
            //L
            derefw(regs + reg1) = read_word(PID, derefw(regs + reg2));
            break;
        case 0x1E:
            //ST
            write_word(PID, derefw(regs + reg2), derefw(regs + reg1));
            break;
        case 0x1F:
            //AND
            derefw(regs + reg1) &= derefw(regs + reg2);
            break;
        case 0x20:
            //OR
            derefw(regs + reg1) |= derefw(regs + reg2);
            break;
        case 0x21:
            //XOR
            derefw(regs + reg1) ^= derefw(regs + reg2);
            break;
        case 0x22:
            //INC
            derefw(regs + reg1)++;
            break;
        case 0x23:
            //DEC
            derefw(regs + reg1)--;
            break;
        case 0x24:
            //CMP
            if(derefw(regs + reg1) == derefw(regs + reg2))
            {
                FLAGS |= 0x01;
            }
            else
            {
                FLAGS &= 0xFE;
            }
            if(derefw(regs + reg1) > derefw(regs + reg2))
            {
                FLAGS |= 0x02;
            }
            else
            {
                FLAGS &= 0xFD;
            }
            if(derefw(regs + reg1) < derefw(regs + reg2))
            {
                FLAGS |= 0x04;
            }
            else
            {
                FLAGS &= 0xFB;
            }
            if(derefw(regs + reg1) >= derefw(regs + reg2))
            {
                FLAGS |= 0x08;
            }
            else
            {
                FLAGS &= 0xF7;
            }
            if(derefw(regs + reg1) <= derefw(regs + reg2))
            {
                FLAGS |= 0x10;
            }
            else
            {
                FLAGS &= 0xEF;
            }
            break;
    //IMMEDIATE OPERATIONS
        case 0x30:
            //MOV
            deref(regs + reg1) = deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x31:
            //ADD
            deref(regs + reg1) += deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x32:
            //SUB
            deref(regs + reg1) -= deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x33:
            //MUL
            deref(regs + reg1) *= deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x34:
            //DIV
            deref(regs + reg1) /= deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x35:
            //SLL
            deref(regs + reg1) = deref(regs + reg1) << deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x36:
            //SRL
            deref(regs + reg1) = deref(regs + reg1) >> deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x37:
            //L
            deref(regs + reg1) = read_byte(PID, derefw(code + PC));
            derefw(PC) += 2;
            break;
        case 0x38:
            //ST
            write_byte(PID, derefw(code + PC), deref(regs + reg1));
            derefw(PC) += 2;
            break;
        case 0x39:
            //AND
            deref(regs + reg1) &= deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x3A:
            //OR
            deref(regs + reg1) |= deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x3B:
            //XOR
            deref(regs + reg1) ^= deref(code + PC);
            derefw(PC) += 2;
            break;
        case 0x3C:
            //INC
            deref(regs + reg1)++;
            break;
        case 0x3D:
            //DEC
            deref(regs + reg1)--;
            break;
        case 0x3E:
            //CMP
            if(deref(regs + reg1) == deref(code + PC))
            {
                FLAGS |= 0x01;
            }
            else
            {
                FLAGS &= 0xFE;
            }
            if(deref(regs + reg1) > deref(code + PC))
            {
                FLAGS |= 0x02;
            }
            else
            {
                FLAGS &= 0xFD;
            }
            if(deref(regs + reg1) < deref(code + PC))
            {
                FLAGS |= 0x04;
            }
            else
            {
                FLAGS &= 0xFB;
            }
            if(deref(regs + reg1) >= deref(code + PC))
            {
                FLAGS |= 0x08;
            }   
            else
            {
                FLAGS &= 0xF7;
            }
            if(deref(regs + reg1) <= deref(code + PC))
            {
                FLAGS |= 0x10;
            }
            else
            {
                FLAGS &= 0xEF;
            }
            derefw(PC) += 2;
            break;
    //WORD IMMEADIATE OPERATIONS
        case 0x40:
            //MOV
            derefw(regs + reg1) = derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x41:
            //ADD
            derefw(regs + reg1) += derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x42:
            //SUB
            derefw(regs + reg1) -= derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x43:
            //MUL
            derefw(regs + reg1) *= derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x44:
            //DIV
            derefw(regs + reg1) /= derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x45:
            //SLL
            derefw(regs + reg1) = derefw(regs + reg1) << derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x46:
            //SRL
            derefw(regs + reg1) = derefw(regs + reg1) >> derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x47:
            //L
            read_word(PID, derefw(code + PC));
            derefw(PC) += 2;
            break;
        case 0x48:
            //ST
            write_word(PID, derefw(code + PC), derefw(regs + reg1));
            derefw(PC) += 2;
            break;
        case 0x49:
            //AND
            derefw(regs + reg1) &= derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x4A:
            //OR
            derefw(regs + reg1) |= derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x4B:
            //XOR
            derefw(regs + reg1) ^= derefw(code + PC);
            derefw(PC) += 2;
            break;
        case 0x4C:
            //INC
            derefw(regs + reg1)++;
            break;
        case 0x4D:
            //DEC
            derefw(regs + reg1)--;
            break;
        case 0x4E:
            //CMP
            if(derefw(regs + reg1) == derefw(code + PC))
            {
                FLAGS |= 0x01;
            }
            else
            {
                FLAGS &= 0xFE;
            }
            if(derefw(regs + reg1) > derefw(code + PC))
            {
                FLAGS |= 0x02;
            }
            else
            {
                FLAGS &= 0xFD;
            }
            if(derefw(regs + reg1) < derefw(code + PC))
            {
                FLAGS |= 0x04;
            }
            else
            {
                FLAGS &= 0xFB;
            }
            if(derefw(regs + reg1) >= derefw(code + PC))
            {
                FLAGS |= 0x08;
            }
            else
            {
                FLAGS &= 0xF7;
            }
            if(derefw(regs + reg1) <= derefw(code + PC))
            {
                FLAGS |= 0x10;
            }
            else
            {
                FLAGS &= 0xEF;
            }
            derefw(PC) += 2;
            break;
    //STACK OPERATIONS
        case 0x50:
            //PUSH
            derefw(SP) -= 2;
            write_word(PID, derefw(SP), derefw(regs + reg1));
            break;
        case 0x51:
            //POP
            derefw(regs + reg1) = read_word(PID, derefw(SP));
            derefw(SP) += 2;
            break;
        case 0x52:
            //PUSH16
            derefw(SP) -= 2;
            write_word(PID, derefw(SP), derefw(regs + reg1));
        case 0x53:
            //POP16
            derefw(regs + reg1) = read_word(PID, derefw(SP));
            derefw(SP) += 2;
            break;
    //CONTROL OPERATIONS
        case 0x60:
            //B
            derefw(PC) = derefw(regs + reg1);
            break;
        case 0x61:
            //BL
            derefw(LR) = derefw(PC);
            derefw(PC) = derefw(regs + reg1);
            break;
        case 0x62:
            //RT
            derefw(PC) = derefw(LR);
            break;
        case 0x70:
            //SYSCALL
            if(operand == 0)
            {
                //PRINT
                print(derefw(regs + 0), deref(regs + 2), deref(regs + 3), deref(regs + 4));
            }
            else if(operand == 1)
            {
                //SETPIXEL
                set_pixel(deref(regs + 0), deref(regs + 1), deref(regs + 2));
            }
            else if(operand == 2)
            {
                //PLOTLINE
                plot_line(deref(regs + 0), deref(regs + 1), deref(regs + 2), deref(regs + 3), deref(regs + 4));
            }
            else if(operand == 3)
            {
                //MENU
                deref(regs + 0) = menu(derefw(regs + 0), derefw(regs + 2), derefw(regs + 4), derefw(regs + 6));
            }
            else if(operand == 4)
            {
                //MENU2
                deref(regs + 0) = menu_2(derefw(regs + 0), derefw(regs + 2));
            }
            else if(operand == 5)
            {
                //CHECKBUTTONS
                deref(regs + 0) = CheckButtons();
            }
            else if(operand == 6)
            {
                //RENDER
                render(vram1);
            }
            break;
        case 0xFF:
            //break for testing instructions, do not include in code
            while (1)
            {
                //do nothing
            }
        default:
            invalid_instruction();
            break;
    }
}