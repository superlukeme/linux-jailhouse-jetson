/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (C) 2018 Evidence Srl
 *
 * Authors:
 *  Luca Cuomo <l.cuomo@evidence.eu.com>
 *
 * This is confidential code meant only for usage by Magneti Marelli
 * in the context of the HERCULES H2020 European project.
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

struct {
	struct jailhouse_cell_desc cell;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[6];
	struct jailhouse_irqchip irqchips[1];
	struct jailhouse_pci_device pci_devices[4];
} __attribute__((packed)) config = {
	.cell = {
		.signature = JAILHOUSE_CELL_DESC_SIGNATURE,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.name = "jetson-tx2-EVI-ee1",
		.flags = JAILHOUSE_CELL_PASSIVE_COMMREG,

		.cpu_set_size = sizeof(config.cpus),
		.num_memory_regions = ARRAY_SIZE(config.mem_regions),
		.num_irqchips = ARRAY_SIZE(config.irqchips),
		.num_pci_devices = ARRAY_SIZE(config.pci_devices),
		.vpci_irq_base = 300,

	},

	.cpus = {
		0x1,
	},

	.mem_regions = {
		/* UART */ {
			.phys_start = 0x3100000,
			.virt_start = 0x3100000,
			.size = 0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO | JAILHOUSE_MEM_ROOTSHARED,
		},
		/* RAM */ {
			.phys_start = 0x270000000,
			.virt_start = 0,
			.size = 0x400000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_LOADABLE,
		},

		/* IVHSMEM  1 (LINUX -> EE1) */ {
                        .phys_start = 0x275000000,
                        .virt_start = 0x275000000,
                        .size = 0x10000,
                        .flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE
                        | JAILHOUSE_MEM_ROOTSHARED,

                },

                /* IVHSMEM  2 (LINUX <- E1) */ {
                        .phys_start = 0x275020000,
                        .virt_start = 0x275020000,
                        .size = 0x10000,
                        .flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE
                         | JAILHOUSE_MEM_ROOTSHARED,
                },

		/* IVHSMEM  3 (EE1 -> EE2)*/ {
                        .phys_start = 0x275040000,
                        .virt_start = 0x275040000,
                        .size = 0x10000,
                        .flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE ,

                },
		/* IVHSMEM  4 (EE1 <- EE2)*/ {
                        .phys_start = 0x275060000,
                        .virt_start = 0x275060000,
                        .size = 0x10000,
                        .flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE ,
                },

	},

	.irqchips = {
		/* GIC */
		{
			.address = 0x03881000,
			.pin_base = 288,
			.pin_bitmap = {
				0,
				0xF << (332 - 320) /* irq 332 and 335 */
			},
		},
	},

	.pci_devices = {
            	/* PCI LINUX -> EE1 Bdf 0000.00.1 */
    		{
                        .type = JAILHOUSE_PCI_TYPE_IVSHMEM,
                        .bdf = 0x1,
                        .bar_mask = {
                                0xffffff00, 0xffffffff, 0x00000000,
                                0x00000000, 0x00000000, 0x00000000,
                        },

                        /*num_msix_vectors needs to be 0 for INTx operation*/
                        .num_msix_vectors = 0,
                        .shmem_region = 2,
                        .shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
                        .domain = 0x0,
                },
		/* PCI LINUX <- EE1 Bdf 0000.01.0 */
                {
                        .type = JAILHOUSE_PCI_TYPE_IVSHMEM,
                        .bdf = 0x1 << 3,
                        .bar_mask = {
                                0xffffff00, 0xffffffff, 0x00000000,
                                0x00000000, 0x00000000, 0x00000000,
                        },

                        /*num_msix_vectors needs to be 0 for INTx operation*/
                        .num_msix_vectors = 0,
                        .shmem_region = 3,
                        .shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
			.domain = 0x0,
                },
		/* PCI EE1 -> EE2 Bdf 0000.00.2 */
                {
                        .type = JAILHOUSE_PCI_TYPE_IVSHMEM,
                        .bdf = 0x2,
                        .bar_mask = {
                                0xffffff00, 0xffffffff, 0x00000000,
                                0x00000000, 0x00000000, 0x00000000,
                        },

                        /*num_msix_vectors needs to be 0 for INTx operation*/
                        .num_msix_vectors = 0,
                        .shmem_region = 4,
                        .shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
                        .domain = 0x0,

                },
		/* PCI EE1 <- EE2 Bdf 0000.02.0 */
                {
                        .type = JAILHOUSE_PCI_TYPE_IVSHMEM,
                        .bdf = 0x2 << 3,
                        .bar_mask = {
                                0xffffff00, 0xffffffff, 0x00000000,
                                0x00000000, 0x00000000, 0x00000000,
                        },

                        /*num_msix_vectors needs to be 0 for INTx operation*/
                        .num_msix_vectors = 0,
                        .shmem_region = 5,
                        .shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
			.domain = 0x0,
                },
        },
};
