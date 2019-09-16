 
#!/bin/sh

if "$1" = "start"; than
	~/qemu/qemu-4.1.0/riscv64-softmmu/qemu-system-riscv64 -M virt -nographic -kernel prplHypervisor.elf -serial mon:stdio -s -S &
elif "$1" = "debug"; than
	~/qemu/qemu-4.1.0/riscv64-softmmu/qemu-system-riscv64 -M virt -nographic -kernel prplHypervisor.elf -serial mon:stdio &
elif "$1" = "kill"; than
	killall qemu-system-riscv64
fi


