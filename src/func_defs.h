#ifndef FUNC_DEFS_H
#define FUNC_DEFS_H
#include "base.h"

void delay(ushort after_ticks);
void invert_line(word line);
void drawbitmap(const byte* ptr, word offset, byte width, byte height,byte color);
void render(word addr);
word print(const byte* str, byte x, byte y,byte color);
byte menu(const byte* option_a, const byte* option_b, const byte* option_c, const byte* option_d);
byte menu_2(const byte* option_a, const byte* option_b);
void set_pixel(byte x, byte y, byte color);
int abs(int number);
void plot_line (int x0, int y0, int x1, int y1, byte color);
byte CheckButtons();
void byte_to_nibble(byte cv2);
void custom_break();
void invalid_instruction(word opc);
void init_prog();
void kill(word PID);
void load_from_rom(word adr);
byte read_byte(word PID, word addr);
word read_word(word PID, word addr);
void write_byte(word PID, word addr, byte data);
void write_word(word PID, word addr, word data);
void exc_instruction(word PID);

void memcpy(word dest, word src, word size);

#endif
