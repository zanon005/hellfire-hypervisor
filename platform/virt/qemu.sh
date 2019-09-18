 
#!/bin/sh

if [ "$#" -lt 1 ]; then
	echo "Usage: $0 start|debug|kill";
fi

if [ "$1" = "start" ]; then
	~/qemu/qemu-4.1.0/riscv64-softmmu/qemu-system-riscv64 -M virt -nographic -kernel prplHypervisor.elf -serial mon:stdio &
elif [ "$1" = "debug" ]; then
	~/qemu/qemu-4.1.0/riscv64-softmmu/qemu-system-riscv64 -M virt -nographic -kernel prplHypervisor.elf -serial mon:stdio -s -S &
elif [ "$1" = "kill" ]; then
	killall qemu-system-riscv64
fi


