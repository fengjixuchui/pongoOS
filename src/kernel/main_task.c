// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
//
//  Copyright (c) 2019-2020 checkra1n team
//  This file is part of pongoOS.
//
#define LL_KTRW_INTERNAL 1
#include <pongo.h>

boot_args * gBootArgs;
void* gEntryPoint;

#define BOOT_FLAG_DEFAULT 0
#define BOOT_FLAG_HARD 1
#define BOOT_FLAG_HOOK 2

extern volatile char gBootFlag;
dt_node_t *gDeviceTree;
uint64_t gIOBase;

void shell_main();

/*

    Name: pongo_main_task
    Description: main task handler

*/
int socnum = 0x0;
void pongo_main_task() {
    /*
        Enable serial TX
    */

    serial_early_init();

    /*
        Turn on IRQ controller
    */

    interrupt_init();

    /*
        Enable IRQ serial RX
    */

    serial_init();

    /*
        Initialize pmgr
    */

    pmgr_init();

    puts("");
    puts("#==================");
    puts("#");
    iprintf("# pongoOS " PONGO_VERSION " (EL%d)\n", get_el());
    puts("#");
    puts("# https://checkra.in");
    puts("#");
    puts("#==================");
    screen_mark_banner();
    iprintf("Booted by: %s\n", dt_get_prop("chosen", "firmware-version", NULL));
    strcpy(dt_get_prop("chosen", "firmware-version", NULL), "pongoOS-");
    strcat(dt_get_prop("chosen", "firmware-version", NULL), PONGO_VERSION);
    iprintf("Built with: GCC %s\n", __VERSION__);
    gDevType = dt_get_prop("arm-io", "device_type", NULL);
    size_t len = strlen(gDevType) - 3;
    char soc_name[9] = {};
    len = len < 8 ? len : 8;
    strncpy(soc_name, gDevType, len);
    if  (strcmp(soc_name, "s5l8960x") == 0) socnum = 0x8960;
    else if(strcmp(soc_name, "t7000") == 0) socnum = 0x7000;
    else if(strcmp(soc_name, "t7001") == 0) socnum = 0x7001;
    else if(strcmp(soc_name, "s8001") == 0) socnum = 0x8001;
    else if(strcmp(soc_name, "t8010") == 0) socnum = 0x8010;
    else if(strcmp(soc_name, "t8011") == 0) socnum = 0x8011;
    else if(strcmp(soc_name, "t8015") == 0) socnum = 0x8015;
    else if(strcmp(soc_name, "s8000") == 0)
    {
        const char *sgx = dt_get_prop("sgx", "compatible", NULL);
        if(strlen(sgx) > 4 && strcmp(sgx + 4, "s8003") == 0)
        {
            socnum = 0x8003;
            soc_name[4] = '3';
        }
        else
        {
            socnum = 0x8000;
        }
    }
    iprintf("Running on: %s\n", soc_name);
    
    shell_main();
}
