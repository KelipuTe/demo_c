#include "undirected_graph.c"

int main() {
  UndirectedGraph *p1Graph = InitUndirectedGraph();

  int arr1Edges[][2] = {{4, 5}, {4, 7}, {5, 1}, {5, 6}, {6, 7}, {0, 1}, {0, 5}, {1, 2}, {1, 6}, {2, 3}, {3, 6}, {3, 7}};
  int arr1EdgesLen = sizeof(arr1Edges) / sizeof(int[2]);
  SetEdge(p1Graph, arr1Edges, arr1EdgesLen);

  BuildMatrix(p1Graph);
  PrintUndirectedGraph(p1Graph);

  BreadthFirstSearch(p1Graph);
  DepthFirstSearch(p1Graph);
  return 0;
}
