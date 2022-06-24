# Huffman-Compression
Huffman coding is a lossless data compression algorithm. The idea is to assign variable-length codes to input characters, lengths of the assigned codes are based on the frequencies of corresponding characters. The most frequent character gets the smallest code and the least frequent character gets the largest code.
## Functions Description
### 1. typedef struct node:
### 2. void create_map(vector<node*>* nodes):
This function takes a pointer to a vector containing node pointers. It 
allocates a node to each ASCII character and inserts it into the vector. 
Time complexity is 255.
### 3. vector<unsigned char> read_file(string filename):
Reads the file to be Compressed. Returns a buffer of unsigned characters 
containing the file.
### 4. void find_freq(vector<unsigned char>
buffer,vector<node*>* nodes):
This function iterates on each character in the buffer containing the text 
file and increments the frequency of that character in the vector 
containing nodes for each ASCII character.
Time complexity N (N representing the number of characters in text file)
### 5. void encode(node *root,vector<encnode>
*code_table,string code):
This function traverses the Huffman tree and calculates the new binary code
for each character. Each time the we travers into the right child we 
concatenate 1 to the string , if we traverse right we concatenate 0.
Time complexity constant.
### 6. node* make_tree(priority_queue <node*, vector<node*>, 
CompareFreq > pq ):
This is the function responsible for creating the Huffman tree. This 
function dequeues two elements from the min heap and creates a new node 
that holds the sum of frequencies of the two elements dequeued then 
adjusting the left and right child accordingly and pushes it back into the 
min heap and loops untile the queue has only 1 node. After the queue has 
only 1 node (which is the root of the Huffman Tree) it is returned.
### 7. void read_header(vector<encnode> *code_table, string*
buffer):
This function is responsible for reading the decompressed file. This 
happens in two stages. The first stage is reading the header of the 
compressed file which contains the character and its new binary code and 
inserts the into a node then inserting them into the Vector Code table. The 
file is read until the string “stop” which declares the end of the 
header.The rest of the string is store in the string variable Buffer.
### 8. void write (vector <encnode> code_table, vector<unsigned
char> buffer):
This function is responsible for making the compressed file. First we write 
the header into the file by looping on the code table nodes and writing it 
in the file which takes constant time. Then writing the bytes to the file 
by looping on the characters of vector Buffer(text file) then finding the 
new binary code concatenating it in a string , each time the string has 8 
characters (indicating 8 bits) the binary string is converted to a 
character and written to the compressed file.
Time complexity: N;
node* remake_tree(vector <encnode> code_table)
This function uses the header of the compressed file (code table) to remake 
the tree that will be used to traverse the body of the compressed file in 
the process of decompression. This function loops on the code table and for 
each character in the code table it takes the code string and for each 1 a 
right node is created and for each 0 a left node is created . node* root 
variable is initialized with the main root of the tree each time after 
finishing the binary code of a character. Complexity of this function will 
be code table length which is a max at 255 multiplied by the size of the 
binary code string (n) then complexity equals O(n).
void traverse(node* root, string code_stream,string* buffer)
This function traverses the tree according to the code stream given (this 
code stream is the equivalent binary code to the whole compressed file) to 
create and return (by reference) a string equivalent to the whole 
decompressed text file. A node* checkpoint is used as an iterator on the 
tree node pointers. For each character in the binary string whether it is a 
‘1’ or ‘0’ tree is traversed such that checkpoint takes the value of the 
right node* or left node* respectively and if the node reached is a leaf 
then checkpoint is reset to the main root of the tree to start again 
decoding the next sequence of binary characters. If the original file size 
is n then this function has a time complexity O(nlogn)
void decompress(vector<encnode> *code_tableptr)
This is the main function to decompress a compressed file. It uses function 
read header to read the whole compressed file then remakes the tree and 
uses the output string of binary sequences to traverse the tree then writes 
the output string , which is now an equivalent string to the whole original 
file, to the decompressed.txt file