/* Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * EXYNOS4 MCT(Multi-Core Timer) support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/clockchips.h>
#include <linux/cpu.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/percpu.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/clocksource.h>
#include <linux/sched_clock.h>
#include <linux/data_protection.h>
#include <asm/proc-fns.h>
#include <linux/rpmb.h>
#include <linux/get_pmd.h>

#define EXYNOS4_MCTREG(x)		(x)
#define EXYNOS4_MCT_G_CNT_L		EXYNOS4_MCTREG(0x100)
#define EXYNOS4_MCT_G_CNT_U		EXYNOS4_MCTREG(0x104)
#define EXYNOS4_MCT_G_CNT_WSTAT		EXYNOS4_MCTREG(0x110)
#define EXYNOS4_MCT_G_COMP0_L		EXYNOS4_MCTREG(0x200)
#define EXYNOS4_MCT_G_COMP0_U		EXYNOS4_MCTREG(0x204)
#define EXYNOS4_MCT_G_COMP0_ADD_INCR	EXYNOS4_MCTREG(0x208)
#define EXYNOS4_MCT_G_TCON		EXYNOS4_MCTREG(0x240)
#define EXYNOS4_MCT_G_INT_CSTAT		EXYNOS4_MCTREG(0x244)
#define EXYNOS4_MCT_G_INT_ENB		EXYNOS4_MCTREG(0x248)
#define EXYNOS4_MCT_G_WSTAT		EXYNOS4_MCTREG(0x24C)
#define _EXYNOS4_MCT_L_BASE		EXYNOS4_MCTREG(0x300)
#define EXYNOS4_MCT_L_BASE(x)		(_EXYNOS4_MCT_L_BASE + (0x100 * x))
#define EXYNOS4_MCT_L_MASK		(0xffffff00)

#define MCT_L_TCNTB_OFFSET		(0x00)
#define MCT_L_ICNTB_OFFSET		(0x08)
#define MCT_L_TCON_OFFSET		(0x20)
#define MCT_L_INT_CSTAT_OFFSET		(0x30)
#define MCT_L_INT_ENB_OFFSET		(0x34)
#define MCT_L_WSTAT_OFFSET		(0x40)
#define MCT_G_TCON_START		(1 << 8)
#define MCT_G_TCON_COMP0_AUTO_INC	(1 << 1)
#define MCT_G_TCON_COMP0_ENABLE		(1 << 0)
#define MCT_L_TCON_INTERVAL_MODE	(1 << 2)
#define MCT_L_TCON_INT_START		(1 << 1)
#define MCT_L_TCON_TIMER_START		(1 << 0)

#define TICK_BASE_CNT	1
#define MCT __attribute__ ((section(".mct")))
#define TEXT_ALIGN __attribute__ ((aligned(4096)))
#define KDP_CODE __attribute__ ((section ("kdp_text")))

enum {
	MCT_INT_SPI,
	MCT_INT_PPI
};

enum {
	MCT_G0_IRQ,
	MCT_G1_IRQ,
	MCT_G2_IRQ,
	MCT_G3_IRQ,
	MCT_L0_IRQ,
	MCT_L1_IRQ,
	MCT_L2_IRQ,
	MCT_L3_IRQ,
	MCT_L4_IRQ,
	MCT_L5_IRQ,
	MCT_L6_IRQ,
	MCT_L7_IRQ,
	MCT_NR_IRQS,
};
void null_func(uint64_t dump){
	return;
}

#ifdef CONFIG_SWTPM_PROTECTION
unsigned long MCT tick_base;
int MCT tpm_enabled = 0;

uint64_t MCT addr_tpmTime = 0;
EXPORT_SYMBOL(addr_tpmTime);

uint64_t MCT prevCounter=0;
#endif
#ifdef CONFIG_DATA_PROTECTION
void MCT *mct_base;
#endif
static void __iomem MCT *reg_base;
static unsigned long MCT clk_rate;
void MCT (*NvOps) (uint64_t) = &null_func;
EXPORT_SYMBOL(NvOps);
int TEXT_ALIGN null_var;

static unsigned int mct_int_type;
static int mct_irqs[MCT_NR_IRQS];

struct mct_clock_event_device {
	struct clock_event_device evt;
	unsigned long base;
	char name[10];
};

void KDP_CODE secure_write(unsigned int value, void* addr){
	if(addr >= (reg_base + 0x100) && addr < reg_base + 0x110)
		return;
	writel_relaxed(value, addr);
}

static void exynos4_mct_write(unsigned int value, unsigned long offset)
{
	unsigned long stat_addr;
	u32 mask;
	u32 i;
	
	unsigned long shadow_flags = 0;

	if(kdp_enabled)
		shadow_flags = shadow_entry_gate();

	secure_write(value, reg_base + offset);

	if(kdp_enabled)
		shadow_exit_gate(shadow_flags);


	if (likely(offset >= EXYNOS4_MCT_L_BASE(0))) {
		stat_addr = (offset & EXYNOS4_MCT_L_MASK) + MCT_L_WSTAT_OFFSET;
		switch (offset & ~EXYNOS4_MCT_L_MASK) {
		case MCT_L_TCON_OFFSET:
			mask = 1 << 3;		/* L_TCON write status */
			break;
		case MCT_L_ICNTB_OFFSET:
			mask = 1 << 1;		/* L_ICNTB write status */
			break;
		case MCT_L_TCNTB_OFFSET:
			mask = 1 << 0;		/* L_TCNTB write status */
			break;
		default:
			return;
		}
	} else {
		switch (offset) {
		case EXYNOS4_MCT_G_TCON:
			stat_addr = EXYNOS4_MCT_G_WSTAT;
			mask = 1 << 16;		/* G_TCON write status */
			break;
		case EXYNOS4_MCT_G_COMP0_L:
			stat_addr = EXYNOS4_MCT_G_WSTAT;
			mask = 1 << 0;		/* G_COMP0_L write status */
			break;
		case EXYNOS4_MCT_G_COMP0_U:
			stat_addr = EXYNOS4_MCT_G_WSTAT;
			mask = 1 << 1;		/* G_COMP0_U write status */
			break;
		case EXYNOS4_MCT_G_COMP0_ADD_INCR:
			stat_addr = EXYNOS4_MCT_G_WSTAT;
			mask = 1 << 2;		/* G_COMP0_ADD_INCR w status */
			break;
		case EXYNOS4_MCT_G_CNT_L:
			stat_addr = EXYNOS4_MCT_G_CNT_WSTAT;
			mask = 1 << 0;		/* G_CNT_L write status */
			break;
		case EXYNOS4_MCT_G_CNT_U:
			stat_addr = EXYNOS4_MCT_G_CNT_WSTAT;
			mask = 1 << 1;		/* G_CNT_U write status */
			break;
		default:
			return;
		}
	}

	/* Wait maximum 1 ms until written values are applied */
	for (i = 0; i < loops_per_jiffy / 1000 * HZ; i++)
		if (readl_relaxed(reg_base + stat_addr) & mask) {
			if(kdp_enabled)
				shadow_flags = shadow_entry_gate();
			secure_write(mask, reg_base + stat_addr);
			if(kdp_enabled)
				shadow_exit_gate(shadow_flags);
			return;
		}

	panic("MCT hangs after writing %d (offset:0x%lx)\n", value, offset);
//	if(kdp_enabled)
//		shadow_exit_gate(shadow_flags);

}

/* Clocksource handling */
static void exynos4_mct_frc_start(void)
{
	u32 reg;

	reg = readl_relaxed(reg_base + EXYNOS4_MCT_G_TCON);
	reg |= MCT_G_TCON_START;
	exynos4_mct_write(reg, EXYNOS4_MCT_G_TCON);
}

/**
 * exynos4_read_count_64 - Read all 64-bits of the global counter
 *
 * This will read all 64-bits of the global counter taking care to make sure
 * that the upper and lower half match.  Note that reading the MCT can be quite
 * slow (hundreds of nanoseconds) so you should use the 32-bit (lower half
 * only) version when possible.
 *
 * Returns the number of cycles in the global counter.
 */
static u64 exynos4_read_count_64(void)
{
	unsigned int lo, hi;
	u32 hi2 = readl_relaxed(reg_base + EXYNOS4_MCT_G_CNT_U);

	do {
		hi = hi2;
		lo = readl_relaxed(reg_base + EXYNOS4_MCT_G_CNT_L);
		hi2 = readl_relaxed(reg_base + EXYNOS4_MCT_G_CNT_U);
	} while (hi != hi2);

	return ((cycle_t)hi << 32) | lo;
}

/**
 * exynos4_read_count_32 - Read the lower 32-bits of the global counter
 *
 * This will read just the lower 32-bits of the global counter.  This is marked
 * as notrace so it can be used by the scheduler clock.
 *
 * Returns the number of cycles in the global counter (lower 32 bits).
 */
static u32 notrace exynos4_read_count_32(void)
{
	u32 count;
	count = readl_relaxed(reg_base + EXYNOS4_MCT_G_CNT_L);
	return count;
}

static cycle_t exynos4_frc_read(struct clocksource *cs)
{
	return exynos4_read_count_32();
}

static void exynos4_frc_resume(struct clocksource *cs)
{
	exynos4_mct_frc_start();
}

static struct clocksource mct_frc = {
	.name		= "mct-frc",
	.rating		= 400,
	.read		= exynos4_frc_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
	.resume		= exynos4_frc_resume,
};

static u64 notrace exynos4_read_sched_clock(void)
{
	return exynos4_read_count_32();
}

static struct delay_timer exynos4_delay_timer;

static cycles_t exynos4_read_current_timer(void)
{
	BUILD_BUG_ON_MSG(sizeof(cycles_t) != sizeof(u32),
			 "cycles_t needs to move to 32-bit for ARM64 usage");
	return exynos4_read_count_32();
}

static void __init exynos4_clocksource_init(void)
{
	exynos4_mct_frc_start();

	exynos4_delay_timer.read_current_timer = &exynos4_read_current_timer;
	exynos4_delay_timer.freq = clk_rate;
	register_current_timer_delay(&exynos4_delay_timer);

	if (clocksource_register_hz(&mct_frc, clk_rate))
		panic("%s: can't register clocksource\n", mct_frc.name);

	sched_clock_register(exynos4_read_sched_clock, 32, clk_rate);
}

static void exynos4_mct_comp0_stop(void)
{
	unsigned int tcon;

	tcon = readl_relaxed(reg_base + EXYNOS4_MCT_G_TCON);
	tcon &= ~(MCT_G_TCON_COMP0_ENABLE | MCT_G_TCON_COMP0_AUTO_INC);

	exynos4_mct_write(tcon, EXYNOS4_MCT_G_TCON);
	exynos4_mct_write(0, EXYNOS4_MCT_G_INT_ENB);
}

static void exynos4_mct_comp0_start(enum clock_event_mode mode,
				    unsigned long cycles)
{
	unsigned int tcon;
	cycle_t comp_cycle;

	tcon = readl_relaxed(reg_base + EXYNOS4_MCT_G_TCON);

	if (mode == CLOCK_EVT_MODE_PERIODIC) {
		tcon |= MCT_G_TCON_COMP0_AUTO_INC;
		exynos4_mct_write(cycles, EXYNOS4_MCT_G_COMP0_ADD_INCR);
	}

	comp_cycle = exynos4_read_count_64() + cycles;
	exynos4_mct_write((u32)comp_cycle, EXYNOS4_MCT_G_COMP0_L);
	exynos4_mct_write((u32)(comp_cycle >> 32), EXYNOS4_MCT_G_COMP0_U);

	exynos4_mct_write(0x1, EXYNOS4_MCT_G_INT_ENB);

	tcon |= MCT_G_TCON_COMP0_ENABLE;
	exynos4_mct_write(tcon , EXYNOS4_MCT_G_TCON);
}

static int exynos4_comp_set_next_event(unsigned long cycles,
				       struct clock_event_device *evt)
{
	exynos4_mct_comp0_start(evt->mode, cycles);

	return 0;
}

static void exynos4_comp_set_mode(enum clock_event_mode mode,
				  struct clock_event_device *evt)
{
	unsigned long cycles_per_jiffy;
	exynos4_mct_comp0_stop();

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		cycles_per_jiffy =
			(((unsigned long long) NSEC_PER_SEC / HZ * evt->mult) >> evt->shift);
		exynos4_mct_comp0_start(mode, cycles_per_jiffy);
		break;

	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_RESUME:
		break;
	}
}

static struct clock_event_device mct_comp_device = {
	.name		= "mct-comp",
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.rating		= 250,
	.set_next_event	= exynos4_comp_set_next_event,
	.set_mode	= exynos4_comp_set_mode,
};

static irqreturn_t exynos4_mct_comp_isr(int irq, void *dev_id)
{
	struct clock_event_device *evt = dev_id;
//	unsigned long shadow_flags = 0;

//	if(kdp_enabled)
//		shadow_flags = shadow_entry_gate();
	pr_info("Exynos mct_comp interrupt=%u\n",exynos4_read_count_32());

	exynos4_mct_write(0x1, EXYNOS4_MCT_G_INT_CSTAT);

	evt->event_handler(evt);
//	if(kdp_enabled)
//		shadow_exit_gate(shadow_flags);

	return IRQ_HANDLED;
}

static struct irqaction mct_comp_event_irq = {
	.name		= "mct_comp_irq",
	.flags		= IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= exynos4_mct_comp_isr,
	.dev_id		= &mct_comp_device,
};

static void exynos4_clockevent_init(void)
{
	mct_comp_device.cpumask = cpumask_of(0);
	clockevents_config_and_register(&mct_comp_device, clk_rate,
					0xf, 0xffffffff);
	setup_irq(mct_irqs[MCT_G0_IRQ], &mct_comp_event_irq);
}

static DEFINE_PER_CPU(struct mct_clock_event_device, percpu_mct_tick);

/* Clock event handling */
static void exynos4_mct_tick_stop(struct mct_clock_event_device *mevt)
{
	unsigned long tmp;
	unsigned long mask = MCT_L_TCON_INT_START | MCT_L_TCON_TIMER_START;
#ifdef CONFIG_SWTPM_PROTECTION
	unsigned long offset = tick_base + MCT_L_TCON_OFFSET;
#else
	unsigned long offset = mevt->base + MCT_L_TCON_OFFSET;
#endif

	tmp = readl_relaxed(reg_base + offset);
	if (tmp & mask) {
		tmp &= ~mask;
		exynos4_mct_write(tmp, offset);
	}
}

static void exynos4_mct_tick_start(unsigned long cycles,
				   struct mct_clock_event_device *mevt)
{
	unsigned long tmp;

	exynos4_mct_tick_stop(mevt);

	tmp = (1 << 31) | cycles;	/* MCT_L_UPDATE_ICNTB */

#ifdef CONFIG_SWTPM_PROTECTION
	/* update interrupt count buffer */
	exynos4_mct_write(tmp, tick_base + MCT_L_ICNTB_OFFSET);
	/* enable MCT tick interrupt */
	exynos4_mct_write(0x1, tick_base + MCT_L_INT_ENB_OFFSET);

	tmp = readl_relaxed(reg_base + tick_base + MCT_L_TCON_OFFSET);

#else
	/* update interrupt count buffer */
	exynos4_mct_write(tmp, mevt->base + MCT_L_ICNTB_OFFSET);
	/* enable MCT tick interrupt */
	exynos4_mct_write(0x1, mevt->base + MCT_L_INT_ENB_OFFSET);

	tmp = readl_relaxed(reg_base + mevt->base + MCT_L_TCON_OFFSET);
#endif
	tmp |= MCT_L_TCON_INT_START | MCT_L_TCON_TIMER_START |
	       MCT_L_TCON_INTERVAL_MODE;
#ifdef CONFIG_SWTPM_PROTECTION
	exynos4_mct_write(tmp, tick_base + MCT_L_TCON_OFFSET);
#else
	exynos4_mct_write(tmp, mevt->base + MCT_L_TCON_OFFSET);
#endif
}

static int exynos4_tick_set_next_event(unsigned long cycles,
				       struct clock_event_device *evt)
{
	struct mct_clock_event_device *mevt = this_cpu_ptr(&percpu_mct_tick);

	exynos4_mct_tick_start(cycles, mevt);

	return 0;
}

static inline void exynos4_tick_set_mode(enum clock_event_mode mode,
					 struct clock_event_device *evt)
{
	struct mct_clock_event_device *mevt = this_cpu_ptr(&percpu_mct_tick);
	unsigned long cycles_per_jiffy;

	exynos4_mct_tick_stop(mevt);

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		cycles_per_jiffy =
			(((unsigned long long) NSEC_PER_SEC / HZ * evt->mult) >> evt->shift);
		pr_info("Mul = %u, shift = %u, cycles_per_jiffy = %lu\n",evt->mult,evt->shift,cycles_per_jiffy);	
		exynos4_mct_tick_start(cycles_per_jiffy, mevt);
		break;

	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_RESUME:
		break;
	}
}

static void exynos4_mct_tick_clear(struct mct_clock_event_device *mevt)
{
	struct clock_event_device *evt = &mevt->evt;

	/*
	 * This is for supporting oneshot mode.
	 * Mct would generate interrupt periodically
	 * without explicit stopping.
	 */
	if (evt->mode != CLOCK_EVT_MODE_PERIODIC)
		exynos4_mct_tick_stop(mevt);

//	printk("Mevt base = %lx, tick base = %lx",mevt->base,tick_base);
#ifdef CONFIG_SWTPM_PROTECTION
	/* Clear the MCT tick interrupt */
	if (readl_relaxed(reg_base + tick_base + MCT_L_INT_CSTAT_OFFSET) & 1)
		exynos4_mct_write(0x1, tick_base + MCT_L_INT_CSTAT_OFFSET);
#else
	/* Clear the MCT tick interrupt */
	if (readl_relaxed(reg_base + mevt->base + MCT_L_INT_CSTAT_OFFSET) & 1)
		exynos4_mct_write(0x1, mevt->base + MCT_L_INT_CSTAT_OFFSET);
#endif
}

void KDP_CODE update_handler(void* unused){
	unsigned long shadow_flags = 0;
	if(kdp_enabled)
		shadow_flags = shadow_entry_gate();
	NvOps(12345);
	if(kdp_enabled)
		shadow_exit_gate(shadow_flags);


}

DECLARE_WORK(wq, &update_handler);
int setEval = 0;
static void KDP_CODE tick_handler(void *dev_id, unsigned long counter){
	struct mct_clock_event_device *mevt = dev_id;
	struct clock_event_device *evt = &mevt->evt;
	uint64_t *tpmTimer;
	unsigned long timeDiff;
	unsigned long adjusted;
	uint64_t time;
	unsigned long shadow_flags = 0;	
	uint64_t count;	
	int freq = 10;
	int skip_time = 150;
	int updateOn = 0;
	int timeSkip = 0;

	count = (uint64_t)exynos4_read_count_64();
	addr_tpmTime = count;
//	 if(updateOn == 1) {
		if(NvOps != &null_func && count - prevCounter >= clk_rate / freq){
			setEval = 1;
			prevCounter = count;
//			if(updateOn == 1)
				schedule_work(&wq);
//			else if(updateOn == 0){
//				if(timeSkip > freq * skip_time) {
//					updateOn = 1;
//				}
//				timeSkip++;	
//			}
		}
		else
			setEval = 0;
//	}
//	else if(updateOn == 0){ 
//                        if(timeSkip > freq * skip_time) {
//                               updateOn = 1;
//                        }
//                        timeSkip++;    
//       }

	exynos4_mct_tick_clear(mevt);

	evt->event_handler(evt);

}

//struct work_struct wq;
struct tasklet_struct my_tasklet;
//DECLARE_TASKLET(update_tasklet, update_handler, 0);


void clockUpdate(void){
	INIT_WORK(&wq, update_handler);
	schedule_work(&wq);
//	tasklet_schedule(&update_tasklet);
}

void KDP_CODE TEXT_ALIGN mct_text_end(void) {}

static irqreturn_t exynos4_mct_tick_isr(int irq, void *dev_id)
{
	unsigned long counter; 
	unsigned long shadow_flags = 0;

	if(kdp_enabled){
		shadow_flags = shadow_entry_gate();
	}

	tick_handler(dev_id,(unsigned long)exynos4_read_count_32());

	if(kdp_enabled){
		shadow_exit_gate(shadow_flags);
	}

	return IRQ_HANDLED;
}

static int exynos4_local_timer_setup(struct clock_event_device *evt)
{
	struct mct_clock_event_device *mevt;
	unsigned int cpu = smp_processor_id();

	mevt = container_of(evt, struct mct_clock_event_device, evt);
#ifdef CONFIG_SWTPM_PROTECTION
	tick_base = EXYNOS4_MCT_L_BASE(cpu);
#endif
	mevt->base = EXYNOS4_MCT_L_BASE(cpu);
	snprintf(mevt->name, sizeof(mevt->name), "mct_tick%d", cpu);

	evt->name = mevt->name;
	evt->cpumask = cpumask_of(cpu);
	evt->set_next_event = exynos4_tick_set_next_event;
	evt->set_mode = exynos4_tick_set_mode;
	evt->features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT;
	evt->rating = 450;

	exynos4_mct_write(TICK_BASE_CNT, mevt->base + MCT_L_TCNTB_OFFSET);

	if (mct_int_type == MCT_INT_SPI) {

		if (evt->irq == -1)
			return -EIO;

		irq_force_affinity(evt->irq, cpumask_of(cpu));
		enable_irq(evt->irq);
	} else {
		enable_percpu_irq(mct_irqs[MCT_L0_IRQ], 0);
	}
	clockevents_config_and_register(evt, clk_rate / (TICK_BASE_CNT + 1),
					0xf, 0x7fffffff);
	pr_info("clock rate = %ld\n",clk_rate);
	return 0;
}

static void exynos4_local_timer_stop(struct clock_event_device *evt)
{
	evt->set_mode(CLOCK_EVT_MODE_UNUSED, evt);
	if (mct_int_type == MCT_INT_SPI) {
		if (evt->irq != -1)
			disable_irq_nosync(evt->irq);
	} else {
		disable_percpu_irq(mct_irqs[MCT_L0_IRQ]);
	}
}

static int exynos4_mct_cpu_notify(struct notifier_block *self,
					   unsigned long action, void *hcpu)
{
	struct mct_clock_event_device *mevt;

	/*
	 * Grab cpu pointer in each case to avoid spurious
	 * preemptible warnings
	 */

	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_STARTING:
		mevt = this_cpu_ptr(&percpu_mct_tick);
		exynos4_local_timer_setup(&mevt->evt);
		break;
	case CPU_DYING:
		mevt = this_cpu_ptr(&percpu_mct_tick);
		exynos4_local_timer_stop(&mevt->evt);
		break;
	}
	return NOTIFY_OK;
}

static struct notifier_block exynos4_mct_cpu_nb = {
	.notifier_call = exynos4_mct_cpu_notify,
};

static void __init exynos4_timer_resources(struct device_node *np, void __iomem *base)
{
	int err, cpu;
	struct mct_clock_event_device *mevt = this_cpu_ptr(&percpu_mct_tick);
	struct clk *mct_clk, *tick_clk;

	tick_clk = np ? of_clk_get_by_name(np, "fin_pll") :
				clk_get(NULL, "fin_pll");
	if (IS_ERR(tick_clk))
		panic("%s: unable to determine tick clock rate\n", __func__);
	clk_rate = clk_get_rate(tick_clk);

	mct_clk = np ? of_clk_get_by_name(np, "mct") : clk_get(NULL, "mct");
	if (IS_ERR(mct_clk))
		panic("%s: unable to retrieve mct clock instance\n", __func__);
	clk_prepare_enable(mct_clk);

	reg_base = base;
#ifdef CONFIG_SWTPM_PROTECTION
	mct_base = base;
#endif
	if (!reg_base)
		panic("%s: unable to ioremap mct address space\n", __func__);

	if (mct_int_type == MCT_INT_PPI) {

		err = request_percpu_irq(mct_irqs[MCT_L0_IRQ],
					 exynos4_mct_tick_isr, "MCT",
					 &percpu_mct_tick);
		WARN(err, "MCT: can't request IRQ %d (%d)\n",
		     mct_irqs[MCT_L0_IRQ], err);
	} else {
		for_each_possible_cpu(cpu) {
			int mct_irq = mct_irqs[MCT_L0_IRQ + cpu];
			struct mct_clock_event_device *pcpu_mevt =
				per_cpu_ptr(&percpu_mct_tick, cpu);

			pcpu_mevt->evt.irq = -1;

			irq_set_status_flags(mct_irq, IRQ_NOAUTOEN);
			if (request_irq(mct_irq,
					exynos4_mct_tick_isr,
					IRQF_TIMER | IRQF_NOBALANCING,
					pcpu_mevt->name, pcpu_mevt)) {
				pr_err("exynos-mct: cannot register IRQ (cpu%d)\n",
									cpu);

				continue;
			}
			pcpu_mevt->evt.irq = mct_irq;
		}
	}

	err = register_cpu_notifier(&exynos4_mct_cpu_nb);
	if (err)
		goto out_irq;

	/* Immediately configure the timer on the boot CPU */
	exynos4_local_timer_setup(&mevt->evt);
	return;

out_irq:
	free_percpu_irq(mct_irqs[MCT_L0_IRQ], &percpu_mct_tick);
}

static void __init mct_init_dt(struct device_node *np, unsigned int int_type)
{
	u32 nr_irqs, i;

	mct_int_type = int_type;

	/* This driver uses only one global timer interrupt */
	mct_irqs[MCT_G0_IRQ] = irq_of_parse_and_map(np, MCT_G0_IRQ);

	/*
	 * Find out the number of local irqs specified. The local
	 * timer irqs are specified after the four global timer
	 * irqs are specified.
	 */
#ifdef CONFIG_OF
	nr_irqs = of_irq_count(np);
#else
	nr_irqs = 0;
#endif
	for (i = MCT_L0_IRQ; i < nr_irqs; i++)
		mct_irqs[i] = irq_of_parse_and_map(np, i);
	
	exynos4_timer_resources(np, of_iomap(np, 0));
	printk("Finish exynos4_timer_resoruces\n");
	exynos4_clocksource_init();
	printk("Finish clocksource init\n");
	exynos4_clockevent_init();
	printk("Finish clock event init\n");
}


static void __init mct_init_spi(struct device_node *np)
{
//	void* addr = (void*)0xc0b63000;
//	void* iomem = (void*)0xf0006000;
	mct_init_dt(np, MCT_INT_SPI);
//	kdp_protect_one_page(addr);
//	kdp_protect_one_page(iomem);
}

static void __init mct_init_ppi(struct device_node *np)
{
//	void* addr = (void*)0xc0b63000;
//	void* iomem = (void*)0xf0006000;
	mct_init_dt(np, MCT_INT_PPI);
//	kdp_protect_one_page(addr);
//	kdp_protect_one_page(iomem);
}
CLOCKSOURCE_OF_DECLARE(exynos4210, "samsung,exynos4210-mct", mct_init_spi);
CLOCKSOURCE_OF_DECLARE(exynos4412, "samsung,exynos4412-mct", mct_init_ppi);
