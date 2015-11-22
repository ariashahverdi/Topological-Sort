/* Hello World program */

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

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
    newNode->ops_edge = 0;
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V)
{
    Graph* my_graph = (Graph*) malloc (sizeof(Graph));
    my_graph-> V = V;
    my_graph->ops_cnst = 0;
   
   my_graph->lists = (AdjList*)malloc(V * sizeof(AdjList));
   int i;
    for (i=0; i<V; i=i+1) {
        my_graph->lists[i].ops_vertix = 0;
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

void DetailedCosts(Graph* graph, FILE *ofp)
{
    fprintf(ofp, "Detailed Costs Are as Follows\n");
    
    fprintf(ofp, "Constant Costs : %d\n", graph->ops_cnst);
    int v;
    for (v = 0; v < graph->V; ++v){
        AdjListNode* pCrawl = graph->lists[v].head;
        fprintf(ofp, "%d (%d) : ", v + 1, graph->lists[v].ops_vertix);
        while (pCrawl){
            fprintf(ofp, " -> %d (%d) ", pCrawl->dest+1, pCrawl->ops_edge);
            pCrawl = pCrawl->next;
        }
        fprintf(ofp, " -> \\ \n");
    }
    fprintf(ofp, "\n");

}

void TotalCosts(Graph* graph, int * Tot_V, int * Tot_E, int * Tot)
{
    printf("Calculating The Total Cost\n");
    
    int v;
    for (v = 0; v < graph->V; ++v){
        AdjListNode* pCrawl = graph->lists[v].head;
        *Tot_V += graph->lists[v].ops_vertix;
        while (pCrawl){
            *Tot_E += pCrawl->ops_edge;
            pCrawl = pCrawl->next;
        }
    }
    *Tot = *Tot_V + *Tot_E + graph->ops_cnst;
}

Graph * GetGraph(char inputFilename[])
{
    printf("Inputing The Graph From File\n");
    
    int V = 0;
    int i = 0;
    int src, dest;
    FILE *ifp = fopen(inputFilename, "r");
    
    char line[5000];
    if (ifp != NULL) {
        while (fgets(line, sizeof line, ifp) != NULL) {V = V + 1;}
        fclose(ifp);
    }
    else
    {
        fprintf(stderr, "Can't open input file %s !\n", inputFilename);
        exit(1);
    }
    
    Graph* mygraph = createGraph(V);
    
    ifp = fopen(inputFilename, "r");
    if (ifp != NULL) {
        while (fgets(line, sizeof line, ifp) != NULL) {
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
        fclose(ifp);
    }
    return mygraph;
}

void PrintResults(Graph * mygraph, int Q[], int res, int argc, char inputFilename[])
{
    printf("Preparing The Result\n");
    
    FILE * ofp;
    char out[4] = "OUT_";
    char *outputFilename = malloc(strlen(out)+strlen(inputFilename)+1);//+1 for the zero-terminator
    strcpy(outputFilename, out);
    strcat(outputFilename, inputFilename);
    
    ofp = fopen(outputFilename, "w");
    
    if (ofp == NULL) {
        fprintf(stderr, "Can't open output file %s !\n", outputFilename);
        exit(1);
    }
    
    int i;
    char word[7] = "in1.txt";
    if(res == 1){
        fprintf(ofp,"** The Graph Has Cycle **\n");
        fclose(ofp);
        return;
    }
    else{
        fprintf(ofp, "The Result of Topological Sort is : \n");
        for (i=0; i<mygraph->V; i++) {
            fprintf(ofp, "%d ", Q[i]);
        }
    }
    fprintf(ofp, "\n\n");
    int Tot_V = 0;
    int Tot_E = 0;
    int Tot = 0;
    TotalCosts(mygraph,&Tot_V,&Tot_E,&Tot);
    
    if((argc > 1) && (strcmp(inputFilename, "in1.txt") == 0)) DetailedCosts(mygraph, ofp);
    
    fprintf(ofp, "Total Cost is : \n");
    fprintf(ofp, "Vertices : %d, Edges : %d\nTotal Cost (Vertices + Edges + Constant Terms)  : %d\n", Tot_V, Tot_E, Tot);
    
    fclose(ofp);
    return;
}

int TopoSortGraph(Graph * mygraph, int Q[])
{
    printf("Sorting The Graph\n");
    

    int v, top = 0, s_pointer =0, front = 0, end = -1;
    int * in_degree = (int *) malloc (mygraph->V * sizeof(int));
    mygraph -> ops_cnst++; //Constant Operatoins
    
    
    for(v=0; v<mygraph->V; v++){
        in_degree[v] = 0; mygraph->lists[v].ops_vertix++; //Charge Vertex
    }
    for(v=0; v<mygraph->V; ++v){
        AdjListNode* Crawl = mygraph->lists[v].head; mygraph->lists[v].ops_vertix++; //Charge Vertex
        while(Crawl){
            in_degree[Crawl->dest]++; Crawl->ops_edge++; //Charge Edge
            Crawl = Crawl->next;
        }
    }
    for(v=0; v<mygraph->V; ++v){
        if(!in_degree[v]){
            Q[++end] = v+1; mygraph->lists[v].ops_vertix++; //Charge Vertex
        }
    }
    while(front <= end){
        AdjListNode* Crawl = mygraph->lists[Q[front++]-1].head; mygraph->lists[Q[front-1]-1].ops_vertix++; //Charge Vertex
        while(Crawl){
            Crawl->ops_edge++; //Charge Edge
            if(!(--in_degree[Crawl->dest])){
                Q[++end] = (Crawl->dest) + 1; mygraph->lists[Crawl->dest].ops_vertix++; //Charge Vertex
            }
            Crawl = Crawl->next;
        }
    }
    
    mygraph -> ops_cnst++; //Constant Operatoins
    return (front < mygraph->V);
}


int main(int argc, char *argv[])
{
    int i = 0;
    printf("Topological Sort Program \n");
    
    Graph * mygraph;

    mygraph = GetGraph(argv[1]);
    
    // Initialize the Queue
    int * Q = (int *) malloc (mygraph->V * sizeof(int));
    for (i=0; i<mygraph->V; i++) {
        Q[i] = 0;
    }
    
    int res;
    res = TopoSortGraph(mygraph, Q);
    
    PrintResults(mygraph, Q, res, argc, argv[1]);
    
    printf("Done !\n");
    
    return 0;
}