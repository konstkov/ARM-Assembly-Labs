#include <stdio.h>
#include "pico/stdlib.h"

__attribute__(( naked )) void asm_test(int *a, int *b, int *c, int *d)
{
  // write your code between push and pop instructions
  asm volatile
  (
	"push {r4, r5, r6, r7} \n" // do not remove

	//"ldr r7, [r0] \n" // example assembly code - replace with your own code
	//"add r4, r7, r2 \n" // example assembly code replace with your own code
	//Write a program that computes M0 = (M0 + M1 * M1) * (M3 + M1 * M1) + M2.
	// add more code here
        "ldr r7, [r1] \n" // copy a word from mem. addr. r1 and store it in r7
        "mul r7, r7, r7 \n" // multiply r7 by r7 and store in r7
        "ldr r6, [r0] \n" // copy a word from mem. add.r r0 and store it in r6
        "add r6, r6, r7 \n" // add r7 and r6 and store in r6 (M0 + M1 * M1)
        "ldr r1, [r3] \n" // copy a word from mem. add.r r3 and store it in r1
        "add r7, r1, r7 \n" // add r1 and r7 and store in r7 (M3 + M1 * M1)
        "mul r7, r6, r7 \n" // multiply r6 and r7 and store in r7 (M0 + M1 * M1) * (M3 + M1 * M1)
        "ldr r6, [r2] \n" // copy a word from mem. addr. r2 and store it in r6
        "add r7, r6, r7 \n" // add r6 and r7 and store in r7
        "str r7, [r0] \n"
	"pop {r4, r5, r6, r7} \n" // do not remove
	"bx lr \n" // do not remove
  );
}


void fail() {
    printf("Failed\n"); // set a break point here
    while(1) {
        tight_loop_contents();
    }
}

void ok() {
    printf("All ok\n"); // set a break point here
}


int main()
{
    const uint led_pin = 22;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    int m0;
    int m1;
    int m2;
    int m3;

    m0 = 1; m1 = 2; m2 = 3; m3 = 4;
    asm_test(&m0, &m1, &m2, &m3);
    if(m0 != 43) fail();

    m0 = 8; m1 = 5; m2 = 6; m3 = 21;
    asm_test(&m0, &m1, &m2, &m3);
    if(m0 != 1524) fail();

    m0 = 3; m1 = 4; m2 = 5; m3 = 1;
    asm_test(&m0, &m1, &m2, &m3);
    if(m0 != 328) fail();

    m0 = 3; m1 = 5; m2 = 7; m3 = 8;
    asm_test(&m0, &m1, &m2, &m3);
    if(m0 != 931) fail();

    m0 = 33; m1 = 22; m2 = 11; m3 = 0;
    asm_test(&m0, &m1, &m2, &m3);
    if(m0 != 250239) fail();

    m0 = 42; m1 = 55; m2 = 12; m3 = 1;
    asm_test(&m0, &m1, &m2, &m3);
    if(m0 != 9280754) fail();

	ok();

    // Loop forever
    while (true) {
        // Blink LED
        gpio_put(led_pin, true);
        sleep_ms(1000);
        gpio_put(led_pin, false);
        sleep_ms(1000);
    }

    return 0;
}
	
