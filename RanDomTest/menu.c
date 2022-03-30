#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"
#define Max_Str 100

void docFile(char* tenFile, Graph g, int* soDinh, int* soCanh)
{
    int m, n;
    int i, j;
    int id, id1, id2;
    double trongSo;
    char ten[Max_Str];
    FILE *f;
    f = fopen(tenFile, "r");
    if(f == NULL)
    {
        printf("File open failed!\n");
        return;
    }
    fscanf(f, "%d %d\n", &m, &n);
    for ( i = 0; i < m; i++)
    {
        fscanf(f, "%d %s\n", &id, ten);
        addVertex(g, id, ten);
    }
    for ( i = 0; i < n; i++)
    {
        fscanf(f, "%d %d %lf\n", &id1, &id2, &trongSo);
        addEdge(g, id1, id2, trongSo);

    }
    *soDinh = m;
    *soCanh = n;
}

int main()
{
    Graph g = createGraph(UNDIRECT_GRAPH);
    int soCanh, soDinh;
    docFile("dothi.txt", g, &soDinh, &soCanh);
    JRB node, node1, node2, node3, node4, nodeKhac;
    int id, idKhac;
    JRB tree, tree2;
    int max = 0;
    int id1, id2;
    char* ten[Max_Str];
    int length, path[100], s, t, w;
    int i, count;
    int input[100];
    int banBeChung[100];

    int menu;
    char c;
    do
    {

        printf("\n=======MENU======\n");
        printf("1. In danh sach dinh va ma tran ke\n");
        printf("2. In cac nut bac 2, va nut bac cao nhat kem id\n");
        printf("3. Tim duong ngan nhat\n");
        printf("4. Tim nut ke xa nhat\n");
        printf("5. Tim nut ke chung\n");
        printf("6. Kiem tra quan he bac cau\n");
        printf("7. Thoat\n");
        printf("---------------------------------------------------\n");
        do
        {
            __fpurge(stdin);
            menu = 0;
            printf("Nhap lua chon cua ban: ");
            scanf("%d%c", &menu, &c);
            if (menu < 1 || menu > 7 || c != '\n')
                printf("Khong dung dinh dang! Vui long nhap lai!!\n");
        } while (menu < 1 || menu > 7 || c != '\n');

        switch (menu)
        {
        case 1:
            docFile("dothi.txt", g, &soDinh, &soCanh);
            printf("Danh sach cac dinh kem id\n");
            jrb_traverse(node, g.vertices)
            {
                printf("%d %s\n", jval_i(node->key), jval_s(node->val));
            }

            printf("Ma tran ke\n");
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
            max = 0;
            printf("\nCac nut bac 2 kem ID: ");
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                if(indegree(g, id, input) == 2) 
                    printf("%s(%d) ", getVertex(g, id),id);
            }
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                if(max < indegree(g, id, input)) max = indegree(g, id, input);
            }
            printf("\nCac nut bac cao nhat (bac %d) kem ID: ", max);
            
            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                if(max == indegree(g, id, input)) 
                    printf("%s(%d) ", getVertex(g, id),id);
            }
            printf("\n");
            break;
        case 3:
            printf("Nhap nut xuat phat: ");
            scanf("%d", &s);
            printf("Nhap nut dich: ");
            scanf("%d", &t);
            if(jrb_find_int(g.edges, s) == NULL || jrb_find_int(g.edges, t) == NULL)
            {
                printf("Id khong hop le!\n");
                break;
            }
            w = Dijkstra(g, s, t, path, &length);
            if(w == INFINITIVE_VALUE)  printf("NOT FOUND\n");
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
        case 4:
            max = 0;
            printf("Nhap id: ");
            scanf("%d", &id);
            node = jrb_find_int(g.edges, id);
            tree = (JRB)jval_v(node->val);
            jrb_traverse(node, tree)
            {
                if (max < jval_d(node->val))
                    max = jval_d(node->val);
            }
            printf("%d\n", (int)max);
            jrb_traverse(node, tree)
            {
                if (max == jval_d(node->val))
                    printf("%d %s\n", jval_i(node->key), getVertex(g, jval_i(node->key)));
            }

            break;
        case 5:
            count = 0;
            printf("Nhap id1: ");
            scanf("%d", &id1);
            printf("Nhap id2: ");
            scanf("%d", &id2);
            node = jrb_find_int(g.edges, id1);
            node2 = jrb_find_int(g.edges, id2);
            if(node == NULL || node2 == NULL || node == node2)
            {
                printf("Khong ton tai or 2 dinh trung nhau\n");
                break;
            }
            tree = (JRB) jval_v(node->val);
            tree2 = (JRB) jval_v(node2->val);
            jrb_traverse(node3, tree)
            {
                id = jval_i(node3->key);
                jrb_traverse(node4, tree2)
                {
                    if(id == jval_i(node4 ->key)) 
                    {
                        banBeChung[count] = id;
                        count++;
                    }
                }
            }
            if (count == 0)
                printf("-5");
            else
            {
                printf("%d\n", count);
                for (int i = 0; i < count; i++)
                {
                    printf("%d %s\n", banBeChung[i], getVertex(g, banBeChung[i]));
                }
            }

            break;
        case 6:
            printf("Nhap id1: ");
            scanf("%d", &s);
            printf("Nhap id2: ");
            scanf("%d", &t);
            node = jrb_find_int(g.edges, s);
            node2 = jrb_find_int(g.edges, t);
            if(node == NULL || node2 == NULL || node == node2)
            {
                printf("-1\n");
                break;
            }
            w=Dijkstra(g, s, t, path, &length);
            if(w != INFINITIVE_VALUE && hasEdge(g, s, t) == 0)
            {
                printf("5\n");
                break;
            }
            printf("-5\n");

            break;
        case 7:
            dropGraph(g);
            break;
        }
    } while (menu != 7);
    return 0;
}
