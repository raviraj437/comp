#include<bits/stdc++.h>
using namespace std;
map< string, vector<string> > mp;
map< string, vector<string> > First;
map< string, vector<string> > Follow;
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
    if(nt=="E")
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
void buildFollow(){
    for(auto it:mp)
        buildFollow(it.first);
}
map<string, map<string, string> > pt;
void buildPT(){
    vector<string> term={"I","(",")","*","+","$"};
    vector<string> nterm={"E","K","T","L","F"};
    for(string s1:nterm){
        for(string s2:mp[s1]){
            vector<string> v1=buildFirst(s1,s2);
            bool flag=false;
            for(string s3:v1){
                if(s3!="")
                    pt[s1][s3]=s2;
                else
                    flag=true;
            }
            if(flag){
                for(string s3:Follow[s1])
                    pt[s1][s3]=s2;
            }
        }
    }
    cout<<"PARSING TABLE"<<endl;
    cout<<"\t";
    for(string s1:term)
        cout<<s1<<"\t";
    cout<<endl;
    for(string s1:nterm){
        cout<<s1<<"\t";
        for(string s2:term){
            if(pt[s1].find(s2)!=pt[s1].end())
                cout<<"\""<<pt[s1][s2]<<"\"\t";
        }
        cout<<endl;
    }


}
void printFirst(){
    cout<<"FIRST"<<endl;
    for(auto it:First){
        cout<<it.first<<"\t";
        for(string s:it.second){
            cout<<"\""<<s<<"\"\t";
        }
        cout<<endl;
    }

}
void printFollow(){
    cout<<"FOLLOW"<<endl;
    for(auto it:Follow){
        cout<<it.first<<"\t";
        for(string s:it.second){
            cout<<"\""<<s<<"\"\t";
        }
        cout<<endl;
    }

}
void init(){
    vector<string> rulE={"TK"};
    vector<string> rulK={"+TK",""};
    vector<string> rulT={"FL"};
    vector<string> rulL={"*FL",""};
    vector<string> rulF={"I","(E)"};
    mp["E"]=rulE;
    mp["K"]=rulK;
    mp["T"]=rulT;
    mp["L"]=rulL;
    mp["F"]=rulF;
    buildFirstNT("E");buildFirstNT("K");buildFirstNT("T");buildFirstNT("L");buildFirstNT("F");
    buildFollow();
    printFirst();
    printFollow();
    buildPT();
}

int main(){
    init();
    cout<<"Enter string to parse"<<endl;
    string s1;
    cin>>s1;
    cout<<left<<setw(15);
    cout<<"STACK"<<left<<setw(15)<<"INPUT"<<left<<setw(15)<<"ACTION"<<endl;
    s1=s1+"$";
    string stk="$E";
    bool flag=true;
    while(flag){
        cout<<left<<setw(15)<<stk<<left<<setw(15)<<s1;
        if(s1=="$"&&stk=="$"){
            cout<<"ACCEPTED"<<endl;
            break;
        }

        if(stk[stk.length()-1]==s1[0]){
            stk.erase(stk.begin()+stk.length()-1);
            s1.erase(s1.begin());
            cout<<endl;
            continue;
        }
        string fl(1,stk[stk.length()-1]);
        if(mp.find(fl)==mp.end()){
            cout<<"ERROR"<<endl;
            break;

        }
        string fll(1,s1[0]);
        if(pt[fl].find(fll)==pt[fl].end()){
            cout<<"ERROR"<<endl;
            break;
        }
        string rl=pt[fl][fll];
        cout<<fl<<"->"<<"\""<<rl<<"\""<<endl;
        reverse(rl.begin(),rl.end());
        stk.erase(stk.begin()+stk.length()-1);
        stk=stk+rl;

    }



    return 0;
}
