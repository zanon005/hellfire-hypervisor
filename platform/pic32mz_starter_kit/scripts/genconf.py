import xml.etree.ElementTree as ET
import sys
import io
from enum import Enum

#Intermediate Physical address of the first VM on the RAM
VMS_RAM_INTERMEDIATE_BASE_ADDRESS = 0x80010000
VMS_RAM_VIRTUAL_BASE_ADDRESS = 0x80000000

#Intermediate Physical address of the first VM on the FLASH
VMS_FLASH_INTERMEDIATE_BASE_ADDRESS = 0x9D010000
VMS_FLASH_VIRTUAL_BASE_ADDRESS = 0x9D000000

# NVMACHINES 1



#M5150 Page Sizes
class PageSizes(Enum):
    PAGEMASK_4KB = 4096
    PAGEMASK_16KB = 16384
    PAGEMASK_64KB = 65536
    PAGEMASK_256KB = 262144
    PAGEMASK_1MB = 1048576
    PAGEMASK_4MB = 4194304
    PAGEMASK_16MB = 16777216
    PAGEMASK_64MB = 67108864
    PAGEMASK_256MB = 268435456
    
#Convert Kseg0 addreess to physical address and
#remove the last 12 bits as required by the TLB entries
def kseg0_addr_to_tlb(addr):
    return (addr & 0x1FFFFFFF) >> 12

def initial_msg(file, inputxml):
    file.write("/* THIS FILE IS AUTOMATICALLY GENERATED. DO NOT MODIFY IT. */\n")
    file.write("/* See the input xml in %s */\n\n" % inputxml)

#Print the debug flags
def debug_flags(str, file):
    file.write('/* Debug UART prints */\n')
    for i in str.split():
        file.write('#define %s\n' % i)
    file.write('\n')
    file.write('/* Hypervisor kernel configuration and board info */\n')
    
#Write the platform configuration
def platform(root, file):
    for child in root:
        if child.tag == 'platform_string':
            file.write('#define CPU_ARCH \"%s\"\n' % child.text)
        if child.tag == 'system_clock':
            file.write('#define CPU_FREQ %s\n' % child.text)
        if child.tag == 'uart_speed':
            file.write('#define UART_SPEED %s\n' % child.text)

#Write the general system configuration
def process_system(root, file):
    for child in root:
        if child.tag == 'debug':
            debug_flags(child.text, file)
        if child.tag == 'cpu':
            file.write('#define CPU_ID \"%s\"\n' % child.text)
        if child.tag == 'platform':
            platform(child, file)
        if child.tag == 'scheduler_quantum_ms':
            file.write('#define MILISECOND (100000000 / 1000)\n')
            file.write ('#define QUANTUM  (%s * MILISECOND)\n' % child.text)
        if child.tag == 'heap_size_bytes':
            file.write ('#define HEAP_SIZE %s\n' % child.text)
        if child.tag == 'heap_address':
            file.write ('#define HEAP_ADDRESS %s\n' % child.text)
    file.write ('\n')

#Parse the memory mapping
def mem_map(root):
    mmap = []
    
    #read the list of memory mappings.
    for child in root:
        if child.tag == 'base_addr':
            info = {'base_addr': int(child.text, 16),
                    'size': child.attrib['size']}

            mmap.append(info)
            
    return mmap       

#Based on the size a memory area determines the best way to map it to the TLB entries.
def select_page_size(size):

    #check if it is possible to use a dual entry for flash
    half_sz = size/2
    selected_pg_size = None
    dual_entry = True
    for name, member in PageSizes.__members__.items():
        if member.value == half_sz:
            selected_pg_size = member
            break
        
    #dual entry can't be used
    if selected_pg_size is None:
        dual_entry = False
        for name, member in PageSizes.__members__.items():
            if member.value == size:
                selected_pg_size = member
                break
    
    return  selected_pg_size, dual_entry

#write the a TLB entry
def write_page_mapping(member, dual_entry, mem_size, mem_base, virtual_base, file):
    if member is None:
        print "Invalide flash size. Valide values are 16KB, 32KB, 64KB, 128KB, 256KB, 512KB or 1MB"
        sys.exit(1)
    
    if dual_entry == True:
        file.write('0x%05x, \t' % kseg0_addr_to_tlb(mem_base))
        file.write('0x%05x, \t' % kseg0_addr_to_tlb(mem_base+(mem_size/2)))
        file.write('%13s, \t' % member.name)                                      
    else:
        file.write('0x%05x, \t' % kseg0_addr_to_tlb(mem_base))
        file.write('      0, \t' )                   
        file.write('%13s, \t' % member.name) 
    
    file.write('0x%05x, \t' % kseg0_addr_to_tlb(virtual_base))
    file.write('2,\\\n')

#process a TLB entry and write it to the file
def process_mem_map(vm_number, mem_size, mem_base, virtual_base, file):
    file.write('\t%s,\t' % vm_number)
    member, dual_entry = select_page_size(mem_size)
    write_page_mapping(member, dual_entry, mem_size, mem_base, virtual_base, file)
    
        
#Configure VMs. This procedure is called for each vm_data entry.          
def conf_vms(root,file):
    
    vm_number = 1
    vm_ram_inter_addr = VMS_RAM_INTERMEDIATE_BASE_ADDRESS
    vm_flash_inter_addr = VMS_FLASH_INTERMEDIATE_BASE_ADDRESS
    total_tlb_entries = 0
    
    vms_info = ''
    
    file.write('/* VMs mapping */\n')
    conf_file.write('#define VMCONF {\\\n')
    for child in root:
        vm_entry_point = None
        os_type = None
        RAM_size_bytes = None
        flash_size_bytes = None
        num_tlb_entries = 2 #RAM and flash mappings are the minimal VM map requirement. They requires two TLB entries. 
        
        #Process the general VM data
        for vm_data in child:
            if vm_data.tag == 'os_type':
                os_type = vm_data.text
     
            if vm_data.tag == 'RAM_size_bytes':
                RAM_size_bytes = int(vm_data.text)

            if vm_data.tag == 'flash_size_bytes':
                flash_size_bytes = int(vm_data.text)
     
            if vm_data.tag == 'vm_entry_point':
                vm_entry_point = vm_data.text

            if vm_data.tag == 'memory_maps':
               num_tlb_entries += len(vm_data)
                

        if flash_size_bytes is None:
            print 'flash_size_bytes not defined.'
            sys.exit(1)
        
        if os_type is None:
            print 'os_type not defined.'
            sys.exit(1)
        
        if RAM_size_bytes is None:
            print 'RAM_size_bytes not defined.'
            sys.exit(1)

        if vm_entry_point is None:
            print 'vm_entry_point not defined.'
            sys.exit(1)

        if num_tlb_entries is None:
            print 'Memory map not defined.'
            sys.exit(1)
           
        #Address where the VM is in the RAM as seeing by the hypervisor (physical intermediate address)
        file.write('\t0x%x, ' % vm_ram_inter_addr)
        
        #VM size not used. Keep zero.
        file.write('0,\t')
     
        #Number of tlb entries.
        file.write('%s,\t' % num_tlb_entries)
     
        #OS type.
        file.write('%s,\t' % os_type)
     
        #interrupt redirect. Not used. Keep 0
        file.write('0,\t')

        #VM entry point
        file.write('%s, \\\n' % vm_entry_point)
     
        mmap = None
        for vm_data in child:
            if vm_data.tag == 'memory_maps':
                mmap = mem_map(vm_data)
       
        process_mem_map(vm_number, flash_size_bytes, vm_flash_inter_addr, VMS_FLASH_VIRTUAL_BASE_ADDRESS, file)
        #Increment the position of the next VM on flash.
        vm_flash_inter_addr += flash_size_bytes
        process_mem_map(vm_number, RAM_size_bytes, vm_ram_inter_addr, VMS_RAM_VIRTUAL_BASE_ADDRESS, file)
        #Increment the position of the next VM on RAM.
        vm_ram_inter_addr += RAM_size_bytes
        
        vms_info += 'VM'+ str(vm_number)+ ' \t' + str(flash_size_bytes) + ' \t' + str(RAM_size_bytes) + '\n'
        
        for m in mmap:
            process_mem_map(vm_number, int(m['size']), int(m['base_addr']), int(m['base_addr']), file)
            
        vm_number+=1
        total_tlb_entries += num_tlb_entries
        if total_tlb_entries > 15:
            print "Your are using too much TLB entries."
            sys.exit(1)
    
    conf_file.write('\t0, \t      0, \t        0, \t       0, \t       0, \t    0}\n')
    
    file.write('\n')
    
    file.write('#define NVMACHINES %d\n' % (vm_number-1))
    
    return vms_info

#Start the xml processament.                

if len(sys.argv) < 2:
    print 'Sintaxe: %s <xml input file>' % sys.argv[0]
    sys.exit(1)
    
tree = ET.parse(sys.argv[1])
root = tree.getroot()

conf_file = open("include/config.h", "w")
initial_msg(conf_file, sys.argv[1])

for child in root:
    if child.tag == 'system':
        process_system(child, conf_file)
    if child.tag == 'virtual_machines':
        vms_info = conf_vms(child, conf_file)
        
        
conf_file.write('\n#define VMCONF_RT {0}\n')

conf_file.write('\n/* VMs info \n')
conf_file.write('VM# \tflash_size \tram_size\n')                    
conf_file.write('%s' % vms_info)                    
conf_file.write('*/')                    
conf_file.close()