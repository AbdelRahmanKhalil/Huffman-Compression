// C++ program to use priority_queue to implement min heap
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include<time.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string.h>
#define ASCII 256

using namespace std;
typedef struct node
{
    int freq;
    unsigned char character;
    node* left;
    node* right;
} node;
typedef struct encnode
{
    unsigned char character;
    string  code;
} encnode;
node* create_node(int freq,unsigned char character)
{
    node* newnode=(node *)malloc(sizeof(node));
    newnode->freq=freq;
    newnode->character=character;
    newnode->left= nullptr;
    newnode->right= nullptr;
    return newnode;
}
struct CompareFreq
{
    bool operator()(node* const& p1, node* const& p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return p1->freq > p2->freq;
    }
};
void create_map(vector<node*>* nodes)
{
    ;
    for( auto it=0; it<ASCII; it++)
    {
        (*nodes).insert((*nodes).begin() + it, create_node(0,(unsigned char)it));

    }

}
vector<unsigned char> read_file(string filename)
{
    std::ifstream input( filename, std::ios::binary );
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});


    return buffer;

}
void find_freq(vector<unsigned char> buffer,vector<node*>* nodes)
{
    int dummy;

    for(auto it=0; it<buffer.size(); it++)
    {

        dummy=(int)buffer[it];
        (*nodes)[dummy]->freq++;

    }
}
node* make_tree(priority_queue <node*, vector<node*>, CompareFreq > pq )
{

    node* first;
    node* second;
    while (pq.size() > 1)
    {


        first=pq.top();
        pq.pop();
        second=pq.top();
        pq.pop();
        node* sum= create_node(first->freq+second->freq,NULL);
        if(first->freq>second->freq)
        {
            sum->right=first;
            sum->left=second;
        }
        else
        {
            sum->right=second;
            sum->left=first;
        }
        pq.push(sum);

    }


    return pq.top();
}




void print2DUtil(node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    cout<<endl;
    for (int i = 10; i < space; i++)
        cout<<" ";

    if(root->character!=NULL&&root->character!='\n')
        cout<<"hi:"<<root->freq<<"  char is="<<root->character<<" * "<<"\n";
    else if(root->character=='\n')
        cout<<"hi:blank:"<<root->freq<<"\n";
    else
        cout<<"hi:"<<root->freq<<"\n";
    // Process left child
    print2DUtil(root->left, space);
}
void encode(node *root,vector<encnode> *code_table,string code)
{
    if(root->left==nullptr&&root->right==nullptr)//Leaf
    {
        encnode new_encode;
        new_encode.character=root->character;
        new_encode.code=code;
        (*code_table).push_back(new_encode);

    }
    if(root->right!=nullptr)
    {
        encode(root->right,code_table,code+"1");
    }

    if(root->left!=nullptr)
    {
        encode(root->left,code_table,code+"0");
    }
}

void read_header(vector<encnode> *code_table, string* buffer)
{
    (*code_table).clear();

    // Read from the text file
    ifstream MyReadFile("write.bin", std::ios::binary);
    string myText1(std::istreambuf_iterator<char>(MyReadFile), {});

    string stop="stop";

    std::string header = myText1.substr(0, myText1.find(stop));

    header+=stop+"\r\n";
    (*buffer)=myText1.substr(myText1.find(stop)+6,myText1.size() );
    stop = "\n\r";
    string myText="";
    int pos=0,i;
        for(i=pos;header[i]!='\r';i++)
        {
            myText=myText+header[i];


        }
        pos=i+2;

    while (myText!="stop"&& myText!="")
    {



        encnode new_encnode;
        string code;

        if(myText[1]!=' ')      //format of \n and \r is *n and *r respectively
        {
            if(myText[1]=='r')
                new_encnode.character='\r';
            else
               new_encnode.character='\n';
             code =myText.substr(3,myText.size());
            new_encnode.code=code;
        }
        else
        {
            unsigned char c=myText[0];
            new_encnode.character=c;

             code =myText.substr(2,myText.size());
            new_encnode.code=code;
        }

                    (*code_table).push_back(new_encnode);
                    myText.clear();
                    for(i=pos;header[i]!='\r';i++)
                    {
                        myText=myText+header[i];

                    }
                    pos=i+2;



    }

// Close the file
    MyReadFile.close();

}
void print2D(node *root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

string search_code_table(vector <encnode> code_table, unsigned char key)
{
    for(auto it=code_table.begin();it!=code_table.end();it++)
    {
        if(key==(*it).character)
            return (*it).code;
    }

}
void write (vector <encnode> code_table, vector<unsigned char> buffer)
{   cout<< "COMPRESSING IN PROGRESS..."<<endl;
    // Create and open a text file
    ofstream MyFile("write.bin");

    for(auto it=code_table.begin();it!=code_table.end();it++)
    {
        // Write to the file
        if ((*it).character=='\n')
            MyFile<< "*n"<< " "<< (*it).code<<endl;
        else if ((*it).character=='\r')
            MyFile<< "*r"<< " "<< (*it).code<<endl;
        else
        MyFile <<(*it).character <<" "<<(*it).code<<endl;
    }
    MyFile << "stop"<<endl;
    unsigned char key;
    string codestr;
    string code_stream="";
    string bytestr;
    int char_value;
    unsigned char compressed;
    for(auto it=buffer.begin();it!=buffer.end();it++)
    {
        key=*it;
        codestr=search_code_table(code_table,key);
        code_stream=code_stream+codestr;
        if(code_stream.size()>8)
        {
             bytestr=code_stream.substr(0,8);
             char_value=stoi(bytestr,0,2);
             compressed=(unsigned char)char_value;
             code_stream.erase(0,8);
             MyFile<<compressed;

        }

    }
    int size_remain=code_stream.size();
    if(size_remain!=0)
    {
        int bits_missing=8-size_remain;
        for(auto it=0;it<bits_missing;it++)
            code_stream=code_stream+"0";
        char_value=stoi(code_stream,0,2);
        compressed=(unsigned char)char_value;
        code_stream.erase(0,8);
             MyFile<<compressed;
    }
    // Close the file
    MyFile.close();
}
node* remake_tree(vector <encnode> code_table)
{
    node* root=create_node(0,NULL);
    node* main = root;
    char c=NULL;
    string code;


    for (auto it=code_table.begin();it!=code_table.end();it++)
    {
        code=(*it).code;
        root=main;
        c=NULL;
        for(int i=0; i<code.length(); i++)
        {


            if(i==code.length()-1)
            {
                c=(*it).character;
            }
            if(code[i]=='1')
            {

                if(root->right==nullptr)
                    root->right=create_node(0,c);
                root=root->right;
            }

            if(code[i]=='0')
            {
                //cout<<"I AM IN "<< code[i]<<endl;
                if(root->left==nullptr)
                    root->left=create_node(0,c);
                root=root->left;
            }


        }


    }
    return main;

}
void traverse(node* root, string code_stream,string* buffer)
{

    node* checkpoint=root;
    int i=0;
    while(i<=code_stream.size())
    {
        if(checkpoint->left==nullptr&&checkpoint->right==nullptr)
            {
                if(checkpoint->character=='\n'&&(*buffer).back()=='\r')
                    (*buffer).pop_back();
                (*buffer).push_back(checkpoint->character);
                checkpoint=root;

            }
        else if(code_stream[i]=='1')
        {
                checkpoint=checkpoint->right;
                i++;
        }

        else
        {

                checkpoint=checkpoint->left;
                i++;
        }

    }
}
void decompress(vector<encnode> *code_tableptr)
{
    string gibberish;
    string buffer;
    ofstream MyFile("decompressed.txt");
      cout<< "***********reading header*********"<<endl;
      read_header(code_tableptr,&gibberish);
      cout<< "***********header finished*********"<<endl;
    string decode="";
    string code;
    string code_stream="";
    string code_stream2="";
    string code_stream3="";
    string code_stream4="";

    node* tree =remake_tree(*code_tableptr);


    std::size_t i = 0;

    for (i = 0; i < gibberish.size()/4; ++i)
  {
      code_stream=code_stream+(bitset<8>(gibberish[i])).to_string();
  }
  cout<<"Decompressed 25%..."<<endl;
  for ( i; i < gibberish.size()/2; ++i)
  {
      code_stream2=code_stream2+(bitset<8>(gibberish[i])).to_string();
  }
  cout<<"Decompressed 50%..."<<endl;
  for (i; i < 3*gibberish.size()/4; ++i)
  {
      code_stream3=code_stream3+(bitset<8>(gibberish[i])).to_string();
  }
  cout<<"Decompressed 75%..."<<endl;
  for (i; i < gibberish.size(); ++i)
  {
      code_stream4=code_stream4+(bitset<8>(gibberish[i])).to_string();
  }
  cout<<"Decompressed 100%..."<<endl;
  code_stream=code_stream+code_stream2+code_stream3+code_stream4;


    gibberish.clear();
    traverse(tree,code_stream,&gibberish);

    FILE * objOutput;
    objOutput = fopen("decompressed.txt", "wt");
    for (int i = 0; i < gibberish.size(); i++)
    {
        fwrite(&gibberish[i],1, sizeof(gibberish[i]),objOutput );

    }
    fclose(objOutput);

}
// Wrapper over print2DUtil()




// Driver code
int main ()
{
    time_t start, end;
     time(&start);
     ios_base::sync_with_stdio(false);
    //int c = (int)'\n';
    std::vector<node*> nodes ;
    node* root= create_node(0,NULL);
    std::vector<unsigned char> buffer;
    priority_queue <node*, vector<node*>, CompareFreq > pq;
    vector <encnode> code_table;
    string code;
    create_map(&nodes);
    cout << "map created"<< endl;
    buffer=read_file("try.txt");
    cout << "reading file"<< endl;

    cout << "calculating frequencies"<< endl;
    find_freq(buffer,&nodes);

    for( auto it=0; it<ASCII; it++) //= for newline
    {
        if(nodes[it]->freq!=0)
            pq.push(nodes[it]);
    }

    root=make_tree(pq);
    cout<<"-----------------------------"<<endl;
    encode(root,&code_table,code);

      write(code_table,buffer);
     decompress(&code_table);

     time(&end);
     double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(5);
    cout << " sec " << endl;
    ifstream MyReadFile("write.bin", std::ios::binary);
    string new_file(std::istreambuf_iterator<char>(MyReadFile), {});
    buffer=read_file("try.txt");
    float ratio = (float)new_file.size()/(float)buffer.size();
    cout<<"Compression ratio= "<<ratio*100<<"%"<<endl;
    return 0;
}
