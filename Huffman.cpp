//Program name: Huffman.cpp
//Author: Dan Kercher
//Date last updated: 12/05/2020
//Purpose: To construct Huffman codes from symbols (ASCII characters) and frequency data
//Note: I based this on a turorial that used an array of characters and ints. This version reads an input file, constructor vectors for the strings (so that LF
// is recognizable) and frequency counts, then constructs a tree based on the Huffman algorithm. 
#include <iostream> 
#include <fstream>
#include <vector>
#include <cstdlib> 
#include <string>
using namespace std;

// Huffman Tree Node
struct HuffmanNode 
{
	// Data
	string symbol;
	int frequency;

	// Chilld Nodes
	struct HuffmanNode* left, * right;
};

// Huffman Tree
struct HuffmanTree 
{
	int size;
	int capacity;

	// Array pointers 
	struct HuffmanNode** array;
};

// HuffmanNode Constructor
struct HuffmanNode* newNode(string symbol, int frequency)
{
	struct HuffmanNode* huffmanNode = new HuffmanNode;

	huffmanNode->left = huffmanNode->right = NULL;
	huffmanNode->symbol = symbol;
	huffmanNode->frequency = frequency;

	return huffmanNode;
}

// Huffman Tree Constructor
struct HuffmanTree* createHuffmanTree(int capacity)
{
	struct HuffmanTree* huffmanTree = new HuffmanTree;

	huffmanTree->size = 0;
	huffmanTree->capacity = capacity;
	huffmanTree->array
		= (struct HuffmanNode**)malloc(huffmanTree->
			capacity * sizeof(struct HuffmanNode*));
	return huffmanTree;
}

// Recursivly Minimize Tree**
void MinimizeTree(struct HuffmanTree* HuffmanTree, int index)
{
	int smallest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < HuffmanTree->size && HuffmanTree->array[left]->
		frequency < HuffmanTree->array[smallest]->frequency)
		smallest = left;

	if (right < HuffmanTree->size && HuffmanTree->array[right]->
		frequency < HuffmanTree->array[smallest]->frequency)
		smallest = right;

	if (smallest != index) 
	{
		struct HuffmanNode* t = *&HuffmanTree->array[smallest];
		*&HuffmanTree->array[smallest] = *&HuffmanTree->array[index];
		*&HuffmanTree->array[index] = t;

		MinimizeTree(HuffmanTree, smallest);
	}
}

// Extract minimum value node from tree** 
struct HuffmanNode* extractMin(struct HuffmanTree* huffmanTree)
{
	struct HuffmanNode* huffmanTreeNode = huffmanTree->array[0];
	huffmanTree->array[0]
		= huffmanTree->array[huffmanTree->size - 1];

	--huffmanTree->size;
	MinimizeTree(huffmanTree, 0);

	return huffmanTreeNode;
}

// The main function that builds Huffman tree**
struct HuffmanNode* buildHuffmanTree(vector<string> symbol, vector<int> frequency, int size)
{
	struct HuffmanNode* left, * right, * top;
	struct HuffmanTree* huffmanTree = createHuffmanTree(size);

	for (int i = 0; i < size; ++i)
		huffmanTree->array[i] = newNode(symbol[i], frequency[i]);

	huffmanTree->size = size;
	int n = huffmanTree->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		MinimizeTree(huffmanTree, i);

	// Keep getting the minimums and inserting the nodes until only one left (see below, that's the final root node)
	while (huffmanTree->size!=1) {

		// Extract the two minimums per the algorithm
		left = extractMin(huffmanTree);
		right = extractMin(huffmanTree);

		// Inset new node into the tree
		top = newNode("$", left->frequency + right->frequency);

		top->left = left;
		top->right = right;

		++huffmanTree->size;
		int i = huffmanTree->size - 1;

		while (i && top->frequency < huffmanTree->array[(i - 1) / 2]->frequency) {

			huffmanTree->array[i] = huffmanTree->array[(i - 1) / 2];
			i = (i - 1) / 2;
		}

		huffmanTree->array[i] = top;
	}

	// Once there is only one node left, it is the root and tree is built
	return extractMin(huffmanTree);
}

// Prints Huffman code results from built tree**
void printCodes(struct HuffmanNode* root, vector<int> vect, int top)
{
	// Left edge = 0
	if (root->left) {

		vect[top] = 0;
		printCodes(root->left, vect, top + 1);
	}

	// Right edge = 1
	if (root->right) {

		vect[top] = 1;
		printCodes(root->right, vect, top + 1);
	}

	// Node is a leaf, get symbol
	if (!(root->left) && !(root->right)) 
	{
		cout << root->symbol << ": ";
		int i;
		for (i = 0; i < top; ++i)
		{
			cout << vect[i];
		}
		cout << "\n";
	}
}


int main()
{
	// vectors to hold input data
	vector<string> strings;
	vector<string> chars;
	vector<int> freqs;

	// Input file stream from freq.txt resource file
	ifstream in("freq.txt", ios::in);

	// Push input file stream to populate vector
	string x;
	while (in >> x)
	{
		strings.push_back(x);
	}

	// Close the file stream
	in.close();

	// Vector length and input character count, used several times below
	int length = strings.size();
	int charCount = length / 2;

	//Display the input vector, this is raw file contents
	cout << "*****Input Vector****\n";
	for (int i = 0; i < length; i++)
	{
		cout << strings[i] << " ";
	}

	// Sort out character symbol strings and add them to their own vector (should be even vector elements starting with 0)
	for (int i = 0; i < length; i = i + 2)
	{
		chars.push_back(strings[i]);
	}

	// Sort out int frequency strings and add them to their own vector (should be odd vector elements starting with 1)
	for (int i = 1; i < length; i = i + 2)
	{
		int tempInt;
		tempInt = stoi(strings[i]);
		freqs.push_back(tempInt);
	}

	// Display the input vector character elements
	cout << "\n*****Character elements****\n";
	for (int i = 0; i < charCount; i++)
	{
		cout << chars[i] << " ";
	}

	// Display the input vector frequency elements
	cout << "\n*****Frequency element****\n";
	for (int i = 0; i < charCount; i++)
	{
		cout << freqs[i] << " ";
	}

	// Build Tree and print code output from input vectors
	cout << "\n*****Huffman Code Output****\n";
	struct HuffmanNode* root = buildHuffmanTree(chars, freqs, charCount);
	int top = 0;
	vector<int> vect(100);
	printCodes(root, vect, top);

	//HuffmanCodes(chars, freqs, charCount);

	return 0;
}
