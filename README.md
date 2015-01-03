tddb44-src
==========

**Modif 03-01-2015 ** : Update scanner.l to take function, close_scope (wrong management of closing the hash_links) and install_symbol (doing sym_pos++ too early) that creates issues
**TEST lab2**
diff results/lab2/symtabtest1.trace src/trace/symtab2a.trace
diff results/lab2/symtabtest2.trace src/trace/symtab2b.trace
diff results/lab2/symtabtest3.trace src/trace/symtab2c.trace

