#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct node{
	int vertex_number;
	char name[3];
	int visit_flag;
	struct node *next; //*Pointer to next node
};
typedef struct node node;

void new_edge(node *vertex, int vertex_number, char **names){ //adds new node to the adjacency list
	node *new_node = (struct node *)(malloc(sizeof(struct node)));	
		new_node->vertex_number = vertex_number;
		strcpy(new_node->name, names[vertex_number]);
		if (vertex->next != NULL)
			new_node->next = vertex->next;
		else
			new_node->next = NULL;

		vertex->next = new_node;	
}

void traverse_adj_list(node *graph, int no_vertices){
	printf("Adjacency List\n");
	for (int i = 0; i < no_vertices; i++){ //loop to traverse all vertices in the vertices array
		node *temp = &graph[i];
		printf("%s --> ", temp->name);
		if (temp->next != NULL){
			temp = temp->next;
			while (temp != NULL){ //loop to traverse its adjacency list
				printf("%s", temp->name);
				if (temp->next != NULL){
					printf(", ");
				}
				temp = temp->next;
			}
		}
		printf("\n");
	}
	printf("\n");
}

void deadlock(node *graph, int no_processes, int no_vertices){
	node *process_vertex, *next;
	int counter = 0,deadlocked_processes[no_processes];	 

	for (int i = 0; i < no_processes; i++){ // dfs traversal for each process vertex				
		process_vertex = &graph[i];
		printf("%s-->", process_vertex->name);
		process_vertex->visit_flag = 1;
		next = process_vertex->next;
		if (next != 0){
			printf("%s-->", next->name);
		}else{
			continue;//if process does not have any resource requests 
		}		
		while (next != 0){
			next = &graph[next->vertex_number]; //move to the adjacency list of that vertex using vertex_number as array index
			next = next->next; //move to the element in the adjacency list
			if (next == 0){
				break; //end of path 
			}
			printf("%s-->", next->name);
			if (graph[next->vertex_number].visit_flag == 1 && next->vertex_number != process_vertex->vertex_number){
				break;// in case there is another loop in the path which does not return to the starting vertex
			}
			if (graph[next->vertex_number].visit_flag == 1){
				deadlocked_processes[counter] = process_vertex->vertex_number;
				counter++;//if the last vertex in the path is equal to the first process vertex-->deadlocked process
				break;
			}
			graph[next->vertex_number].visit_flag = 1;
		}
		printf("\n");
		for (int w = 0; w < no_vertices; w++){
			graph[w].visit_flag = 0; //reset visit flag 
		}
	}
	if (counter == 0){ //no deadlocked process
		printf("\nNo Deadlocked Process\n");
		return;
	}
	printf("\nDeadlocked Processes: ");
	for (int t = 0; t < counter; t++){ //display the deadlocked process in the array
		printf("%s ", graph[deadlocked_processes[t]].name);
	}
	printf("\n\n");
}

int resource_count(char buffer[150]){ //counts the number of unique resources in the file
	char *copy = malloc(sizeof(char) * 150);
	int count = 0;	
	strcpy(copy, buffer);
	for (int i = 0; i < strlen(copy); i++){
		for (int j = i + 1; copy[j] != '\0'; j++){
			if (copy[j] == copy[i]){
				for (int k = j; copy[k] != '\0'; k++){
					copy[k] = copy[k + 1];
				}
			}
		}
		if (isdigit(copy[i])){
			count++;
		}
	}
	return count;
}

int main(){
	int fd,flag = 1,no_processes = 1;
	fd = open("process.txt", O_RDONLY);
	char buffer[150],ch[2];
		
	while (flag > 0){ //read file
		int i = 0;
		while (1){
			flag = read(fd, ch, 1);
			if (flag <= 0){//end of file
				break;
			}
			if (ch[0] == '\n'){
				no_processes++; //increments no of process for each end of line in file
				strncat(buffer, ":\n", 3);
				continue;
			}
			if (ch[0] == 'P'){// no need to store the  process in string
				flag = read(fd, ch, 1);//skip
				flag = read(fd, ch, 1);// skip 
				continue;
			}
			if (i == 0)	{ 
				strncpy(buffer, ch, 1);
				i++;
			}else{
				strncat(buffer, ch, 1);
			}
		}
	} 
	// 
	if (buffer[strlen(buffer) - 2] == ':') 		no_processes--; //in case there is an additional end of line at the end of the file
	
	int no_resources = resource_count(buffer);

	printf("\nString read from file -- \n%s\n\n", buffer);
	printf("Number of Processes: %d\nNumber of Resources: %d\n\n",no_processes,no_resources);
	int no_vertices = no_processes + no_resources;

	char *names[no_vertices],v_name[3];	

	for (int i = 0; i <= no_processes; i++){// assign process name to the names array
		sprintf(v_name, "P%d", i + 1);
		names[i] = strdup(v_name);
		memset(v_name, 0, strlen(v_name));
	}

	int z = 1;
	for (int i = no_processes; i < no_vertices; i++){// assign resource name to the names array
		sprintf(v_name, "R%d", z);
		names[i] = strdup(v_name);//returns pointer of string
		memset(v_name, 0, strlen(v_name));//empty string
		z++;
	}

	node graph[no_vertices]; //array of nodes for each vertex in the graph

	for (int i = 0; i < no_vertices; i++){
		graph[i].vertex_number = i;
		graph[i].visit_flag = 0;
		strcpy(graph[i].name, names[i]);
		graph[i].next = NULL;
	}

	int process_vertex = 0,swap=0;	
	for (int k = 0; k < strlen(buffer); k++){
		if (buffer[k] == ')'){
			swap = 1; // resource to process
		}
		if (buffer[k] == ':'){ 
			process_vertex++;
			swap = 0; //process to resource
		}
		if (isdigit(buffer[k]) && swap == 0){
			int x = buffer[k] + no_processes - 1 - '0'; //convert char to int (-1 and + no_process to convert to respective array index)
			new_edge(&graph[x], process_vertex, names);
		}
		if (isdigit(buffer[k]) && swap == 1){
			int x = buffer[k] + no_processes - 1 - '0';
			new_edge(&graph[process_vertex], x, names);
		}
	}
	
	traverse_adj_list(graph, no_vertices);
	printf("Modified Depth First Search Traversal Path\n");	
	deadlock(graph, no_processes, no_vertices);

	return 0;
}