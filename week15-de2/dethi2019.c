#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include "weightedGraphlib.h"
#include "weightedGraphlib.c"
#include "../week4/dllist.h"
#include "../week4/dllist.c"

typedef struct Page
{
    char *p_name;
    int p_id;
    double pr_new;
    double pr_old;
}Page;

void printError(int mode)   //Ham in loi do phai viet printf nhieu
{
    switch (mode)
    {
        case 1:
            printf("Khong the mo file\n");
            break;
        
        case 2:
            printf("-1");
            break;
        
        case 3:
            printf("Khong co chuc nang\n");
            break;
    }
}

void docFile(FILE *fwp, FILE *fpc, Graph graph, Page *p, int V) //Ham doc ca 2 file nen hoi dai
{
    fwp = fopen("../../Data/webpages.txt", "r");
    fpc = fopen("../../Data/pageConnections.txt", "r");
    if(!fwp || !fpc)
    {
        printError(1);
        return;
    }
    rewind(fwp);
    rewind(fpc);
    fscanf(fwp, "%d", &V);  //Doc dong dau tien cuar file webpage 
    printf("So luong webpage: %d\n", V);
    int i = 0;
    while(!feof(fwp))
    {
        fscanf(fwp, "%s %d", p[i].p_name, &p[i].p_id);
        printf("%s %d\n", p[i].p_name, p[i].p_id);
        addVertex(graph, i, p[i].p_name);
        p[i].pr_old = 1;
        p[i].pr_new = 0;
        i++;
    }
    fscanf(fpc, "%d", &V);
    char c;
    int start, end, find;
    while(!feof(fpc))
    {
        fscanf(fpc, "%d", &start);
        c = fgetc(fpc);
        while(c != '\n' && c != EOF)
        {
            fscanf(fpc, "%d", &end);
            find = 0;
            for(i = 0; i < V; i++)
            {
                if(p[i].p_id == start)
                {
                    start = i;
                    find++;
                }
                if(p[i].p_id == end)
                {
                    end = i;
                    find++;
                }
                if(find == 2)
                    break;
            }
            addEdge(graph, start, end, 1);
            c = fgetc(fpc);
        }
    }
    int n, *adj = (int*)malloc(sizeof(int) * V), max = -1, min = INT_MAX;
    for(i = 0; i < V; i++)
    {
        n = indegree(graph, i, adj);
        if(n >= max)
        {
            max = n;
            start = i;
        }
        if(n <= min)
        {
            min = n;
            end = i;
        }
    }
    printf("webpage: %s co nhieu lien ket toi nhat\n", p[start].p_name);
    printf("webpage: %s co it lien ket toi nhat\n", p[end].p_name);
    free(adj);
    fclose(fwp);
}

void maxminWeight(Graph graph, Page *p, int V)
{
    int n, i, j, max_id, min_id;
    double max = -1, sum = 0, min = INT_MAX;
    int *adj = (int*)malloc(sizeof(int) * V);
    for(i = 0; i < V; i++)
    {
        if(p[i].pr_old >= max)
        {
            max = p[i].pr_old;
            max_id = i;
        }
        if(min >= p[i].pr_old)
        {
            min = p[i].pr_old;
            min_id = i;
        }
    }
    printf("webpage %s co trong so cao nhat la %.2lf\n", getVertex(graph, max_id), max);
    printf("webpage %s co trong so thap nhat la %.2lf\n", getVertex(graph, min_id), min);
    free(adj);
}

void BFS(Graph g, Page *p, int *dist, int src, int V)
{
    Dllist q = new_dllist(), tmp;
    int count, i, u, v, n;
    int *adj = (int*)malloc(V * sizeof(int));
    double weight;
    bool visited[V];
    memset(visited, false, sizeof(visited));
    dll_append(q, new_jval_i(src));
    visited[src] = 1;
    dist[src] = 0;
    //JRB enode, tree;
    while(!dll_empty(q))
    {
        tmp = dll_first(q);
        u = tmp->val.i;    
        dll_delete_node(tmp);
        n = outdegree(g, u, adj);
        //enode = jrb_find_int(g->edges, u);
        for(i = 0; i < n; i++)
        {
            v = adj[i];
            //tree = jrb_find_int((JRB)(enode->val).v, v);
            //(tree->val).d = p[u].pr_old / n;
            p[v].pr_new += p[u].pr_old / n;
            if(!visited[v])
            {
                dist[v] = dist[u] + 1;
                visited[v] = true;
                dll_append(q, new_jval_i(v));
            }
        }
    }
    for(i = 0; i < V; i++)
        if(indegree(g, i, adj))
        {
            p[i].pr_old = p[i].pr_new;
            p[i].pr_new = 0;
        }
    free(adj);
}

int compare(const void *a, const void *b)
{
    const Page *p_a = (const Page*)a;
    const Page *p_b = (const Page*)b;
    return (double)p_b->pr_old > (double)p_a->pr_old;
}

void Spambot(Graph graph, int V)
{
    int i, n, cd = 0, cr = 0;
    int *adj = (int*)malloc(sizeof(int) * V);
    for(i = 0; i < V; i++)
    {
        n = indegree(graph, i, adj);
        if(n == 0)
            cr++;
        n = outdegree(graph, i, adj);
        if(n == 0)
            cd++;
    }
    printf("Co %d webpage chi co lien ket toi", cd);
    printf("\nCo %d webpage chi co lien ket ra\n", cr);
    free(adj);
}

bool isInteger(char *s)
{
    for(int i = 0; i < strlen(s); i++)
        if(!isdigit(s[i]))
            return false;
    return true;
}

int main()
{
    int V, opt, i, m, a, b, n, j;
    char term, id1[10], id2[10];
    FILE *fwp = NULL, *fpc = NULL;
    Graph graph = createGraph();
    fwp = fopen("../../Data/webpages.txt", "r");
    fscanf(fwp, "%d", &V);
    int dist[V];
    memset(dist, 0, sizeof(dist));
    Page *p = (Page*)malloc(V * sizeof(Page));
    for(i = 0; i < V; i++)
        p[i].p_name = (char*)malloc(sizeof(char) * 251);
    fclose(fwp);
    while(1)
    {
        printf("\n===== Pagerank =====\n");
        printf("\n1. Thong tin webpage");
        printf("\n2. Lap 1 lan");
        printf("\n3. Lap m lan");
        printf("\n4. Spam bot");
        printf("\n5. Khoang cach nho nhat giua 2 trang");
        printf("\n6. Thoat chuong trinh");
        printf("\nChon chuc nang: ");
        scanf("%d", &opt);
        switch (opt)
        {
            case 1:
                docFile(fwp, fpc, graph, p, V);
                break;

            case 2:
                BFS(graph, p, dist, 0, V);
                maxminWeight(graph, p, V);
                break;

            case 3:
                do
                {
                    printf("Nhap so lan lap: ");
                    scanf("%d", &m);
                    if(m < 10 || m > 50)
                        printf("Gia tri m phai nam trong khoang 10-50\n");
                }while(m < 10 || m > 50);
                for(i = 0; i < m; i++)
                    BFS(graph, p, dist, 0, V);
                qsort(p, V, sizeof(Page), compare);
                printf("Top 3 trang web quan trong nhat: \n");
                for(int i = 0; i < V; i++)
                    printf("%s %.2lf\n", p[i].p_name, p[i].pr_old);
                break;

            case 4:
                Spambot(graph, V);
                break;

            case 5:
                printf("Nhap id1: ");
                scanf("%s", id1);
                if(!isInteger(id1))
                {
                    printError(2);
                    break;
                }
                printf("Nhap id2: ");
                scanf("%s", id2);
                if(!isInteger(id2))
                {
                    printError(2);
                    break;
                }
                m = atoi(id1);
                n = atoi(id2);
                if(m == n)
                    printError(2);
                for(i = 0; i < V; i++)
                {
                    if(p[i].p_id == m)
                        a = i;
                    if(p[i].p_id == n)
                        b = i;
                }
                BFS(graph, p, dist, a, V);
                for(i = 0; i < V; i++)
                    if(i == b && dist[i] != 0)
                        printf("Khoang cach nho nhat giua cac trang: %d\n", dist[b]);
                    else printError(-1);
                break;

            case 6:
                printf("Thoat\n");
                for(i = 0; i < V; i++)
                    free(p[i].p_name);
                free(p);
                dropGraph(graph);
                exit(0);
                break;
            
            default:
                printError(3);
                break;
        }
    }
    return 0;
}