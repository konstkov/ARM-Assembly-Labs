#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>

__attribute__(( naked )) void reverse(char *a)
{
	// the pointer to a character array is passed to the
	// subroutine in R0
	// remember to follow AAPCS:
	// push any register above r3 at the beginning and pop the same registers at the end
	// note: lr needs to be pushed because we are calling another subroutine
    // cortex-M0 requires popping to PC if LR was pushed. See the code below
	asm volatile
	(
	// in this exercise we need to calculate offset. Quick draft:
	// 1) determine the case of the letter (already done in previous exercise)
	// 2) depending on the case we will use different number of 'base' ASCII code when calculating the offset
	// 1) calculate offset from 'A' (or 'a') by subtracting current ASCII code from that of 'A'/'a'; A - #65, a - #97
	// 2) apply the same offset from the end of alphabet by subtracting calculated offset from 'Z'/'z'; Z - #90, z - #122
	//
			"push { r5, r6, lr } \n" // we need to save return address because we call another subroutine

			// Register Mapping:
			
			// r0, original pointer, also used by putchar for printing the character
			// r5: holds the pointer the copy of pointer from r0 for incrementing it
			// r6: stores the value of dereferenced pointer

			"mov r5, r0 \n" // copy the pointer from r0 and store it in r5

			"repeat: \n" // repeat the procedure
			"ldrb r6, [r5] \n" // dereference the pointer from r5 and store it in r6 (it is an unsigned byte)

			"cmp r6, #0 \n" // compare the value stored in r6 to '0' ( in order to detect terminating null)
			"beq out \n" // if it equals 0 - jump to 'out' branch

			"cmp r6, #64 \n" // compare the value from r6 to 64 (65 is ASCII code of first capital letter - 'A')
			"bhi is_uppercase \n" // if it is unsigned (it is according to the assignment) and higher jump to 'compare' branch

			"is_uppercase: \n" // 'is_uppercase' validation branch
			"cmp r6, #90 \n" // compare the value from r6 to '90' (ASCII code for last capital letter - 'Z')
			"bls capital \n" // if it is less or equal - it is a capital letter;
			"cmp r6, #96 \n" // but if it is greater, compare it to # 96 (ASCII code of 'a' is #97)
			"bhi is_lowercase \n"// if it is greater perform another comparison ( to ASCII code if 'z')
			"b print \n" // otherwise jump to branch print

			"lowercase: \n" // branch 'lowercase'
			"add r6, r6, #32 \n" // 32 is the difference in ASCII table between capital and lowercase letters
			"b print \n" // jump to branch 'print'

			"is_lowercase: \n" // 'is_lowercase' validation branch
			"cmp r6, #122 \n" // compare to lowercase 'z' character
			"bls lowercase \n" // if it is less or equal jump to 'lowercase' branch

			"capital: \n" // we can calculate offset from capital 'A' first
			"sub r6, r6, #65 \n" // subtract ASCII code of capital 'A' (#65) from value in r6 and store in r6
			"mov r0, #90 \n"// load value 90 into a register
			"sub r6, r0, r6 \n"// subtract the same offset from the end of alphabet (from 'Z' (#90)) and store the result in r2

			"print: \n" // 'print' branch
			"add r5, r5, #1 \n" // increment the pointer by 1 ( because char is 1 byte)
			"mov r0, r6 \n" // copy the value stored in r6 to r0 (putchar uses r0 for printing characters)
			"bl putchar \n"
			"b repeat \n" // repeat the cycle

			// to return a value copy it ro R0 before returning from subroutine

			"out: \n" // pop back the registers and end the program
			"pop { r5, r6, pc } \n" // cortex-M0 requires popping to PC if LR was pushed
            // popping to PC will cause return from subroutine (~same as "bx lr")
	);
}

void fail() {
    printf("Failed\n"); // set a break point here
    while(1) {
        tight_loop_contents();
    }
}

void ok() {
    printf("\nAll ok\n"); // set a break point here
}

int main(void) {

    const uint led_pin = 22;
    int len = 0;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

	// TODO: insert code here
	printf("\nExercise5\n");

	char test1[] = "Computer Architecture\n";
	const char test2[] = "Computer Architecture\n";
	reverse(test1);
	puts(test1);
	reverse(test1);
	puts(test1);
	if(strcmp(test1, test2)) {
		fail(); // error - string modified
	}
	char test3[] = "Johnny Ca$h:Live @Folsom\n";
	const char test4[] = "Johnny Ca$h:Live @Folsom\n";
	reverse(test3);
	puts(test3);
	reverse(test3);
	puts(test3);
	if(strcmp(test3, test4)) {
        fail(); // error - string modified
	}

	char test5[] = "If you like to gamble, I tell you I'm your man\n";
	const char test6[] = "If you like to gamble, I tell you I'm your man\n";
	reverse(test5);
	puts(test5);
	reverse(test5);
	puts(test5);
	if(strcmp(test5, test6)) {
        fail(); // error - string modified
	}

    ok();

    // Loop forever and blink an LED to indicaste success
    while (true) {
        // Blink LED
        gpio_put(led_pin, true);
        sleep_ms(1000);
        gpio_put(led_pin, false);
        sleep_ms(1000);
    }
	return 0 ;
}
