#include<iostream>
#include<fstream>
#include<queue>
#include<windows.h>
#include<iomanip>
using namespace std;

class huffmannode
{
public:

	int freq;
	char  ch;
	string code;
	huffmannode* left;
	huffmannode* right;
	huffmannode* next;

};
class huffmantree
{
	huffmannode* root;
	int count;
public:
	huffmantree()
	{
		root = NULL;
	}
	void createtree(char arr1[], int freq[], int n)
	{
		count = n + n - 1;
		for (int i = 0; i < n; i++)
		{

			huffmannode* temp = new huffmannode;
			temp->ch = arr1[i];
			temp->code = "";
			temp->freq = freq[i];
			temp->left = temp->right = NULL;
			huffmannode* move = root;
			if (root == NULL || temp->freq <= move->freq)
			{
				temp->next = root;
				root = temp;
			}
			else
			{
				while (move->next != NULL && move->next->freq < temp->freq)
				{
					move = move->next;
				}
				temp->next = move->next;
				move->next = temp;
			}
		}
		for (int i = 0; i < n - 1; i++)
		{

			huffmannode* n1 = root;
			huffmannode* n2 = root->next;
			huffmannode* n3 = new huffmannode;
			n3->ch = '$';
			n3->freq = n1->freq + n2->freq;
			n3->code = "0000";
			n3->left = n1;
			n3->right = n2;
			root = n2->next;
			n1->next = NULL;
			n2->next = NULL;

			huffmannode* move = root;
			if (root == NULL || n3->freq <= move->freq)
			{
				n3->next = root;
				root = n3;
			}
			else
			{
				while (move->next != NULL && move->next->freq < n3->freq)
				{
					move = move->next;
				}
				n3->next = move->next;
				move->next = n3;
			}






		}




	}
	void decompresstree()
	{
		root = NULL;
		int old_count = count;
		count = 0;
		ifstream fin("compressedtree.txt");
		fin >> count;
		huffmannode** arr = new huffmannode * [count];
		int counter = 0;
		while (!fin.eof())
		{


			huffmannode* n1 = new huffmannode;
			fin >> n1->freq;
			fin >> n1->ch;
			fin >> n1->code;
			n1->left = n1->right = NULL;
			n1->next = NULL;

			arr[counter] = n1;
			counter++;


		}
		int minus = 0;
		for (int i = 0; i < count; i++)
		{

			int left = (i * 2) + 1 - minus;
			int right = (i * 2) + 2 - minus;
			if (arr[i]->ch == '$')
			{
				arr[i]->left = arr[left];
				arr[i]->right = arr[right];


			}
			else
			{
				minus += 2;
			}

		}

		root = arr[0];
	}
	void decodemsg()
	{
		ofstream fout("decode.txt");
		ifstream read("encoded.txt");
		char  ch = ' ';
		char  in = ' ';
		huffmannode* move = root;
		while (!read.eof())
		{
			read >> in;

			if (in == '0')
			{
				move = move->left;
			}
			else
			{
				move = move->right;
			}
			if (move->ch != '$')
			{
				
				fout << move->ch;
				cout << move->ch;
				move = root;
			}
		}


	}
	void compresstree(huffmannode* root)
	{
		ofstream tree("compressedtree.txt");
		{

			huffmannode** dis;
			int counter = 0;
			dis = new huffmannode * [count];
			for (int i = 0; i < count; i++)
			{
				dis[i] = NULL;
			}
			queue<huffmannode*> qe;
			bool flag = true;
			qe.push(root);
			dis[counter] = root;
			counter++;
			while (!qe.empty())
			{
				huffmannode* c = qe.front();

				if (c->left != NULL)
				{
					qe.push(c->left);
					dis[counter] = c->left;
					counter++;
				}
				if (c->right != NULL)
				{
					qe.push(c->right);
					dis[counter] = c->right;
					counter++;
				}
				qe.pop();

			}
			tree << count << endl;
			for (int i = 0; i < count; i++)
			{
				tree << dis[i]->freq;
				tree << " ";
				tree << dis[i]->ch;
				tree << " ";
				tree << dis[i]->code;
				tree << endl;

			}
		}

		tree.close();
	}
	void inorder(huffmannode* root)//code allocation
	{
		static string code = "";
		if (root->left != NULL && root->right != NULL)

		{
			code.push_back('0');
			inorder(root->left);
			code.push_back('1');
			inorder(root->right);
			if (!code.empty())
				code.pop_back();

		}
		else if (root->left == NULL && root->right == NULL)
		{
			root->code = code;
			code.pop_back();
		}
	}

	huffmannode* getRoot()
	{
		return root;
	}
	void givecode(char arr[], string s[], huffmannode* root)
	{
		static int i = 0;
		if (root->left != NULL && root->right != NULL)
		{
			givecode(arr, s, root->left);
			givecode(arr, s, root->right);
		}
		else if (root->left == NULL && root->right == NULL)
		{
			arr[i] = root->ch;
			s[i] = root->code;
			i++;
		}

	}

};
struct trie_node
{
	char data;
	trie_node* child[26];
	bool terminal;
	trie_node(char x)
	{
		this->data = x;
		terminal = false;
		for (int i = 0; i < 26; i++)
		{
			child[i] = NULL;
		}

	}
};



class trie_tree
{
public:
	trie_node* root;
	trie_tree()
	{
		root = new trie_node('\0');
	}
	void insert(string str)
	{

		trie_node* temp = root;
		for (int i = 0; i < str.length(); i++)
		{
			int j = str[i] - 'a';
			if (temp->child[j] != NULL)
			{
				temp = temp->child[j];
			}
			else
			{
				temp->child[j] = new trie_node(str[i]);
				temp = temp->child[j];
			}
		}
		temp->terminal = true;

	}
	bool search(string str)
	{
		trie_node* temp = root;
		for (int i = 0; i < str.length(); i++)
		{
			int j = str[i] - 'a';
			if (temp->child[j] != NULL)
			{
				temp = temp->child[j];
			}
			else
			{
				return false;
			}
		}
		return temp->terminal;
	}
	trie_node* getRoot()
	{
		return root;
	}
	void remove(string str, trie_node* root)
	{
		static int i = 0;
		char ch = str[i];
		i++;
		trie_node* temp = root;
		if (temp->terminal == true)
		{
			temp->terminal = false;
			return;
		}
		int j = ch - 'a';
		remove(str, temp->child[j]);
		temp->child[j] = NULL;
	}

	bool check(trie_node* temp)
	{
		for (int i = 0; i < 26; i++)
		{
			if (temp->child[i] != NULL)
			{
				return 0;
			}
		}
		return 1;
	}
	void printsuggestion(trie_node* temp, string str, string suggestion[], int& count, bool& flag)
	{
		static string s = "";
		if ((check(temp) && temp->terminal) || count == 10)
		{
			return;
		}

		if (flag == true)
		{
			s.clear();
			for (int i = 0; i < str.length(); i++)
			{


				s.push_back(str[i]);
				if (temp->child[str[i] - 'a'] != NULL)
				{
					temp = temp->child[str[i] - 'a'];
				}
				else
				{
					return;
				}

			}

			flag = 0;
		}


		if (temp != NULL)
		{
			if (temp->terminal == 1 && s.length() > str.length() + 1)
			{
				suggestion[count] = s;
				count++;
			}

			for (int i = 0; i < 26; i++)
			{
				if (temp->child[i] != NULL)
				{
					char c = static_cast<char>(i + 97);
					s.push_back(c);
					printsuggestion(temp->child[i], str, suggestion, count, flag);

					s.pop_back();

				}
			}
		}


		return;
	}
	string* getsuggestion(string str)
	{
		trie_node* temp = root;

		for (int i = 0; i < str.length(); i++)
		{
			int x = str[i] - 97;
			if (temp->child[x] == NULL)
			{
				cout << "No Suggestion possible" << endl;
				break;
			}

			temp = temp->child[x];
		}
		string* ans = new string[10];
		int count = 0;
		bool flag = 1;
		temp = root;
		printsuggestion(temp, str, ans, count, flag);
		return ans;
	}

};
void gotorowcolumn(int row, int col)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cordinates;
	cordinates.X = row;
	cordinates.Y = col;
	SetConsoleCursorPosition(h, cordinates);
	Sleep(250);

}
void instructionmenu()
{
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	system("COLOR 09");
	cout << setw(58) << "Welcome to editor";
	Sleep(5000);
	system("cls");
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	system("COLOR 05");
	cout << "Insructions to use " << endl;
	cout << "1. Press Tab for suggestion" << endl;
	cout << "2. Press L_Shift for old file" << endl;
	cout << "3. Press R_Shift to save file" << endl;
	cout << "4. Press Page_Up to compress file" << endl;
	cout << "5. Press Page_Down to decompress file" << endl;
	cout << "Note: Write the text. Wait For few moments Do not press any key" << endl;
	Sleep(10000);
	system("cls");

}
int  main()
{
	huffmantree obj;
	trie_tree ttree;

	string ss = " ";
	ifstream wrote("outfile.txt");
	while (!wrote.eof())
	{
		wrote >> ss;
		ttree.insert(ss);
	}
	wrote.close();
	instructionmenu();
	ofstream write;
	ofstream enter;

	bool flag = true;
	int a = 0;
	string str = "";
	system("COLOR 0B");
	cout << "\t\t\tPress 1 for new file" << endl;
	cout << "\t\t\tPress 2 for old file" << endl;
	cin >> a;
	if (a == 2)
	{
		write.open("Text.txt", ios::app);
		enter.open("Copy.txt", ios::app);

	}
	else if (a == 1)
	{
		write.open("Text.txt");
		enter.open("Copy.txt");
	}
	system("cls");
	while (flag)
	{

		system("COLOR 07");
		if (GetAsyncKeyState(0X41))
		{
			str += 'a';
			cout << 'a';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X42))
		{
			str += 'b';
			cout << 'b';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X43))
		{
			str += 'c';
			cout << 'c';
			Sleep(600);

		}
		if (GetAsyncKeyState(0X44))
		{
			str += 'd';
			cout << 'd';
			Sleep(600);

		}
		if (GetAsyncKeyState(0X45))
		{
			str += 'e';
			cout << 'e';
			Sleep(600);

		}
		if (GetAsyncKeyState(0X46))
		{
			str += 'f';
			cout << 'f';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X47))
		{
			str += 'g';
			cout << 'g';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X48))
		{
			str += 'h';
			cout << 'h';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X49))
		{
			str += 'i';
			cout << 'i';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X4A))
		{
			str += 'j';
			cout << 'j';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X4B))
		{
			str += 'k';
			cout << 'k';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X4C))
		{
			str += 'l';
			cout << 'l';
			Sleep(600);
		}if (GetAsyncKeyState(0X4D))
		{
			str += 'm';
			cout << 'm';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X4E))
		{
			str += 'n';
			cout << 'n';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X4F))
		{
			str += 'o';
			cout << 'o';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X50))
		{
			str += 'p';
			cout << 'p';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X51))
		{
			str += 'q';
			cout << 'q';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X52))
		{
			str += 'r';
			cout << 'r';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X53))
		{
			str += 's';
			cout << 's';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X54))
		{
			str += 't';
			cout << 't';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X55))
		{
			str += 'u';
			cout << 'u';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X56))
		{
			str += 'v';
			cout << 'v';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X57))
		{
			str += 'w';
			cout << 'w';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X58))
		{
			str += 'x';
			cout << 'x';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X59))
		{
			str += 'y';
			cout << 'y';
			Sleep(600);
		}
		if (GetAsyncKeyState(0X5A))
		{
			str += 'z';
			cout << 'z';
			Sleep(600);
		}
		if (GetAsyncKeyState(VK_SPACE))
		{

			write << str;
			write << " ";
			enter << str;
			enter << " ";
			str.clear();
			cout << " ";
			Sleep(300);
		}
		if (GetAsyncKeyState(VK_TAB))
		{
			if (!str.empty())
			{
				string* ans = ttree.getsuggestion(str);
				system("cls");
				system("COLOR 04");
				cout << "Top Suggestions are" << endl;
				for (int i = 0; i < 10; i++)
				{
					cout << ans[i] << endl;
				}
				int  y = 1;
				gotorowcolumn(0, y);
				bool suggest = true;
				while (suggest)
				{
					if (GetAsyncKeyState(VK_OEM_2))
					{
						y += 1;
						gotorowcolumn(0, y);

					}
					if (y >= 11)
					{
						y = 0;

						gotorowcolumn(0, y);

					}
					if (GetAsyncKeyState(VK_OEM_1))
					{
						str.clear();
						str = ans[y - 1];
						suggest = false;
						system("cls");
						enter.close();
						ifstream red("Copy.txt");
						string ch = "";
						while (!red.eof())
						{
							red >> ch;
							if (!red.eof())
							{
								cout << ch << " ";
							}


						}
						cout << " " << str;
						red.close();
						enter.open("Copy.txt", ios::app);



					}
				}

			}

		}
		if (GetAsyncKeyState(VK_LSHIFT) && str.empty())
		{

			write.close();
			ifstream read("Text.txt");
			string ch = "";
			while (!read.eof())
			{
				read >> ch;
				if (!read.eof())
				{
					cout << ch << " ";
				}

			}
			read.close();
			Sleep(300);
			if (a == 2)
			{
				write.open("Text.txt", ios::app);
			}
			else
			{
				write.open("Text.txt");
			}

		}
		if (GetAsyncKeyState(VK_PRIOR))
		{
			system("COLOR 08");
			system("cls");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			cout << setw(50) << "Wait For 5 Sec Your File is compressing" << endl;
			ifstream fin("Copy.txt");
			int  arr[26];
			for (int i = 0; i < 26; i++)
			{
				arr[i] = 0;
			}

			while (!fin.eof())
			{
				char pp = 'a';
				char tt = ' ';
				fin >> tt;
				for (int i = 0; i < 26; i++)
				{
					if (tt == pp)
					{
						arr[i] += 1;
					}
					pp++;
				}

			}

			fin.close();
			int count = 0;
			for (int i = 0; i < 26; i++)
			{

				if (arr[i] != 0)
				{
					count++;
				}
			}



			char* arr1 = new char[count];
			int* freq = new int[count];
			int c1 = 0, f1 = 0;//counter for the frequency
			for (int i = 0; i < 26; i++)
			{
				if (arr[i] != 0)
				{
					arr1[c1] = static_cast<char>(i + 97);
					freq[f1] = arr[i];
					f1++;
					c1++;
				}
			}

			obj.createtree(arr1, freq, count);
			huffmannode* temp = obj.getRoot();
			obj.inorder(temp);//code generation
			string* s = new string[count];
			char* ch = new char[count];
			obj.givecode(ch, s, temp);
			ifstream encode("Copy.txt");
			ofstream fout("encoded.txt");

			while (!encode.eof())
			{
				char bb = ' ';
				encode >> bb;
				for (int i = 0; i < count; i++)
				{
					if (bb == ch[i])
					{
						fout << s[i];

					}
				}
			}
			fout.close();
			encode.close();
			obj.compresstree(temp);
			Sleep(5000);
			system("cls");
		}
		if (GetAsyncKeyState(VK_NEXT))
		{
			system("cls");
			system("COLOR 07");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			cout << setw(50) << "Wait For 5 Sec Your File is decompressing" << endl;
			obj.decompresstree();
			Sleep(5000);
			system("cls");
			obj.decodemsg();
			Sleep(10000);
			system("cls");

		}
		if (GetAsyncKeyState(VK_RSHIFT))
		{
			write << str;
			enter << str;
			flag = false;
			write.close();
			enter.close();
			system("cls");
			system("COLOR E4");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			cout << "File closing and saving" << endl;
			system("cls");
			cout << endl << endl << endl << endl << endl;
			cout << "\t\t\t\t\t  ______      _ _   " << endl;
			cout << "\t\t\t\t\t |  ____|    (_) |  " << endl;
			cout << "\t\t\t\t\t | |__  __  ___| |_ " << endl;
			cout << "\t\t\t\t\t |  __| \\ \\/ / | __|" << endl;
			cout << "\t\t\t\t\t | |____ >  <| | |_ " << endl;
			cout << "\t\t\t\t\t |______/_/\\_\\_|\\__|" << endl;




		}




	}





}
