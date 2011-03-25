#pragma once

void os_console_init       (void);
void os_get_console_size   (unsigned int* width, unsigned int* height);
void os_set_console_color  (BOOL fg_red, BOOL fg_green, BOOL fg_blue, 
                            BOOL bg_red, BOOL bg_green, BOOL bg_blue, 
                            BOOL fg_bright, BOOL bg_bright);
void os_move_console_cursor(unsigned int x, unsigned int y);
void os_sockets_init(void);
void os_sockets_cleanup(void);

