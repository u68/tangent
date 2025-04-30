#include "base.h"
#include "thefont.h"
#include "config.h"
#include "func_defs.h"
#include "io.h"
#include "desktop.h"

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

byte babs(byte in)
{
    if ((in & 0x80) != 0)
    {
        return -in;
    }
    return in;
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
    const byte error[] = "BRK TRIGGERED";
	const byte message[] = "OUT OF BOUNDS";
	while(1)
	{
		print(error,1,1,2);
		print(message,1,2,1);
	}

}
void invalid_instruction(word opc)
{
	const byte error[] = "BRK TRIGGERED";
	const byte message[] = "INST INVALID: 9100";
	derefw(0x9100) = opc;
	while(1)
	{
		print(error,1,1,2);
		print(message,0,2,1);
	}

}

void main()
{
    byte progs = 0;
    word PID = 0;
    word i = 0;

    word prog_size = PROGRAM_SIZE;
    word prog_count = PROGRAM_COUNT;
    const byte test_win[] = "test";
    for (i=0;i<0xEE00;i++)
    {
    	deref(i)=0;
    }
    deref(0xf037) = 0;
    for (i=0xF800;i<0xFFFF;i++)
    {
    	deref(i)=0;
    }
    deref(0xf037) = 4;
    for (i=0xF800;i<0xFFFF;i++)
    {
    	deref(i)=0;
    }
    deref(PROGRAM_COUNT) = 0; //program count
    //init DE
    //load_from_rom(0x5000);
    derefw(0x9C06) = 30;
    load_from_rom(0x5000);
    new_win(15,15,150,40,test_win);
    //load_from_rom(0x5000);
    //load_from_rom(0x5000);
    //load_from_rom(0x5000);

    while (1)
    {
        for (progs = 0;progs < 16;progs++)
        {
        	//derefw(0x9108) = (((derefw(prog_count) << progs) & 32768) >> 15);

            if ((((derefw(prog_count) << progs) & 32768) >> 15) == 1)
            {
            	//indexer = prog_size*progs
            	/*indexer = 0;
            	temp = prog_size;
            	for (i = 0;i < progs;i++)
            	{
            		indexer = indexer + temp; //this was because compiler had a hard fucking time
            	}
            	*/
                PID = (prog_size*progs)+0x9C02; //program * size + prog adr + prog count + vram1/2
                //derefw(0x9108) = (prog_size);
                exc_instruction(PID);
            }
        }
    }
}
