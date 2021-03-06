#include <stdio.h>
#include "week4/jrb.h"
#include "week4/jval.h"
#include "week4/dllist.h"
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
/*
int DAG(Graph graph)
{
  int visited[1000] = {};
  int start;
   int n, output[100], i, u, v;
   Dllist node, stack;
    JRB node2;
  jrb_traverse(node2, graph.vertices)
    {
      start=jval_i(node2->key);
      stack = new_dllist();
      dll_append(stack, new_jval_i(start));
      while ( !dll_empty(stack) )
	{
	  node = dll_last(stack);
	  u = jval_i(node->val);
	  dll_delete_node(node);
	  if (!visited[u])
	    {
	      visited[u] = 1;
	      if (u == start) return 0;
	      n = getAdjacentVertices(graph.edges, u, output);
	      for (i=0; i<n; i++)
		{
		  v = output[i];
		  if (!visited[v])
		    dll_append(stack, new_jval_i(v));
		}
	    }
	}
    }
  return 1;
}
*/

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

int main()
{
  Graph g = createGraph();
  addVertex(g, 0, "V0");
  addVertex(g, 1, "V1");
  addVertex(g, 2, "V2");
  addVertex(g, 3, "V3");
  addEdge(g, 0, 1);
  addEdge(g, 1, 2);
  addEdge(g, 2, 0);
  addEdge(g, 1, 3);
  if (DAG(g))
    printf("The graph is acycle\n");
  else
    printf("Have cycles in the graph\n");
}
