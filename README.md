tddb44-src
==========

**Modif 03-01-2015 ** : Update scanner.l to take function, close_scope (wrong management of closing the hash_links) and install_symbol (doing sym_pos++ too early) that creates issues. All tests are satisfied".
**TEST lab2 (in tddb44-src folder)**
diff results/lab2/symtabtest1.trace src/trace/symtab2a.trace
diff results/lab2/symtabtest2.trace src/trace/symtab2b.trace
diff results/lab2/symtabtest3.trace src/trace/symtab2c.trace


**Modif 03-01-2015 **:Create new branch on git repo and preparation for the lab have been done.

**TEST lab3 (in tddb44-src/results/lab3 folder)**
./diesel -a -b -c -f -p parstest1.d	
./diesel -a -b -c -f -p parstest2.d
./diesel -a -b -c -f -p parstest3.d

diff parstest1.trace parstesttest1.trace		Passed
diff parstest2.trace parstesttest2.trace		Passed (give more error info)
diff parstest3.trace parstesttest3.trace		Passed

**TEST 4
./diesel -a -b -c -f -p semtest1.d 
./diesel -a -b -c -f -p semtest2.d 

diff semtest1.trace semtesttest1.trace
diff semtest2.trace semtesttest2.trace

