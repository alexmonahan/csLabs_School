/*
 * CS 106B Trailblazer
 * This file declares the functions you will write in this assignment.
 * 
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * !!!IF YOU MODIFY THIS FILE, YOU *WILL* LOSE POINTS ON YOUR GRADE!!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, based on past code by Keith Schwarz
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _trailblazer_h
#define _trailblazer_h

#include "basicgraph.h"
#include "set.h"
#include "vector.h"
#include "queue.h"
#include "stack.h"
#include "Color.h"
#include "pqueue.h"

/*
 * The path-searching algorithms you must implement.
 */

void mergeClusters(int index1, int index2, Vector<Set<Vertex* > > & vecSet);
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> depthHelperExtension(BasicGraph& graph, Vertex* start, Vertex* end, Stack<Vector<Vertex*> > & stack, Stack<Vector<Vertex*> > & backStack);
void changeColorsBreadth(Vector<Vertex*>& solution);
Vector<Vertex*> aHelperExtension(BasicGraph& graph, Vertex* end, PriorityQueue<Vector<Vertex*> > & queue, PriorityQueue<Vector<Vertex*> > & backQueue, Vertex * start);
void setUp(BasicGraph& graph, Vector<Set<Vertex* > > & vecSet, PriorityQueue<Edge*> & queue);
Vector<Vertex*> dHelper(BasicGraph& graph, Vertex* end, PriorityQueue<Vector<Vertex*> > & queue);
Vector<Vertex*> breadthHelper(BasicGraph& graph, Vertex* end, Queue<Vector<Vertex*> > & queue);
void giveUp(BasicGraph& graph, Stack<Vector<Vertex*> > & stack);
Vector<Vertex*> depthHelper(BasicGraph& graph, Vertex* end, Stack<Vector<Vertex*> > & stack);
double getCost(BasicGraph& graph, Vector<Vertex* > & currPath);
int getIndex(BasicGraph& graph, Vector<Set<Vertex* > > & vecSet, PriorityQueue<Edge*> & queue, Vertex * vertex);
Vector<Vertex*> aHelper(BasicGraph& graph, Vertex* end, PriorityQueue<Vector<Vertex*> > & queue);
/*
 * The minimum spanning tree / maze generation algorithm you must implement.
 */
Set<Edge*> kruskal(BasicGraph& graph);

/*
 * Returns a 'heuristic' value, or rough estimation, of the distance between
 * this vertex and the given other vertex.
 * The heuristic function is guaranteed to be an 'admissible heuristic',
 * meaning that it is never an overestimation of the distance.
 * (The word 'extern' means this function is defined elsewhere.
 *  You do not need to worry about it.)
 */
extern double heuristicFunction(Vertex* from, Vertex* to);

#endif

/*
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 */

