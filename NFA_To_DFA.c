//Student Num : 2010111661
//Student Name: JunSu. Lim
//Description : ���� �� 3��, �Է� ���� 2���� e-NFA�� DFA�� �������ִ� ���α׷�.
#include<stdio.h>
#include<string.h>
#include<memory.h>
#define NFA_TABLE_SIZE_ROW 3  
#define NFA_TABLE_SIZE_COL 4
#define ALPHABET_LENGTH    26

typedef char string[5];
typedef struct _NFA
{
	//NFA_Table[x][0] : state
	string NFA_Table[NFA_TABLE_SIZE_ROW][NFA_TABLE_SIZE_COL];
	string Epsilon_Table[NFA_TABLE_SIZE_ROW][2];//e-closure 
}NFA;

typedef struct _DFA
{
	//DFA_Table[x][0] : DFA state
	string DFA_Table[NFA_TABLE_SIZE_ROW][NFA_TABLE_SIZE_COL - 1];
	char Check_Table[ALPHABET_LENGTH]; //�ߺ� ���� Ȯ�� ���̺�
}DFA;

//epilon closure function
void Epsilon_Closure_Func(NFA* ptrNFA, int* ptrNumOfStates)
{
	//NFA_Table[x][3] : epsilon
	int i, j, colTraverseNum = 0;
	char tempString[10];
	char noInputEpsilon[2];
	noInputEpsilon[0] = '-';
	noInputEpsilon[1] = '\0';
	memset(tempString, 0, sizeof(tempString));

	for (i = 0; i < *(ptrNumOfStates); i++)
	{
		strcpy_s(tempString, sizeof(tempString), &(ptrNFA->NFA_Table[i][0]));
		for (j = i; j<*(ptrNumOfStates); j++)
		{
			//epilon col�� '-'�� �ִ��� Ȯ��
			if (strcmp(&ptrNFA->NFA_Table[j][3], &noInputEpsilon) != 0)
			{
				strcat_s(tempString, sizeof(tempString), &(ptrNFA->NFA_Table[j][3]));
				colTraverseNum++;
			}

		}
		tempString[colTraverseNum+1] = '\0';

		//epsilon table ����
		//ptrNFA->Epilson_Table[i][0] : e-closure(x)
		//ptrNFA->Epilson_Table[i][1] : e-closure(x) = result value
		strcpy_s(&(ptrNFA->Epsilon_Table[i][0]), sizeof(ptrNFA->Epsilon_Table[i][0]), &(ptrNFA->NFA_Table[i][0]));
		strcpy_s(&(ptrNFA->Epsilon_Table[i][1]), sizeof(ptrNFA->Epsilon_Table[i][1]), &tempString);

	}

	for (i = 0; i < 3; i++)
		fprintf(stdout, "\ne-closure(%s) : %s\n", ptrNFA->Epsilon_Table[i][0], &(ptrNFA->Epsilon_Table[i][1]));

}

//NFA�� DFA�� �ٲ� �� �ߺ��Ǵ� ���ڸ� �����ϴ� �Լ�
//DFA* ptrDFA : DFA�� ������ ����ü ������
//ptrNumOfStates : ������ ����
void Check_Duplicate_Character(DFA* ptrDFA, int* ptrNumOfStates)
{
	int i, j, k;
	char ch;
	char tempString[10];
	if (ptrDFA != NULL)
	{
		for (k = 1; k < NFA_TABLE_SIZE_COL - 1; k++)
		{
			for (i = 0; i < *(ptrNumOfStates); i++)
			{
				memset(tempString, 0, sizeof(tempString));
				memset(ptrDFA->Check_Table, 0, sizeof(ptrDFA->Check_Table));
				for (j = 0; j < strlen(ptrDFA->DFA_Table[i][k]); j++)
				{
					//�ߺ� ���ڸ� �����ϱ� ���� ��Ʈ�� ���
					//3���� �迭�� ����
					//i : row
					//k : col
					//j : string�� ������
					if (ptrDFA->Check_Table[ptrDFA->DFA_Table[i][k][j] - 65] == 0)
					{
						ptrDFA->Check_Table[ptrDFA->DFA_Table[i][k][j] - 65] = 1;
						tempString[j] = ptrDFA->DFA_Table[i][k][j];
					}
				}
				tempString[strlen(tempString)] = '\0';
				strcpy_s(&(ptrDFA->DFA_Table[i][k]), sizeof(ptrDFA->DFA_Table[i][k]), tempString);
			}
		}
	}
	else
	{
		fprintf(stderr, "DFA table error\n");
	}
}
//NFA�� DFA�� ����
//NFA* ptrNFA : NFA�� ������ ����ü ������
//DFA* ptrDFA : DFA�� ������ ����ü ������
//ptrNumOfStates : ������ ����
void Convert_NFA_To_DFA_Func(NFA* ptrNFA, DFA* ptrDFA, int* ptrNumOfStates)
{
	int i, j, k;
	char tempString[10];
	char noInputEpsilon[2];
	noInputEpsilon[0] = '-';
	noInputEpsilon[1] = '\0';
	memset(tempString, 0, sizeof(tempString));

	for(i=0; i<*(ptrNumOfStates); i++)
		strcpy_s(&(ptrDFA->DFA_Table[i][0]), sizeof(ptrDFA->DFA_Table[i][0]), &(ptrNFA->Epsilon_Table[i][1]));

	for (k = 1; k < NFA_TABLE_SIZE_COL - 1; k++)
	{
		for (i = 0; i < *(ptrNumOfStates); i++)
		{
			for (j = i; j < *(ptrNumOfStates); j++)
			{
				if (strcmp(&ptrNFA->NFA_Table[j][k], &noInputEpsilon) != 0)
					strcat_s(tempString, sizeof(tempString), &(ptrNFA->NFA_Table[j][k]));


			}
			strcpy_s(&(ptrDFA->DFA_Table[i][k]), sizeof(ptrDFA->DFA_Table[i][k]), tempString);
			memset(tempString, 0, sizeof(tempString));
		}
	}

	//NFA���� DFA�� �ٲ� �� �߻��� �ߺ� ���� ����
	Check_Duplicate_Character(ptrDFA, ptrNumOfStates);

	fprintf(stdout, "\nDFA Table : \n");
	fprintf(stdout, "STATES    0    1     \n");
	for (i = 0; i < 3; i++)
		fprintf(stdout, "%s    %s    %s\n", ptrDFA->DFA_Table[i][0], ptrDFA->DFA_Table[i][1], ptrDFA->DFA_Table[i][2]);

}
int main()
{
	int i, numOfStates;
	NFA nfa;
	DFA dfa;
	fprintf(stdout, "Input Number Of States : ");
	fscanf_s(stdin, "%d", &numOfStates, sizeof(numOfStates));
	fprintf(stdout, "Input NFA Table : \n");
	fprintf(stdout, "STATES    0    1     EPSILON\n");

	for (i = 0; i < NFA_TABLE_SIZE_ROW; i++)
	{
		fscanf_s(stdin, "%s %s %s %s", &nfa.NFA_Table[i][0], sizeof(string), &nfa.NFA_Table[i][1], sizeof(string),
			&nfa.NFA_Table[i][2], sizeof(string), &nfa.NFA_Table[i][3], sizeof(string));
	}

	Epsilon_Closure_Func(&nfa, &numOfStates);
	Convert_NFA_To_DFA_Func(&nfa, &dfa, &numOfStates);


	return 0;
}