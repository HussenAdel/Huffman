#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cstdlib>
#include <fstream>
#include<algorithm>
#include <queue>
#include <string>
#include <bits/stdc++.h>
#include <bitset>
#include <sstream>
#include <string>
#include <dirent.h>
#include <string.h>
#include <chrono>
#include <time.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;

typedef struct node
{
    node* left;
    node* right;
    int freq;
    char c;

} Node;

typedef pair <int,int> ipair;

struct comp
{
    bool operator()(ipair l, ipair r)
    {
        return l.second > r.second;
    }
};

int executionTime;
int folderExecutionTime;
int sizeOffile=0;
int sizeOfCompfile=0;

Node* newnode(int freq,char c) //Node used in the first part of the  assignment Node initialization
{
    node*p=NULL;
    p=(node*)malloc(sizeof(node));
    p->freq=freq;
    p->c=c;
    p->left=NULL;
    p->right=NULL;
    return p;
}
int numberOfFiles(char* folderName)
{
    DIR *folder;
    struct dirent *entry;
    size_t found=0;
    int files = 0;
    int j=0;
    int i=0;
    string str="";
    folder = opendir(folderName);
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return 0;
    }
    while( (entry=readdir(folder)) )
    {

        files++;
        if (files>=3)
        {
            str="";
        j=0;
        while(entry->d_name[j]!='\0')
            {
                str.append(1,entry->d_name[j]);
                j++;

            }
            found=str.find(".txt");
             if (str.length()>=4&&found==str.length()-4)
             {
                 i++;
             }
             else
             {
                 char s[100];
                 strcpy(s,folderName);
            strcat (s,"\\");
            strcat (s,entry->d_name);
                 i+=numberOfFiles(s);
             }
        }
    }
    return i;

}
void readFilesfromFolder(char* folderName,string opString[])
{
    string str="";
    DIR *folder;
    size_t found=0;
    char s[100];
    char foldername2[100];
    int files = 0;
    int i=0,j=0;
    bool flag=false;
    folder = opendir(folderName);
    DIR *f=folder;
    strcpy(foldername2,folderName);
    while (!flag)
    {
        struct dirent *entry;
        flag=true;
        f=folder;
        files=0;
        strcpy(folderName,foldername2);
    while( (entry=readdir(f)) )
    {

        files++;
        if(files>=3)
        {

            str="";
            j=0;
            while(entry->d_name[j]!='\0')
            {
                str.append(1,entry->d_name[j]);
                j++;

            }
            found=str.find(".txt");
             if (str.length()>4&&found==str.length()-4)
             {
            strcpy(s,folderName);
            strcat (s,"\\");
            strcat (s,entry->d_name);
            opString[i]=s;
            i++;
             }
            else
            {
                flag=false;
                strcpy(s,folderName);
                strcat (s,"\\");
                strcat (s,entry->d_name);
                folder=opendir(s);
                strcpy(foldername2,s);


            }

        }
    }
    closedir(f);
}
}
void Prefix(Node* node,string pre, unordered_map<char, string> & umap)
{
    if (node == NULL)
        return;
    if(node->right== NULL && node->left== NULL)
    {
        if (pre=="")
        pre="0";
        umap[node->c]=pre;
        pre = pre.substr(0, pre.size()-1);
        return;
    }
    Prefix(node->left,pre+"0",umap);
    Prefix(node->right,pre+"1",umap);
    pre = pre.substr(0, pre.size()-1);

}
string readfile(string filename)
{
    string text;
    std::ifstream input( filename, std::ios::binary );
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    sizeOffile += input.tellg();
    for(int i=0; i < buffer.size(); i++)
    {
        text+=buffer[i];
    }
    input.close();
    if (buffer.size()==0)
    {
        cout<<"error in file";
        exit(0);
    }
    return text;
// Close the file
}
char* encode(string t)
{
    string temp;
    char* bin=(char*)malloc((t.length()/8));
    for (int i=0; i<t.size()/8; i++)
    {
        temp = t.substr(i*8, 8);
        string bit_string = temp;
        bitset<8> b(bit_string);
        unsigned char c = ( b.to_ulong() & 0xFF);
        bin[i]=c;
    }
    return bin;
}
void writecompfile(Node** node,string t,int nofchar,string filename,unordered_map<char, string> & umap)
{
    ofstream MyFile(filename, ios::out | ios::app | ios::binary);
    char* bin;
    char* prebin;
    int sn,tn=0,no=0;
    int i=0;
    int pad=0;
    string s="",pre;
    unsigned char c;
    char sas[2];
    char co[1000000];
    for (i=0; i<t.length(); i++)
    {
        c=t[i];
        s.append(umap[c]);
    }
    //padding
    if(t.length()%8!=0)
    {
        pad=8-(int)(s.length()%8);
        for (i=0; i<pad; i++)
        {
            s="0"+s;
        }
    }
    for(i=0; i<nofchar; i++)
    {
        c=node[i]->c;
        pre=umap[c];
        int x=8-(int)(pre.length()%8);;
        x=x+pre.length();
        x=x/8;
        tn=tn+x;
    }
    int n=s.length()/8;
    bin=encode(s);
    co[no]=(char)pad;
    no++;
    co[no]=(char)nofchar;
    no++;
    co[no]=(char)tn;
    no++;
    int b=(n >>16);
    int a=(n >> 8);
    co[no]=(char)b;
    no++;
    co[no]=(char)a;
    no++;
    co[no]=(char)n;
    no++;
    for(i=0; i<nofchar; i++)
    {
        c=node[i]->c;
        pre=umap[c];
        co[no]=(char)c;
        no++;
        co[no]=(char)pre.length();
        no++;
        pad=8-(int)(pre.length()%8);
        for (int j=0; j<pad; j++)
        {
            pre="0"+pre;
        }
        prebin=encode(pre);
        sn=pre.length()/8;
        for (int j=0; j<sn; j++)
        {
            co[no]=prebin[j];
            no++;
        }
    }

    for (i=0; i<n; i++)
    {
        co[no]=bin[i];
        no++;
    }
    MyFile.write(co,no);
    sizeOfCompfile+= no;//MyFile.tellp();
    //cout<<sizeOfCompfile<<endl;
    MyFile.close();
}
string decode(string s,unordered_map<string, char> & umap)
{
    string ch="";
    string result="";
    for(int i=0; i<s.length(); i++)
    {
        ch+=s[i];
        if (umap.find(ch) != umap.end())
        {
            result.append(1,umap[ch]);
            ch="";
        }
    }
    return result;
}
void decompress(int folderFlag,string pre,int i3)
{
    int i=0,bin;
    string t;
    string s="",finalstr="",str2="";
    string str;
    string temp;
    int temp2;
    int enclen;
    unordered_map<string, char> umap;
    unsigned char ca;
    ca=pre[0];
    int pad=(int)ca;
    pre=pre.substr(1, pre.size());
    ca=pre[0];
    int nofchar=(int)ca;
    pre=pre.substr(1, pre.size());
    ca=pre[0];
    int B=(int)ca;
    pre=pre.substr(1, pre.size());
    ca=pre[0];
    temp2=(int)ca;
    temp2=(temp2<<16);
    ca=pre[1];
    enclen=(int)ca;
    enclen=(enclen<<8);
    ca=pre[2];
    enclen=temp2+enclen+ca;
    pre=pre.substr(3, pre.size());
    t=pre.substr((2*nofchar)+B, pre.size());
    pre=pre.substr(0,(2*nofchar)+B);
    while(pre.size())
    {
        str2="";
        char c=pre[0];
        pre=pre.substr(1, pre.size());
        int csize=pre[0];
        pre=pre.substr(1, pre.size());
        int x=8-(int)(csize%8);;
        x=x+csize;
        int ssax=x/8;
        str=pre.substr(0,ssax);
        pre=pre.substr(ssax,pre.size());
        for (int i=0; i<str.length(); i++)
        {
            ca=str[i];
            bin=(int)ca;
            temp=(bitset<8>(bin)).to_string();
            str2=str2+temp;
        }
        int pad2=8-(int)(csize%8);
        str2=str2.substr(pad2, str2.size());
        umap[str2]=c;

    }
//    for (auto x : umap)
//      cout << x.first << "-->" << x.second << endl;
    for(i=0; i<t.length(); i++)
    {
        ca=t[i];
        temp=(bitset<8>(ca)).to_string();
        s.append(temp);
    }
    s=s.substr(pad, s.size());
    finalstr=decode(s,umap);
    string filename;
    if(folderFlag==1)
    {
        char* dirname = "decompFolder";
        string directoryName="decompFolder";
        if(i3==0)
        {
            mkdir(dirname);
            string a = "rmdir /s /q " + directoryName;
            system(a.c_str());
        }
        mkdir(dirname);
        stringstream ss;
        ss<<(i3+1);
        string v;
        ss>>v;
        filename="decompFolder\\file"+v+".txt";
    }
    else
    {
        filename="decomp.txt";
    }
    int l = finalstr.length();
    char encoded_bytes[l];
    for(int i =0; i<l; i++)
    {
        encoded_bytes[i] = (char)finalstr[i];
    }
    ofstream file(filename,ios::out|ios::trunc|ios::binary);
    file.write(encoded_bytes,l);
    sizeOffile+=file.tellp();
    file.close();
}
void compress(string filename,bool folder,string filename2)
{
    clock_t tStart = clock();
    node* tree[1000];
    node* root;
    int n=0,nofchar;
    int j;
    bool flag=false;
    int a,fa,b,fb;
    //unsigned char ca;
    string t=readfile(filename);
    unordered_map<char, int> freqMap;
    for (int i=0; i<t.length(); i++)
    {
        if (freqMap.find(t[i]) == freqMap.end())
        {
            tree[n]=newnode(1,t[i]);
            n++;
        }
        freqMap[t[i]]++;
    }
    for(int i=0; i<n; i++)
    {
        tree[i]->freq=freqMap[tree[i]->c];
    }
    nofchar=n;
    priority_queue <ipair, vector<ipair>,comp > pq;
    for (int i=0; i<n; i++)
    {
        pq.push(make_pair(i,tree[i]->freq));
    }
    while (!pq.empty())
    {
        a=pq.top().first;
        fa=pq.top().second;
        pq.pop();
        if (pq.empty())
        {
            root=tree[a];
        }
        else
        {
            b=pq.top().first;
            fb=pq.top().second;
            pq.pop();
            node* p;
            p=newnode(fa+fb,NULL);

            tree[n]=p;
            tree[n]->left=tree[a];
            tree[n]->right=tree[b];

            pq.push(make_pair(n,tree[n]->freq));
            n++;
        }
    }
    unordered_map<char, string> umap;
    string pre="";
    Prefix(root,pre,umap);

    if (folder)
        writecompfile(tree,t,nofchar,filename2,umap);
    else
    {
        filename="comp"+filename;
        ofstream MyFile(filename);
        MyFile<<(unsigned char)1;
        MyFile<<char(false);
        MyFile.close();
        writecompfile(tree,t,nofchar,filename,umap);
    }
    clock_t tEnd = clock();
    executionTime=tEnd-tStart;
    if (folder)
    {
        folderExecutionTime+=executionTime;
    }
    cout<<"\t\t\t\t*********************** Huffman Code ***********************"<<endl;
    cout <<"\t\t\t\t"<<"Byte"<<"        "<<"Code"<<"                  "<<"New Code"<<endl<<endl;
for (int i=0;i<nofchar;i++)
{

    unsigned char c=tree[i]->c;

    string Code=(bitset<8>(c)).to_string();
    string NewCode=umap[c];
    if (c=='\n'||c=='\r')
    cout <<"\t\t\t\t"<<"[Enter]"<<"     "<<Code<<"              "<<NewCode<<endl;
    else
    cout <<"\t\t\t\t"<<c<<"           "<<Code<<"              "<<NewCode<<endl;

}
cout<<"\t\t\t\t************************************************************"<<endl;
}
void compressfolder(char* folderName)
{
    int n=numberOfFiles(folderName);
    string opString[10];
    readFilesfromFolder(folderName,opString);
    string filename="comp.txt";
    ofstream MyFile(filename);
    MyFile<<char(n);
    MyFile<<char(true);
    MyFile.close();
    for(int i=0; i<n; i++)
    {
        compress(opString[i],true,filename);
    }

}
void decompress2(string filename)
{
    clock_t tStart = clock();
    string myText,tot="";
    int n,i,B,nofchar,enclen,filelen,j=0,temp2;
    unsigned char c;
    ifstream input( filename, std::ios::binary );
    // copies all data into buffer
    vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    int ss=buffer.size();
    input.close();
    for (i=0; i<ss; i++)
    {
        c=buffer[i];
        tot+=c;
    }
    if (!buffer.size())
    {
        cout<<"error in file";
        exit(0);
    }
    n=(int)tot[0];
    int folderFlag=(int)tot[1];
    tot=tot.substr(2, tot.size());
    for (i=0; i<n; i++)
    {
        c=tot[1];
        nofchar=int(c);
        c=tot[2];
        B=int(c);
        c=tot[3];
        temp2=(int)c;
        temp2=(temp2<<16);
        c=tot[4];
        enclen=(int)c;
        enclen=(enclen<<8);
        c=tot[5];
        enclen=temp2+enclen+c;
        filelen=2*(nofchar)+B+enclen+6;
        myText=tot.substr(0,filelen);
        tot=tot.substr(filelen, tot.size());
        decompress(folderFlag,myText,i);
    }
    clock_t tEnd = clock();
    executionTime=tEnd-tStart;
}
int main()
{
    int Option;
    string filename;
    char* foldername=(char*)malloc(100);
    do
    {
        cout <<"Please Select An Option :\n";
        cout <<"--------------------------\n";
        cout <<" 1) Compress File \n";
        cout <<" 2) Decompress File \n";
        cout <<" 3) Compress Folder \n";
        cout <<" 4) Quit \n";
        cout <<"Choose an Option: ";
        cin >>Option;
        cout <<"\n";
        switch(Option)
        {
        case 1 :
            executionTime=0;
            folderExecutionTime=0;
            sizeOffile=0;
            sizeOfCompfile=2;
            system("@cls||clear");
            cout << "Enter The File Name :";
            cin >> filename;
            compress(filename,false,"");
            cout<<"\t\t\t\t******************** details of program ********************"<<endl;
            if(sizeOffile<1024)
                cout<<"\t\t\t\tsize of file= "<<sizeOffile <<" bytes ("<<((float)sizeOffile/1024)<<endl;
            else if(sizeOffile>=1024&&sizeOffile<1048576)
            {
                cout<<"\t\t\t\tsize of file= "<<((float)sizeOffile/1024) <<" Kilobytes"<<endl;
            }
            else
            {
                cout<<"\t\t\t\tsize of file= "<<((float)sizeOffile/1048576) <<" Megabytes"<<endl;
            }
            if(sizeOfCompfile<1024)
                cout<<"\t\t\t\tsize of compressed file= "<<sizeOfCompfile <<" bytes ("<<((float)sizeOfCompfile/1024)<<endl;
            else if(sizeOfCompfile>=1024&&sizeOfCompfile<1048576)
            {
                cout<<"\t\t\t\tsize of compressed file= "<<((float)sizeOfCompfile/1024) <<" Kilobytes"<<endl;
            }
            else
            {
                cout<<"\t\t\t\tsize of compressed file= "<<((float)sizeOfCompfile/1048576) <<" Megabytes"<<endl;
            }
                cout << "\t\t\t\tTime taken by function: "<< (double)(executionTime)/CLOCKS_PER_SEC << "seconds" << endl;
                cout<<"\t\t\t\tcompression ratio is "<<((float)(sizeOffile-sizeOfCompfile)/sizeOffile)*100<<"%"<<endl;
                cout<<"\t\t\t\t************************************************************"<<endl;
                cout<<"\t\t\t\t************************************************************"<<endl;
            break;
        case 2 :
            executionTime=0;
            folderExecutionTime=0;
            sizeOffile=0;
            sizeOfCompfile=0;
            system("@cls||clear");
            cout << "Enter The File Name :";
            cin >> filename;
            decompress2(filename);
            cout<<"******************** details of program ********************"<<endl;
            if(sizeOffile<1024)
                cout<<"size of file= "<<sizeOffile <<" bytes ("<<((float)sizeOffile/1024)<<endl;
            else if(sizeOffile>=1024&&sizeOffile<1048576)
            {
                cout<<"size of file= "<<((float)sizeOffile/1024) <<" Kilobytes"<<endl;
            }
            else
            {
                cout<<"size of file= "<<((float)sizeOffile/1048576) <<" Megabytes"<<endl;
            }
            cout << "Time taken by function: "<< (double)(executionTime)/CLOCKS_PER_SEC << "seconds" << endl;
            cout<<"************************************************************"<<endl;
            break;
        case 3:
            executionTime=0;
            folderExecutionTime=0;
            sizeOffile=0;
            sizeOfCompfile=2;
            system("@cls||clear");
            cout << "Enter The Folder Name :";
            cin >> foldername;
            compressfolder(foldername);
            cout<<"\t\t\t\t******************** details of program ********************"<<endl;
            if(sizeOffile<1024)
            {
                cout<<"\t\t\t\tsize of file= "<<sizeOffile <<" bytes ("<<((float)sizeOffile/1024)<<endl;
            }
            else if(sizeOffile>=1024&&sizeOffile<1048576)
            {
                cout<<"\t\t\t\tsize of file= "<<((float)sizeOffile/1024) <<" Kilobytes"<<endl;
            }
            else
            {
                cout<<"\t\t\t\tsize of file= "<<((float)sizeOffile/1048576) <<" Megabytes"<<endl;
            }
            if(sizeOfCompfile<1024)
                cout<<"\t\t\t\tsize of compressed file= "<<sizeOfCompfile <<" bytes ("<<((float)sizeOfCompfile/1024)<<endl;
            else if(sizeOfCompfile>=1024&&sizeOfCompfile<1048576)
            {
                cout<<"\t\t\t\tsize of compressed file= "<<((float)sizeOfCompfile/1024) <<" Kilobytes"<<endl;
            }
            else
            {
                cout<<"\t\t\t\tsize of compressed file= "<<((float)sizeOfCompfile/1048576) <<" Megabytes"<<endl;
            }
            cout << "\t\t\t\tTime taken by function: "<< (double)(folderExecutionTime)/CLOCKS_PER_SEC << "seconds" << endl;
            cout<<"\t\t\t\tcompression ratio is "<<((float)(sizeOffile-sizeOfCompfile)/sizeOffile)*100<<"%"<<endl;
            cout<<"\t\t\t\t************************************************************"<<endl;
            cout<<"\t\t\t\t************************************************************"<<endl;
            break;
        case 4:
            system("@cls||clear");
            exit(0);
            break;
        default:
            system("@cls||clear");
            break;
        }
    }
    while(Option!=4);
    return 0;
}
