#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define QUEUE_SIZE 58229

bool graph[58229][58229];
short degree_vertex[58229];
//gcc t1.c  -mcmodel=medium
int landmarks[100];
int update_log_semaphore;
int all_pair_done;
int (*distance_pair)[58229];

void vertices_pair(){
    FILE *fp = fopen("path_to_find.txt","w");
    srand(time(NULL));
    for(int i=0;i<1000;i++){
        int r1 = rand() % 58229;
        int r2 = rand() % 58229;
        fprintf(fp,"%d %d\n",r1,r2);
    }
    return ;

}

void initQueue(int queue[QUEUE_SIZE], int* front, int* rear) {
    *front = -1;
    *rear = -1;
}

// Function to check if the queue is empty
bool isQueueEmpty(int front, int rear) {
    return front == -1 || front > rear;
}

// Function to enqueue an element
void enqueue(int queue[QUEUE_SIZE], int* rear, int element) {
    if (*rear < QUEUE_SIZE - 1) {
        queue[++(*rear)] = element;
    }
}

// Function to dequeue an element
int dequeue(int queue[QUEUE_SIZE], int* front) {
    if (!isQueueEmpty(*front, *front)) {
        return queue[(*front)++];
    }
    return -1;  // Queue is empty
}

int calculateDistance(int node, int num_nodes) {
    bool visited[num_nodes];
    memset(visited, false, sizeof(visited));

    int distance[num_nodes];
    memset(distance, -1, sizeof(distance));

    int queue[QUEUE_SIZE];
    int front = 0, rear = 0;

    // Initialize with the current node and set its distance to 0
    visited[node] = true;
    distance[node] = 0;
    queue[rear++] = node;

    // Continue BFS until the queue is empty
    while (front < rear) {
        int current = queue[front++];

        for (int neighbor = 0; neighbor < num_nodes; neighbor++) {
            if (graph[current][neighbor] && !visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                queue[rear++] = neighbor;

                // Check if the neighbor is a landmark
                for (int i = 0; i < 100; i++) {
                    if (landmarks[i] == neighbor) {
                        return landmarks[i];
                    }
                }
            }
        }
    }

    // If we reach here, there is no connection to any landmark
    return -1;
}


void find_landmarks(){
    int index=0;
    while(index<50){
        int mi=-1;int ind=-1;
        for(int i=0;i<58229;i++){
            if(mi<degree_vertex[i]){
                mi=degree_vertex[i];
                ind=i;
            }
        }
        landmarks[index]=ind;
        degree_vertex[ind]=-1;
        index++;
    }

    while(index<100){
        int r1 = rand() % 58229;
        if(degree_vertex[r1]!=-1){
            landmarks[index]=r1;
            degree_vertex[r1]=-1;
            index++;
        }
    }
    //for(int i=0;i<58229;i++){degree_vertex[i]=-1;}

    return ;

}

void assigned_landmark(){

    for (int i = 0; i < 58229; i++) {
        if (degree_vertex[i] != -1) {
           int nearest_land=calculateDistance(i,58229);
           degree_vertex[i]=nearest_land;

        }
    }





    FILE *fp = fopen("landmark.txt","w");
    for(int i=0;i<100;i++){
        int parent = landmarks[i];
        fprintf(fp,"%d ",parent);
        for(int j=0;j<58229;j++){
            if(degree_vertex[j]==parent){
                fprintf(fp,"%d ",j);
            }
        }
        printf("%d ",parent);
        fprintf(fp,"\n");
    }



}


void updateGraph() {

    // Generate a random probability to decide whether to add or remove an edge
    double probability = (double)rand() / RAND_MAX;

    if (probability < 0.8) {
        // Remove an edge
        // Select a random edge (node0, node1) and remove it from the graph
        // Log the removal in "update.log" with the timestamp
        // Handle synchronization to ensure correct logging
        int r1 = rand() % 58229;
        int r2 = rand() % 58229;
        if(graph[r1][r2]==0){return;}
        graph[r1][r2]=0;
        graph[r2][r1]=0;
        FILE *fp = fopen("update_graph.txt","a");
        fprintf(fp,"REMOVE %d %d \n",r1,r2);
        fclose(fp);


    } else {
        // Add an edge
        // Select a random pair of nodes (node0, node1) and add an edge if it doesn't exist
        // Log the addition in "update.log" with the timestamp
        // Handle synchronization to ensure correct logging
        int r1 = rand() % 58229;
        int r2 = rand() % 58229;
        if(graph[r1][r2]==1){return;}
        graph[r1][r2]=1;
        graph[r2][r1]=1;
        FILE *fp = fopen("udate_graph.txt","a");
        fprintf(fp,"ADD %d %d \n",r1,r2);
        fclose(fp);

    }
}


void *updateGraphThread(void *arg) {
    while (1) {
        printf("inside \n");
        while(update_log_semaphore==1);
        update_log_semaphore=1;
        // Call updateGraph() to perform edge updates
        updateGraph();
        // Add synchronization to prevent contention with other threads
        update_log_semaphore=0;
        if(all_pair_done==1){break;}
    }
    return NULL;
}



int calculateDistanceNodes(int node1, int node2, int num_nodes) {
    bool visited[num_nodes];
    memset(visited, false, sizeof(visited));

    int distance[num_nodes];
    memset(distance, -1, sizeof(distance));

    int queue[QUEUE_SIZE];
    int front = 0, rear = 0;

    visited[node1] = true;
    distance[node1] = 0;
    queue[rear++] = node1;

    while (front < rear) {
        int current = queue[front++];

        if (current == node2) {
            return distance[current];
        }

        for (int neighbor = 0; neighbor < num_nodes; neighbor++) {
            if (graph[current][neighbor] && !visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                queue[rear++] = neighbor;
            }
        }
    }

    return -1; // Node2 not reachable from node1
}



void findDistancesBetweenLandmarks(int step) {
    for (int i = 0; i < 100; i=i+step) {
        for (int j = 0; j < 100; j++) {
            if(distance_pair[landmarks[i]][landmarks[j]]!=0){continue;}
            int distance = calculateDistanceNodes(landmarks[i], landmarks[j], 58229);
            distance_pair[landmarks[i]][landmarks[j]]=distance;
            distance_pair[landmarks[j]][landmarks[i]]=distance;
        }
    }
}

void path_in_same_partition(int step){
    for(int i=0;i<58229;i=i+step){
        for(int j=0;j<58229;j++){
            if(distance_pair[i][j]!=0){continue;}
            int distance = calculateDistanceNodes(i, j, 58229);
            distance_pair[i][j]=distance;
            distance_pair[j][i]=distance;
        }
    }

}


void path_finder(void *arg){
    int r1 = rand();
    if(r1%2==0){
        path_in_same_partition(r1%7);
    }else{
        findDistancesBetweenLandmarks(r1%7);
    }
}




int main(){
    const char* filename = "Brightkite_edges.txt";

    // Open the file for reading
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    char line[1000];
    while(fgets(line,1000,file)!=NULL){
        int n1,n2;
        if (sscanf(line, "%d %d", &n1,& n2) == 2){
            graph[n1][n2]=1;
            graph[n2][n1]=1;
            degree_vertex[n1]++;
            degree_vertex[n2]++;
        }else{
            return 1;
        }
    }
    fclose(file);
    printf("read done");

    vertices_pair();
    find_landmarks();
    assigned_landmark();

    pthread_t updateThreads[5];
    pthread_t finderThreads[20];
    distance_pair = malloc(sizeof(int[58229][58229]));


    for (int i = 0; i < 5; i++) {
        if (pthread_create(&updateThreads[i], NULL, updateGraphThread, NULL) != 0) {
            perror("Error creating update thread");
            return 1;
        }
    }




    for (int i = 0; i < 10; i++) {
        if (pthread_create(&finderThreads[i], NULL, path_finder, NULL) != 0) {
            perror("Error creating path thread");
            return 1;
        }
    }

    all_pair_done=1;
    for (int i = 0; i < 5; i++) {
        pthread_join(updateThreads[i], NULL);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(finderThreads[i], NULL);
    }


all_pair_done=1;


    return 0;
}
