tddb44-src
==========

**Modif 03-01-2015: Update scanner.l to take function, close_scope (wrong management of closing the hash_links) and install_symbol (doing sym_pos++ too early) that creates issues. All tests are satisfied".
**TEST lab2 (in tddb44-src folder)**
diff results/lab2/symtabtest1.trace src/trace/symtab2a.trace
diff results/lab2/symtabtest2.trace src/trace/symtab2b.trace
diff results/lab2/symtabtest3.trace src/trace/symtab2c.trace


**Modif 03-01-2015: Create new branch on git repo and preparation for the lab have been done.
**Modif 05-01-2015: Add all code and found error in scanner.l for DIV and line 390 $5->value in var_decl)
**Info: See page 35 for diesel script flags
**TEST lab3 (in tddb44-src/results/lab3 folder)**
./diesel -a -b -c -f -p parstest1.d	
./diesel -a -b -c -f -p parstest2.d
./diesel -a -b -c -f -p parstest3.d
./diesel -a -b -c -f -p -y semtest1.d

diff parstest1.trace parstesttest1.trace		Passed
diff parstest2.trace parstesttest2.trace		Passed (give more error info)
diff parstest3.trace parstesttest3.trace		Passed
diff semtest1.trace semtesttest1.trace			Passed (not the same text because no type checking, but same symbol table)


**Modif 05-01-2015: Create new branch on git repo and preparation for the lab4 have been done.
**TEST 4
./diesel -b -p -f -y -a semtest1.d 				
./diesel -b -p -f -y -a semtest2.d				Not the good flags for this test (see next line)
./diesel -b -p -f  semtest2.d 					

diff semtest1.trace semtesttest1.trace
diff semtest2.trace semtesttest2.trace


**Modif 07-01-2015: Deicde to optimize what needed by defaut + ad_cast + binary_relation
We choose to implement the optimization of the cast (quite simple and short to do)
**TEST lab5
./diesel -b -p -f -a opttest1.d		(no opt)	
./diesel -b -p -a opttest1.d		(opt)	Passed (some differences due to optional features)	

diff opttest1.trace opttesttest1.trace

**Modif 09-01-2015: Update the result repository content and implement functions in quad.cc
**TEST lab6
./diesel -b -q -y quadtest1.d		Passed (may differs from example due to optional features last lab)

diff quadtest1.trace quadtesttest1.trace

**Modif 14-01-2015: Got issue with the codetest1.d without regarding the lab7 content. Need to search for it in previous modification (think about quads and optimize: may have done some illegal operations by errors or cast)
**TEST lab7
./diesel -y codetest1.d		

diff codetest1.trace codetesttest1.trace
