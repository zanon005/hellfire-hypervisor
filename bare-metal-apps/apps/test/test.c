
/* Simple UART and Blink Bare-metal application sample using virtualized IO. */

#include <arch.h>
#include <libc.h>
#include <hypercalls.h>
#include <platform.h>
#include <io.h>


int main() {
	uint64_t id;
	uint32_t test;

	//limpa interrupções.
	write_csr(sip,read_csr(sip)^0x2ULL);

	//habilita interrupções.
	write_csr(sie,read_csr(sie)|0x2ULL);


	while(1){
		id = get_guestid();
		printf("Guest %d running! sie:%x sip:%x\n", id,read_csr(sie),read_csr(sip));
		mdelay(1000);
	}
    
	
	return 0;
}

