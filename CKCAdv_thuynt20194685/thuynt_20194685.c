#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"

int docFileUser(Graph g, char* tenFile)
{
    FILE *f;
    f = fopen(tenFile, "r");
    if(f == NULL)
    {
        printf("File open failed!\n");
        exit(1);
    }

    int id;
    char name[26];
    int n, i;
    fscanf(f, "%d\n", &n);
    for ( i = 0; i < n; i++)
    {
        fscanf(f, "%d %s", &id, name);
        addVertex(g, id, name);
    }
    return n;
    
}

void docFileFriend(Graph g, char* tenFile, int n)
{
    FILE *f;
    f = fopen(tenFile, "r");
    if(f == NULL)
    {
        printf("File open failed!\n");
        exit(1);
    }

    int id1, id2;
    const char s[2] = " ";
    char *token;
    char t[100];
    for (int i = 0; i < n; i++)
    {
      fscanf(f, "%d%*c", &id1);
      fgets(t, 100, f);
      if (strlen(t) == 0 || strlen(t) == 1)
        continue;
      token = strtok(t, s);
      while (token != NULL)
      {
        id2 = atoi(token);
        addEdge(g, id1, id2, 1.0);
        token = strtok(NULL, s);
      }
    }

}

int getIDbyName(Graph g, char name[])
{
    JRB node;
    jrb_traverse(node, g.vertices)
    {
        if(strcmp(jval_s(node->val), name) == 0) return jval_i(node->key);
    }
    return -1;
}

char* nameByID(Graph g, int id)
{
    JRB node;
    jrb_traverse(node, g.vertices)
    {
        if(id == jval_i(node->key)) return jval_s(node->val);
    }
    return "-1";
}

int main()
{
    Graph g = createGraph(UNDIRECT_GRAPH);
    JRB node, node1, node2;
    JRB tree, tree2;
    int id, id1, id2;
    float max = 0;
    char name[26];
    char name1[26], name2[26];
    int length, path[100];
    int s, t, w;
    int count;
    int banChung[20];

    int n = docFileUser(g, "users.txt");
    docFileFriend(g, "friends.txt", n);

    int menu;
    char c;
    int mark5, mark2;
    do
    {

        printf("\n=======MENU======\n");
        printf("1. Input Data\n");
        printf("2. List friend\n");
        printf("3. Popular person\n");
        printf("4. Check friend\n");
        printf("5. Suggest friend\n");
        printf("6. Exit\n");
        printf("---------------------\n");
        do
        {
            __fpurge(stdin);
            menu = 0;
            printf("Nhap lua chon cua ban: ");
            scanf("%d%c", &menu, &c);
            if (menu < 1 || menu > 6 || c != '\n')
                printf("Khong dung dinh dang! Vui long nhap lai!!\n");
        } while (menu < 1 || menu > 6 || c != '\n');

        switch (menu)
        {
        case 1:
            n = docFileUser(g, "users.txt");
            docFileFriend(g, "friends.txt", n);
            printf("%d\n", n);
            jrb_traverse(node, g.vertices)
            {
                printf("%d %s\n", jval_i(node->key), jval_s(node->val));
            }

            jrb_traverse(node, g.edges)
            {
                id = jval_i(node->key);
                printf("%d ", id);
                node = jrb_find_int(g.edges, id);
                tree = (JRB) jval_v(node->val);
                jrb_traverse(node1, tree)
                {
                    printf("%d  ", jval_i(node1->key));
                }
                printf("\n");
            }
            printf("submitted at 7:57\n");
            break;
        case 2:
            mark2 = 0;
            printf("Nhap ten: ");
            scanf("%s", name);
            id = getIDbyName(g, name);
            if(id == -1) 
            {
                printf("Khong ton tai nguoi nay!\n");
                break;
            }
            jrb_traverse(node, g.edges)
            {
                id1 = jval_i(node->key);

                if (id == id1)
                {
                    mark2 = 1;
                    node = jrb_find_int(g.edges, id1);
                    tree = (JRB)jval_v(node->val);
                    
                        jrb_traverse(node1, tree)
                        {
                            printf("%s\n", nameByID(g, jval_i(node1->key)));
                        }
                    
                }
            }
            if(mark2 == 0) printf("Khong co ban!\n");

                printf("\n");

            break;
        case 3:
            printf("Em chua lam duoc a.\n");
            break;
        case 4:
            printf("Nhap ten ng1: ");
            scanf("%s", name1);
            printf("Nhap ten ng2: ");
            scanf("%s", name2);
            id1 = getIDbyName(g, name1);
            id2 = getIDbyName(g, name2);
            if(id1 == -1 || id2 == -1 || id1 == id2)
            {
                printf("Khong ton tai it nhat 1 nguoi, hoac hai nguoi trung nhau!\n");
                break;
            }
            if(hasEdge(g, id1, id2)) printf("Hai nguoi la ban!\n");
            else printf("Chua ket ban!\n");

            break;
        case 5:
            mark5 = 0;
            printf("Nhap ten: ");
            scanf("%s", name);
            id = getIDbyName(g, name);
            if(id == -1) 
            {
                printf("Khong ton tai nguoi nay!\n");
                break;
            }

            jrb_traverse(node, g.edges)
            {
                if(id == jval_i(node->key)) mark5 = 1;
            }
            if(mark5 == 0)
            {
                printf("Nguoi nay chua co ban, nen khong co goi y!\n");
                break;
            }

            printf("Goi y ket ban: \n");
            printf("- Co ban chung: ");
            jrb_traverse(node, g.vertices)
            {
                t = jval_i(node->key);
                if(id == t) continue;
                w = Dijkstra(g, id, t, path, &length);
                if (w != INFINITIVE_VALUE && hasEdge(g, id, t) == 0)
                {
                    if(w == 2)
                        printf("%s ", nameByID(g, t));
                }
            }
            printf("\n");

            printf("- Co cac moi quan he ban be gian tiep: ");
            jrb_traverse(node, g.vertices)
            {
                t = jval_i(node->key);
                if(id == t) continue;
                w = Dijkstra(g, id, t, path, &length);
                if (w != INFINITIVE_VALUE && hasEdge(g, id, t) == 0)
                {
                        printf("%s ", nameByID(g, t));
                }
            }
            printf("\n");
            break;
        case 6:
            dropGraph(g);
            break;
        }
    } while (menu != 6);
    return 0;
}
