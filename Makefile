KERNELDIR := /home/haosou/linux-xlnx
PWD :=$(shell pwd)
MAKE := make 
obj-m := $(MODNAME).o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
cp:
	cp  *.ko $(TARGETDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers