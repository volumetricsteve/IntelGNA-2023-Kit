# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile - Intel GMM & Neural Network Accelerator (Intel GNA) Linux driver
# Copyright (c) 2018-2021, Intel Corporation.
#
ifneq ($(KERNELRELEASE),)

ccflags-y += -Werror
ccflags-y += -I$(src)/include
intel_gna-y := device.o \
							hw.o \
							ioctl.o \
							mem.o \
							pci.o \
							request.o \
							score.o

obj-m += intel_gna.o

else

PWD := $(shell pwd)
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$(PWD)

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif
