#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "week4/jval.h"
#include "week4/jrb.h"
#include "week4/dllist.h"

#define INFINITIVE_VALUE  10000000

typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

typedef struct {
   Graph graph;
   int rootID;
} HuffmanTree;

typedef Dllist PriorityQueue;

typedef struct {
	int size;
	char bits[20]; // maximum 20 bits, it is safe enough
} Coding;

typedef struct {
	int size;
	int * nodes;
} HuffmanTreeArray;

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

void addVertex(Graph g, int id, int letter)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) // only add new vertex 
         jrb_insert_int(g.vertices, id, new_jval_i(letter));            
}

int getVertex(Graph g, int id)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) 
        return 0;
     else                
        return jval_i(node->val);
}     

void addEdge(Graph graph, int v1, int v2, int weight)
{
     JRB node, tree;
     if (getEdgeValue(graph, v1, v2)==INFINITIVE_VALUE)
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_i(weight));
     }
}

int getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INFINITIVE_VALUE;
    else
       return jval_i(node->val);       
}

int indegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph.edges)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}

int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}



void add2Queue(PriorityQueue q, int graphnode, int frequence) {
	Dllist node = dll_first(q);
	while (node!=q) {
		// printf("Node %d %d\n", dll_val(node).iarray[0], dll_val(node).iarray[1] );
		if ( frequence <= dll_val(node).iarray[1] ) break;
		node =  dll_next(node);
	}
	if ( node==q ) {
		// printf("Node nil\n");
		dll_append(q, new_jval_iarray(graphnode, frequence));
	} else {
		// printf("Before %d %d\n", dll_val(node).iarray[0], dll_val(node).iarray[1]);
		dll_insert_b(node, new_jval_iarray(graphnode, frequence));	
	}
}

HuffmanTree makeHuffman(int fTable[]) {
	int lastNodeID = 0;
	HuffmanTree	hTree;
	int i;
	Dllist n1, n2;

	PriorityQueue queue = new_dllist();
	hTree.graph = createGraph();

	for (i=0; i<256; i++)
		if (fTable[i]) {
			// make new graphNode to add to the priority queue
			lastNodeID++;
			addVertex(hTree.graph, lastNodeID, i);
			add2Queue(queue, lastNodeID, fTable[i]);
			// printf("last %d %c %d\n", lastNodeID, i, fTable[i]);
		}
	
/*	for (n1 = dll_first(queue); n1!=queue; n1=dll_next(n1))
	{
		printf("%d %d\n", dll_val(n1).iarray[0], dll_val(n1).iarray[1]);
	}
*/	
	// regroup nodes in priority queue
	for (;;)
	{
		n1 = dll_first(queue);
		if ( n1==queue ) break;
		n2 = dll_next(n1);
		if ( n2==queue ) break;
		// add new node in the graph
		lastNodeID++;
		addVertex(hTree.graph, lastNodeID, -1);
		addEdge(hTree.graph, lastNodeID, dll_val(n1).iarray[0], 0); 
		addEdge(hTree.graph, lastNodeID, dll_val(n2).iarray[0], 1); 
		//printf("last %d\n", lastNodeID);
		// modify the queue
		dll_delete_node(n1);
		dll_delete_node(n2);
		add2Queue(queue, lastNodeID, dll_val(n1).iarray[1]+dll_val(n2).iarray[1]);
	}
	n1 = dll_first(queue);
	if ( n1==queue ) 
		hTree.rootID = 0;
	else
		hTree.rootID = dll_val(n1).iarray[0];	
	
	free_dllist(queue);

	return hTree;
}

void getCode(int node, Graph g, int len, char code[], Coding htable[]) 
{
	int ch, k, i;
	int output[10];
	ch = getVertex(g, node);
	if (ch != -1) {
		htable[ch].size = len;
		memcpy(htable[ch].bits, code, len);	
	} else {
		k = outdegree(g, node, output);
		for (i=0; i<k; i++) {
			code[len] = getEdgeValue(g, node, output[i]);
			getCode(output[i], g, len+1, code, htable);
		}
	}
}

void createHuffmanTable(HuffmanTree htree, Coding htable[]) {
	int i;
	char code[20];
	for (i=0; i<256; i++) {
		htable[i].size=0;
	}
	if (htree.rootID == 0) return;
	getCode(htree.rootID, htree.graph, 0, code, htable); 
}
/*
void compressFile(char* input, char*output) {
	HuffmanTree htree;
	Coding htable[256] = {};
	int i, k, n, nbit, size;
	
	FILE* f1, *f2;

	int fTable[256] = {} ;
	
	char* huffman;
	
	f1 = fopen(input, "rb");
	if (f1==NULL) {
		perror("Can not open input file\n");
		exit(2);
	}
	
	size = 0;
	while ( (i=fgetc(f1)) != -1 ) {
		fTable[i]++;
		size++;
	}
	
	printf("Input: %d bytes\n", size);
	
	if (size==0) return;
	
	htree = makeHuffman (fTable);
	createHuffmanTable(htree, htable);
	
	f2 = fopen(output, "w+b");
	if (f2==NULL) {
		perror("Can not open output file\n");
		exit(2);
	}
	fwrite("HM", 2, 1, f2);
	writeHuffmanTable(htable, f2);
	
	fseek(f1, 0, SEEK_SET);
	nbit = 0;
	huffman = malloc(size);
	while ( (i=fgetc(f1)) != -1 ) {
		addHuffmanChar(i, htable, huffman, &nbit);
	}
	fwrite(&nbit, sizeof(int), 1, f2);
	n = nbit/8;
	if (nbit%8) n++;
	fwrite(huffman, n, 1, f2);
	free(huffman);
	printf("Compressed: %d bytes\n", n);
	
	fclose(f1);
	fclose(f2);
}
*/
/*
typedef struct {
	int size;
	int * nodes;
} HuffmanTreeArray;
*/

HuffmanTreeArray makeHuffmanTreeArray(Coding htable[]) 
{
	int i, k, maxlen, size, exp2, cursor;
	HuffmanTreeArray hArray;
	
	maxlen = 0;
	for (i=0; i<256; i++)
		if (htable[i].size > maxlen) maxlen = htable[i].size;
	size = 1;
	exp2 = 1;
	for (i=1; i<=maxlen; i++) {
		exp2 *= 2;	
		size += exp2;
	}
	hArray.size = size;
	hArray.nodes = malloc(hArray.size*sizeof(int));
	for (i=0; i<hArray.size; i++) hArray.nodes[i] = -1;
	for (i=0; i<256; i++) {
		setRoot(&cursor);
		if (htable[i].size) {
			for (k=0; k<htable[i].size; k++) {
				if (htable[i].bits[k]==0) goLeft(&cursor);
				else goRight(&cursor);
			}
			if (cursor<hArray.size) hArray.nodes[cursor] = i;
		}
	}
	
	return hArray;
}


HuffmanTreeArray tree2array(HuffmanTree tree)
{
  Coding htable[256];
  HuffmanTreeArray hArray;
  createHuffmanTable(tree,htable);
  return makeHuffmanTreeArray(htable);
  
}

void destroyHuffmanTreeArray(HuffmanTreeArray htree)
{
	free(htree.nodes);
}
	
int main(String argv[], int argc)
{
   char buffer[1000] = "fdfdfdfgjkfjkgrutrtrtkgfkgjjfdfsdiruiedfffdffgffg";
   char huffman[1000];
   Coding htable[256];
   int nbit, bytes;
   nbit = compress(buffer, huffman, htable);
   bytes = decompress(huffman, nbit, htable, buffer);
   buffer[bytes] = '\0';
   printf("%s\n", buffer);
   compressFile("hello.txt", "huffman.dat");
   decompressFile("huffman.dat", "hello_new.txt");
}
