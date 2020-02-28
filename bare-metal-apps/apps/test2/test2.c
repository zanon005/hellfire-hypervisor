
/* Simple UART and Blink Bare-metal application sample using virtualized IO. */

#include <arch.h>
#include <libc.h>
#include <hypercalls.h>
#include <platform.h>
#include <io.h>


int main() {
	uint64_t id;
	while(1){
		id = get_guestid();
		printf("Guest %d running!\n", id);
		mdelay(1000);
	}
    
	
	return 0;
}

