#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"

int docFileSanPham(Graph gSanPham, char* tenFile)
{
    int m;
    FILE *f;
    int id;
    char name[30];
    f = fopen(tenFile, "r");
    if(f == NULL)
    {
        printf("File open failed!\n");
        exit(1);
    }
    fscanf(f, "%d\n", &m);
    for (int i = 0; i < m; i++)
    {
        fscanf(f, "%s %d\n", name, &id);
        addVertex(gSanPham, id, name);
    }
    return m;
    
}

int bienDoi(int idKhoHang)
{
    return idKhoHang*100;
}
int bienDoiNguoc(int idKhoHangTrongG)
{
    return idKhoHangTrongG/100;
}

int docFileKhoHang(Graph gSanPham, Graph gKhoHang, Graph g, char *tenFile, int m)
{
    int n, e;
    int i, j;
    FILE *f;
    char name[30];
    int id;
    int id1, id2, w;
    int idSanPham, soLuong;
    f = fopen(tenFile, "r");
    if(f == NULL)
    {
        printf("File open failed!\n");
        exit(1);
    }
    fscanf(f, "%d\n", &n);
    for (i = 0; i < n; i++)
    {
        fscanf(f, "%s %d\n", name, &id );
        addVertex(gKhoHang, id, name);
        addVertex(g, bienDoi(id), name);
        for ( j = 0; j < m; j++)
        {
            fscanf(f, "%d %d\n", &idSanPham, &soLuong);
            addEdge(g, bienDoi(id), idSanPham, soLuong);
        }
        
    }
    fscanf(f, "%d\n", &e);
    for(i = 0; i < e; i++)
    {
        fscanf(f, "%d %d %d\n", &id1, &id2, &w);
        addEdge(gKhoHang, id1, id2, (double)w);
    }
    return n;
    
}

int main()
{

    Graph g = createGraph(UNDIRECT_GRAPH);
    Graph gSanPham = createGraph(UNDIRECT_GRAPH);
    Graph gKhoHang = createGraph(UNDIRECT_GRAPH);
    int m = docFileSanPham(gSanPham, "sanpham.txt");
    int n = docFileKhoHang(gSanPham, gKhoHang, g, "khohang.txt", m);
    JRB node, node1, node2;
    JRB tree, tree1, tree2;
    int id1, id2;
    int idKhoHang, idSanPham;
    int soLuong, idKhoHangduTru;
    int soHang, soHangDuTru;
    int menu;
    char c;
    do
    {

        printf("\n=======MENU======\n");
        printf("1. \n");
        printf("2. \n");
        printf("3. \n");
        printf("4. \n");
        printf("5. Thoat\n");
        printf("-----------------------------------------\n");
        do
        {
            __fpurge(stdin);
            menu = 0;
            printf("Nhap lua chon cua ban: ");
            scanf("%d%c", &menu, &c);
            if (menu < 1 || menu > 5 || c != '\n')
                printf("Khong dung dinh dang! Vui long nhap lai!!\n");
        } while (menu < 1 || menu > 5 || c != '\n');

        switch (menu)
        {
        case 1:
            printf("C-Advanced HK20182\n");
            break;
        case 2:
            jrb_traverse(node, gSanPham.vertices)
            {
                printf("%s %d\n", jval_s(node->val), jval_i(node->key));
            }
            break;
        case 3:
            printf("Nhap vao id1: ");
            scanf("%d", &id1);
            printf("Nhap vao id2: ");
            scanf("%d", &id2);
            node1 = jrb_find_int(gKhoHang.vertices, id1);
            node2 = jrb_find_int(gKhoHang.vertices, id2);
            if(node1 == NULL || node2 == NULL || node1 == node2)
            {
                printf("-1\n");
                break;
            }
            if(hasEdge(gKhoHang, id1, id2) == 0)
            {
                printf("-1\n");
                exit(1);
            }
            node1 = jrb_find_int(gKhoHang.edges, id1);
            tree1 = (JRB) jval_v(node1->val);
            node2 = jrb_find_int(tree1, id2);
            printf("%d km", (int)jval_d(node2->val));

            break;
        case 4:
            jrb_traverse(node, gKhoHang.vertices)
            {
                printf("%s\n", jval_s(node->val));
                idKhoHang = bienDoi(jval_i(node->key));
                node1 = jrb_find_int(g.edges, idKhoHang);
                tree1 = (JRB) jval_v(node->val);
                jrb_traverse(node2, tree1)
                {
                    printf("%s %d\n", getVertex(gSanPham, jval_i(node2->key)), (int)jval_d(node2->val));

                }
            }

            break;
        case 5:
            break;
        }
    } while (menu != 5);
}
