//Alex Monahan
//Section Leader: Annie S.
//Source: Week 9, Section 8 handout aided in the development of this lab
//My extension implements the depth first search as bidirectional!
//You can switch my code back to using my regular depthHelper method if you DO NOT want the extension
//when you are testing!

#include "trailblazer.h"
#include "stack.h"
#include "queue.h"
#include "pqueue.h"
#include "set.h"

using namespace std;

/* My depth first search. I implemented this method with a stack, as shown in class. We
 start by reseting the path. I then check if the start and end Vertex are the same, and, if so,
I simply return this path. Otherwise, I create my stack, push on the firstPath, and call the
helper method.*/

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    start->setColor(GREEN);
    Vector<Vertex*> path;
    if(start == end){ //path of one
        path.add(start);
        return path;
    }

    Stack<Vector<Vertex*> > stack; //our stack
    Stack<Vector<Vertex*> > backStack; //our stack

    Vector<Vertex* > firstPath;
    Vector<Vertex* > backPath;

    firstPath.add(start);
    backPath.add(end);
    end->visited = true;
    backStack.push(backPath);
    start->visited = true; //set as visited
    stack.push(firstPath);

    //path = depthHelper(graph, end, stack); // helper method

    path = depthHelperExtension(graph, start, end, stack, backStack);
    return path;

}

Vector<Vertex*> depthHelperExtension(BasicGraph& graph, Vertex* start, Vertex* end, Stack<Vector<Vertex*> > & stack, Stack<Vector<Vertex*> > & backStack){
    Vertex* back = end;
    Vertex* front = start;
     Vector<Vertex* > currPath;
     Vector<Vertex* > backPath;
    while(true){
        if(stack.isEmpty() && backStack.isEmpty()) break;
        if(!stack.isEmpty()){
            currPath = stack.pop();
            Vertex* next = currPath[currPath.size()-1];

            next->setColor(GREEN);

            front = next;
            for(Edge* edge : graph.getEdgeSet(next)){
                 Vertex* neighbor = edge->finish;

                 Vector<Vertex* > newPath = currPath;
                 if(neighbor->visited == false){
                     if(neighbor->getColor() == GREEN ){

                        Vector<Vertex* > path;
                         for(Vertex* v: graph.getVertexSet()){
                             if(v->getColor() == GREEN){
                                 v->visited=true;
                             }
                         }
                         //currPath += backPath;
                         return currPath;
                     }
                    newPath.add(neighbor); //add neighbor to the path
                    neighbor->visited = true;
                    stack.push(newPath);
                 }
            }
        }

        if(!backStack.isEmpty()){
            backPath = backStack.pop();
            Vertex* next = backPath[backPath.size()-1];
            next->setColor(GREEN);
            back=next;
            for(Edge* edge : graph.getEdgeSet(next)){
                 Vertex* neighbor = edge->finish;
                 Vector<Vertex* > newPath = currPath;
                 if(neighbor->getColor() != GREEN){
                    newPath.add(neighbor); //add neighbor to the path
                    //neighbor->visited = true;
                    backStack.push(newPath);
                 }
            }
        }

    }

    //if we have not returned anything at this point, we return an empty path
    Vector<Vertex*> emptyPath;
    return emptyPath;
}

/* The helper method used for depth first search. If the stack is empty, we return
 an empty vector as there is no path. Otherwise, we pop() off the current path, set
the final point in this vector to green, find its neighbors using a similar algorithm
as described in the Section 8 handout, and then check what colors need to be changed
to GRAY with the giveUp method*/
Vector<Vertex*> depthHelper(BasicGraph& graph, Vertex* end, Stack<Vector<Vertex*> > & stack){
    while(!stack.isEmpty()){
        Vector<Vertex* > currPath = stack.pop();
        Vertex* next = currPath[currPath.size()-1];
        next->setColor(GREEN);
        if(next == end) return currPath;
        for(Edge* edge : graph.getEdgeSet(next)){
             Vertex* neighbor = edge->finish;
             Vector<Vertex* > newPath = currPath;
             if(neighbor->visited == false){
                newPath.add(neighbor); //add neighbor to the path
                neighbor->visited = true;
                stack.push(newPath);
             }
        }

        giveUp(graph, stack); //determines if the vertex colors should be changed to gray
    }

    //if we have not returned anything at this point, we return an empty path
    Vector<Vertex*> emptyPath;
    return emptyPath;
}

/*A helper method to determine in depth first search if we are giving up on a vertex. First,
we get all possible vertexes that are left in any path in the stack. Then, if there are any
verticies on the graph that are green but are NOT left in the stack, we know we are giving up
on them, and we change them to the color of gray.*/
void giveUp(BasicGraph& graph, Stack<Vector<Vertex*> > & stack){
    Stack<Vector<Vertex*> > stackCopy = stack;
    Set<Vertex* > mySet;

    while(!stackCopy.isEmpty()){
        Vector<Vertex*> vec = stackCopy.pop();
        for(Vertex* next : vec){
            mySet.add(next);
        }
    }
    for(Vertex* allV : graph.getVertexSet()){
        if(allV->getColor() == GREEN){
            if(!mySet.contains(allV)) allV->setColor(GRAY); //set given up vertices to GRAY
        }
    }


}

/*A helper method for breadth first search that changes the path to the solution to green*/
void changeColorsBreadth(Vector<Vertex*>& solution){
    for(Vertex* change : solution){
        change->setColor(GREEN);
    }
}


/*My breathFirstSearch, implemented with a queue, as described in class. Colors are turned
yellow once the vertex is enqueued.*/
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {

    graph.resetData(); //reset data in the beginning

    start->setColor(YELLOW);

    Vector<Vertex*> path;
    if(start == end){
        path.add(start);
        return path;
    }

    Queue<Vector<Vertex*> > queue;
    Vector<Vertex* > firstPath;
    firstPath.add(start);
    start->visited = true; //set the path as visited
    queue.enqueue(firstPath);
    path = breadthHelper(graph, end, queue);
    return path;
}

/*My helper method for the breadth search. As we can see, we are using a queue. We set
all the enqueued neighbors to have a color of YELLOW if they have not changed their color already.*/
Vector<Vertex*> breadthHelper(BasicGraph& graph, Vertex* end, Queue<Vector<Vertex*> > & queue){
    while(!queue.isEmpty()){
        Vector<Vertex* > currPath = queue.dequeue();
        Vertex* next = currPath[currPath.size()-1];
        next->visited = true;
        next->setColor(GREEN);


        if(next == end){
            //changeColorsBreadth(currPath);
            return currPath;
        }
        for(Edge* edge : graph.getEdgeSet(next)){
             Vertex* neighbor = edge->finish;
             Vector<Vertex* > newPath = currPath;
             if(neighbor->visited == false){
                newPath.add(neighbor);
                neighbor->visited = true;
                neighbor->setColor(YELLOW);
                queue.enqueue(newPath);
             }
        }
    }

    Vector<Vertex*> emptyPath;
    return emptyPath;
}


/* My dijkstra algorithm. I reset the data initially. It is very similar to my prior
 breath first search, as I am using a PriorityQueue. I also call a helper method here.*/
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {    
    graph.resetData(); //reset data in the beginning

    start->setColor(YELLOW);
    start->cost = 0;

    Vector<Vertex*> path;
    if(start == end){ //start = end in this case
        path.add(start);
        return path;
    }
//set up for djikstra
    PriorityQueue<Vector<Vertex*> > queue; //the PriorityQueue
    Vector<Vertex* > firstPath;
    firstPath.add(start);
    start->visited = true;
    queue.enqueue(firstPath, 0);
    path = dHelper(graph, end, queue); //helper method
    return path;

}

/*The helper method. Note -- almost exactly the same as the past breadth search,
but now I include the priority by calculating the cost of the path with getCost.*/
Vector<Vertex*> dHelper(BasicGraph& graph, Vertex* end, PriorityQueue<Vector<Vertex*> > & queue){
    while(!queue.isEmpty()){
        Vector<Vertex* > currPath = queue.dequeue();
        Vertex* next = currPath[currPath.size()-1];
        next->setColor(GREEN);

        if(next == end){
           // changeColorsBreadth(currPath);
            return currPath;
        }
        for(Edge* edge : graph.getEdgeSet(next)){
             Vertex* neighbor = edge->finish;
             neighbor->previous = next;
             Vector<Vertex* > newPath = currPath;
             newPath.add(neighbor);
             if(neighbor->visited == false){
                neighbor->visited = true;
                neighbor->setColor(YELLOW); //we want to change the color of the given vertex
                neighbor->cost = getCost(graph, newPath);
                queue.enqueue(newPath, getCost(graph, newPath)); //enqueue according to cost
             } else{
                 //we need to check if a more efficient path can be formed now
                    PriorityQueue<Vector<Vertex*> > copyq;
                    while(!queue.isEmpty()){
                        Vector<Vertex*> curr = queue.dequeue();
                        if(curr[curr.size()-1] == neighbor && getCost(graph, curr) > getCost(graph, newPath)){
                            double cost = getCost(graph, newPath); //the cost
                            neighbor->cost = getCost(graph, newPath);
                            copyq.enqueue(newPath, getCost(graph, newPath));
                        } else {
                            copyq.enqueue(curr, getCost(graph, curr));
                        }
                    }
                    queue = copyq;
             }
        }
     }

    Vector<Vertex*> emptyPath; //if no paths, we return an empty set
    return emptyPath;
}

/*Looks at all of the edges to find the cost of specific verticies.*/
double getCost(BasicGraph& graph, Vector<Vertex* > & currPath){
    double cost = 0;
    int num = currPath.size() - 1;
    for(int i = 0; i < num; i++){
        Edge * edge = graph.getEdge(currPath[i], currPath[i+1]);
        double edgeCost = edge->cost;
        cost += edgeCost;
    }

    return cost; //return the final cost

}

/*My a-star method. It is very similar to the previous method, but we add
in the necessary heuristic function.*/
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //reset data in the beginning

    start->setColor(YELLOW); //set the color to Yellow
    end->setColor(YELLOW);
    Vector<Vertex*> path;
    if(start == end){ //if the path starts and ends at the same place
        path.add(start);
        return path;
    }

    PriorityQueue<Vector<Vertex*> > queue; //the PriorityQueue
    PriorityQueue<Vector<Vertex*> > backQueue; //the PriorityQueue

    Vector<Vertex* > firstPath;
    Vector<Vertex* > lastPath;

    firstPath.add(start);
    lastPath.add(end);
    //end->visited = true;
    start->visited = true; //set the visited to true
    queue.enqueue(firstPath, heuristicFunction(start, end)); //we add in the heurisitic function
    backQueue.enqueue(lastPath, heuristicFunction(end, start)); //we add in the heurisitic function
    //path = aHelperExtension(graph, end, queue, backQueue, start);
    path = aHelper(graph, end, queue); //helper method
    return path;
}

/*Vector<Vertex*> aHelperExtension(BasicGraph& graph, Vertex* end, PriorityQueue<Vector<Vertex*> > & queue, PriorityQueue<Vector<Vertex*> > & backQueue, Vertex * start){
    while(start != end){
        Vertex* back;
        Vertex* next;
        Vertex * temp;
        Vector<Vertex* > currPath;
        Vector<Vertex* > backPath;
        if(!queue.isEmpty()){
            currPath = queue.dequeue();
            next = currPath[currPath.size()-1];
            temp = next;
            next->setColor(GREEN);
            start = temp;

        }

        if(!backQueue.isEmpty()){
            backPath = backQueue.dequeue();
            back = backPath[backPath.size()-1];
            back->setColor(GREEN);
            end = back;
        }
        if(back == next){
            Vector<Vertex* > pathCombo = currPath;
            for(int i = 1; i < backPath.size(); i++){
                pathCombo.add(backPath[i]);
            }
            return pathCombo;
        }

        if(!queue.isEmpty()){
            for(Edge* edge : graph.getEdgeSet(next)){
                 Vertex* neighbor = edge->finish;
                 neighbor->previous = next;
                 Vector<Vertex* > newPath = currPath;
                 newPath.add(neighbor);
                 if(neighbor->visited == false){
                    neighbor->visited = true;
                    neighbor->setColor(YELLOW); //we want to change the color of the given vertex
                    neighbor->cost = getCost(graph, newPath);
                    queue.enqueue(newPath, getCost(graph, newPath) + heuristicFunction(neighbor, back)); //enqueue according to cost

                 } else{
                        PriorityQueue<Vector<Vertex*> > copyq;
                        while(!queue.isEmpty()){
                            Vector<Vertex*> curr = queue.dequeue();
                            if(curr[curr.size()-1] == neighbor && getCost(graph, curr) > getCost(graph, newPath)){
                                double cost = getCost(graph, newPath); //the cost
                                 neighbor->cost = getCost(graph, newPath);
                                copyq.enqueue(newPath, getCost(graph, newPath) + heuristicFunction(neighbor, back));
                            } else {
                                copyq.enqueue(curr, getCost(graph, curr) + heuristicFunction(curr[(curr.size() - 1)], back));
                            }
                        }
                        queue = copyq;
                 }
            }
        }



        /*for(Edge* edge : graph.getEdgeSet(back)){
             Vertex* neighbor = edge->finish;
             //neighbor->previous = next;
             Vector<Vertex* > newPath = currPath;
             newPath.add(neighbor);
             if(neighbor->visited == false){
                neighbor->visited = true;
                neighbor->setColor(YELLOW); //we want to change the color of the given vertex
                neighbor->cost = getCost(graph, newPath);
                backQueue.enqueue(newPath, getCost(graph, newPath) + heuristicFunction(temp, neighbor)); //enqueue according to cost
             } else{
                    PriorityQueue<Vector<Vertex*> > copyq;
                    while(!backQueue.isEmpty()){
                        Vector<Vertex*> curr = backQueue.dequeue();
                        if(curr[curr.size()-1] == neighbor && getCost(graph, curr) < getCost(graph, newPath)){
                            double cost = getCost(graph, newPath); //the cos
                             neighbor->cost = getCost(graph, newPath);
                            copyq.enqueue(newPath, getCost(graph, newPath) + heuristicFunction(temp, neighbor));
                        } else {
                            copyq.enqueue(curr, getCost(graph, curr) + heuristicFunction(temp, curr[(curr.size() - 1)]));
                        }
                    }
                    backQueue = copyq;
             }
        }

        if(!backQueue.isEmpty()){
            for(Edge* edge : graph.getEdgeSet(back)){
                 Vertex* neighbor = edge->finish;
                 Vector<Vertex* > newPath = backPath;
                 if(neighbor->visited == false){
                    newPath.add(neighbor);
                    neighbor->visited = true;
                    neighbor->setColor(YELLOW);
                    backQueue.enqueue(newPath, heuristicFunction(temp, neighbor));
                 }
            }
        }



    }

    Vector<Vertex*> emptyPath; //if no paths, we return an empty set
    return emptyPath;

}
   */


//The A* algorithm is always more efficient than my djikstra method. This is the helper method
//Note -- it is very similar to my djikstra method.
Vector<Vertex*> aHelper(BasicGraph& graph, Vertex* end, PriorityQueue<Vector<Vertex*> > & queue){



    while(!queue.isEmpty()){
        Vector<Vertex* > currPath = queue.dequeue();
        Vertex* next = currPath[currPath.size()-1];
        next->setColor(GREEN);
        if(next == end){
            return currPath;
        }
        //bulk of the algorithm begins
        for(Edge* edge : graph.getEdgeSet(next)){
             Vertex* neighbor = edge->finish;
             neighbor->previous = next;
             Vector<Vertex* > newPath = currPath;
             newPath.add(neighbor);
             if(neighbor->visited == false){
                neighbor->visited = true;
                neighbor->setColor(YELLOW); //we want to change the color of the given vertex
                neighbor->cost = getCost(graph, newPath);
                queue.enqueue(newPath, getCost(graph, newPath) + heuristicFunction(neighbor, end)); //enqueue according to cost

             } else{
                    PriorityQueue<Vector<Vertex*> > copyq;
                    while(!queue.isEmpty()){
                        Vector<Vertex*> curr = queue.dequeue();
                        if(curr[curr.size()-1] == neighbor && getCost(graph, curr) > getCost(graph, newPath)){
                            double cost = getCost(graph, newPath); //the cost
                             neighbor->cost = getCost(graph, newPath);
                            copyq.enqueue(newPath, getCost(graph, newPath) + heuristicFunction(neighbor, end));
                        } else {
                            copyq.enqueue(curr, getCost(graph, curr) + heuristicFunction(curr[(curr.size() - 1)], end));
                        }
                    }
                    queue = copyq;
             }
        }
     }

    Vector<Vertex*> emptyPath; //if no paths, we return an empty set
    return emptyPath;


}

//Finds the minimum spanning tree. We use a priority queue and a vector with entires as sets of Vertex pointers to complete this method.
//We return the minimum spanning tree.
Set<Edge*> kruskal(BasicGraph& graph) {

    Vector<Set<Vertex* > > vecSet;
    PriorityQueue<Edge*> queue; // put edges and weights in a priority queue

    setUp(graph, vecSet, queue); //sets up the graph

    Set<Edge*> mst;
    Edge* currEdge;
    bool doNotAddToTree;

    while(!queue.isEmpty()){
        doNotAddToTree = false;
        currEdge = queue.dequeue();
        for (Set<Vertex*> currSet : vecSet) {
            //we must check if a set contains both the start and the end
           if (currSet.contains(currEdge->start) && currSet.contains(currEdge->finish)) doNotAddToTree = true;
        }
        if(!doNotAddToTree){
            //we merge the clusters in this scenario
            mergeClusters(getIndex(graph, vecSet, queue, currEdge->start),getIndex(graph, vecSet, queue, currEdge->finish), vecSet);
            mst.add(currEdge); //add the edge to the minimum spanning tree
        }
    }

    return mst;

}

//we find the first index with the Vertex from the edge and return this index
int getIndex(BasicGraph& graph, Vector<Set<Vertex* > > & vecSet, PriorityQueue<Edge*> & queue, Vertex * vertex){
    int size = vecSet.size();
    for(int i = 0; i < size; i++){
        if(vecSet[i].contains(vertex)) return i;
    }
    return size;
}

//We merge the two clusers together in the Vector of Sets of Vertex pointers
void mergeClusters(int index1, int index2, Vector<Set<Vertex* > > & vecSet){
    Set<Vertex* > temp = vecSet[index1];
    for(Vertex * vertex : temp){
        vecSet[index2].add(vertex);
    }
    vecSet.remove(index1);

}

//We set up the initial priority queue with every edge according to its cost and also the
//vector set, where the sets initially contain individual Vertexes
void setUp(BasicGraph& graph, Vector<Set<Vertex* > > & vecSet, PriorityQueue<Edge*> & queue){
    Set<Vertex*> singleSet;
    for(Vertex* single : graph.getVertexSet()){
        singleSet.add(single);
        vecSet.add(singleSet);
        singleSet.clear();
    }

    for (Edge* edge : graph.getEdgeSet()) { //add edges in the graph
        double cost = edge->cost; //cost of the edge
        queue.enqueue(edge, cost);
    }

}
