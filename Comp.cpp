#include <stdio.h>
#include <string.h>
#include <string>     
#include "comp_functions.h"

/*
1) push ax, push [5], push [ax], push [ax + 2], то же самое с pop
2) call, условные и (безусловные)+ jump.
3) Процессор.
*/

int main()
{
	int len = 0;
	struct label labels[10000] = {};
	int num_labels = 0;

	int* code = AssemblerMain(&len, labels, &num_labels);
	if (code == NULL)
	{
		return -1;
	}
	FILE* Comp_Code = fopen("..\\Comp_Code.bin", "wb");

	fprintf(Comp_Code, "%d", code[0]);
	for (int i = 1; i < len; i++)
	{
		fprintf(Comp_Code, " %d", code[i]);
	}
	fclose(Comp_Code);

	return 0;
}