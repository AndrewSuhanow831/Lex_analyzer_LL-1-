#include <stdio.h>
#include <stdlib.h.>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <utility>
#include <malloc.h>
#define N 4
#define NMAX 10
	
typedef struct tnode //описание структуры дерева
{
		char tfield;
		char nfield;
		struct tnode *left;
		struct tnode *right;
		struct tnode *parent;
} btree ;	

struct tnode *talloc(void) // выделение пам€ти дл€ структуры дерева
{
	return (btree*) malloc(sizeof(btree));
}

typedef struct st { //описание структуры стека
  char elem[NMAX]; // массив д€ хранени€ элементов стека
  int top; // индекс массива
}stack;

stack *salloc(void) // выделение пам€ти дл€ структуры стека 
{
	return (stack*) malloc(sizeof(stack*));
}

void init(stack *stk) // инициализаци€ стека
{
  stk->top = 0;
}

void push (stack *stk, char f) // поместить эемент в стек
{
  if(stk->top < NMAX) 
  {
    stk->elem[stk->top] = f;
    stk->top++;
	}
}

char pop(stack *stk) // вернуть эемент стека с вершины и удалить его в стеке
{
  char el;
  if((stk->top) > 0)
  {
    stk->top--;
    el = stk->elem[stk->top];
    return el;
  }
  else
  {
    printf("Stack underflow!\n");
    return 0;
  }
}

bool isempty(stack *stk) // пуст ли стек?
{
	bool bln = false;
  if(stk->top == 0)
  	bln = true; 
  return bln;
}

void stkPrint(stack *stk, int count) //печать count штук эементов стека, конечно же с верхушки
{ 
	int i = stk->top; // i - количество элементов в стеке
  	if(isempty(stk)) 
  		return; // стек пуст
  	do 
  	{
    	i--;
    	printf("%c", stk->elem[i]);
  	}
  while(--count > 0);
}

bool stkCheck(stack *stk, char key) // проверка всех элементов стека на соответствие key 
{ 
	bool bln = false;
	int i = stk->top; // i - количество элементов в стеке
  	if(isempty(stk)) 
  		return bln; // стек пуст
  	do 
  	{
    	i--;
    	if (stk->elem[i] == key)
    	{
    		bln = true;
    		break;
    	}
  	}
  	while(i>0);
  	return bln;
}

bool term (char key) //€вл€етс€ t терминалом?
{
	bool bln = false;
	int tr = (int)(key);
	if ((tr > 47) && (tr < 58))
	bln = true;
	return bln;
}

btree *move_in_depth (btree *root, char key) // поиск в глубину корн€ с текущим нетерминалом key. ≈сли таковой отсутствует, то возвращает корень 
{
	btree *root2 = root;
	if ((root->left == NULL) && (root->right == NULL))
	{
		if ((root->nfield == key) || (root->parent == NULL))
			return root;	
		else
			return root->parent;	
	}
	else
	{
		if (root->left != NULL)
		{
			root = move_in_depth(root->left, key);
			if ((root != root2) || (root->nfield == key))
				return root;
		}
		if (root->right != NULL)
		{
			root = move_in_depth(root->right, key);
			if ((root != root2) || (root->nfield == key))
				return root;
		}
		if (root->parent != NULL)
			return root->parent;
		return root;	
	}
}

btree *addtree (btree *root, btree *tmp, char key) // создание бинарного дерева
{
	btree *root2  = root, *root3;
	bool bln = false;
	if ((key == '\n') || (key == ' '))
		return root;
	else if (root == NULL)
	{
		root = talloc();
		root->tfield = '\0';
		root->nfield = '\0';
		root->right = NULL;
		root->left = NULL;
		root->parent = NULL;
		if (term(key))
			root->tfield = key;
		else
			root->nfield = key;
	}
	else if (key == '=')
	{
		tmp = talloc();
		tmp->tfield = '\0';
		tmp->nfield = '\0';
		tmp->left = NULL;
		tmp->right = NULL;	
		root->left = tmp;
		tmp->parent = root;
		root = root->left;
	}
	else if (key == '|')
	{
		root = root->parent;
		tmp = talloc();
		tmp->tfield = '\0';
		tmp->nfield = '\0';
		tmp->left = NULL;
		tmp->right = NULL;
		tmp->parent = root;	
		root->right = tmp;
		root = root->right;	
	}
	else
	{
		while (root2->parent != NULL)
			root2 = root2->parent;
		root3 = move_in_depth(root2, key);
		if (root3 != root2)
			root = root3;
		if (term(key))
			root->tfield = key;
		else
			root->nfield = key;
	}
		return root;
}

void treeprint (btree *root) // печать дерева
{
	if (root != NULL)
	{
		treeprint(root->left);
		if (root->nfield != '\0')
		   printf("%c", root->nfield);
		if (root->tfield != '\0')
		   printf("%c", root->tfield);
		printf("\n");
		treeprint(root->right);
	}
}

stack *pop_nonterm (stack *stk, btree *root) // поместить в стек все нетерминалы
{
	while (root != NULL)
	{
		stk = pop_nonterm (stk, root->left);
		if ((root->nfield != '\0') && (!(stkCheck(stk, root->nfield))))
			push(stk, root->nfield);
		stk = pop_nonterm (stk, root->right);
		if ((root->nfield != '\0') && (!(stkCheck(stk, root->nfield))))
				push(stk, root->nfield);
		return stk;			
	}
	return stk;
}

stack *pop_term (stack *stk, btree *root) // поместить в стек все терминалы
{
	while (root != NULL)
	{
		stk = pop_term (stk, root->left);
		if ((root->tfield != '\0') && (!(stkCheck(stk, root->tfield))))
			push(stk, root->tfield);
		stk = pop_term (stk, root->right);
		if ((root->tfield != '\0') && (!(stkCheck(stk, root->tfield))))
				push(stk, root->tfield);
		return stk;			
	}
	return stk;
}

void  counting (btree *root, int *dim_n, int *dim_t) // подсчет ко-ва всех терминалов и нетерминаов
{
//	int count_n = 0;
//	int count_t = 0;
		if (root != NULL)
	{
		counting(root->left, dim_n, dim_t);
		if (root->nfield != '\0')
			++*dim_n;
		if (root->tfield != '\0')
			++*dim_t;
		counting(root->right, dim_n, dim_t);
	}
}

char itoa (int i) // число -> символ 
{
	i = i + 48;
	char a = (char)(i);
	return a;
}

char **dalloc(char **A, int  row, int col) // выделение пам€ти дл€ двумерного динамического массива
{
    A = (char **)malloc(row*sizeof(char *));
    for(int i = 0; i <= row; i++) {
        A[i] = (char *)malloc(col*sizeof(char));
    }
    return A;
}

void create_dArray (char **Arr, int row, int col) //  создание динамического двумерного массива
{
	for (int i = 0; i <= row; i++) 
        for (int j = 0; j <= col; j++)
		{
        	if ((j == 0) && (i != 0))
				Arr[i][j] = itoa(i);
			else	 
				Arr[i][j] = '\0';
		}
}

void dfree(char **A, int row) // освобождение пам€ти от двумерного динамического массива
{
    for(int i = 0; i <= row; i++) 
        free(A[i]);
    free(A);
}

void dprint(char **A,  int row,  int col) // печать двумерного динамического массива
{
    for(int i = 0; i <= row; i++) {
        for(int j = 0; j <= col; j++) {
            printf("%*c", 5, A[i][j]);
        }
        printf("\n");
    }
}

void create_table (btree *root, stack *stk, char **Arr, int row, int col) // создание каркаса таблицы парсинга
{
	int i = 1, j = col;
	stk = pop_nonterm (stk, root);
	while (!isempty(stk))
		Arr[i++][0] = pop(stk); 
	stk = pop_term (stk, root);		
	while (!isempty(stk))
		Arr[0][--col] = pop(stk); 
	Arr[0][j] = '$';	
}


stack *FIRST (btree *root, stack *stk, char key) // функци€ FIRST 
{
	char chr;
	if (term(key))
		push(stk, key);
	else
	{
	while (root->left != NULL)
		root = root->left;
	chr = root->tfield;
	root = root->parent;
	if (root->right != NULL)
		push(stk, root->right->tfield);
	push(stk, chr);
	}
	return stk;	
}

void add_parTable (btree *root, stack *stk, char **parArr, int parRow, int parCol, char **supArr, int *supRow, int supCol) // запонение таблицы парсинга 
{
	char symbol;
	int row = 1, col;
	for (int i = 1; i <= parRow; i++)
	{
		for (int j = 1; j < parCol; j++)
			{
				
				while (root->parent != NULL)
					root = root->parent;
				root = move_in_depth(root,parArr[i][0]);
				stk = FIRST (root, stk, parArr[i][0]);
				col = supCol;
				while (!isempty(stk))
				{
					symbol = pop(stk);	
					if (symbol == parArr[0][j])
					{
						char chr = itoa(row);
						parArr[i][j] = chr;
						if (i != parRow)
						{
							supArr[row][col--] = root->left->tfield;
							supArr[row][col] = root->left->nfield;
							row++;
							break;
						}
						else 
						{
							supArr[row++][1] = symbol;
							break;
						}
					}
				}
			}
	}
	*supRow = --row;
}

void output_file (FILE *F, char **parArr, int parRow, int parCol, char **supArr, int supRow, int supCol) // создание Ћќ√-файла
{
	for (int i = 0; i <= parRow; i++) 
        {
			for(int j = 0; j <= parCol; j++)
        	{
        		fprintf(F,"%2c",parArr[i][j]);
        	}
        fprintf(F,"\n");
		}
	for (int i = 0; i <= supRow; i++) 
        {
			for(int j = 0; j <= supCol; j++)
        	{
        		fprintf(F,"%2c",supArr[i][j]);
        	}
        fprintf(F,"\n");
		}	
}

char find_cell(char **Arr, int row, int col, char X, char a) // нахождение соответствующей €чейки таблицы парсинга
{
	int u,t;
    for(int i = 1; i <= row; i++)
		if (X == Arr[i][0])
				u = i;
	for(int j = 1; j <= col; j++) 
		if (a == Arr[0][j])
				t = j;			
	return Arr[u][t];
}				 

int atoi (char a) // символ -> число
{
	int i = (int)(a);
	i = i - 48;
	return i;
}

void parser (stack *stk, char **parArr, int row, int col, char **supArr, int supCol, char *arr) // функци€ синтаксического анализатора
{
	char X, ip, a, chr;
	int  i = 0, index, count = 0;
	push(stk,parArr[1][0]);
	ip = arr[i];
	X = stk->elem[stk->top-1];
	while (!(isempty(stk)))
	{
		a = ip;
		chr = find_cell(parArr, row, col, X, a);
		if  (X == a)
			{
				stk->top--;
				ip = arr[++i];
				printf("sootvetstvie \"%c\"\n", a);
			}
		else if (term(X))
		{
			printf("ERROR");
			break;		
		}
		else if (chr == '\0')	
		{
			printf("ERROR_1");
			break;		
		}			
		else if (chr != '\0')
		{
			stk->top--;
			index = atoi(chr);
//			printf("index = %d",index);
			printf("%c", X);
			printf("->");
			for (int j = supCol; j > 0; j--)
				if (supArr[index][j] != '\0')
				{
					push(stk, supArr[index][j]);
					count++;
				}
			stkPrint(stk, count);
			count = 0;
			printf("\n");
		}
		X = stk->elem[stk->top-1];
	}
}


int main()
{
	int dim_u = 0, dim_t = 0, i = 0;
	char el, *str;
	btree *root = NULL;
	btree *tmp = NULL;
	stack *stk;
	FILE *F1;
	F1 = fopen("GRAMMATIKA.txt","r");
	while (!feof(F1))
	{
		fscanf(F1, "%c",&el);
		if (!feof(F1))
		   root = addtree (root, tmp, el);
	}	
	fclose(F1);
	while (root->parent != NULL)
		root = root->parent;
	treeprint(root);
	counting(root, &dim_u, &dim_t);
	stk = salloc();
	init(stk);
	char **parArr = dalloc(parArr, dim_u, dim_t + 1);
	create_dArray(parArr, dim_u, dim_t + 1);
	int supCol = 2; //размерность столбцов вспомогательной таблицы парсинга
	int supRow; //размерность строк вспомогательной таблицы парсинга
	char **supArr = dalloc(supArr, dim_u * dim_t, supCol);
	create_dArray(supArr, dim_u * dim_t, supCol);
	create_table(root, stk, parArr, dim_u, dim_t + 1);
	add_parTable(root, stk, parArr, dim_u, dim_t + 1, supArr, &supRow, supCol);
	dprint(parArr, dim_u, dim_t + 1);
	dprint(supArr, supRow, supCol);
	F1 = fopen("LOG-file.txt","w");
	output_file(F1,parArr, dim_u, dim_t+1, supArr, supRow, supCol);
	fclose(F1);
	F1 = fopen("INPUT.txt","r");
	str = (char*)malloc(NMAX * sizeof(int));
	while (!feof(F1))
		fscanf(F1, "%c",&str[i++]);
	fclose(F1);
	parser(stk, parArr, dim_u, dim_t+1, supArr, supCol, str);
}
