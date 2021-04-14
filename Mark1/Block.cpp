#include<bits/stdc++.h>
#include <sqlite3.h>
#include "sha256.h"
#include "DES.h"
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
        cout<<"Give s such that s = ( r + bx )mod( p-1 ) where x= YOUR UNIQUE ID and p=101 and b=1"<<endl;
        cin>>ad;
        Transaction* trn=new Transaction(am,snd,rcv,ad);
        glob->addBlock(trn);
    }
    glob->printChain();
    //ch->viewUser("vanshaj");
    return 0;
}