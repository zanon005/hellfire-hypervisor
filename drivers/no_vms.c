#include <hypercall.h>
#include <globals.h>
#include <driver.h>
#include <hypercall_defines.h>
#include <scheduler.h>

/**
 * @brief Driver init call.  
 */
void remove_vms(){
	uint32_t i;
	
	for(i=0;i<NVMACHINES;i++){
		queue_remtail(scheduler_info.vcpu_ready_list);
	}
    
    INFO("All VMs removed from execution.");

}

driver_init(remove_vms);

