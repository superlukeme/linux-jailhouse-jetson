#
# Jailhouse, a Linux-based partitioning hypervisor
#
# Copyright (c) Siemens AG, 2013
#
# Authors:
#  Jan Kiszka <jan.kiszka@siemens.com>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.
#

LINUXINCLUDE := -I$(src)/../hypervisor/arch/$(SRCARCH)/include \
		-I$(src)/../hypervisor/include \
		-I$(src)/../include
KBUILD_CFLAGS := -Wextra

ifneq ($(wildcard $(obj)/../include/jailhouse/config.h),)
KBUILD_CFLAGS += -include $(obj)/../include/jailhouse/config.h
endif

OBJCOPYFLAGS := -O binary

GCOV_PROFILE := n

CONFIGS = $(shell cd $(src); ls $(SRCARCH)/*.c)

always := $(CONFIGS:.c=.cell)

targets += $(CONFIGS:.c=.o) $(CONFIGS:.c=.cell)

# prevent deleting intermediate files which would cause rebuilds
.SECONDARY: $(addprefix $(obj)/,$(CONFIGS:.c=.o))

$(obj)/%.cell: $(obj)/%.o
	$(call if_changed,objcopy)
