// implement fork from user space

#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
static void
pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	int r;

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at uvpt
	//   (see <inc/memlayout.h>).

	// LAB 4: Your code here.

	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.

	// LAB 4: Your code here.

	panic("pgfault not implemented");
}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why do we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
//
static int
duppage(envid_t envid, unsigned pn)
{
	int r;
  uint32_t perm = PTE_P | PTE_COW;
  void * addr = (void *) pn * PGSIZE;

  if (uvpt[pn] & PTE_COW | uvpt[pn] & PTE_W) {
    if (uvpt[pn] | PTE_U) {
      perm |= PTE_U;
    }
    
    // Map child.
    if ((r = sys_page_map(thisenv->env_id, addr, envid, addr, perm)) < 0) {
      panic("sys_page_map: %e", r);
    }

    // Map parent.
    if ((r = sys_page_map(thisenv->env_id, addr, thisenv->env_id, addr, perm)) < 0) {
      panic("sys_page_map: %e", r);
    }
  } else {
    // Just map
    if ((r = sys_page_map(thisenv->env_id, addr, envid, addr, uvpt[pn] & 0xFFF)) < 0) {
      panic("sys_page_map: %e", r);
    }
  }

	// LAB 4: Your code here.
	panic("duppage not implemented");
	return 0;
}

//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use uvpd, uvpt, and duppage.
//   Remember to fix "thisenv" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	// LAB 4: Your code here.
  // Install pgfault handler.
  envid_t envid;
  unsigned pn;
	set_pgfault_handler(pgfault);

  // Create a child.
  envid = sys_exofork();
  if (envid < 0) {
    panic("Failed to create fork env.");
  }

	if (envid == 0) {
		// We're the child.
		// The copied value of the global variable 'thisenv'
		// is no longer valid (it refers to the parent!).
		// Fix it and return 0.
		thisenv = &envs[ENVX(sys_getenvid())];
		return 0;
	}

  // Copy each page below UTOP.
  for (pn = 0; pn < UTOP / PGSIZE; pn ++) {
      uint32_t pdx = ROUNDDOWN(pn, NPDENTRIES) / NPDENTRIES;
          if ((uvpd[pdx] & PTE_P) == PTE_P &&
               ((uvpt[pn] & PTE_P) == PTE_P)) {
                    duppage(child_envid, page_num);
              }
  }

  // Set up child's exception stack.
  if (sys_page_alloc(envid, (void *) (UXSTACKTOP - PGSIZE), PTE_P | PTE_U | PTE_W) < 0) {
    panic("Failed to alloc stack for child");
  }


  // Set user page fault entrypoint.
  if (sys_env_set_pgfault_upcall(envid, pgfault) != 0) {
    panic("Failed to set up page fault handler");
  }

  // Set child env runnable.
	if (sys_env_set_status(envid, ENV_RUNNABLE) < 0) {
		panic("sys_env_set_status failed.");
  }

	panic("fork not implemented");
}

// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
