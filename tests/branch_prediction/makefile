all: branch_always branch_predictably branch_randomly branch_mixed

branch_always:
	gcc -O0 branch_always.c -o branch_always

branch_predictably:
	gcc -O0 branch_predictably.c -o branch_predictably

branch_randomly:
	gcc -O0 branch_randomly.c -o branch_randomly

branch_mixed:
	gcc -O0 branch_mixed.c -o branch_mixed

clean:
	rm branch_always branch_predictably branch_randomly branch_mixed
