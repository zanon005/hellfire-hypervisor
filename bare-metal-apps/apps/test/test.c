
/* Simple UART and Blink Bare-metal application sample using virtualized IO. */

#include <arch.h>
#include <libc.h>
#include <hypercalls.h>
#include <platform.h>
#include <io.h>


int main() {
	uint64_t id;
	uint32_t test;

	enable_interrupts();

	while(1){
		id = get_guestid();
		printf("Guest %d running! sie:%x sip:%x\n", id,read_csr(sie),read_csr(sip));
		mdelay(1000);
	}
    
	
	return 0;
}

