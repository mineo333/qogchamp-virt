# IOAPIC Util
The point of this repo is to dump information about the IOAPIC in the Linux Kernel.

This utility was built for kernel version `5.13.0-39-generic`, but it should still work with later kernel versions.

# Using this utility

The one thing that needs to be configured for this to work is `io_apic_addr`. This is the base **physical** address of the IO-APIC
Without a proper `io_apic_addr`, this utility will literally fall over and die. 

To get the `io_apic_addr` I recommend you dump the ACPI tables. 

This can be done by doing the following in any terminal:

`sudo acpidump -b` - This will create a datadump of the ACPI tables in raw bytes via a series of `.dat` files. This is not very useful. 

`isal apic.dat` - This will produce a file called the `apic.dsl`. This is readable. Open it and scroll down until you see `[I/O APIC]`. In that section you will see an entry called `Address`. This value always prefixed with `FEC` and is always 32 bits. This is hte `io_apic_addr`. 

To change the `io_apic_addr`, open `ioapic_util.c` and change the static variable near the top.

Then compile this utility using `make` and run `sudo insmod ioapic_util.ko`.

The information about the IO-APIC will then be placed in the kernel ring buffer which may be access using `sudo dmesg`.