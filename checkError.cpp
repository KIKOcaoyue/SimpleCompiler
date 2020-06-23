#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
set<string> resd;
set<string> sepl;
int totaline = 0;
int state = 0;
string buff;
string vars,nums;
set<int> errors;
struct WORD
{
	string name;
	string value;
};
vector<WORD> word;
int totword = 0;
//---------------------------------------------------------------
//FIRST AND FOLLOW AND TABLE
int n = 0;
map<string,int> mp; //映射 
vector<int> G[maxn],V[maxn],M[maxn]; //G存推导单线边 ,V存反边，即右侧连左侧 ,M为右侧整体映射右侧每个部分的标号 ，即右侧连接出去双线边 
vector<int> LEFT,RIGHT;
map<int,set<string> > FIRST,FOLLOW;
int cnt = 0;
int vis[maxn];
int viss[maxn];
string a[maxn];
string TABLE[maxn][maxn];
int conn[maxn][maxn];
string getstr(int id);
//FIRST AND FOLLOW AND TABLE END 
//------------------------------------------------------------- 
//PDA
stack<int> anstack;
vector<string> outputstream;
struct INPUTSTREAM
{
	string name;
	string value;
};
struct PHRASERROR
{
	string num;
	string value;
};
vector<INPUTSTREAM> inputstream;
vector<PHRASERROR> phraseError;
//PDA END
//------------------------------------------------------------- 
//-------------------------------------------------------------
//FourElement
stack<string> semanticstk;
int fourtot = 0;
int tempcount = 0;
struct FOURELEMENT
{
	int cnt;
	string op;
	string arg1;
	string arg2;
	string result;	
};
vector<FOURELEMENT> fourelement;
string OP,ARG1,ARG2,RES;
stack<int> if_fj,if_rj,repeat;
string newTemp()
{
	string ans = "T";
	tempcount++;
	ans += to_string(tempcount);
	return ans;
}
map<string,string> mpvalue;
set<string> actionset;
void backpatch(int id,int res)
{
	fourelement[id-1].result = to_string(res);
}
string trim(string s)
{
	string ans;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]==' ') continue;
		ans += s[i];
	}
	return ans;
}
bool isAction(int id)
{
	string str = trim(getstr(id));
	//cout<<"检查动作: "<<str<<endl;
	if(actionset.find(str)!=actionset.end()) return true;
	return false;
}
//------------------------------------------------------------- 
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
					word.push_back({"RESERVED",vars});
					//printf("RESERVED %s\n",vars.c_str()); 
				}
				else
				{
					if(notnum_WORD(vars)) 
					{
						word.push_back({"ID",vars});
						//printf("ID %s\n",vars.c_str());
					}	
				}
				if(buff[i]!=' ' && buff[i]!='\n' && buff[i]!='\t')
				{
					//sig += buff[i];
					//cout<<"sig="<<sig<<endl;
					string tmp;
					tmp += buff[i];
					word.push_back({"SPECIAL",tmp});
					//printf("SPECIAL %c\n",buff[i]);
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
				word.push_back({"SPECIAL","="});
				//printf("=\n"); 
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
				word.push_back({"NUM",nums});
				//printf("NUM %s\n",nums.c_str());
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
			else if(state==0 && special_WORD(buff[i]) && buff[i]!='\n' && buff[i]!=' ' && buff[i]!='\t' && buff[i]!=':' && buff[i]!='>' && buff[i]!='<')
			{
				string tmp;
				tmp += buff[i];
				word.push_back({"SPECIAL",tmp});
				//printf("SPECIAL %c\n",buff[i]);
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
				word.push_back({"SPECIAL",":="});
				//printf("SPECIAL %s\n",":=");
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
				word.push_back({"SPECIAL",">="});
				//printf("SPECIAL %s\n",">=");
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
				word.push_back({"SPECIAL","<="});
				//printf("SPECIAL %s\n","<=");
				i++;
				state = 0;
			}
			else if(state==7 && buff[i]!='=')
			{
				word.push_back({"SPECIAL",">"});
				//printf("SPECIAL %s\n",">");
				state = 0;
			}
			else if(state==8 && buff[i]!='=')
			{
				word.push_back({"SPECIAL","<"});
				//printf("SPECIAL %s\n","<");
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
bool equal(char a,char b)
{
	if(a>='a' && a<='z' && b>='a' && b<='z') return true;
	if(a>='A' && a<='Z' && b=='\'') return true;
	if(a=='\'' && b=='\'') return true;
	if(a=='>' && b=='=') return true;
	if(a=='<' && b=='=') return true;
	if(a=='!' && b=='=') return true;
	if(a==':' && b=='=') return true;
	return false;
}
string handle(string tmp)
{
	string ans;
	for(int i=0;i<tmp.size();i++)
	{
		ans += tmp[i];
		if(!equal(tmp[i],tmp[i+1])) ans += " ";
	}
	return ans;
}
string getstr(int now)
{
	for(auto it=mp.begin();it!=mp.end();++it)
	{
		if(it->second==now) return it->first;
	}
	return "null"; 
}
string RIGchange(string s)
{
	string ans;
	bool found = false;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='-' && s[i+1]=='>')
		{
			found = true;
			i++;
			continue;
		}
		if(found)
		{
			ans += s[i];
		}
	}
	ans = handle(ans);
	return ans; 
}
string LEFchange(string s)
{
	string ans;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='-') break;
		ans += s[i];
	}
	return ans;
}
string judge(string s)
{
	if(s=="$") return "NULL";
	if(s[0]>='A' && s[0]<='Z') return "NONTERMINAL";
	if(s=="if" || s=="then" || s=="else" || s=="repeat" || s=="until" || s=="read" || s=="print" || s=="endif" || s=="int" || s=="double") return "REVERSED";
	if(s=="+" || s=="-" || s=="*" || s=="/" || s=="=" || s=="<" || s==">" || s=="(" || s==")" || s==":=" || s==";" || s==">=" || s=="<=" || s=="!=" || s=="[" || s=="]") return "SPECIAL";
	for(int i=0;i<s.size();i++)
	{
		if(s[i]>='0' && s[i]<='9') return "NUM"; 
	}
	return "ID";
}
bool check(string stra,string strb)
{
	//cout<<"左边是:"<<stra<<" 类型是:"<<judge(stra)<<endl;
	//cout<<"右边是:"<<strb<<" 类型是:"<<judge(strb)<<endl;
	if(judge(stra)=="REVERSED" && judge(strb)=="REVERSED")
	{
		return stra==strb;
	}
	if(judge(stra)=="SPECIAL" && judge(strb)=="SPECIAL")
	{
		return stra==strb;
	}
	if(stra=="i" && judge(strb)=="ID") return true;
	if(stra=="n" && judge(strb)=="NUM") return true;
	return false;
}
bool isTerminal(string s)
{
	if(isAction(mp[s])) return false;
	if(judge(s)=="REVERSED" || judge(s)=="SPECIAL" || judge(s)=="ID" || judge(s)=="NUM" || s=="i" || s=="n") return true;
	return false;
}
string erasextrakonge(string s)
{
	string ans;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]==' ' && s[i+1]==' ')
		{
			i++;
			continue;
		}
		ans += s[i];
	}
	return ans;
}
void read()
{
	//freopen("init1.txt","r",stdin);
	//freopen("tbbs.txt","r",stdin);
	ifstream fin("tbbs.txt");
	n = 0;
	while(fin>>a[n])
	{
		string leftmp;
		for(int i=0;i<a[n].size();i++)
		{
			if(a[n][i]=='-') break;
			leftmp += a[n][i];
		}
		if(!mp[leftmp])
		{
			++cnt;
			mp[leftmp] = 3*cnt;
		}
		leftmp.clear();
		n++;
	}
	for(int j=0;j<n;j++)
	{
		string leftmp,rigtmp;
		bool found = false;
		for(int i=0;i<a[j].size();i++)
		{
			if(a[j][i]=='-' && a[j][i+1]=='>')
			{
				found = true;
				i++;
				continue;
			}
			if(!found) leftmp += a[j][i];
			else if(found) rigtmp += a[j][i];
		}
		if(!mp[leftmp]) 
		{
			++cnt;
			mp[leftmp] = 3*cnt; //左边为%3==0 
			//if(mp[leftmp]==3) FOLLOW[mp[leftmp]].insert("$#");
		}
		if(!mp[rigtmp]) 
		{
			++cnt;
			mp[rigtmp] = 3*cnt+1; //右边为%3==1 
		}
		G[mp[leftmp]].push_back(mp[rigtmp]);
		V[mp[rigtmp]].push_back(mp[leftmp]);
		string tmp = handle(rigtmp); //空格拆开 
		string at;
		for(int i=0;i<tmp.size();i++)
		{
			if(tmp[i]==' ')
			{
				if(!mp[at])
				{
					++cnt;
					mp[at] = 3*cnt+2;
				}
				if(!conn[mp[rigtmp]][mp[at]])
				{
					conn[mp[rigtmp]][mp[at]] = 1;
					if(mp[at]) M[mp[rigtmp]].push_back(mp[at]);
				}
				at.clear();
				continue;
			}
			at += tmp[i];
		}
		LEFT.push_back(mp[leftmp]);
		RIGHT.push_back(mp[rigtmp]);
		leftmp.clear();
		rigtmp.clear();
		found = false;
	}
	if(!mp["$"])
	{
		++cnt;
		mp["$"] = 3*cnt+2;
	}
}
void dfs(int fa,int now)
{
	vis[fa] = 1;
	bool allnull = true;
	for(int i=0;i<M[now].size();i++)
	{
		int to = M[now][i];
		string str = getstr(to);
		if(str[0]>='A' && str[0]<='Z')
		{
			if(vis[to])
			{
				bool foundnull = false;
				for(auto it=FIRST[to].begin();it!=FIRST[to].end();++it)
				{
					if(*it=="@#")
					{
						foundnull = true;
						continue;
					}
					FIRST[fa].insert(*it);
				}
				if(!foundnull) 
				{
					allnull = false;
					break;
				}
			}
			else
			{
				bool foundnull = false;
				for(int j=0;j<G[to].size();j++)
				{
					int too = G[to][j];
					dfs(to,too);
				}
				for(auto it=FIRST[to].begin();it!=FIRST[to].end();++it)
				{
					if(*it=="@#")
					{
						foundnull = true;
						continue;
					}
					FIRST[fa].insert(*it);
				}
				if(!foundnull) 
				{
					allnull = false;
					break;
				}
			}
		}
		else
		{
			allnull = false;
			if(str.substr(0,3)=="int") FIRST[fa].insert("int#");
			else if(str.substr(0,6)=="double") FIRST[fa].insert("double#");
			else if(str.substr(0,2)=="!=") FIRST[fa].insert("!=#");
			else if(str.substr(0,2)==">=") FIRST[fa].insert(">=#");
			else if(str.substr(0,2)=="<=") FIRST[fa].insert("<=#");
			else if(str.substr(0,2)==":=") FIRST[fa].insert(":=#");
			else if(str.substr(0,2)=="if") FIRST[fa].insert("if#");
			else if(str.substr(0,6)=="else") FIRST[fa].insert("else#");
			else if(str.substr(0,6)=="repeat") FIRST[fa].insert("repeat#");
			else if(str.substr(0,5)=="until") FIRST[fa].insert("until#");
			else if(str.substr(0,4)=="then") FIRST[fa].insert("then#");
			else if(str.substr(0,5)=="endif") FIRST[fa].insert("endif#");
			else if(str.substr(0,5)=="print") FIRST[fa].insert("print#");
			else if(str.substr(0,4)=="read") FIRST[fa].insert("read#");
			else if(str.substr(0,2)=="==") FIRST[fa].insert("==#");
			else 
			{
				string tmp;
				tmp += str[0];
				tmp += "#";
				if(tmp=="@#") allnull = true;
				FIRST[fa].insert(tmp);
			}
			break;
		}
	}
	if(allnull) FIRST[fa].insert("@#");
} 
void gao()
{
	for(int i=1;i<maxn;i++) //i找 A-> B-> C-> 
	{
		if(i%3==0)
		{
			if(getstr(i)=="null") continue;
			for(int j=0;j<G[i].size();j++) //j找 A->BC A->DE 
			{
				int now = G[i][j];
				dfs(i,now);
			}
		}
	}
}
void output()
{
	for(int i=1;i<maxn;i++)
	{
		if(i%3==0)
		{
			if(getstr(i)=="null") continue;
			cout<<getstr(i)<<"  FIRST:"<<endl;
			for(auto it=FIRST[i].begin();it!=FIRST[i].end();++it)
			{
				cout<<*it;
			}
			cout<<endl;
		}
	}
}
void output2()
{
	for(int i=1;i<maxn;i++)
	{
		if(i%3==1) continue;
		if(getstr(i)=="null") continue;
		if(getstr(i)[0]>='A' && getstr(i)[0]<='Z')
		{
			cout<<getstr(i)<<"  FOLLOW:"<<endl;
			for(auto it=FOLLOW[i].begin();it!=FOLLOW[i].end();++it)
			{
				cout<<*it;
			} 
			cout<<endl;
		}
	}
}
void dfs2(int now)
{
	viss[now] = 1;
	string noword = getstr(now);
	//cout<<"现在是:"<<noword<<endl;
	for(int i=0;i<RIGHT.size();i++)
	{
		int to = RIGHT[i];
		//cout<<"右边是:"<<getstr(to)<<endl;
		bool found = false;
		bool allnull = true, met = false;
		//cout<<"to是:"<<getstr(to)<<endl;
		for(int j=0;j<M[to].size();j++)
		{
			if(M[to][j]==now && j==M[to].size()-1)
			{
				found = true;
				met = true;
				break;
			}
			else if(M[to][j]==now)
			{
				found = true;
				continue;
			}
			if(found)
			{
				met = true;
				//cout<<"pass"<<endl;
				//cout<<"getstr是:"<<getstr(M[to][j])<<endl;
				if(getstr(M[to][j])[0]>='A' && getstr(M[to][j])[0]<='Z')
				{
					bool foundnull = false;
					for(auto it=FIRST[M[to][j]].begin();it!=FIRST[M[to][j]].end();++it)
					{
						if(*it=="@#")
						{
							foundnull = true;
							continue;
						}
						FOLLOW[now].insert(*it);
					}
					if(!foundnull) 
					{
						allnull = false;
						break;
					}
				}
				else
				{
					allnull = false;
					string str = getstr(M[to][j]);
					if(str.substr(0,3)=="int") FOLLOW[now].insert("int#");
					else if(str.substr(0,6)=="double") FOLLOW[now].insert("double#");
					else if(str.substr(0,2)=="!=") FOLLOW[now].insert("!=#");
					else if(str.substr(0,2)==">=") FOLLOW[now].insert(">=#");
					else if(str.substr(0,2)=="<=") FOLLOW[now].insert("<=#");
					else if(str.substr(0,2)==":=") FOLLOW[now].insert(":=#");
					else if(str.substr(0,2)=="if") FOLLOW[now].insert("if#");
					else if(str.substr(0,6)=="else") FOLLOW[now].insert("else#");
					else if(str.substr(0,6)=="repeat") FOLLOW[now].insert("repeat#");
					else if(str.substr(0,5)=="until") FOLLOW[now].insert("until#");
					else if(str.substr(0,4)=="then") FOLLOW[now].insert("then#");
					else if(str.substr(0,5)=="endif") FOLLOW[now].insert("endif#");
					else if(str.substr(0,5)=="print") FOLLOW[now].insert("print#");
					else if(str.substr(0,4)=="read") FOLLOW[now].insert("read#");
					else if(str.substr(0,2)=="==") FIRST[now].insert("==#");
					else 
					{
						string tmp;
						tmp += str[0];
						tmp += "#";
						FOLLOW[now].insert(tmp);
					}
					break;
				}
			}
		}
		//cout<<allnull<<"和"<<met<<endl;
		//if(now==mp["E'"]) cout<<allnull<<"和"<<met<<"和"<<found<<endl;
		if(allnull && met && found)
		{
			//cout<<"求得是:"<<getstr(now)<<endl; 
			//cout<<"now是:"<<getstr(now)<<endl;
			//cout<<getstr(now)<<"的左边是:"<<getstr(V[to][0])<<endl;
			for(int i=0;i<V[to].size();i++)
			{
				//cout<<"现在是:"<<getstr(V[to][i])<<endl;
				if(viss[V[to][i]])
				{
					//cout<<"现在是:"<<getstr(V[to][i])<<"求得是:"<<getstr(now)<<endl;
					if(!FOLLOW[V[to][i]].empty())
					{
						for(auto it=FOLLOW[V[to][i]].begin();it!=FOLLOW[V[to][i]].end();++it)
						{
							FOLLOW[now].insert(*it);
						}
						//break;
					}
				}
				else
				{
					viss[V[to][i]] = 1;
					//cout<<"左边是:"<<getstr(V[to][0])<<endl;
					dfs2(V[to][i]);
					for(auto it=FOLLOW[V[to][i]].begin();it!=FOLLOW[V[to][i]].end();++it)
					{
						FOLLOW[now].insert(*it);
					}
				}
			}
		}
	}
}
void gao2()
{
	FOLLOW[3].insert("$#");
	for(int i=1;i<maxn;i++)
	{
		if(i%3==1) continue;
		if(getstr(i)=="null") continue;
		if(getstr(i)[0]>='A' && getstr(i)[0]<='Z') dfs2(i);
	}
}
void gao3()
{
	for(int i=0;i<n;i++)
	{
		string tmp = RIGchange(a[i]);
		string leftmp = LEFchange(a[i]); //左边 
		string tt;
		bool allnull = true;
		for(int j=0;j<tmp.size();j++)
		{
			if(tmp[j]==' ')
			{
				bool foundnull = false;
				if(tt[0]>='A' && tt[0]<='Z')
				{
					for(auto it=FIRST[mp[tt]].begin();it!=FIRST[mp[tt]].end();++it)
					{
						if(*it=="@#")
						{
							foundnull = true;
							continue;
						}
						//cout<<mp[leftmp]<<endl;
						if(a[i]=="A->BA'") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "A->BA'addsub";
						else if(a[i]=="A'->+A") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "A'->+Aadd";
						else if(a[i]=="A'->-A") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "A'->-Asub";
						else if(a[i]=="B->CB'") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "B->CB'muldiv";
						else if(a[i]=="B'->*B") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "B'->*Bmul";
						else if(a[i]=="B'->/B") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "B'->/Bdiv";
						else if(a[i]=="C->(A)") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "C->(A)tranac";
						else if(a[i]=="C->i") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "C->assc i";
						else if(a[i]=="C->n") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "C->assc n";
						else if(a[i]=="K->F:=Z;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "K->F:=Zeq;";
						else if(a[i]=="F->i") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "F->assf i";
						else if(a[i]=="F->n") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "F->assf n";
						else if(a[i]=="L->[GR]") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "L->[GR]comp";
						else if(a[i]=="R-><G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R-><Gcompl";
						else if(a[i]=="R->>G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->>Gcompg";
						else if(a[i]=="R-><=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R-><=Gcomple";
						else if(a[i]=="R->>=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->>=Gcompge";
						else if(a[i]=="R->=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->=Gcompe";
						else if(a[i]=="R->!=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->!=Gcompne";
						else if(a[i]=="M->intH;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "M->intH;";
						else if(a[i]=="M->doubleH;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "M->doubleH;";
						else if(a[i]=="H->i") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "H->i";
						else if(a[i]=="N->repeatIuntilT;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "N->repeat reIuntilTrebackpatch;";
						else if(a[i]=="O->ifJthenUelseQendif;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "O->ifJiffj thenUiffjbackpatch ifrj elseQifrjbackpatch endif;";
						else TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = a[i];
						//cout<<"第一维是:"<<leftmp<<" 第二维是:"<<(*it).substr(0,(*it).length()-1)<<" 值是:"<<a[i]<<endl;
					}
					tt.clear();
					if(!foundnull)
					{
						allnull = false;
						break;
					}
				}
				else
				{
					allnull = false;
					string str = tt;
					string tai = a[i];
					if(a[i]=="A->BA'") tai = "A->BA'addsub";
					else if(a[i]=="A'->+A") tai = "A'->+Aadd";
					else if(a[i]=="A'->-A") tai = "A'->-Asub";
					else if(a[i]=="B->CB'") tai = "B->CB'muldiv";
					else if(a[i]=="B'->*B") tai = "B'->*Bmul";
					else if(a[i]=="B'->/B") tai = "B'->/Bdiv";
					else if(a[i]=="C->(A)") tai = "C->(A)tranac";
					else if(a[i]=="C->i") tai = "C->assc i";
					else if(a[i]=="C->n") tai = "C->assc n";
					else if(a[i]=="K->F:=Z;") tai = "K->F:=Zeq;";
					else if(a[i]=="F->i") tai = "F->assf i";
					else if(a[i]=="F->n") tai = "F->assf n";
					else if(a[i]=="L->[GR]") tai = "L->[GR]comp";
					else if(a[i]=="R-><G") tai = "R-><Gcompl";
					else if(a[i]=="R->>G") tai = "R->>Gcompg";
					else if(a[i]=="R-><=G") tai = "R-><=Gcomple";
					else if(a[i]=="R->>=G") tai = "R->>=Gcompge";
					else if(a[i]=="R->=G") tai = "R->=Gcompe";
					else if(a[i]=="R->!=G") tai = "R->!=Gcompne";
					else if(a[i]=="M->intH;") tai = "M->intH;";
					else if(a[i]=="M->doubleH;") tai = "M->doubleH;";
					else if(a[i]=="H->i") tai = "H->i";
					else if(a[i]=="N->repeatIuntilT;") tai = "N->repeat reIuntilTrebackpatch;";
					else if(a[i]=="O->ifJthenUelseQendif;") tai = "O->ifJiffj thenUiffjbackpatch ifrj elseQifrjbackpatch endif;";
					else tai = a[i];
					if(str.substr(0,3)=="int") TABLE[mp[leftmp]][mp["int"]] = tai;
					else if(str.substr(0,6)=="double") TABLE[mp[leftmp]][mp["double"]] = tai;
					else if(str.substr(0,2)=="!=") TABLE[mp[leftmp]][mp["!="]] = tai;
					else if(str.substr(0,2)==">=") TABLE[mp[leftmp]][mp[">="]] = tai;
					else if(str.substr(0,2)=="<=") TABLE[mp[leftmp]][mp["<="]] = tai;
					else if(str.substr(0,2)==":=") TABLE[mp[leftmp]][mp[":="]] = tai;
					else if(str.substr(0,2)=="if") TABLE[mp[leftmp]][mp["if"]] = tai;
					else if(str.substr(0,6)=="else") TABLE[mp[leftmp]][mp["else"]] = tai;
					else if(str.substr(0,6)=="repeat") TABLE[mp[leftmp]][mp["repeat"]] = tai;
					else if(str.substr(0,5)=="until") TABLE[mp[leftmp]][mp["until"]] = tai;
					else if(str.substr(0,4)=="then") TABLE[mp[leftmp]][mp["then"]] = tai;
					else if(str.substr(0,5)=="endif") TABLE[mp[leftmp]][mp["endif"]] = tai;
					else if(str.substr(0,5)=="print") TABLE[mp[leftmp]][mp["print"]] = tai;
					else if(str.substr(0,4)=="read") TABLE[mp[leftmp]][mp["read"]] = tai;
					else if(str.substr(0,2)=="==") TABLE[mp[leftmp]][mp["=="]] = tai;
					else 
					{
						string ttmp;
						ttmp += tt[0];
						if(ttmp=="@") allnull = true;
						TABLE[mp[leftmp]][mp[ttmp]] = tai;
						//cout<<"第一维是:"<<leftmp<<" 第二维是:"<<ttmp<<" 值是:"<<a[i]<<endl;
					}
				}
				continue;
			}
			tt += tmp[j];
		}
		if(allnull)
		{
			for(auto it=FOLLOW[mp[leftmp]].begin();it!=FOLLOW[mp[leftmp]].end();++it)
			{
				if(a[i]=="A->BA'") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "A->BA'addsub";
				else if(a[i]=="A'->+A") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "A'->+Aadd";
				else if(a[i]=="A'->-A") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "A'->-Asub";
				else if(a[i]=="B->CB'") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "B->CB'muldiv";
				else if(a[i]=="B'->*B") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "B'->*Bmul";
				else if(a[i]=="B'->/B") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "B'->/Bdiv";
				else if(a[i]=="C->(A)") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "C->(A)tranac";
				else if(a[i]=="C->i") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "C->assc i";
				else if(a[i]=="C->n") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "C->assc n";
				else if(a[i]=="K->F:=Z;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "K->F:=Zeq;";
				else if(a[i]=="F->i") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "F->assf i";
				else if(a[i]=="F->n") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "F->assf n";
				else if(a[i]=="L->[GR]") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "L->[GR]comp";
				else if(a[i]=="R-><G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R-><Gcompl";
				else if(a[i]=="R->>G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->>Gcompg";
				else if(a[i]=="R-><=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R-><=Gcomple";
				else if(a[i]=="R->>=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->>=Gcompge";
				else if(a[i]=="R->=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->=Gcompe";
				else if(a[i]=="R->!=G") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "R->!=Gcompne";
				else if(a[i]=="M->intH;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "M->intH;";
				else if(a[i]=="M->doubleH;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "M->doubleH;";
				else if(a[i]=="H->i") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "H->i";
				else if(a[i]=="N->repeatIuntilT;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "N->repeat reIuntilTrebackpatch;";
				else if(a[i]=="O->ifJthenUelseQendif;") TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = "O->ifJiffj thenUiffjbackpatch ifrj elseQifrjbackpatch endif;";
				else TABLE[mp[leftmp]][mp[(*it).substr(0,(*it).length()-1)]] = a[i];
				//cout<<"第一维是:"<<leftmp<<" 第二维是:"<<(*it).substr(0,(*it).length()-1)<<" 值是:"<<a[i]<<endl;
			}
		}
	}
}
void output3()
{
	for(int j=1;j<maxn;j++)
	{
		if(j%3==2 || (j%3==1 && getstr(j).size()==1))
		{
			if(getstr(j)[0]>='A' && getstr(j)[0]<='Z') continue;
			if(getstr(j)=="null") continue;
			if(getstr(j)=="@") continue;
			cout<<setw(15)<<getstr(j); 
		}
	}
	cout<<endl;
	for(int i=1;i<maxn;i++)
	{
		if(i%3) continue;
		if(getstr(i)=="null") continue;
		cout<<getstr(i);
		for(int j=1;j<maxn;j++)
		{
			if(j%3==2 || (j%3==1 && getstr(j).size()==1))
			{
				if(getstr(j)[0]>='A' && getstr(j)[0]<='Z') continue;
				if(getstr(j)=="null") continue;
				if(getstr(j)=="@") continue;
				cout<<setw(15)<<TABLE[i][j];
			}	
		}
		cout<<endl;
	}
}
void PDAinit()
{
	//ifstream fin("output.txt");
	//freopen("PDAtest.txt","r",stdin);
	anstack.push(mp["$"]);
	anstack.push(3);
//	inputstream.push_back("i");
//	inputstream.push_back("+");
//	inputstream.push_back("i");
//	inputstream.push_back("*");
//	inputstream.push_back("i");
//	inputstream.push_back("$");
//	string tmpa,tmpb;
//	while(fin>>tmpa>>tmpb)
//	{
//		inputstream.push_back({tmpa,tmpb});
//	}
	for(int i=0;i<word.size();i++)
	{
		inputstream.push_back({word[i].name,word[i].value});
	}
	inputstream.push_back({"TERMINAL","$"});
}
void ACTIONgao(string act,string now);
bool PDAgao()
{
	int nowid = 0;
	while(nowid<inputstream.size() && !anstack.empty())
	{
		string now = inputstream[nowid].value;
		int sktop = anstack.top();
		anstack.pop();
		string nowtmp;
		nowtmp.clear();
		if(check(getstr(sktop),now) && now!="$")
		{
			nowid++;
			continue;
		}
		else if(getstr(sktop)==now && now=="$") return true;
		if(isTerminal(getstr(sktop)) && isTerminal(now))
		{
			if(!check(getstr(sktop),now)) 
			{
				string tmp;
				tmp += now;
				tmp += " can not match!"; 
				phraseError.push_back({"num",tmp});
				nowid++;
			}
		}
		if(isAction(sktop))
		{
			//cout<<"这个东西是动作: "<<getstr(sktop)<<endl;
			ACTIONgao(trim(getstr(sktop)),trim(now));
			continue;
		}
		if(judge(now)=="ID") nowtmp = TABLE[sktop][mp["i"]];
		else if(judge(now)=="REVERSED") nowtmp = TABLE[sktop][mp[now]];
		else if(judge(now)=="SPECIAL") nowtmp = TABLE[sktop][mp[now]];
		else if(judge(now)=="NUM") nowtmp = TABLE[sktop][mp["n"]];
		else if(judge(now)=="NULL") nowtmp = TABLE[sktop][mp["$"]];
		//if(nowtmp.empty()) return false;
		//cout<<"现在是:"<<nowtmp<<endl;
		nowtmp = erasextrakonge(nowtmp);
		//cout<<"改变后:"<<nowtmp<<endl;
		string nowttmp = RIGchange(nowtmp); //将推导右边拿出来，并且加入空格分隔
		nowttmp = erasextrakonge(nowttmp);
		//cout<<"右边是:"<<nowttmp<<endl;
		outputstream.push_back(nowtmp);
		if(nowttmp=="@ ")
		{
			continue;
		} 
		string nowtt;
		stack<int> tmpstk;
		while(!tmpstk.empty()) tmpstk.pop();
		for(int i=0;i<nowttmp.size();i++)
		{
			if(nowttmp[i]==' ')
			{
				//cout<<"插入了: "<<nowtt<<"编号是: "<<mp[nowtt]<<endl; 
				tmpstk.push(mp[nowtt]);
				nowtt.clear();
				continue;
			}
			nowtt += nowttmp[i];
		}
		while(!tmpstk.empty())
		{
			int tmptop = tmpstk.top();
			tmpstk.pop();
			anstack.push(tmptop);
		}
	}
	return false;
}
void PDAoutput()
{
	cout<<"compile success"<<endl;
	for(string s:outputstream) cout<<s<<endl;
}
void ACTIONgao(string act,string now)
{
	//cout<<"动作: "<<act<<"  输入流: "<<now<<endl;
	if(act=="addsub")
	{
		if(OP=="+" || OP=="-")
		{
			ARG2 = semanticstk.top();
			semanticstk.pop();
			ARG1 = semanticstk.top();
			semanticstk.pop();
			RES = newTemp();
			fourelement.push_back({++fourtot,OP,ARG1,ARG2,RES});
			mpvalue["A"] = RES;
			semanticstk.push(RES);
			OP.clear();
		}
	}
	else if(act=="add")
	{
		OP = "+";
	}
	else if(act=="sub")
	{
		OP = "-";
	}
	else if(act=="muldiv")
	{
		if(OP=="*" || OP=="/")
		{
			ARG2 = semanticstk.top();
			semanticstk.pop();
			ARG1 = semanticstk.top();
			semanticstk.pop();
			RES = newTemp();
			fourelement.push_back({++fourtot,OP,ARG1,ARG2,RES});
			mpvalue["B"] = RES;
			semanticstk.push(RES);
			OP.clear();
		}
	}
	else if(act=="mul")
	{
		OP = "*";
	}
	else if(act=="div")
	{
		OP = "/";
	}
	else if(act=="tranac")
	{
		mpvalue["C"] = mpvalue["A"];
	}
	else if(act=="assc")
	{
		mpvalue["C"] = now;
		semanticstk.push(now);
	}
	else if(act=="assf")
	{
		mpvalue["F"] = now;
		semanticstk.push(now);
	}
	else if(act=="assh")
	{
		mpvalue["H"] = now;
		semanticstk.push(now);
	}
	else if(act=="eq")
	{
		OP = ":=";
		ARG1 = semanticstk.top();
		semanticstk.pop();
		RES = semanticstk.top();
		semanticstk.pop();
		fourelement.push_back({++fourtot,OP,ARG1,"/",RES});
		OP.clear();
	}
	else if(act=="comp")
	{
		if(OP=="<" || OP==">" || OP=="=" || OP=="<=" || OP==">=" || OP=="!=")
		{
			ARG2 = semanticstk.top();
			semanticstk.pop();
			ARG1 = semanticstk.top();
			semanticstk.pop();
			RES = newTemp();
			fourelement.push_back({++fourtot,OP,ARG1,ARG2,RES});
			mpvalue["L"] = RES;
			semanticstk.push(RES);
			OP.clear();
		}
		
	}
	else if(act=="compl")
	{
		OP = "<";
	}
	else if(act=="compg")
	{
		OP = ">";
	}
	else if(act=="comple")
	{
		OP = "<=";
	}
	else if(act=="compge")
	{
		OP = ">=";
	}
	else if(act=="compe")
	{
		OP = "=";
	}
	else if(act=="compne")
	{
		OP = "!=";
	}
	else if(act=="re")
	{
		fourelement.push_back({++fourtot,"RP","/","/",to_string(fourtot+1)});
		repeat.push(fourtot);
	}
	else if(act=="rebackpatch")
	{
		ARG1 = semanticstk.top();
		semanticstk.pop();
		//backpatch(repeat.top(),fourtot+1);
		fourelement.push_back({++fourtot,"FJ",ARG1,"/",to_string(repeat.top())});
		repeat.pop(); 
	}
	else if(act=="iffj")
	{
		OP = "FJ";
		ARG1 = semanticstk.top();
		semanticstk.pop();
		fourelement.push_back({++fourtot,OP,ARG1,"/",RES});
		if_fj.push(fourtot);
		OP.clear();
	}
	else if(act=="iffjbackpatch")
	{
		backpatch(if_fj.top(),fourtot+2);
		if_fj.pop();
	}
	else if(act=="ifrj")
	{
		OP = "RJ";
		fourelement.push_back({++fourtot,OP,"/","/","/"});
		if_rj.push(fourtot);
		OP.clear();
	}
	else if(act=="ifrjbackpatch")
	{
		backpatch(if_rj.top(),fourtot+1);
		if_rj.pop();
	}
	else if(act=="intt")
	{
		
	}
	else if(act=="doublee")
	{
		
	}
}
void outputfour()
{
	cout<<"FourElement:"<<endl;
	for(int i=0;i<fourelement.size();i++)
	{
		cout<<i+1<<":("<<fourelement[i].op<<","<<fourelement[i].arg1<<","<<fourelement[i].arg2<<","<<fourelement[i].result<<")"<<endl;
	}
}
void FOURinit()
{
	ifstream ffin("FOUTinit.txt");
	string str;
	while(ffin>>str)
	{
		if(!mp[str])
		{
			++cnt;
			mp[str] = 3*cnt+2;
		}
		actionset.insert(str);
	}
}
void outputPhraseError()
{
	for(int i=0;i<phraseError.size();i++)
	{
		cout<<phraseError[i].value<<endl;
	}
	//for(string s : phraseError) cout<<s<<endl;
}
int main()
{
	freopen("result.txt","w",stdout);
	init_WORD();
	getToken_WORD();
	//cout<<word.size()<<endl;
	read();
	gao();
	output();
	gao2();
	output2();
	gao3();
	output3();
	PDAinit();
	FOURinit();
	//bool success = PDAgao();
	PDAgao();
	if(phraseError.size()==0)
	{
		PDAoutput();
		outputfour();
	}
	else 
	{
		cout<<"语法分析错误"<<endl;
		outputPhraseError();
	}
	//cout<<mp["a"]<<endl;
	return 0;
} 
