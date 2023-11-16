#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <queue>
#include <unordered_map>


using namespace std;

class Node //узел дерева
{ 
public:
    string key;
    unsigned long long size;
    Node *R;
    Node *L;

    bool operator()(const Node &x, const Node &y) //объединение узлов
{
        return x.size >= y.size;
    }

    Node(const string &value = "", unsigned long long cast = 0, Node *left = NULL, Node *right = NULL) //конструктор
{
        key = value;
        size = cast;
        L = left;
        R = right;
    }

    Node *join(Node node) 
{
        return new Node(node.key + key, node.size + size, new Node(node), this);
    }
};

Node *build_tree(priority_queue<Node, vector<Node>, Node> leafs) //построение дерева
{
    while (leafs.size() > 1) 
    {
        Node *n = new Node(leafs.top());
        leafs.pop();
        leafs.push(*n->join(*new Node(leafs.top())));
        leafs.pop();
    }
    return new Node(leafs.top());
}

void create_huffman_codes(Node *top, string code, unordered_map<string, string> &huffmans_codes) //делаем коды
{
    if (top == nullptr)
        return;
    if (!top->L && !top->R) 
    {
        huffmans_codes[top->key] = code;
    }
    create_huffman_codes(top->L, code + "0", huffmans_codes);
    create_huffman_codes(top->R, code + "1", huffmans_codes);
}
void code_text(const char *input_name = "input.txt", const char *output_name = "codetext.txt") 
{
    FILE *input_file = fopen("input.txt", "r");
    unsigned long long *alfabet = new unsigned long long[256];
    for (int i = 0; i < 256; i++) {
        alfabet[i] = 0;
    }
    unsigned char number = 0;
    while (!feof(input_file)) 
    {
        number = fgetc(input_file);
        if (!feof(input_file)) 
        {
            alfabet[number]++;
        }
    }
    fclose(input_file);

    priority_queue<Node, vector<Node>, Node> leafs; //Создаём листья
    for (int i = 0; i < 256; i++) 
    {
        if (alfabet[i] != 0) 
        {
            string s(1, (char) i);
            Node new_leaf(s, alfabet[i]);
            leafs.push(new_leaf);
        }
    }
    Node *tree = build_tree(leafs); //создание дерева

    unordered_map<string, string> huffmans_codes;
    create_huffman_codes(tree, "", huffmans_codes); //генерация

    // cout << "Huffman Codes are :\n" << '\n'; //вывод кодов
    // for (auto pair: huffmans_codes) {
    //     cout << pair.first << " " << pair.second << '\n';
    // }
    FILE *output_file = fopen(output_name, "w +");
    input_file = fopen(input_name, "r");


    number = 0;
    unsigned char k = 0;
    unsigned int len = 0;
    unsigned int bit_len = 0;
    unsigned char letter = 0;
    char col_letters = leafs.size();
    fputc(col_letters, output_file);

    for (int i = 0; i < 256; i++) //букв + колличество
    {
        if (alfabet[i] != 0) 
        {
            fputc((char) i, output_file);
            fwrite(reinterpret_cast<const char *>(&alfabet[i]), sizeof(unsigned long long), 1, output_file);
        }
    }

    while (!feof(input_file)) // Сжатие в файле
    { 
        number = fgetc(input_file);
        if (!feof(input_file)) 
        {
            string s(1, number);
            if (bit_len + huffmans_codes[s].length() <= 8) 
            {
                for (int i = 0; i < huffmans_codes[s].length(); i++) 
                {
                    letter = letter << 1 | (huffmans_codes[s][i] - '0');
                }
                bit_len += huffmans_codes[s].length();
            } else 
            {
                for (int i = 0; i < 8 - bit_len; i++) 
                {
                    letter = letter << 1 | (huffmans_codes[s][i] - '0');
                }
                if (huffmans_codes[s].length() - 8 + bit_len >= 8) 
                {
                    int i = 8 - bit_len;
                    while (i + 7 < huffmans_codes[s].length()) 
                    {
                        k = 0;
                        for (int j = 0; j < 8; j++) {
                            k = k << 1 | (huffmans_codes[s][i + j] - '0');
                        }
                        i += 8;
                        fputc(letter, output_file);
                        letter = k;
                    }
                    k = 0;
                    len = 0;

                    for (int j = i; j < huffmans_codes[s].length(); j++) 
                    {
                        k = k << 1 | (huffmans_codes[s][j] - '0');
                        len++;
                    }
                } else 
                {
                    len = 0;
                    for (int i = 8 - bit_len; i < huffmans_codes[s].length(); i++) 
                    {
                        k = k << 1 | (huffmans_codes[s][i] - '0');
                        len++;
                    }
                }
                bit_len = 8;
            }
            if (bit_len == 8) 
            {

                fputc(letter, output_file);

                letter = k;
                bit_len = len;
                k = 0;
                len = 0;
            }
        } else if (bit_len < 8) 
        {
            letter = letter << 8 - bit_len;
            fputc(letter, output_file);
        }
    }
    cout << "Your file succesfully coded!";
    fclose(input_file);
    fclose(output_file);
  }
void decode_text(const char* input_name="codetext.txt", const char* output_name="output.txt") //функция декодирования
{ 
    unsigned long long * alfabet = new unsigned long long [256];
    for(int i=0; i<256; i++)
    {
        alfabet[i] = 0;
    }
    FILE* input_file = fopen(input_name, "rb"); // Open input file
    if (input_file == nullptr) 
    {
        throw invalid_argument("File not found.");
    }
    unsigned char col = 0;
    unsigned int col_letters = 0;
    col = fgetc(input_file);
    if(!feof(input_file))
    {
        col_letters =(int) col;
    }

    unsigned char character = 0;
    for(int i = 0; i< col_letters; i++)  //считываем букву и колличество
    { 
        character = fgetc(input_file);
        if(!feof(input_file)){
            unsigned long long col_of;
            fread(reinterpret_cast<char*>(&alfabet[character]), sizeof(unsigned long long), 1, input_file);
            // cout<< character<< ':'<< alfabet[character]<<endl;
        }
        else{
            throw invalid_argument("Can't decompress file.");
        }
    }

    priority_queue<Node, vector<Node>, Node> tree;
    for(int i=0; i<256; i++){
        if(alfabet[i] != 0){
            string s(1, (char)i);

            Node new_leaf(s, alfabet[i]);
            tree.push(new_leaf);
        }
    }
    character = 0;
    Node *n = build_tree(tree);

    FILE* output_file = fopen(output_name, "wb+");
    Node *nodes = n;
    unsigned char letter = 0;
    while (!feof(input_file)) // Decompressing the file
    { 
        character = fgetc(input_file);
        if(!feof(input_file)){
            for (int i = 7; i > -1; i--)
            {
                if(((character >> i) & 1) == 1)
                {
                    if(nodes->R == NULL)
                    {
                        letter = nodes->key[0];
                        if(alfabet[letter]>0){
                            alfabet[letter]--;
                            fputc(letter, output_file);
                            nodes = n->R;
                        }
                    }
                    else
                    {
                        nodes = nodes->R;
                    }
                }

                else if(((character >> i) & 1) == 0)
                {
                    if(nodes->L == NULL)
                    {
                        letter = nodes->key[0];
                        if(alfabet[letter]>0)
                        {
                            fputc(letter, output_file);
                            nodes = n->L;
                            alfabet[letter]--;
                        }
                    }
                    else
                    {
                        nodes = nodes->L;
                    }
                }
            }
        }
    }
    cout << "Your file succesfully decoded!";
    fclose(input_file);
    fclose(output_file);
}

int main() 
{
    int choice;
    cout << "Input 1(CODE file) or 2(DECODE file)" << endl;
    cin >> choice;

    if (choice == 1)
    {
        code_text();
    } 
    else if (choice == 2)
    {
        decode_text();
    }
	else
	{
		cout << "Your input don't correct";
	}
    return 0;

}
