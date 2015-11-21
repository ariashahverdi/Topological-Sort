/* Hello World program */

#include<stdio.h>
#include <stdlib.h>

#define MAX_VER_NUM 100

typedef struct AdjListNode{
    int ops_edge;
    int dest;
    struct AdjListNode* next;
}AdjListNode;

typedef struct AdjList{
    int ops_vertix;
    struct AdjListNode* head;
}AdjList;

typedef struct Graph{
    int ops_cnst;
    int V;
    struct AdjList* lists;
}Graph;


AdjListNode* newAdjListNode(int dest)
{
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V)
{
    Graph* my_graph = (Graph*) malloc (sizeof(Graph));
    my_graph-> V = V;
   
   my_graph->lists = (AdjList*)malloc(V * sizeof(AdjList));
   int i;
    for (i=0; i<V; i=i+1) {
        my_graph->lists[i].head = NULL;
    }
    return my_graph;
}

void addEdge(Graph* graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    AdjListNode* newNode = newAdjListNode(dest-1);
    newNode->next = graph->lists[src-1].head;
    graph->lists[src-1].head = newNode;
}

void printGraph(Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v){
        AdjListNode* pCrawl = graph->lists[v].head;
        printf("%d :", v + 1);
        while (pCrawl){
            printf(" -> %d", pCrawl->dest+1);
            pCrawl = pCrawl->next;
        }
        printf(" -> \\ \n");
    }
    printf("\n");
}

Graph * GetGraph(char inputFilename[])
{
    printf("Input The Graph From File\n");
    
    FILE *ifp, *ofp;
    char *mode = "r";
    char outputFilename[] = "out.list";

    FILE * fp;
    size_t len = 0;
    ssize_t read;
    
    int V = 0;
    int i = 0;
    int src, dest;
    FILE *in = fopen(inputFilename, "r");
    
    char line[5000];
    if (in != NULL) {
        while (fgets(line, sizeof line, in) != NULL) {V = V + 1;}
        fclose(in);
    }
    
    Graph* mygraph = createGraph(V);
    
    in = fopen(inputFilename, "r");
    if (in != NULL) {
        while (fgets(line, sizeof line, in) != NULL) {
            char *start = line;
            int field;
            int n;
            i = 0;
            while (sscanf(start, "%d%n", &field, &n) == 1) {
                if (i==0) {
                    src = field;
                }
                else{
                    dest = field;
                    addEdge(mygraph, src, dest);
                }
                start += n;
                i++;
            }
        }
        fclose(in);
    }
    return mygraph;
}


int TopoSortGraph(Graph * mygraph, int Q[], int S[])
{
    printf("Sorting The Graph\n");
    int v, top = 0, s_pointer =0, front = 0, end = 0;
    int * in_degree = (int *) malloc (mygraph->V * sizeof(int));
    
    for(v=0; v<mygraph->V; ++v){
        Q[v] = 0;
        in_degree[v] = 0; mygraph->lists[v].ops_vertix++; //Accessing Each Vertex
    }
    for(v=0; v<mygraph->V; ++v){
        AdjListNode* Crawl = mygraph->lists[v].head; mygraph->lists[v].ops_vertix++; //Accessing The Vertex
        while(Crawl){
            in_degree[Crawl->dest]++; Crawl->ops_edge++; //Accessing Each Edge
            Crawl = Crawl->next;
        }
    }
    for(v=0; v<mygraph->V; ++v){
        if(in_degree[v] == 0){
            Q[top] = v;
            top++;
        }
    }
    while(top>0){//not sure!!
        S[s_pointer] = Q[top-1] + 1;
        AdjListNode* Crawl = mygraph->lists[Q[top-1]].head;
        s_pointer++;
        top--;
        
        while(Crawl){
            in_degree[Crawl->dest]--;
            if(in_degree[Crawl->dest] == 0){
                Q[top] = Crawl->dest;
                top++;
            }
            Crawl = Crawl->next;
        }
    }

    for(v=0; v<mygraph->V; ++v){
        if (in_degree[v] != 0) {
            return -1; //There is a Cycle!
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    printf("Topological Sort Program \n");
    
    Graph * mygraph;

    mygraph = GetGraph(argv[1]);
    
    int * Q = (int *) malloc (mygraph->V * sizeof(int));
    int * S = (int *)malloc(mygraph->V * sizeof(int));
    
    //printGraph(mygraph);
    
    int res;
    res = TopoSortGraph(mygraph, S, Q);
    
    printf("\n\n");
    int i = 0;
    if(res == -1){
        printf("**The Graph Has Cycle **\n");
    }
    else{
        for (i=0; i<mygraph->V; ++i) {
            printf("%d ", S[i]);
        }
    }
    printf("\n\n");
    
    //print the adjacency list representation of the above graph
    //printGraph(mygraph);
    
    /*
    
    ofp = fopen(outputFilename, "w");
    
    if (ofp == NULL) {
        fprintf(stderr, "Can't open output file %s!\n",
                outputFilename);
        return 0;
    }
    */
     
     return 0;

}