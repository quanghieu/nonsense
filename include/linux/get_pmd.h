#ifndef __LINUX_GET_PMD_H
#define __LINUX_GET_PMD_H

pmd_t* lookup_pmd(unsigned long address, struct task_struct *tsk);

#endif
