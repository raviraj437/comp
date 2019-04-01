#include<bits/stdc++.h>
using namespace std;
map< string, vector<string> > mp;
map< string, vector<string> > First;
map< string, vector<string> > Follow;
vector< vector< vector<string> > > dfa;
map< int,map<string,int> > dfamp;
class pti{
    public:
    bool shift,action;
    int ns;
    vector<string> rul;
    bool acc;

};
map<int, map<string,pti> > pt;
vector<string> term,nterm;
map<string,int> newmap(){
    map<string,int> mpp;
    return mpp;
}
vector< vector<string> > transit(vector< vector<string> > state,string sym){
    vector< vector<string> > nexts;
    for(vector<string> item:state){
        if(item[2]=="")
            continue;
        string fl(1,item[2][0]);
        if(sym==fl){
            vector<string> vs={item[0],item[1]+fl,item[2].substr(1)};
            if(find(nexts.begin(),nexts.end(),vs)==nexts.end())
                nexts.push_back(vs);
        }
    }
    return nexts;
}
vector< vector<string> > closure(vector< vector<string> > state){
    vector< vector<string> > cl=state;
    vector< vector<string> > temp;
    for(vector<string> item:cl){
        if(item[2]=="")
            continue;
        string fl(1,item[2][0]);
        if(find(nterm.begin(),nterm.end(),fl)!=nterm.end()){
            for(string rl:mp[fl]){
                vector<string> nextitem={fl,"",rl};
                if(find(cl.begin(),cl.end(),nextitem)==cl.end())
                    if(find(temp.begin(),temp.end(),nextitem)==temp.end())
                        temp.push_back(nextitem);
            }
        }    
    }
    if(temp.empty())
        return cl;
    for(vector<string> item:temp)
        cl.push_back(item);
    cl=closure(cl);
    return cl;    
}
int build(vector< vector<string> > state){
    state=closure(state);
    sort(state.begin(),state.end());
    auto it=find(dfa.begin(),dfa.end(),state);
    //return number of current state;
    if(it!=dfa.end()){
        return it-dfa.begin();
    }
    else{
        dfa.push_back(state);
        int curs=dfa.size()-1;
        dfamp[curs]=newmap();
        vector<string> symbols;
        for(auto vc:state){
            if(vc[2]!=""){
                string fl(1,vc[2][0]);
                if(find(symbols.begin(),symbols.end(),fl)==symbols.end())
                    symbols.push_back(fl);
            }
        }
        for(string sym:symbols){
            int ns=build(transit(state,sym));
            dfamp[curs][sym]=ns;
        }
        return curs;
    }
}

void buildFirstNT(string nt);
vector<string> buildFirst(string nt,string s){
    vector<string> ans;
    if(nt==s)
        return ans;
    if(s==""){
        ans.push_back("");
        return ans;
    }
    string fl(1,s[0]);
    if(mp.find(fl)==mp.end()){
        ans.push_back(fl);
        return ans;
    }
    if(First.find(fl)==First.end()){
        buildFirstNT(fl);
    }
    for(string s:First[fl])
            ans.push_back(s);
    auto it=find(ans.begin(),ans.end(),"");
    if(it!=ans.end()){
        ans.erase(it);
        vector<string> v=buildFirst(nt,s.substr(1));
        for(string s:v)
            if(find(ans.begin(),ans.end(),s)==ans.end())
                ans.push_back(s);
    }
    return ans;

}
void buildFirstNT(string nt){
        if(First.find(nt)!=First.end())
            return;
        set<string> curF;
        for(string s:mp[nt]){
            vector<string> v=buildFirst(nt,s);
            for(string x:v)
                curF.insert(x);
        }
        vector<string> curFv;
        for(string s:curF)
            curFv.push_back(s);
        First[nt]=curFv;

}
void buildFollow(string nt){
    vector<string> ans;
    if(nt=="X")
        ans.push_back("$");
    for(auto it:mp){
        for(string rl:it.second){
            int found;
            while((found=rl.find(nt))!=string::npos){
                rl=rl.substr(found+1);
                vector<string> fst=buildFirst(it.first,rl);
                bool empflag=false;
                if(find(fst.begin(),fst.end(),"")!=fst.end()){
                    empflag=true;
                    fst.erase(find(fst.begin(),fst.end(),""));
                }
                for(string s1:fst)
                    if(find(ans.begin(),ans.end(),s1)==ans.end())
                        ans.push_back(s1);
                if(empflag&&(it.first!=nt)){
                    if(Follow.find(it.first)==Follow.end()){
                        buildFollow(it.first);
                    }
                    for(string s1:Follow[it.first])
                        if(find(ans.begin(),ans.end(),s1)==ans.end())
                            ans.push_back(s1);

                }        
            }
        }
    }
    Follow[nt]=ans;
} 
void init(){
    term={"a","b"};
    nterm={"X","S","A"};
    vector<string> rulX={"S"};
    vector<string> rulS={"AA"};
    vector<string> rulA={"aA","b"};
    mp["X"]=rulX;
    mp["S"]=rulS;
    mp["A"]=rulA;
    vector< vector<string> > start={{"X","","S"}};
    build(start);
    bool flag=true;
    cout<<"--------ITEMS ARE--------"<<endl;
    for(int i=0;i<dfa.size();i++){
        cout<<"ITEMS IN I"<<i<<endl;
        for(vector<string> item:dfa[i])
            cout<<item[0]<<"->"<<item[1]<<"."<<item[2]<<endl;
    }
    cout<<"----PARSING TABLE----"<<endl;
    cout<<setw(15)<<"ACTION"<<setw(60)<<"GOTO"<<endl;
    nterm={"S","A"};
    term.push_back("$");
    for(string sy:term){
        cout<<setw(15)<<sy;
    }
    for(string sy:nterm)
        cout<<setw(15)<<sy;
    cout<<endl;
    
    bool valid=true;
    for(int i=0;i<dfa.size();i++){
        for(string sy:term){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=true;
                it.action=true;
                it.ns=dfamp[i][sy];
                it.acc=false;
                pt[i][sy]=it;
            }
        }
        for(string sy:nterm){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=false;
                it.action=false;
                it.ns=dfamp[i][sy];
                it.acc=false;
                pt[i][sy]=it;
            }
        }
    }
    vector<string> acr={"X","S",""};
    for(int i=0;i<dfa.size();i++){
        for(auto rule:dfa[i]){
            if(rule[2]!="")
                continue;
            if(rule==acr){
                pti it;
                it.acc=true;
                pt[i]["$"]=it;
            }
            buildFollow(rule[0]);
            for(string x:Follow[rule[0]]){                
                pti it;
                it.action=true;
                it.shift=false;
                it.rul=rule;
                pt[i][x]=it;
            }
        }
    }
    for(auto df:pt){
        string opp="I";
        opp=opp+to_string(df.first);
        cout<<left<<setw(15)<<opp;
        for(string s:term){
            if(df.second.find(s)!=df.second.end()){
                pti it=df.second[s];
                if(it.acc)
                    cout<<left<<setw(15)<<"ACC";
                else if(it.shift){
                    opp="S";
                    opp+=to_string(it.ns);
                    cout<<left<<setw(15)<<opp;
                }
                    
                else{
                    cout<<left<<setw(15);
                    string op="R(";
                    op+=it.rul[0];
                    op=op+"->";
                    op=op+it.rul[1];
                    op=op+")";
                    cout<<op;
                }
                    
                
                
            }
            else
                cout<<left<<setw(15)<<"";
        ;
        }
        for(string s:nterm){
            cout<<left<<setw(15);
            if(df.second.find(s)!=df.second.end())
                cout<<df.second[s].ns;
            else
                cout<<left<<setw(15)<<"";
           
        }
        cout<<endl;
        
    }
    if(flag)
        cout<<"Given Grammar is LR(1)"<<endl;
    else
        cout<<"Given Grammar is not LR(1)"<<endl;
}
int main(){
    init();
    cout<<"Enter string to parse"<<endl;
    string s1;
    cin>>s1;
    cout<<left<<setw(15);
    cout<<"STACK"<<left<<setw(15)<<"INPUT"<<left<<setw(15)<<"ACTION"<<endl;
    s1=s1+"$";
    string stk="$0";
    while(1){
        cout<<left<<setw(15)<<stk<<left<<setw(15)<<s1;
        string cs(1,stk[stk.length()-1]);
        string nip(1,s1[0]);
        if(pt[stoi(cs)].find(nip)==pt[stoi(cs)].end()){
            cout<<"ERROR"<<endl;
            break;
        }
        pti it=pt[stoi(cs)][nip];
        if(it.acc){
            cout<<"ACCEPTED"<<endl;
            break;
        }
        else if(it.action&&it.shift){
            cout<<"SHIFT"<<endl;
            stk+=nip;
            stk+=to_string(it.ns);
            s1=s1.substr(1);
        }
        else{
            cout<<"REDUCE "<<it.rul[0]<<"->"<<it.rul[1]<<endl;
            int len=it.rul[1].length();
            for(int i=0;i<2*len;i++){
                stk.erase(stk.begin()+stk.length()-1);
            }
            string ccs(1,stk[stk.length()-1]);
            cs=ccs;
            stk+=it.rul[0];
            stk+=to_string(pt[stoi(cs)][it.rul[0]].ns);
        }
    }

}