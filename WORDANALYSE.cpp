#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e5+7;
set<string> resd;
set<string> sepl;
int totaline = 0;
int state = 0;
string buff;
string vars,nums;
set<int> errors;
void init_WORD()
{
	freopen("init.txt","r",stdin);
	bool found = false;
	string s;
	while(cin>>s)
	{
		if(s=="------") found = true;
		if(found==false) resd.insert(s);
		else sepl.insert(s);
	}
}
bool special_WORD(char ch)
{
	string tmp;
	tmp += ch;
	if(ch>='0' && ch<='9') return true;
	if(ch=='\n' || ch==' ' || ch=='\t')
	{
		return true;
	}
	else
	{
		if(sepl.find(tmp)!=sepl.end()) return true;
		return false; 
	}
}
bool notnum_WORD(string s)
{
	for(int i=0;i<s.size();i++)
	{
		if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z')) return true;
	}
	return false;
}
bool has_WORD(char ch)
{
	if(ch>='a' && ch<='z') return true;
	if(ch>='A' && ch<='Z') return true;
	if(ch>='0' && ch<='9') return true;
	if(ch=='.' || ch==';' || ch=='=' | ch=='<' || ch=='>' || ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='(' || ch==')' || ch==':' || ch=='\n' || ch=='\t' || ch==' ' || ch=='{' || ch=='}') return true;
	return false;
}
void getToken_WORD()
{
	freopen("wordtest.txt","r",stdin);
	char ch;
	while((ch=getchar())!=EOF)
	{
		//bool found = false;
		if(!has_WORD(ch)) errors.insert(totaline+1);
		if(ch!='\t') buff.push_back(ch);
		if(ch!='\n') continue;
		state = 0;
		//printf("%d: ",++totaline);
		//cout<<buff;
//		for(int i=0;i<buff.size();i++)
//		{
//			if(!has(buff[i])) found = true;
//		}
//		if(found)
//		{
//			errors.push_back(totaline);
//			continue;
//		}
		for(int i=0;i<buff.size();)
		{
			//{abc}
			if(buff[i]=='{' && state==0) 
			{
				state = 1;
				i++;
			}
			else if((state==1 || state==2) && buff[i]!='}')
			{
				state = 2;
				i++;
			}
			else if(state==2 && buff[i]=='}')
			{
				state = 3;
				i++;
			}
			else if(state==3 && buff[i]=='\n')
			{
				state = 0;
				i++;
			}
			//word
			else if((state==0 || state==4) && !special_WORD(buff[i]))
			{
				vars.push_back(buff[i]);
				state = 4;
				i++;
			}
			else if(state==4 && special_WORD(buff[i]))
			{
				if(resd.find(vars)!=resd.end())
				{
					printf("RESERVED %s\n",vars.c_str()); 
				}
				else
				{
					if(notnum_WORD(vars)) printf("ID %s\n",vars.c_str());	
				}
				if(buff[i]!=' ' && buff[i]!='\n' && buff[i]!='\t')
				{
					//sig += buff[i];
					//cout<<"sig="<<sig<<endl;
					printf("SPECIAL %c\n",buff[i]);
				}
				state = 0;
				vars.clear();
				i++;
			}
			//=
			else if(state==0 && buff[i]=='=')
			{
				state = 0;
				i++;
				printf("=\n"); 
			}
			//num
			else if((state==0 || state==5) && ((buff[i]>='0' && buff[i]<='9') || buff[i]=='.'))
			{
				nums.push_back(buff[i]);
				state = 5;
				i++;
			}
			else if(state==5 && special_WORD(buff[i]))
			{
				printf("NUM %s\n",nums.c_str());
				i++;
				state = 0;
				nums.clear();
			}
			//' '
			else if(state==0 && buff[i]==' ')
			{
				state = 0;
				i++;
			}
			//'\t'
			else if(state==0 && buff[i]=='\t')
			{
				state = 0;
				i++;
			}
			//()
			else if(state==0 && special_WORD(buff[i]) && buff[i]!='\n' && buff[i]!=' ' && buff[i]!='\t' && buff[i]!=':' && buff[i]!='>' && buff[i]!='<' && buff[i]!='!')
			{
				printf("SPECIAL %c\n",buff[i]);
				i++;
				state = 0;
			}
			//'\n'
			else if(state==0 && buff[i]=='\n')
			{
				state = 0;
				i++;
			}
			else if(state==0 && buff[i]==':')
			{
				state = 6;
				i++;
			}
			else if(state==6 && buff[i]=='=')
			{
				printf("SPECIAL %s\n",":=");
				i++;
				state = 0;
			}
			else if(state==0 && buff[i]=='>')
			{
				state = 7;
				i++;
			}
			else if(state==7 && buff[i]=='=')
			{
				printf("SPECIAL %s\n",">=");
				i++;
				state = 0;
			}
			else if(state==0 && buff[i]=='<')
			{
				state = 8;
				i++;
			}
			else if(state==8 && buff[i]=='=')
			{
				printf("SPECIAL %s\n","<=");
				i++;
				state = 0;
			}
			else if(state==7 && buff[i]!='=')
			{
				printf("SPECIAL %s\n",">");
				state = 0;
			}
			else if(state==8 && buff[i]!='=')
			{
				printf("SPECIAL %s\n","<");
				state = 0;
			}
			else if(state==0 && buff[i]=='!')
			{
				state = 9;
				i++;
			}
			else if(state==9 && buff[i]=='=')
			{
				printf("SPECIAL %s\n","!=");
				i++;
				state = 0;
			}
		}
		buff.clear();
	}
}
void debug_WORD()
{
//	for(auto it=resd.begin();it!=resd.end();++it)
//	{
//		cout<<*it<<endl;
//	}
//	for(auto it=sepl.begin();it!=sepl.end();++it)
//	{
//		cout<<*it<<endl;
//	}
}
void outputerr_WORD()
{
	printf("error lines:\n");
	for(auto it=errors.begin();it!=errors.end();++it) printf("%d\n",*it);
}
int main()
{
	freopen("output.txt","w",stdout);
	init_WORD();
	//debug();
	getToken_WORD();
	//outputerr_WORD();
	return 0;
} 
