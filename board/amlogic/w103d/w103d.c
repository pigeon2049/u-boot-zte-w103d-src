// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2026 pigeon2049 <lai@live.cn>
 *
 * Derived from board/amlogic/sei510/sei510.c (BayLibre, Neil Armstrong).
 * Only the serial-number string differs; the S905L3A shares the G12A
 * serial/ethaddr layout with the other G12A set-top-box boards.
 */

#include <dm.h>
#include <env.h>
#include <env_internal.h>
#include <init.h>
#include <net.h>
#include <asm/io.h>
#include <asm/arch/axg.h>
#include <asm/arch/sm.h>
#include <asm/arch/eth.h>
#include <asm/arch/mem.h>

int misc_init_r(void)
{
	meson_generate_serial_ethaddr();

	env_set("serial#", "AMLG12AW103D");

	return 0;
}
