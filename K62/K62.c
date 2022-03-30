#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"

int menuVaLuaChon(int a, int b)
{
    int luaChon, buff;
    char c;
    printf("\n+-----+--------------------------------------------------+\n");
    printf("|%25s%6s%25s|\n", "", "MENU", "");
    printf("+-----+--------------------------------------------------+\n");
    printf("|%5d|%-50s|\n", 1, "In ma tran ke");
    printf("+-----+--------------------------------------------------+\n");
    printf("|%5d|%-50s|\n", 2, "In danh sach ke");
    printf("+-----+--------------------------------------------------+\n");
    printf("|%5d|%-50s|\n", 3, "Dua ra...");
    printf("+-----+--------------------------------------------------+\n");
    printf("|%5d|%-50s|\n", 4, "Tim duong di ngan nhat");
    printf("+-----+--------------------------------------------------+\n");
    printf("|%5d|%-50s|\n", 5, "Tim duong di bo ngan nhat");
    printf("+-----+--------------------------------------------------+\n");
    printf("|%5d|%-50s|\n", 6, "Thoat");
    printf("+-----+--------------------------------------------------+\n");

    do
    {
        __fpurge(stdin);
        printf("Nhap vao lua chon cua ban: ");
        buff = scanf("%d%c", &luaChon, &c);
        if (buff != 2 && c != '\n' || luaChon < a || luaChon > b)
            printf("Ban da nhap sai cu phap\n");
    } while (buff != 2 && c != '\n' || luaChon < a || luaChon > b);
    return luaChon;
}

int docFile(Graph g, Graph gDiBo, char* tenFile)
{
    char str[100];
    int id1, id2;
    int w;
    int m, n;
    int i, j, k;
    FILE *f;
    f = fopen(tenFile, "r");
    if(f == NULL) 
    {
        printf("File open failed!\n");
        exit(1);
    }
    fscanf(f, "%d %d\n", &n, &m);
    for (i = 0; i < m; i++)
    {
        fscanf(f, "%d %d %d\n", &id1, &id2, &w);
        addVertex(g, id1, "1");
        addVertex(g, id2, "1");
        addEdge(g, id1, id2, (double)w);
        if(w >= 50)
        {
            addVertex(gDiBo, id1, "1");
            addVertex(gDiBo, id2, "1");
            addEdge(gDiBo, id1, id2, (double)w);
        }
    }
    return n;
    
}

int main()
{
    Graph g = createGraph(UNDIRECT_GRAPH);
    Graph gDiBo = createGraph(UNDIRECT_GRAPH);
    int soDinh = docFile(g, gDiBo, "dothi.txt");
    JRB node, node1, node2;
    JRB tree, tree1, tree2;
    JRB nodeKhac;
    int id, idKhac;
    int id1, id2;
    int mark = 0;
    int input[100];
    int max = 0;
    int s, t;
    double w;
    int length;
    int path[100];
    int path2[200];
    int length2;
    int i;


    int thoat, luaChon;
    thoat = 0;
    do
    {
        luaChon = menuVaLuaChon(1, 6);
        switch (luaChon)
        {
        case 1:
            //ham1
            docFile(g, gDiBo, "dothi.txt");
            printf("%5s", " ");
            jrb_traverse(node, g.edges)
            {
                printf("%-5d", jval_i(node->key));
            }
            printf("\n");
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                printf("%-5d", id);
                node = jrb_find_int(g.edges, id);
                tree = (JRB) jval_v(node->val);
                jrb_traverse(node1, g.edges)
                {
                    idKhac = jval_i(node1->key);
                    nodeKhac = jrb_find_int(tree, idKhac);
                    if(nodeKhac == NULL)
                        printf("%-5d", 0);
                    else printf("%-5d", 1);
                }
                printf("\n");
            }
            break;
        case 2:
            //ham2
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                printf("Castle %d: ", id);
                node = jrb_find_int(g.edges, id);
                tree = (JRB) jval_v(node->val);
                jrb_traverse(node1, tree)
                {
                    printf("%d  ", jval_i(node1->key));
                }
                printf("\n");
            }
            break;
        case 3:
            //ham3
            printf("Cac thanh tri chi den duoc bang cach di bo: ");
            jrb_traverse(node, g.edges)
            {
                mark = 0;
                id = jval_i(node->key);
                node = jrb_find_int(g.edges, id);
                tree = (JRB) jval_v(node->val);
                jrb_traverse(node1, tree)
                {
                    if(jval_d(node1->val) < 50) mark = 1;
                }
                if(mark == 0) printf("%d ", id);
            }
            printf("\nCac thanh tri co ban bac vao lon nhat: ");
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                if(max < indegree(g, id, input)) max = indegree(g, id, input);
            }
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                if(max == indegree(g, id, input)) 
                    printf("%d ", id);
            }
            break;
        case 4:
            //ham3
            printf("Nhap thanh tri xuat phat: ");
            scanf("%d", &s);
            printf("Nhap thanh tri dich: ");
            scanf("%d", &t);
            if(jrb_find_int(g.edges, s) == NULL || jrb_find_int(g.edges, t) == NULL)
            {
                printf("Id khong hop le!\n");
                break;
            }
            w = Dijkstra(g, s, t, path, &length);
            if(w == INFINITIVE_VALUE)  printf("ROUTE NOT FOUND\n");
            else 
            {
                printf("Do dai: %d\n ", (int)w);
                printf("%d", path[0]);
                for ( i = 1; i < length; i++)
                {
                    printf(" => %d", path[i]);
                }
            }
            break;
        case 5:
            //ham3
            printf("Nhap thanh tri xuat phat: ");
            scanf("%d", &s);
            printf("Nhap thanh tri dich: ");
            scanf("%d", &t);
            if(jrb_find_int(gDiBo.edges, s) == NULL || jrb_find_int(gDiBo.edges, t) == NULL)
            {
                printf("Id khong hop le or id nay ko co duong di bo!\n");
                break;
            }
            w = Dijkstra(gDiBo, s, t, path2, &length2);
            if(w == INFINITIVE_VALUE)  printf("ROUTE NOT FOUND\n");
            else 
            {
                printf("Do dai: %d\n ", (int)w);
                printf("%d", path2[0]);
                for ( i = 1; i < length2; i++)
                {
                    printf(" => %d", path2[i]);
                }
            }
            break;
        case 6:
            dropGraph(g);
            thoat = 1;
            break;
        }
    } while (thoat != 1);
}
