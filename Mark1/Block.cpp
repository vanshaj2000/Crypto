#include<bits/stdc++.h>
#include "sha256.h"
#include "DES.h"
using namespace std;

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
    void mine(int diff)
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
    }
    int getIndex()
    {
        return index;
    }
    string getHash()
    {
        return myHash;
    }
    string prevHash2()
    {
        return prevHash;
    }
    bool ZKP()//Zero-Knowledge Proof
    {
        int x=tr->sAdr,g=2,p=13;
        int y=pow(g,x);
        y=y%p;
        for(int i=0;i<p-1;i++)
        {
            for(int j=0;j<2;j++)
            {
                int h=pow(2,i);
                h=h%p;
                int s=(i+(j*x))%(p-1);
                int first=pow(g,s);
                first=first%p;
                int sec=h*(pow(y,j));
                sec=sec%p;
                if(first==sec)
                {
                    cout<<"\nTransaction verified";
                    return true;
                }
            }
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
    void addBlock(Transaction* data)
    {
        vUser[data->sender].push_back(data);
        vUser[data->reciever].push_back(data);
        Block* preB=BC[BC.size()-1];
        Block* nw=new Block((preB->index)+1,preB->myHash,data);
        nw->mine(2);
        BC.push_back(nw);
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
            printf("\nBlock ===================================");
            printf("\nIndex: %d", currentBlock->getIndex());
            printf("\nAmount: %f", currentBlock->tr->amount);
            cout<<"\nSenderKey: "<<currentBlock->tr->sender;
            cout<<"\nRecieverKey: "<<currentBlock->tr->reciever;
            cout<<"\nprevHash: "<<currentBlock->prevHash;
            cout<<"\nmyHash: "<<currentBlock->myHash;
            printf("\nIs Block Valid?: %d\n", currentBlock->ZKP());
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
int main()
{
    Blockchain* ch=new Blockchain();
    int n;
    cout<<"Enter the number of transactions\n";
    cin>>n;
    for(int i=0;i<n;i++)
    {
        string snd,rcv;
        cin>>snd>>rcv;
        int am,ad;
        cin>>am>>ad;
        Transaction* trn=new Transaction(am,snd,rcv,ad);
        ch->addBlock(trn);
    }
    ch->printChain();
    ch->viewUser("vanshaj");
    return 0;
}

/*def mine(index, traID, blockchain):
    #start time to mine blocks
    start = time.time()
    #get the traID for the previous block
    traID = blockchain[index - 1][1]
    #calculate y
    y = (GENERATOR ** traID) % PRIME
    #bruteforce values of r and b to solve for the ZKP
    for i in range(0, PRIME - 1):
        for j in range(0, 2):
            #calculate h
            h = (GENERATOR ** i) % PRIME
            #calculate s
            s = (i + j * traID) % (PRIME - 1)
            #calculate first and second
            first = (GENERATOR ** s) % PRIME
            second = (h * (y ** j)) % PRIME
            #check if both are equal
            if first == second:
                duration = time.time() - start
                #return the [r,b] tuple and the time it took to mine the block
                return [i,j], duration
*/