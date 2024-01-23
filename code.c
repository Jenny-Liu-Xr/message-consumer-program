#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure that represents String
typedef struct data
{
	char  tradeNum[11];
	char  userID[9];
	double money;
}DATA,*LPDATA;

//Structure that represents Node
typedef struct node
{
	DATA data;
	struct node* next;
}NODE,*LPNODE;

//Structure that represents List
typedef struct list
{
	LPNODE headNode;
	int sizeList;
}LIST,*LPLIST;

//Allocates memory for a new list and initializes it 
LPLIST createList()
{
	LPLIST list = (LPLIST)malloc(sizeof(LIST));
	if (list == NULL) //If fail
	{
		printf("Error\n");
		exit(0);
	}
	
	list->headNode = NULL;
	list->sizeList = 0;
	return list;
}

//Allocates memory for a new list Node
LPNODE createNode(DATA data)
{
	LPNODE newNode = (LPNODE)malloc(sizeof(NODE));
	if (newNode == NULL) //If fail
	{
		printf("Error\n");
		exit(0);
	}
	
	newNode->data = data;
	newNode->next = NULL;
	
	return newNode;
}

//Insert node into linked list
void insertNode(LPLIST list, DATA data)
{
	LPNODE newNode = createNode(data);
	LPNODE pMove = list->headNode;
	if (pMove == NULL) //If List is null, add node
	{
		list->headNode = newNode;
		++list->sizeList;
		return;
	}
	while (pMove->next!=NULL)
	{
		pMove = pMove->next;
	}
	
	pMove->next = newNode;
	++list->sizeList;
}

//Print list
void printList(LPLIST list)
{
	LPNODE pMove = list->headNode;
	double sum = 0;
	printf("\n**********TOTAL TRANSACTION**********\n");
	while (pMove != NULL)
	{
		printf("Transaction ID:%s\nCustomer ID:%s\nAmount:$%.2lf\n", pMove->data.tradeNum,pMove->data.userID,pMove->data.money);
		sum += pMove->data.money;
		pMove = pMove->next;
		printf("\n");
	}
	printf("**********SUMMARY IMFOMATION**********\n");
	printf("Current Transaction Count:%d\n",list->sizeList);
	printf("Average Trade Amout:$%.2lf\n", sum / list->sizeList);
}

//Count number of transactions
int countChar(char tempMsg[])
{
	char* pStr = tempMsg;
	int count = 0;
	while ((pStr = strstr(pStr, "|")) != NULL) {
		count++;
		pStr += strlen("|");
	}
	return count;
}

//Verification String
void verifyMSG(LPLIST list,char tempMsg[])
{
	int count = countChar(tempMsg);
	if (count <=0)
	{
		//If did not type '|'
		printf("Input error!Please input correctly!\n");
		exit(0);
	}
	
	char* nPos = NULL;
	char* nStr = NULL;
	char** nPosStr = (char**)malloc(sizeof(char*)*count*100);
	if (nPosStr == NULL)
	{
		printf("Error!\n");
		exit(0);
	}
	int nIndex=0;
	
	//The first split: split by '|' 
	nPos = strtok(tempMsg, "|");
	nPosStr[nIndex] = nPos;
	++nIndex;
	while (nPos != NULL)
	{
		nPos = strtok(NULL, "|");
		nPosStr[nIndex] = nPos;
		++nIndex;
		if (nPos == NULL)
		{
			break;
		}
	}
	
	//The second split: split by ','
	for (int i = 0; i < count; ++i)
	{
		DATA tempData = { 0 };
		nStr = strtok(nPosStr[i], ","); //split transaction ID
		strcpy(tempData.tradeNum, nStr);
		nStr = strtok(NULL, ","); // split customer ID
		strcpy(tempData.userID, nStr);
		nStr = strtok(NULL, ","); // split amount
		tempData.money=atof(nStr);
		
		insertNode(list, tempData);
	}
	free(nPosStr); //Free the memory of the string
}

//Free data memory
void freeList(LPLIST list)
{
	LPNODE pMove = list->headNode;
	LPNODE pMovePrev = NULL;
	if (list->sizeList == 0)
	{
		return;
	}
	while (pMove->next != NULL) //Clean up nodes
	{
		pMovePrev = pMove;
		pMove = pMove->next;
		free(pMovePrev);
	}
	if (pMove !=NULL && pMove->next == NULL) //Clean up the last node
	{
		free(pMove);
	}
	
	pMove = NULL;
	pMovePrev = NULL;
	
}

int main()
{
	LPLIST list = createList();
	char tempMsg[1024] = { 0 };
	printf("**********WELCOME TO MCP SERVER**********\n");
	printf("Please intput a msg and hit the Enter key:\n");
	scanf("%s", tempMsg);
	verifyMSG(list,tempMsg);
	printList(list);
	freeList(list);
	system("pause");
	
	return 0;
}
