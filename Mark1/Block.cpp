#include<bits/stdc++.h>
#include "sha256.h"
#include "DES.h"
using namespace std;

struct Transaction
{
    double amount;
    string sender;
    string reciever;
    Transaction(double am,string s1,string r1)
    {
        amount=am;
        sender=s1;
        reciever=r1;
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
        string ch(diff,'0');
        while(myHash.substr(0,diff)!=ch)
        {
            myHash=calculateHash(tr->reciever+tr->sender+to_string(tr->amount)+to_string(nonce));
            nonce++;
        }
        cout<<"Block Mined with hash "<<myHash<<endl;
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
    bool isHashValid()
    {
        return true;
    }//Proof of Work
};
class Blockchain
{
public:
    vector<Block*> BC;
    Blockchain()
    {
        Transaction* tt=new Transaction(0,"Gen","Gen");
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
        Block* preB=BC[BC.size()-1];
        Block* nw=new Block((preB->index)+1,preB->myHash,data);
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
            printf("\n\nBlock ===================================");
            printf("\nIndex: %d", currentBlock->getIndex());
            printf("\nAmount: %f", currentBlock->tr->amount);
            cout<<"\nSenderKey: "<<currentBlock->tr->sender;
            cout<<"\nRecieverKey: "<<currentBlock->tr->reciever;
            cout<<"\nprevHash: "<<currentBlock->prevHash;
            cout<<"\nmyHash: "<<currentBlock->myHash;
            printf("\nIs Block Valid?: %d", currentBlock->isHashValid());
        }
    }
};
int main()
{
    Blockchain* ch=new Blockchain();
    string snd,rcv;
    cin>>snd>>rcv;
    int am;
    cin>>am;
    Transaction* trn=new Transaction(am,snd,rcv);
    ch->addBlock(trn);
    string snd1,rcv1;
    cin>>snd1>>rcv1;
    int am1;
    cin>>am1;
    Transaction* trn1=new Transaction(am1,snd1,rcv1);
    ch->addBlock(trn1);
    ch->printChain();
    return 0;
}