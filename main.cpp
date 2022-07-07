#include <iostream>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <vector>

using namespace std;

struct set
{
	int num;
	set* pnext=NULL;
};

void insertWord(wstring a,vector<wstring> &edges,vector<set*> &graph,wstring def);
void split(vector<wstring> &edges,vector<set*> &graph,int loc,int k,wstring a);
void printAll(vector<wstring> edges,vector<set*> graph,int loc,wstring arr);

int main()
{
	_setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
	vector<wstring> edges;
	vector<set*> graph;
	edges.push_back(L"");
	set* root=new set;
	root->num=0;
	graph.push_back(root);
	wstring def,a;
	int n;
	wcout<<L"n : ";
	wcin>>n;
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
	printAll(edges,graph,0,L"");
	
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
