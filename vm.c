//James Smith 
//Systems Software 
//Professor Montagne
//COP 3402
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// to compile: gcc main.c
// to run AND send output to a txt file: ./a.out input.txt > output.txt
//*************questions to ask**********
// array will consist of two parts, first is all text from input file,
// second part is stack of results from text

// how to scan numbers when they have spaces in between them
// what is the ir
// how do numbers get pushed to stack
// do you need to make ir register a separate variable or can you just use the pas array for that
// are op codes accurrate

typedef struct instruction//struct for setting and quickly accessing Instructions
{

	int op;
	int l;
	int m;
} instruction;

																//Declaration of Global Variables
#define ARRAY_SIZE 500
int PAS[ARRAY_SIZE] = {0};
int bars[ARRAY_SIZE] = {0};
int bp, sp, pc, gp, halt;
instruction ir;



void print_stack(int pc, int bp, int sp, int gp, int *PAS, int *bars){ //recieves all aspects of pas to format and manipulate variables into stack format
	int count;

	printf("%d\t%d\t%d\t", pc, bp, sp); //stack columns

	for (count = gp; count <= sp; count++)
	{
		if (bars[count] == 1)
			printf("| %d ", PAS[count]);
		else
			printf("%d ", PAS[count]);
	}
	printf("\n");
}

int base(int *PAS, int bp, int l)
{
	int arb = bp; // arb = activation record base
	while (l > 0){ // find base L levels down
		arb = PAS[arb];
		l--;
	}
	return arb;
}

void print_instruction(int pc, instruction ir)
{
	char opname[4];
	switch (ir.op)
	{
	case 1:
		strcpy(opname, "LIT");
		break;
	case 2:
		switch (ir.m){
		case 0:
			strcpy(opname, "RTN");
			break;
		case 1:
			strcpy(opname, "ADD");
			break;
		case 2:
			strcpy(opname, "SUB");
			break;
		case 3:
			strcpy(opname, "MUL");
			break;
		case 4:
			strcpy(opname, "DIV");
			break;
		case 5:
			strcpy(opname, "EQL");
			break;
		case 6:
			strcpy(opname, "NEQ");
			break;
		case 7:
			strcpy(opname, "LSS");
			break;
		case 8:
			strcpy(opname, "LEQ");
			break;
		case 9:
			strcpy(opname, "GTR");
			break;
		case 10:
			strcpy(opname, "GEQ");
			break;
		default:
			strcpy(opname, "err");
			break;
		}
		break;
	case 3:
		strcpy(opname, "LOD");
		break;
	case 4:
		strcpy(opname, "STO");
		break;
	case 5:
		strcpy(opname, "CAL");
		break;
	case 6:
		strcpy(opname, "INC");
		break;
	case 7:
		strcpy(opname, "JMP");
		break;
	case 8:
		strcpy(opname, "JPC");
		break;
	case 9:
		switch (ir.m)
		{
		case 1:
			strcpy(opname, "WRT");
			break;
		case 2:
			strcpy(opname, "RED");
			break;
		case 3:
			strcpy(opname, "HLT");
			break;
		default:
			strcpy(opname, "err");
			break;
		}
		break;
	default:
		strcpy(opname, "err");
		break;
	}
	printf("%d\t%s\t%d\t%d\t", (pc - 3) / 3, opname, ir.l, ir.m);
}

int main(int argc, char const *argv[])
{
	FILE *fp = fopen(argv[1], "r");
	int i = 0;
	if (fp == NULL)
	{
		printf("Error Reading File!\n");
		return 1;
	}
	while ((fscanf(fp, "%d", &PAS[i])) != EOF)
		i++;

	gp = i;
	pc = 0;
	bp = i;
	sp = i - 1;
	halt = 1;

	printf("VM Exectution:\n");
	printf("\t\t\t\tPC\tBP\tSP\tstack\n");
	printf("Initial Values:\t%d\t%d\t%d\n", pc, bp, sp);

	while (halt)//while loop 
	{
		ir.op = PAS[pc];
		ir.l = PAS[pc + 1];
		ir.m = PAS[pc + 2];
		pc += 3;

		print_instruction(pc, ir);

		switch (ir.op)
		{

		case 1: // LIT
			sp = sp + 1;
			PAS[sp] = ir.m;

			break;

		case 2: // OPR

			switch (ir.m)
			{

			case 0: // RTN
				sp = bp - 1;
				bp = PAS[sp + 2];
				pc = PAS[sp + 3];
				bars[sp + 1] = 0;
				break;

			case 1: // ADD
				PAS[sp - 1] = PAS[sp - 1] + PAS[sp];
				sp = sp - 1;
				break;

			case 2: // SUB
				PAS[sp - 1] = PAS[sp - 1] - PAS[sp];
				sp = sp - 1;
				break;

			case 3: // MUL
				PAS[sp - 1] = PAS[sp - 1] * PAS[sp];
				sp = sp - 1;
				break;

			case 4: // DIVEQL
				PAS[sp - 1] = PAS[sp - 1] / PAS[sp];
				sp = sp - 1;
				break;

			case 5: // GEQ
				PAS[sp - 1] = PAS[sp - 1] == PAS[sp];
				sp = sp - 1;
				break;

			case 6: // NEQ
				PAS[sp - 1] = PAS[sp - 1] != PAS[sp];
				sp = sp - 1;
				break;

			case 7: // LSS
				PAS[sp - 1] = PAS[sp - 1] < PAS[sp];
				sp = sp - 1;
				break;

			case 8: // LEQ
				PAS[sp - 1] = PAS[sp - 1] <= PAS[sp];
				sp = sp - 1;
				break;

			case 9: // GTR
				PAS[sp - 1] = PAS[sp - 1] > PAS[sp];
				sp = sp - 1;
				break;

			case 10: // GEQ
				PAS[sp - 1] = PAS[sp - 1] >= PAS[sp];
				sp = sp - 1;
				break;

			default: // GEQ
				printf("Error at pc %d", pc);
				break;
			}

			break;

		case 3: // LOD
			sp = sp + 1;
			PAS[sp] = PAS[base(PAS, bp, ir.l) + ir.m];

			break;

		case 4: // STO
			PAS[base(PAS, bp, ir.l) + ir.m] = PAS[sp];
			sp = sp - 1;

			break;

		case 5: // CAL
			PAS[sp + 1] = base(PAS, bp, ir.l);
			PAS[sp + 2] = bp;
			PAS[sp + 3] = pc;
			bp = sp + 1;
			pc = ir.m;
			bars[sp + 1] = 1;

			break;

		case 6: // INC
			sp = sp + ir.m;
			break;

		case 7: // JMP
			pc = ir.m;
			break;

		case 8: // JPC
			if (PAS[sp] == 0)
				pc = ir.m;
			sp = sp - 1;
			break;

		case 9: // WRT

			switch (ir.m)
			{
			case 1: // WRT
				printf("\nOutput : %d\n", PAS[sp]);
				sp = sp - 1;
				printf("\t\t\t\t");
				break;

			case 2: // RED
				sp = sp + 1;
				printf("\nInput : \t\t");
				scanf("%d", &PAS[sp]);
				break;

			case 3: // HLT
				halt = 0;
				break;
			}
			break;
		default:
			printf("error");
			break;
		}
		print_stack(pc, bp, sp, gp, PAS, bars);//finally, print stack after a cycle through the switches
	}
	printf("\n");
	fclose(fp);//close file pointer
	return 0;//finally done
	
}