#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

long long total_length;
long long total_bits;

string original_file_content;

struct HuffmanTreeNode {
    char data;
    long long frequency;

    HuffmanTreeNode* left;
    HuffmanTreeNode* right;

    HuffmanTreeNode(char data, long long frequency){
        this->data = data;
        this->frequency = frequency;
        left = nullptr;
        right = nullptr;
    }

    HuffmanTreeNode(char data, long long frequency, HuffmanTreeNode* left, HuffmanTreeNode* right){
        this->data = data;
        this->frequency = frequency;
        this->left = left;
        this->right = right;
    }
};

struct maxHeapCompare{
    bool operator()(const HuffmanTreeNode* lhs, const HuffmanTreeNode* rhs){
        return lhs->frequency < rhs->frequency;
    }
};

struct minHeapCompare{
    bool operator()(const HuffmanTreeNode* lhs, const  HuffmanTreeNode* rhs){
        return lhs->frequency > rhs->frequency;
    }
};

priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, maxHeapCompare> maxHeap;
priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, minHeapCompare> minHeap;

void getCharFrequenciesInFile(string file_name){
    ifstream file;
    file.open("./" + file_name);

    if(!file){
        cout << "Can't find file " << file_name << "\n";
        exit(0);
    } else {
        unordered_map<char,long long> frequency_map;

        string cur_line;

        bool first_line = true;

        while(getline(file, cur_line)) {
            original_file_content += cur_line;
            if(!first_line){
                frequency_map['\n']++;
                original_file_content += '\n';
                total_bits += 8;
                total_length++;
            }
            for(auto ch : cur_line){
                frequency_map[ch]++;
                total_length++;
                total_bits += 8;
            }
            first_line = false;
        }

        for(auto to : frequency_map){
            auto* huffmanTreeNode = new HuffmanTreeNode(to.first, to.second);
            maxHeap.push(huffmanTreeNode);
        }

        for(auto to : frequency_map){
            auto* huffmanTreeNode = new HuffmanTreeNode(to.first, to.second);
            minHeap.push(huffmanTreeNode);
        }


        file.close();
    }
}

HuffmanTreeNode* buildTree(priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, minHeapCompare> minHeap){
    HuffmanTreeNode* leftNode;
    HuffmanTreeNode* rightNode;
    HuffmanTreeNode* parentNode;

    while(minHeap.size() != 1){
        leftNode = minHeap.top(); minHeap.pop();
        rightNode = minHeap.top(); minHeap.pop();
        parentNode = new HuffmanTreeNode('#', leftNode->frequency + rightNode->frequency, leftNode, rightNode);
        minHeap.push(parentNode);
    }
    return minHeap.top();
}

void traverseTree(HuffmanTreeNode* root, string code = ""){
    if(root == nullptr){
        return;
    }
    if(root->data != '#'){
        if(root->data == '\n'){
            cout << "new line";
        } else if(root->data == ' '){
            cout << "space";
        } else {
            cout << root->data;
        }
        printf(" %.3f ", root->frequency * 1.0 / total_length);
        cout << code << "\n";
    }
    traverseTree(root->left, code + "0");
    traverseTree(root->right, code + "1");
}

void getBinarySequence(HuffmanTreeNode* root, unordered_map<char, string>& binaryCodes, string code = ""){
    if(root == nullptr){
        return;
    }
    if(root->data != '#'){
        binaryCodes[root->data] = code;
    }
    getBinarySequence(root->left, binaryCodes, code + "0");
    getBinarySequence(root->right, binaryCodes, code + "1");
}

void createAndSaveToFile(string codeSequence, string fileName){
    ofstream file(fileName);
    file << codeSequence;
    file.close();
}

int main(){
    getCharFrequenciesInFile("sample_data.txt");


    //probabilities in descending order
    while(!maxHeap.empty()){
        HuffmanTreeNode* maxNode = maxHeap.top();
        maxHeap.pop();

        if(maxNode->data == '\n'){
            cout << "new line";
        } else if(maxNode->data == ' '){
            cout << "space";
        } else {
            cout << maxNode->data;
        }
        cout << ": ";
        printf("%.3f\n", maxNode->frequency * 1.0 / total_length);
    }
    cout << "\n";
    // root of huffman tree
    HuffmanTreeNode* root = buildTree(minHeap);

    traverseTree(root);

    cout << "\n";

    unordered_map<char, string> binaryCodes;
    getBinarySequence(root, binaryCodes);

    string binarySequence = "";
    

    for(auto to : original_file_content){
        binarySequence += binaryCodes[to];
    }

    createAndSaveToFile(binarySequence, "binary_sequence.txt");

    cout << "Number of bits in the original text: " << total_bits << " bits\n";
    cout << "Number of bits in the compressed text: " << binarySequence.size() << " bits\n";
    printf("Compression ratio: %.2f\n", total_bits * 1.0 / binarySequence.size());
    printf("Average code length: %.2f bits/symbol\n", binarySequence.size() * 1.0 / total_length);
    
}
