all: branch_always branch_predictably branch_randomly branch_mixed

branch_always: branch_always.c
	gcc -O0 branch_always.c -o branch_always

branch_predictably: branch_predictably.c
	gcc -O0 branch_predictably.c -o branch_predictably

branch_randomly: branch_randomly.c
	gcc -O0 branch_randomly.c -o branch_randomly

branch_mixed: branch_mixed.c
	gcc -O0 branch_mixed.c -o branch_mixed

clean:
	rm branch_always branch_predictably branch_randomly branch_mixed
