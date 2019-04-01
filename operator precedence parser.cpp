#include<bits/stdc++.h>
using namespace std;
char pec[256][256]={0};
int main(){
pec['I']['*']=pec['I']['+']=pec['I']['$']=pec['*']['*']=pec['*']['+']=pec['*']['$']=pec['+']['+']=pec['+']['$']='>';
pec['*']['I']=pec['+']['I']=pec['+']['*']=pec['$']['I']=pec['$']['*']=pec['$']['+']='<';
    string ip;
    cout<<"ENTER THE STRING TO BE PARSED"<<endl;
    cin>>ip;
    string ooo="$";
    ooo=ooo+ip;
    ip=ooo+"$";
    int count=0;
    for(int i=0;i<ip.length();i++)
        if(ip[i]=='I')
            count++;
    string cur="$";
    for(int i=1;i<ip.length();i++){
        cur+=pec[ip[i-1]][ip[i]];
        cur+=ip[i];
    }
    cout<<cur<<endl;
    size_t found=cur.find("<I>");
    while(found!=string::npos){
        cur.replace(found,3,"E");
        cout<<cur<<endl;
        found=cur.find("<I>");
    }
    found=cur.find("E");
    while(found!=string::npos){
        cur.replace(found,1,"");
        found=cur.find("E");
    }
    cout<<cur<<endl;
    ip=cur;
     cur="$";
    for(int i=1;i<ip.length();i++){
        cur+=pec[ip[i-1]][ip[i]];
        cur+=ip[i];
    }
    cout<<cur<<endl;
    size_t found2=cur.find("<+>");
    found=cur.find("<*>");
    while(found2!=string::npos||found!=string::npos){
        if(found==string::npos)
            cur.replace(found2,3,"");
        else if(found2==string::npos)
            cur.replace(found,3,"");
        else{
            if(found<found2){
                cur.replace(found,3,"");
            }
            else
                cur.replace(found2,3,"");
        }
        ip="";
        for(int i=0;i<cur.length();i++)
            if(cur[i]=='*'||cur[i]=='$'||cur[i]=='+')
                ip=ip+cur[i];
        cur="$";
        for(int i=1;i<ip.length();i++){
            cur+=pec[ip[i-1]][ip[i]];
            cur+=ip[i];
        }
        cout<<cur<<endl;
        found=cur.find("<*>");
        found2=cur.find("<+>");
    }

    return 0;
}
