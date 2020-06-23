//Student Num : 2010111661
//Student Name: JunSu. Lim
//Description : 상태 수 3개, 입력 문자 2개의 e-NFA를 DFA로 변경해주는 프로그램.
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
	char Check_Table[ALPHABET_LENGTH]; //중복 문자 확인 테이블
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
			//epilon col에 '-'가 있는지 확인
			if (strcmp(&ptrNFA->NFA_Table[j][3], &noInputEpsilon) != 0)
			{
				strcat_s(tempString, sizeof(tempString), &(ptrNFA->NFA_Table[j][3]));
				colTraverseNum++;
			}

		}
		tempString[colTraverseNum+1] = '\0';

		//epsilon table 생성
		//ptrNFA->Epilson_Table[i][0] : e-closure(x)
		//ptrNFA->Epilson_Table[i][1] : e-closure(x) = result value
		strcpy_s(&(ptrNFA->Epsilon_Table[i][0]), sizeof(ptrNFA->Epsilon_Table[i][0]), &(ptrNFA->NFA_Table[i][0]));
		strcpy_s(&(ptrNFA->Epsilon_Table[i][1]), sizeof(ptrNFA->Epsilon_Table[i][1]), &tempString);

	}

	for (i = 0; i < 3; i++)
		fprintf(stdout, "\ne-closure(%s) : %s\n", ptrNFA->Epsilon_Table[i][0], &(ptrNFA->Epsilon_Table[i][1]));

}

//NFA를 DFA로 바꿀 때 중복되는 문자를 제거하는 함수
//DFA* ptrDFA : DFA를 가지는 구조체 포인터
//ptrNumOfStates : 상태의 개수
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
					//중복 문자를 제거하기 위해 비트맵 사용
					//3차원 배열로 접근
					//i : row
					//k : col
					//j : string의 포인터
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
//NFA를 DFA로 변경
//NFA* ptrNFA : NFA를 가지는 구조체 포인터
//DFA* ptrDFA : DFA를 가지는 구조체 포인터
//ptrNumOfStates : 상태의 개수
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

	//NFA에서 DFA로 바꿀 때 발생한 중복 문자 제거
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