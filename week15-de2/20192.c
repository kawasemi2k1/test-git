#include <stdio.h>
#include "libfdr/jrb.h"
#include "libfdr/jval.h"
#include "libfdr/dllist.h"
#include <string.h>
#include <stdlib.h>

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph()
{
    Graph graph;
    graph.edges = make_jrb();
    graph.vertices = make_jrb();
    return graph;
}

void addVertex(Graph graph, int id, char *name)
{
    JRB node;
    JRB tree;
    node = jrb_find_int(graph.vertices, id);
    if (node == NULL)
    {
        jrb_insert_int(graph.vertices, id, new_jval_s(name));
        tree = make_jrb();
        jrb_insert_int(graph.edges, id, new_jval_v(tree));
    }
    else
        return;
}

char *getVertex(Graph graph, int id)
{
    JRB node;
    node = jrb_find_int(graph.vertices, id);
    if (node == NULL)
        return NULL;
    else
        return jval_s(node->val);
}

void addEdge(Graph graph, int v1, int v2)
{
    JRB node, tree, node2;
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
    {
        tree = make_jrb();
        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
    else
    {
        tree = make_jrb();
        tree = (JRB)jval_v(node->val);
        node2 = jrb_find_int(tree, v2);
        if (node2 == NULL)
        {
            jrb_insert_int(tree, v2, new_jval_i(1));
        }
        // jrb_insert_int(tree,v2,new_jval_i(1));
        node->val = new_jval_v(tree);
    }
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return 0;
    else
    {
        tree = (JRB)jval_v(node->val);
        jrb_traverse(node, tree) if (v2 == jval_i(node->key)) return 1;
    }
    return 0;
}

int outdegree(Graph graph, int v, int *output)
{
    JRB node, tree;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node == NULL)
        return 0;
    else
    {
        tree = (JRB)jval_v(node->val);
        total = 0;
        jrb_traverse(node, tree)
            output[total++] = jval_i(node->key);
        return total;
    }
}

int indegree(Graph graph, int v, int *output)
{
    JRB node, tree;
    node = NULL;
    int key;
    int total;
    total = 0;
    jrb_traverse(node, graph.vertices)
    {
        key = jval_i(node->key);
        if (hasEdge(graph, key, v) == 1 && key != v)
            output[total++] = key;
    }
    return total;
}

int getAdjacentVertices(JRB graph, int v, int *output)
{
    JRB node, tree;
    int total;
    node = jrb_find_int(graph, v);
    if (node == NULL)
        return 0;
    else
    {
        tree = (JRB)jval_v(node->val);
        total = 0;
        jrb_traverse(node, tree)
            output[total++] = jval_i(node->key);
        return total;
    }
}

int DAG(Graph graph)
{
    int visited[1000];
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, graph.vertices)
    {
        memset(visited, 0, sizeof(visited));

        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack, new_jval_i(start));

        while (!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);
            if (!visited[u])
            {
                visited[u] = 1;
                n = outdegree(graph, u, output);
                for (i = 0; i < n; i++)
                {
                    v = output[i];
                    if (v == start) // cycle detected
                        return 0;
                    if (!visited[v])
                        dll_append(stack, new_jval_i(v));
                }
            }
        }
    }
    return 1; // no cycle
}

void dropGraph(Graph graph)
{
    JRB node;
    jrb_traverse(node, graph.vertices)
        jrb_free_tree(jval_v(node->val));
    jrb_traverse(node, graph.edges)
        jrb_free_tree(jval_v(node->val));
}

void deleteEdge(Graph graph, int v1, int v2)
{
    JRB node, node2;
    if (hasEdge(graph, v1, v2) == 0)
        return;
    else
    {
        JRB tree;
        node = jrb_find_int(graph.edges, v1);
        tree = (JRB)jval_v(node->val);
        node2 = jrb_find_int(tree, v2);
        jrb_delete_node(node2);
    }
}

void topologicalSort(Graph g, int output[], int *n)
{
    Dllist queue;
    JRB node, node2;
    int OUT[100], OUT2[100];
    int totalOut;
    int t;
    int giaTri;
    t = 0;
    Dllist dlltmp;
    queue = new_dllist();
    jrb_traverse(node, g.vertices) if (indegree(g, jval_i(node->key), OUT) == 0)
        dll_append(queue, node->key);

    while (!dll_empty(queue))
    {
        dlltmp = dll_first(queue);
        output[t] = jval_i(dlltmp->val);
        t++;
        totalOut = outdegree(g, jval_i(dlltmp->val), OUT);
        giaTri = jval_i(dlltmp->val);
        dll_delete_node(dlltmp);
        for (int i = 0; i < totalOut; i++)
        {
            // node2=jrb_find_int(g.edges,OUT[i]);
            deleteEdge(g, giaTri, OUT[i]);
            if (indegree(g, OUT[i], OUT2) == 0)
                dll_append(queue, new_jval_i(OUT[i]));
        }
    }
    *n = t;
}

int main()
{
    // int i;
    // int n;
    // int output[100];
    // Graph g = createGraph();
    // addVertex(g, 0, "CS102");
    // addVertex(g, 1, "CS140");
    // addVertex(g, 2, "CS160");
    // addVertex(g, 3, "CS302");
    // addVertex(g, 4, "CS311");
    // addVertex(g, 5, "MATH300");
    // addEdge(g, 0, 1);
    // addEdge(g, 0, 2);
    // addEdge(g, 1, 3);
    // addEdge(g, 5, 4);
    // addEdge(g, 3, 4);
    // if (!DAG(g))
    // {
    //     printf("Can not make topological sort\n");
    //     return 1;
    // }
    // topologicalSort(g, output, &n);
    // printf("The topological order:\n");
    // for (i = 0; i < n; i++)
    //     printf("%s\n", getVertex(g, output[i]));

    int menu, m;
    do
    {
        printf("\n--------MENU--------\n");
        printf("1. In so luong web kem id, webpage nhieu va it lien ket toi nhat\n");
        printf("2. Sau lan lap dau tien\n");
        printf("3. Sau m lan lap\n");
        printf("4. So luong webpage chi co lien ket den, or chi co lien ket ra\n");
        printf("5. Khoang cach nho nhat giua hai webpage\n");
        printf("6. Thoat\n");
        printf("Nhap lua chon: ");
        scanf("%d", &menu);
        switch(menu)
        {
            case 1:
            break;

            case 2:
            break;

            case 3:
            do
            {
                printf("Nhap so nguyen duong m (thuoc [10; 50] ): ");
                scanf("%d", &m);

            } while (m < 10 || m > 50);
            
            
            break;

            case 4:
            break;

            case 5:
            break;

            case 6:
            break;

            default:
            break;
        }

    }while(menu !=6);
    
}
