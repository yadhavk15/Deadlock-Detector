# Deadlock-Detector
A C program to detect potential deadlocks in a computer system using Depth-First Search algorithm to traverse the graph created from the process-resource allocation adjacency list.

#Problem Definition
In an operating system, a deadlock happens when a process enters a waiting state because a requested system resource is held by another waiting process, which in turn is waiting for another resource held by another waiting process. 

The program needs to  identify which process if any would be deadlocked process from a file which contains the processes and resources so as to simulate a resource allocation table found in Linux.

|  Process |  Resource Held |  Resource Requested |
| ------------ | ------------ | ------------ |
| P1 | H(R1,R3,R5) |  R(R2)   |
| P2 | H(R6)  | R(R5)   |
| P3  | H()   | R(R1)  |
| P4  | H(R4)   | R(R1)  |


#Solution Definition

##Struct node
- It contains the name of the vertex, the vertex_number which is its index in the array of vertices.
- visit_flag which indicates if the node has been visited during graph traversal 
- *next which is a pointer to the next node in its adjacency list.

##new_edge(node *vertex, int vertex_number, char **names)
- It creates an edge between the vertex(1st parameter) and another vertex specified by its vertex_number(2nd parameter).
- The new_node created in the function using the vertex_number is added to the front of the adjacency list of the vertex. 
- Array names contain all the names of the vertices in the graph.

##deadlock(node *graph, int no_processes, int no_vertices)
###Modified  Depth-first search algorithm where :
1. we start from a vertex(starting vertex) in the graph, mark it as visited.
2. move to the next vertex(x) in its adjacency list, mark it as visited.
3. If a vertex is marked as visited and is equal to the starting process vertex, the process is deadlocked as there is a cycle. Break the loop. Skip step 4,5,6.
4. If a vertex is marked as visited and is not equal to the starting vertex, then there are other loops in that path, we break the loop as we already covered the full path without deadlock. Skip step 5,6.
5. If next is null, break the loop as the end of the path is reached. Skip step 6.
6. move to that x’s adjacency list and go to step 1.
8. The first for loop iterates over all the processes(starting vertex) in the graph array(1st parameter).
9. The above modified DFS algorithm is run if the process’s adjacency list contains resources(indicating that the process is requesting a  resource). Else it means the process is not deadlocked since it is not requesting any resources.
10. The inner while loop iterates until it encounters a visited node or stops if the adjacency list has no more elements. It also prints all the nodes it traverses. 
11. A process is deadlocked if the conditions mentioned above(step 3) are fulfilled. Then that process’s vertex number is stored in the deadlocked_processes array.
12. The visit flag is reset after each traversal of a  process vertex in the graph array.
13. After the for loop stops, the deadlocked processes if any are printed before exiting the function.

##int main()
1. It reads the process file, separates each process record by a “:” and does not store the process characters(P1,P2…) for easier string manipulation, and counts the number of processes.
2. The number of processes, resources in the file are stored in the variables no_ processes and no_resources respectively.
3. The names of the respective processes and resources are then included in the names array. 
4. Then an array of nodes that represent the graph is initialized.
5. The edges are then created by reading the string read from the file and using the new_edge() function. The process and resource parameters are swapped when an “)” is encountered in the string. 
6. If a process is requesting a resource, the resource is put in the process’s adjacency list and if a resource is held by a process, the process is put in the resource’s adjacency list.
7. The traverse_adj_list(graph, no_vertices) is executed to display the final adjacency list.
8. The deadlock(graph, no_processes, no_vertices) is executed. It shows the paths taken to traverse the graph and shows the deadlocked processes in it.
