#include <iostream>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

struct set
{
	int num;
	set* pnext=NULL;
};

//working with file
void saveF(wstring file_name,vector<wstring> edges,vector<set*> graph);
bool getF(wstring file_name,vector<wstring> &edges,vector<set*> &graph);

//working with graph
void insertWord(wstring a,vector<wstring> &edges,vector<set*> &graph,wstring def);
void split(vector<wstring> &edges,vector<set*> &graph,int loc,int k,wstring a);
void printAll(vector<wstring> edges,vector<set*> graph,int loc,wstring arr);
void delAll(vector<set*> &graph);
bool search(vector<wstring> edges,vector<set*> graph,vector<wstring> &favourite,wstring keyword);
void displayHistory(queue<wstring> history);
bool changeDef(wstring keyword,vector<wstring> &edges,vector<set*> &graph,wstring new_def);

int main()
{
	_setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    queue<wstring> history;
    vector<wstring> favourite;
	vector<wstring> edges;
	vector<set*> graph;
	wstring file_name;
	wstring def,a;
	int n,option=1;
	
	wcout<<L"1.new file"<<endl;
	wcout<<L"2.open file"<<endl;
	wcin>>option;
	if (option==1)
	{
		edges.push_back(L"");
		set* root=new set;
		root->num=0;
		graph.push_back(root);
	}
	else
	{
		wcout<<L"file name:"<<endl;
		wcin.ignore(1,'\n');
		wcin.ignore(1,'\n');
		getline(std::wcin,file_name);
		if (getF(file_name,edges,graph)==false)
		{
			edges.push_back(L"");
			set* root=new set;
			root->num=0;
			graph.push_back(root);
		}
	}
	while (option!=0)
	{
		system("cls");
		wcout<<L"0.exit the program"<<endl;
		wcout<<L"1.insert new word"<<endl;
		wcout<<L"2.search for a keyword"<<endl;
		wcout<<L"3.search history"<<endl;
		wcout<<L"4.change defintion"<<endl;
		
		while (!(wcin>>option)) 
		{
			wcin.clear();
			wcin.ignore(1,'\n');
			wcout<<L"please input an integer"<<endl;
		}
		if (option==1)
		{
			wcout<<L"number of words : ";
			while (!(wcin>>n)) 
			{
				wcin.clear();
				wcin.ignore(1,'\n');
				wcout<<L"please input an intger"<<endl;
			}
			wcin.ignore(10,'\n');
			for (int i=0;i<n;i++)
			{
				wcout<<L"   word : ";
				wcin.ignore(10,'\n');
				getline(std::wcin,a);
				wcout<<L"   def  : ";
				wcin.ignore(10,'\n');
				getline(std::wcin,def);
				insertWord(a,edges,graph,def);
			}
		}
		else if (option==2)
		{
			wcout<<"keyword: ";
			wcin.ignore(1,'\n');
			wcin.ignore(10,'\n');
			getline(std::wcin,a);
			if (history.size()==10) history.pop();
			history.push(a);
			if (search(edges,graph,favourite,a)==false) 
			{
				wcout<<L"cannot find the keyword"<<endl;
				wcout<<L"press any key and 'enter' to continue"<<endl;
				wcin.ignore(10,'\n');
				getline(std::wcin,a);
			}
		}
		else if (option==3)
		{
			wcout<<L"history : "<<endl;
			displayHistory(history);
			wcout<<L"press any key and 'enter' to continue"<<endl;
			wcin.ignore(1,'\n');
			wcin.ignore(10,'\n');
			getline(std::wcin,a);
		}
		else if (option==4)
		{
			wcout<<L"  word     : ";
			wcin.ignore(10,'\n');
			wcin.ignore(10,'\n');
			getline(std::wcin,a);
			wcout<<L"  new def  : ";
			wcin.ignore(10,'\n');
			getline(std::wcin,def);
			if (changeDef(a,edges,graph,def)==false); 
			{
				wcout<<L"cannot find the keyword"<<endl;
			}
			wcout<<L"press any key and 'enter' to continue"<<endl;
			wcin.ignore(10,'\n');
			getline(std::wcin,a);
		}
	}
	if (file_name.size()>0)
	{
		wcout<<L"do you want to save changes? (1-yes, 2-no)"<<endl;
		wcin>>option;
		if (option==1)
		{
			saveF(file_name,edges,graph);
		}
	}
	else
	{
		wcout<<L"do you want to save this new file? (1-yes, 2-no)"<<endl;
		wcin>>option;
		if (option==1)
		{
			wcout<<L"file name:"<<endl;
			wcin.ignore(1,'\n');
			wcin.ignore(1,'\n');
			wcin>>file_name;
			saveF(file_name,edges,graph);
		}
	}
	delAll(graph);
}

bool changeDef(wstring keyword,vector<wstring> &edges,vector<set*> &graph,wstring new_def)
{
	int t=0;
	set* cur=graph[0]->pnext;
	while (cur)
	{
		if (edges[cur->num][0]==keyword[t])
		{
			for (int i=0;i<edges[cur->num].size();i++)
			{
				if (i+t==keyword.size() || edges[cur->num][i]!=keyword[i+t]) return false;
			}
			t+=edges[cur->num].size();
			cur=graph[cur->num];
			if (t==keyword.size())
			{
				edges[cur->num+1]=L"$"+new_def;
				return true;
			}
		}
		else cur=cur->pnext;
	}
}

bool getF(wstring file_name,vector<wstring> &edges,vector<set*> &graph)
{
	set* tmp,*head=NULL;
	int t,n=0,loc=0;
	file_name+=L".txt";
	string a(file_name.begin(),file_name.end());
	wstring lin=L"";
	wifstream fin(a);
	fin>>n;
	if (n<=0) 
	{
		fin.close();
		return false;
	}
	getline(fin,file_name);
	for (int i=0;i<n;i++)
	{
		getline(fin,file_name);
		edges.push_back(file_name);
		getline(fin,lin);
		string b(lin.begin(),lin.end());
		istringstream line(b);
		while (line>>t)
		{
			if (graph.size()==loc)
			{
				tmp=new set;
				tmp->num=t;
				graph.push_back(tmp);
			}
			else
			{
				tmp=new set;
				tmp->num=t;
				tmp->pnext=graph[loc]->pnext;
				graph[loc]->pnext=tmp;
			}
		}
		loc++;
	}
	fin.close();
	return true;
}

void saveF(wstring file_name,vector<wstring> edges,vector<set*> graph)
{
	set* cur=NULL;
	file_name+=L".txt";
	string a(file_name.begin(),file_name.end());
	wofstream fout(a);
	fout<<edges.size()<<endl;
	for (int i=0;i<edges.size();i++)
	{
		fout<<edges[i]<<endl;
		cur=graph[i];
		while (cur)
		{
			fout<<cur->num<<L" ";
			cur=cur->pnext;
		}
		fout<<endl;
	}
	fout.close();
}

void displayHistory(queue<wstring> history)
{
	while (history.size()>0)
	{
		wcout<<history.front()<<endl;
		history.pop();
	}
}

bool search(vector<wstring> edges,vector<set*> graph,vector<wstring> &favourite,wstring keyword)
{
	int t=0;
	set* cur=graph[0]->pnext;
	while (cur)
	{
		if (edges[cur->num][0]==keyword[t])
		{
			for (int i=0;i<edges[cur->num].size();i++)
			{
				if (i+t==keyword.size() || edges[cur->num][i]!=keyword[i+t]) return false;
			}
			t+=edges[cur->num].size();
			cur=graph[cur->num];
			if (t==keyword.size())
			{
				wcout<<L"definition:";
				for (int i=1;i<edges[cur->num+1].size();i++) wcout<<edges[cur->num+1][i];
				wcout<<endl<<L"add word to favourite list? (0-no,1-yes)"<<endl;
				wcin.ignore(1,'\n');
				wcin.ignore(1,'\n');
				while (!wcin>>t)
				{
					wcin.clear();
					wcin.ignore(1,'\n');
				}
				if (t==1)
				{
					favourite.push_back(edges[cur->num]);
					favourite.push_back(edges[cur->num+1]);
				}
				return true;
			}
		}
		cur=cur->pnext;
	}
}

void insertWord(wstring a,vector<wstring> &edges,vector<set*> &graph,wstring def)
{
	set* cur;
	int loc=0,curloc=0;
	while (loc<graph.size())
	{
		cur=graph[loc];
		while (cur->pnext)
		{
			loc=cur->pnext->num;
			if (edges[loc][0]==a[0])
			{
				wstring newA;
				int k=0;
				for (k;k<edges[loc].size();k++)
				{
					if (edges[loc][k]!=a[k])
					{
						set *tmp=new set;
						def=L"$"+def;
						cur->pnext->num=graph.size();
						split(edges,graph,loc,k,a);
						tmp->num=graph.size();
						graph[graph.size()-1]->pnext=new set;
						graph[graph.size()-1]->pnext->num=graph.size();
						edges.push_back(def);
						graph.push_back(tmp);
						return;
					}
				}
				for (k=edges[loc].size();k<a.size();k++)
				{
					newA+=a[k];
				}
				curloc=loc;
				a=newA;
				cur=graph[loc];
			}
			else 
			{
				cur=cur->pnext;
			}
		}
		if (!cur->pnext)
		{
			set* tmp=new set;
			tmp->num=graph.size();
			tmp->pnext=graph[curloc]->pnext;
			graph[curloc]->pnext=tmp;
			tmp=new set;
			tmp->num=graph.size();
			tmp->pnext=new set;
			tmp->pnext->num=graph.size()+1;
			graph.push_back(tmp);
			edges.push_back(a);
			tmp=new set;
			tmp->num=graph.size();
			graph.push_back(tmp);
			def=L"$"+def;
			edges.push_back(def);
			return;
		}
	}
}

void split(vector<wstring> &edges,vector<set*> &graph,int loc,int k,wstring a)
{
	set* tmp;
	wstring new1,new2,newa;
	for (int i=0;i<k;i++)
	{
		new1+=edges[loc][i];
	}
	for (int i=k;i<edges[loc].size();i++)
	{
		new2+=edges[loc][i];
	}
	for (int i=k;i<a.size();i++)
	{
		newa+=a[i];
	}
	if (new2.size()>0)
	{
		edges[loc]=new2;
		edges.push_back(new1);
		tmp=new set;
		tmp->num=graph.size();
		tmp->pnext=new set;
		tmp->pnext->num=loc;
		tmp->pnext->pnext=new set;
		tmp->pnext->pnext->num=graph.size()+1;
		graph.push_back(tmp);
		edges.push_back(newa);
		tmp=new set;
		tmp->num=graph.size();
		graph.push_back(tmp);
		return;
	}
	else
	{
		tmp=new set;
		tmp->num=graph.size();
		tmp->pnext=graph[loc]->pnext;
		graph[loc]->pnext=tmp;
		edges.push_back(newa);
		tmp=new set;
		tmp->num=graph.size();
		graph.push_back(tmp);
	}
}

void printAll(vector<wstring> edges,vector<set*> graph,int loc,wstring arr)
{
	if (edges[loc][0]!=L'$') arr+=edges[loc];
	else 
	{
		wcout<<L"word : "<<arr;
		wcout<<endl<<L"definition : ";
		for (int i=1;i<edges[loc].size();i++) wcout<<edges[loc][i];
		wcout<<endl;
		return;
	}
	set* cur=graph[loc]->pnext;
	while (cur)
	{
		printAll(edges,graph,cur->num,arr);
		cur=cur->pnext;
	}
}

void delAll(vector<set*> &graph)
{
	for (int i=0;i<graph.size();i++)
	{
		set *cur=graph[i],*del;
		while (cur)
		{
			del=cur;
			cur=cur->pnext;
			delete del;
		}
	}
}
