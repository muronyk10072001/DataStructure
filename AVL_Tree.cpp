#include <iostream>

using namespace std;

struct Branch
{
	int Key;
	int SubTree;
	Branch *LeftBranch, *RightBranch;
	Branch *Parent;
};

class Tree
{
	int tabs;
	//Branch *Position;
public:
	Branch *Main;
	Tree(int Key)
	{
		Main = new (Branch);
		Main->LeftBranch = NULL;
		Main->RightBranch = NULL;
		Main->Parent = NULL;
		Main->Key = Key;
		Main->SubTree = 0;
		tabs = 0;
	}
	void Insert(int);
	void Remove(int);
	void Print();
	void PrintTree(Branch *);
	void PrintSubTree(Branch *);
	Branch *SmallLeftTurn(Branch *);
	Branch *SmallRightTurn(Branch *);
};

int main()
{
	int a, b;
	cin >> a;
	Tree ExampleTree(a);
	ExampleTree.PrintSubTree(ExampleTree.Main);
	cin>>a;
	while (a != 0)
	{
		ExampleTree.Insert(a);
		ExampleTree.PrintSubTree(ExampleTree.Main);
		ExampleTree.PrintTree(ExampleTree.Main);
		cin >> a;
	}
	cin>>a;
	while (a != 0)
	{
		ExampleTree.Remove(a);
		ExampleTree.PrintSubTree(ExampleTree.Main);
		ExampleTree.PrintTree(ExampleTree.Main);
		cin >> a;
	}

	return 0;
}

void Tree::PrintTree(Branch *Node)
{
	if (!Node) return;
	tabs++;

	PrintTree(Node->LeftBranch);

	for (int i = 0; i < tabs; i++) cout << "  ";
	cout << Node->Key << endl;

	PrintTree(Node->RightBranch);

	tabs--;
	return;
}

void Tree::PrintSubTree(Branch *Node)
{
	if (!Node) return;
	tabs++;

	PrintSubTree(Node->LeftBranch);

	for (int i = 0; i < tabs; i++) cout << "  ";
	cout << Node->SubTree << endl;

	PrintSubTree(Node->RightBranch);

	tabs--;
	return;
}

Branch *Tree::SmallLeftTurn(Branch *Position)
{
	int Tmp;
	Branch *tmp;
	Branch *t = Position->RightBranch; //b=t t=b
	t->Parent = Position->Parent;
	Position->Parent = t;
	Position->RightBranch = t->LeftBranch;
	t->LeftBranch = Position;

	Tmp = t->Key;
	tmp = t;
	t = t->Parent;
	if (t == NULL)
	{
		Main = tmp;
		return tmp;
	}
	if (Tmp <= t->Key) t->LeftBranch = tmp;
	else t->RightBranch = tmp;

	return tmp;
}

Branch *Tree::SmallRightTurn(Branch *Position)
{
	int Tmp;
	Branch *tmp;
	Branch *t = Position->LeftBranch; //b=t t=b
	t->Parent = Position->Parent;
	Position->Parent = t;
	Position->LeftBranch = t->RightBranch;
	t->RightBranch = Position;

	Tmp = t->Key;
	tmp = t;
	t = t->Parent;
	if (t == NULL)
	{
		Main = tmp;
		return tmp;
	}
	if (Tmp <= t->Key) t->LeftBranch = tmp;
	else t->RightBranch = tmp;

	return tmp;
}

void Tree::Insert(int Key)//можливо можна оптимізувати власне вставку
{
	Branch *t = Main, *Position;
	Position = t;
	int Value;

	while (t != NULL)
	{
		Position = t;
		if (t->Key <= Key) t = t->RightBranch;
		else t = t->LeftBranch;
	}//при виході p - вказівник на останній елемент (листок)

	t = Position;//вказівник на оастанній
	if (Position->Key <= Key)
	{
		Position->SubTree++;
		Position->RightBranch = new Branch;
		Position = Position->RightBranch;
	}
	else
	{
		Position->SubTree--;
		Position->LeftBranch = new Branch;
		Position = Position->LeftBranch;
	}

	Position->Parent = t;
	Position->RightBranch = NULL;
	Position->LeftBranch = NULL;
	Position->Key = Key;
	Position->SubTree = 0;//ми додали ключ де треба. Тепер збалансуємо дерево

	if (t->SubTree == 0) return;
	Value = t->Key;
	Position = t->Parent;
	//cout<<"erfer 1"<<endl;
	while (Position != NULL)// (Position->Parent != NULL)
	{
		if (Value < Position->Key)//отже це ліва дитина
		{
			Position->SubTree--;
			if (Position->SubTree == 0) return;//значить ми збалансували це піддерево. Вихід
			if (Position->SubTree == -2)//завелика висота. Балансуєм
			{
				if (Key <= Value)//малий поворот
				{
					Position = SmallRightTurn(Position);
					Position->SubTree = 0;
					t = Position->RightBranch;
					t->SubTree = 0;
				}
				else//великий поворот
				{
					Position = SmallLeftTurn(Position->LeftBranch);
					Position = SmallRightTurn(Position->Parent);
					if (Position->SubTree == 1)
					{
						t = Position->LeftBranch;
						t->SubTree = -1;
						t = Position->RightBranch;
						t->SubTree = 0;
					}
					else
					{
						t = Position->LeftBranch;
						t->SubTree = 0;
						t = Position->RightBranch;
						t->SubTree = 1;
					}
					Position->SubTree = 0;
				}
				return;
			}
		}
		else//права дитина
		{
			Position->SubTree++;
			if (Position->SubTree == 0) return;//значить це піддерево стало збалансоване
			if (Position->SubTree == 2)//завелика висота. Балансуєм
			{
				if (Key >= Value)
				{
					Position = SmallLeftTurn(Position);
					Position->SubTree = 0;
					t = Position->LeftBranch;
					t->SubTree = 0;
				}
				else//це великий лівий поворот
				{
					Position = SmallRightTurn(Position->RightBranch);
					Position = SmallLeftTurn(Position->Parent);
					if (Position->SubTree == 1)
					{
						t = Position->LeftBranch;
						t->SubTree = -1;
						t = Position->RightBranch;
						t->SubTree = 0;
					}
					else
					{
						t = Position->LeftBranch;
						t->SubTree = 0;
						t = Position->RightBranch;
						t->SubTree = 1;
					}
					Position->SubTree = 0;
				}
				return;
			}
		}
		Value = Position->Key;
		Position = Position->Parent;
	}
}

void Tree::Remove(int Key)
{
	Branch *t = Main, *p;
	int Value;

	while (t != NULL && t->Key != Key)
	{
		if (Key >= t->Key) t = t->RightBranch;
		else t = t->LeftBranch;
	}

	p = t;
	if (p == NULL) return;
	if (p->RightBranch == NULL)
	{
		if ((p->Parent)->Key <= p->Key)
			(p->Parent)->RightBranch = p->LeftBranch;
		else
			(p->Parent)->LeftBranch = p->LeftBranch;
		t = p->Parent;
		if (p->LeftBranch != NULL)
			(p->LeftBranch)->Parent = p->Parent;
		if (p->Key < t->Key)
			t->SubTree++;
		else
			t->SubTree--;
		delete p;
		
		if ((p->Parent)->SubTree != 0) return;/////////////////////////////
	}
	else
	{
		p = t->RightBranch;
		while (p->LeftBranch != NULL)
		{
			p = p->LeftBranch;
		}
		t->Key = p->Key;
		if (p == t->RightBranch)
		{
			(p->Parent)->RightBranch = p->RightBranch;
			(p->Parent)->SubTree--;
			if ((p->Parent)->SubTree == -1) return;/////////////////////////
		}
		else
		{
			(p->Parent)->LeftBranch = p->RightBranch;
			(p->Parent)->SubTree++;
			if ((p->Parent)->SubTree == 1) return;//////////////////////////
		}
		if (p->RightBranch != NULL) (p->RightBranch)->Parent = p->Parent;
		t = p->Parent;
		delete p;
	}

	Value = t->Key;
	t = t->Parent;
	while (t != NULL)
	{
		if (t->Key <= Value)
		{
			t->SubTree--;
			if (t->SubTree == -1) return;
			if (t->SubTree == -2)//балансуємо
			{
				if ((t->LeftBranch)->SubTree == 1)//великий поворот
				{
					t = SmallLeftTurn(t->LeftBranch);
					t = SmallRightTurn(t->Parent);
					if (t->SubTree == 0)
					{
						(t->LeftBranch)->SubTree = 0;
						(t->RightBranch)->SubTree = 0;
					}
					if (t->SubTree == 1)
					{
						(t->LeftBranch)->SubTree = -1;
						(t->RightBranch)->SubTree = 0;
					}
					if (t->SubTree == -1)
					{
						(t->LeftBranch)->SubTree = 0;
						(t->RightBranch)->SubTree = 1;
					}
					t->SubTree = 0;
				}
				else//малий поворот
				{
					t = SmallRightTurn(t);
					if (t->SubTree == -1)
					{
						t->SubTree = 0;
						(t->RightBranch)->SubTree = 0;
						return;
					}
					else
					{
						t->SubTree = -1;
						(t->RightBranch)->SubTree = -1;
					}//у такому виподку висота заг піддерева -1, тому йдемо далі вверх
				}
			}
		}
		else
		{
			t->SubTree++;
			if (t->SubTree == 1) return;
			if (t->SubTree == 2)
			{
				if ((t->RightBranch)->SubTree == 1)//великий поворот
				{
					p = SmallRightTurn(p->RightBranch);
					p = SmallLeftTurn(p->Parent);
					if (t->SubTree == 0)
					{
						(t->LeftBranch)->SubTree = 0;
						(t->RightBranch)->SubTree = 0;
					}
					if (t->SubTree == 1)
					{
						(t->LeftBranch)->SubTree = -1;
						(t->RightBranch)->SubTree = 0;
					}
					if (t->SubTree == -1)
					{
						(t->LeftBranch)->SubTree = 0;
						(t->RightBranch)->SubTree = 1;
					}
					t->SubTree = 0;
				}
				else
				{
					t = SmallLeftTurn(t);
					if (t->SubTree == -1)
					{
						t->SubTree = 0;
						(t->LeftBranch)->SubTree = 0;
						return;
					}
					else
					{
						t->SubTree = 1;
						(t->LeftBranch)->SubTree = -1;
					}//у такому виподку висота заг піддерева -1, тому йдемо далі вверх
				}
			}
		}
		Value = t->Key;
		t = t->Parent;
	}
}
