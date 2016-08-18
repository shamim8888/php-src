/*
   +----------------------------------------------------------------------+
   | Zend OPcache                                                         |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Dmitry Stogov <dmitry@zend.com>                             |
   +----------------------------------------------------------------------+
*/

#define GDB_JIT_NOACTION    0
#define GDB_JIT_REGISTER    1
#define GDB_JIT_UNREGISTYER 2

struct _gdb_jit_code_entry {
	struct _gdb_jit_code_entry *next_entry;
	struct _gdb_jit_code_entry *prev_entry;
	const char                 *symfile_addr;
	uint64_t                    symfile_size;
};

struct _gdb_jit_descriptor {
	uint32_t                    version;
	uint32_t                    action_flag;
	struct _gdb_jit_code_entry *relevant_entry;
	struct _gdb_jit_code_entry *first_entry;
};

struct _gdb_jit_descriptor __jit_debug_descriptor = {
	1, GDBJIT_NOACTION, NULL, NULL
};

zend_never_inline void __jit_debug_register_code()
{
	__asm__ __volatile__("");
}

int gdb_jit_register(const char*symfile_addr, uint64_t symfile_size)
{
	struct _gdb_jit_code_entry *entry;

	entry = (struct _gdb_jit_code_entry*) malloc(sizeof(struct _gdb_jit_code_entry));
	if (!entry) {
		return -1;
	}
	entry->symfile_addr = symfile_addr;
	entry->symfile_size = symfile_size;
	entry->prev_entry = NULL;
	entry->next_entry = __jit_debug_descriptor.first_entry;
	if (__jit_debug_descriptor.first_entry) {
		__jit_debug_descriptor.first_entry->prev_entry = entry;
	}
	__jit_debug_descriptor.first_entry = entry;
	__jit_debug_descriptor.relevant_entry = entry;

	__jit_debug_descriptor.action_flag = GDB_JIT_REGISTER;
	__jit_debug_register_code();

	return 0;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
