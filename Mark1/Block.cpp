#include<bits/stdc++.h>
#include <sqlite3.h>
#include "sha256.h"
typedef long long ll;
using namespace std;
/*
Unique ID's
Vanshaj-123
Utkarsh-321
Kartik-132
Pandey-231
*/
vector<string> users={"Vanshaj","Utkarsh","Kartik","Saksham"};
vector<int> ys={53,89,68,34};
ll r;
string hex2bin(string s)
{
    unordered_map<char, string> mp;
    mp['0']="0000";
    mp['1']="0001";
    mp['2']="0010";
    mp['3']="0011";
    mp['4']="0100";
    mp['5']="0101";
    mp['6']="0110";
    mp['7']="0111";
    mp['8']="1000";
    mp['9']="1001";
    mp['A']="1010";
    mp['B']="1011";
    mp['C']="1100";
    mp['D']="1101";
    mp['E']="1110";
    mp['F']="1111";
    string bin="";
    for (int i=0;i<s.size();i++)
        bin+=mp[s[i]];
    return bin;
}
string bin2hex(string s)
{
    unordered_map<string, string> mp;
    mp["0000"]="0";
    mp["0001"]="1";
    mp["0010"]="2";
    mp["0011"]="3";
    mp["0100"]="4";
    mp["0101"]="5";
    mp["0110"]="6";
    mp["0111"]="7";
    mp["1000"]="8";
    mp["1001"]="9";
    mp["1010"]="A";
    mp["1011"]="B";
    mp["1100"]="C";
    mp["1101"]="D";
    mp["1110"]="E";
    mp["1111"]="F";
    string hex="";
    int i=0;
    while(i<s.length())
    {
        string ch="";
        ch+=s[i];
        ch+=s[i+1];
        ch+=s[i+2];
        ch+=s[i+3];
        hex+=mp[ch];
        i+=4;
    }
    return hex;
}
string permute(string k,int* arr,int n)
{
    string per="";
    int i=0;
    while(i<n)
    {
        per+=k[arr[i]-1];
        i++;
    }
    return per;
}
string shift_left(string k,int shifts)
{
    string s="";
    int i=0;
    while(i<shifts)
    {
        for(int j=1;j<28;j++)
            s+=k[j];
        s+=k[0];
        k=s;
        s="";
        i++;
    }
    return k;
}
string xor_(string a,string b)
{
    string ans="";
    int i=0;
    while(i<a.size())
    {
        if (a[i]==b[i])
            ans+="0";
        else
            ans+="1";
        i++;
    }
    return ans;
}
string encrypt(string pt,vector<string> rkb,vector<string> rk)
{
    pt=hex2bin(pt);
    int initial_perm[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                             60, 52, 44, 36, 28, 20, 12, 4,
                             62, 54, 46, 38, 30, 22, 14, 6,
                             64, 56, 48, 40, 32, 24, 16, 8,
                             57, 49, 41, 33, 25, 17, 9, 1,
                             59, 51, 43, 35, 27, 19, 11, 3,
                             61, 53, 45, 37, 29, 21, 13, 5,
                             63, 55, 47, 39, 31, 23, 15, 7 };

    pt=permute(pt,initial_perm,64);
    string left=pt.substr(0,32);
    string right=pt.substr(32,32);
    int exp_d[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                      6, 7, 8, 9, 8, 9, 10, 11,
                      12, 13, 12, 13, 14, 15, 16, 17,
                      16, 17, 18, 19, 20, 21, 20, 21,
                      22, 23, 24, 25, 24, 25, 26, 27,
                      28, 29, 28, 29, 30, 31, 32, 1 };
  
    int s[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
  
                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };
  

    int per[32] = { 16, 7, 20, 21,
                    29, 12, 28, 17,
                    1, 15, 23, 26,
                    5, 18, 31, 10,
                    2, 8, 24, 14,
                    32, 27, 3, 9,
                    19, 13, 30, 6,
                    22, 11, 4, 25 };
  
    for(int i=0;i<16;i++)
    {
        string right_expanded=permute(right, exp_d, 48);
        string x = xor_(rkb[i],right_expanded);
        string op="";
        for(int i=0;i<8;i++)
        {
            int row=2*int(x[i*6]-'0')+int(x[i*6+5]-'0');
            int col=8*int(x[i*6+1]-'0')+4*int(x[i*6+2]-'0')+2*int(x[i*6+3]-'0')+int(x[i*6+4]-'0');
            int val=s[i][row][col];
            op+=char(val/8+'0');
            val=val%8;
            op+=char(val/4+'0');
            val=val%4;
            op+=char(val/2+'0');
            val=val%2;
            op+=char(val+'0');
        }
        op=permute(op,per,32);
        x=xor_(op,left);
        left=x;
        if(i!=15){
            swap(left, right);
        }
    }
    string combine=left+right;
    int final_perm[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                           39, 7, 47, 15, 55, 23, 63, 31,
                           38, 6, 46, 14, 54, 22, 62, 30,
                           37, 5, 45, 13, 53, 21, 61, 29,
                           36, 4, 44, 12, 52, 20, 60, 28,
                           35, 3, 43, 11, 51, 19, 59, 27,
                           34, 2, 42, 10, 50, 18, 58, 26,
                           33, 1, 41, 9, 49, 17, 57, 25 };
  
    string cipher=bin2hex(permute(combine,final_perm,64));
    return cipher;
}
string getDES(string pt)
{
    string key;
    key="133457799BBCDFF1";
    key=hex2bin(key);
    int keyp[56] = { 57, 49, 41, 33, 25, 17, 9,
                     1, 58, 50, 42, 34, 26, 18,
                     10, 2, 59, 51, 43, 35, 27,
                     19, 11, 3, 60, 52, 44, 36,
                     63, 55, 47, 39, 31, 23, 15,
                     7, 62, 54, 46, 38, 30, 22,
                     14, 6, 61, 53, 45, 37, 29,
                     21, 13, 5, 28, 20, 12, 4 };
  

    key=permute(key,keyp,56);
    int shift_table[16] = { 1, 1, 2, 2,
                            2, 2, 2, 2,
                            1, 2, 2, 2,
                            2, 2, 2, 1 };
  

    int key_comp[48] = { 14, 17, 11, 24, 1, 5,
                         3, 28, 15, 6, 21, 10,
                         23, 19, 12, 4, 26, 8,
                         16, 7, 27, 20, 13, 2,
                         41, 52, 31, 37, 47, 55,
                         30, 40, 51, 45, 33, 48,
                         44, 49, 39, 56, 34, 53,
                         46, 42, 50, 36, 29, 32 };
  
    string left=key.substr(0,28);
    string right=key.substr(28,28);
    vector<string> rkb;
    vector<string> rk;
    int i=0;
    while(i<16)
    {
        left=shift_left(left,shift_table[i]);
        right=shift_left(right,shift_table[i]);
        string combine=left+right;
        string RoundKey=permute(combine,key_comp,48);
        rkb.push_back(RoundKey);
        rk.push_back(bin2hex(RoundKey));
        i++;
    }
    string cipher=encrypt(pt,rkb,rk);
    return cipher;
}
struct Transaction
{
    double amount;
    string sender;
    string reciever;
    int sAdr;
    Transaction(double am,string s1,string r1,int ss)
    {
        amount=am;
        sender=s1;
        reciever=r1;
        sAdr=ss;
    };
};
class Block
{
public:
    int index;
    string myHash;
    string prevHash;
    Transaction *tr;
    int nonce;
    Block()//Default, not being used
    {
        int x=2;
    }
    Block(int ind,string prevH,Transaction* gen)
    {
        index=ind;
        prevHash=prevH;
        tr=gen;
        nonce=0;
        myHash=calculateHash(tr->reciever+tr->sender+to_string(tr->amount));
    }
    string calculateHash(string inp)
    {
        string temp=getSHA(inp);
        string ans="";
        for(int i=0;i<64;i+=16)
            ans+=getDES(temp.substr(i,16));
        return ans;
    }
    int getIndex()
    {
        return index;
    }
    bool mine(int diff)
    {
        cout<<"Mining Begins"<<endl;
        string ch(diff,'0');
        while(myHash.substr(0,diff)!=ch)
        {
            myHash=calculateHash(tr->reciever+tr->sender+to_string(tr->amount)+to_string(nonce));
            //cout<<myHash<<endl;
            nonce++;
        }
        cout<<"Block Mined with hash "<<endl<<myHash<<endl;
        return ZKP();
    }
    bool ZKP()//Zero-Knowledge Proof
    {
        int indd=-1;
        for(int i=0;i<users.size();i++)
        {
            if(tr->sender==users[i])
                indd=i;
        }
        if(indd==-1)
        {
            cout<<"User is not verified"<<endl;
            return false;
        }
        ll s=tr->sAdr,g=2,p=101;
        ll h=(((long long int)(pow(g, r)))%p);
        ll b=1;
        ll y=ys[indd];
        ll B = (((long long int)(h * (long long int)(pow(ys[indd], b))) % p));
        ll A = ( (long long int)( pow(g, s) )) % p;
        if(A==B)
        {
            cout<<"Transaction Verified"<<endl;
            return true;
        }
        cout<<"Transaction is not verified"<<endl;
        return false;
    }
};
class Blockchain: public Block
{
public:
    map<string,vector<Transaction*>> vUser;
    vector<Block*> BC;
    Blockchain()
    {
        Transaction* tt=new Transaction(0,"Gen","Gen",0);
        Block* gen=new Block(0,"0",tt);
        BC.push_back(gen);
    }
    vector<Block*> getchain()
    {
        return BC;
    }
    Block* getLatest()
    {
        return BC[BC.size()-1];
    }
    void addBlock(Transaction* data2,int indi,string prevH,string myH)
    {
        vUser[data2->sender].push_back(data2);
        vUser[data2->reciever].push_back(data2);
        Block* n2=new Block();
        n2->myHash=myH;
        n2->prevHash=prevH;
        n2->tr=data2;
        n2->index=indi;
        BC.push_back(n2);
    }
    void addBlock(Transaction* data)
    {
        vUser[data->sender].push_back(data);
        vUser[data->reciever].push_back(data);
        Block* preB=BC[BC.size()-1];
        Block* nw=new Block((preB->index)+1,preB->myHash,data);
        if(nw->mine(2))
        {
            sqlite3* DB;
            char* messaggeError;
            int exit=0;
            exit=sqlite3_open("blockchain.db", &DB);
            string str="INSERT INTO BLOCK VALUES(";
            str+=to_string(nw->index)+",";
            str+="'"+nw->tr->sender+"',";
            str+="'"+nw->tr->reciever+"',";
            str+="'"+nw->prevHash+nw->myHash+"',";
            str+="'"+to_string(nw->tr->sAdr)+"',";
            str+=to_string(nw->tr->amount);
            str+=");";
            //cout<<str<<endl;
            exit=sqlite3_exec(DB,str.c_str(),NULL,0,&messaggeError);
            if(exit!=SQLITE_OK)
            {
                cerr<<"Error Insert"<<endl;
                sqlite3_free(messaggeError);
            }
            else
                cout<<"Records created Successfully!"<<endl;       
            sqlite3_close(DB);
            BC.push_back(nw);
        }
    }
    bool isChainValid()
    {
        if(BC.size()<=1)
            return true;
        Block* st=BC[0];
        for(int i=1;i<BC.size();i++)
        {
            if(BC[i]->prevHash!=st->myHash)
                return false;
            st=BC[i];
        }
        return true;
    }
    void printChain()
    {
        for (int i=0;i<BC.size();i++)
        {
            Block* currentBlock=BC[i];
            cout<<"\n********************Block***********************"<<endl;
            printf("Index: %d\n", currentBlock->getIndex());
            printf("Amount: %f\n", currentBlock->tr->amount);
            cout<<"Sender: "<<currentBlock->tr->sender<<endl;
            cout<<"Reciever: "<<currentBlock->tr->reciever<<endl;
            cout<<"prevHash: "<<currentBlock->prevHash<<endl;
            cout<<"myHash: "<<currentBlock->myHash<<endl;
            //printf("\nIs Block Valid?: %d\n", currentBlock->ZKP());
        }
    }
    void viewUser(string user)
    {
        if(vUser[user].size()==0)
        {
            cout<<"No succesfull transactions by this user"<<endl;
            return;
        }
        for(int i=0;i<vUser[user].size();i++)
        {
            Transaction* temp=vUser[user][i];
            cout<<"********Succesfull Transaction********"<<endl;
            cout<<"Sender Name- "<<temp->sender<<endl;
            cout<<"Reciever Name- "<<temp->reciever<<endl;
            cout<<"Amount exchanged- "<<temp->amount<<endl;
        }
    }
};
Blockchain* glob;
static int callback(void* data,int argc,char** argv,char** azColName)
{
    string prevH,myH;
    int ini=0;
    string snd,rcv;
    int am,ad;
    //fprintf(stderr,"%s: ",(const char*)data);
    for(int i=0;i<argc;i++)
    {
        if(i==0)
            ini=stoi(argv[i]);
        if(i==1)
            snd=argv[i];
        if(i==2)
            rcv=argv[i];
        if(i==3)
        {
            string temp=argv[i];
            prevH=temp.substr(0,64);
            myH=temp.substr(64,64);
        }
        if(i==4)
            ad=stoi(argv[i]);
        if(i==5)
            am=stod(argv[i]);
    }
    Transaction* n3=new Transaction(am,snd,rcv,ad);
    glob->addBlock(n3,ini,prevH,myH);
    return 0;
}
int main()
{
    glob=new Blockchain();
    sqlite3* DB;
    int exit=0;
    exit=sqlite3_open("blockchain.db", &DB);
    string query="SELECT * FROM BLOCK;";
    sqlite3_exec(DB,query.c_str(),callback,NULL,NULL);
    sqlite3_close(DB);
    int n;
    cout<<"Enter the number of transactions\n";
    cin>>n;
    for(int i=0;i<n;i++)
    {
        string snd,rcv;
        cin>>snd>>rcv;
        int am,ad;
        cin>>am;
        cout<<"Choose a r between 0 and 20"<<endl;
        int tt;
        cin>>tt;
        r=tt;
        cout<<"Give s such that s=(r+b*x)mod(p-1) where x is your Unique ID and p=101 and b=1"<<endl;
        cin>>ad;
        Transaction* trn=new Transaction(am,snd,rcv,ad);
        glob->addBlock(trn);
    }
    glob->printChain();
    //ch->viewUser("vanshaj");
    return 0;
}