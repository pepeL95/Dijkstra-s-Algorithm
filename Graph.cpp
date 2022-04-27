# include "Graph.hpp"
# include "Exceptions.hpp"
# include <iostream>
# include <queue>

// overload "less than" operator for functionality in STL priority queue
bool operator < (std::pair<Vertex*, unsigned long> p1, std::pair<Vertex*, unsigned long> p2) {return p1.second > p2.second;}

//------------Edge Code Starts--------------------------------------------------------------------------
Vertex* Edge::getOpposite(std::string vId) {
/* Returns the other endpoint vertex of current edge (i.e. opposite vertex from v) */
    if (v1->getId() == vId) return v2; 
    return v1;
}

//------------Graph Code Starts--------------------------------------------------------------------------

std::list<Vertex*> Graph::computeAdjacencyList(std::string label){
    std::list<Vertex*> adjacencyList;
    for (std::list<Edge*>::iterator it = incidenceList[label].begin();it!=incidenceList[label].end();it++)
       adjacencyList.push_back((*it)->getOpposite(label));
    return adjacencyList;
}

Vertex* Graph::getVertex(std::string label) {
    try {if (V.find(label) == V.end()) throw GraphExceptions("Vertex Not Found");}
    catch(GraphExceptions &e) {std::cout<<e.what()<<std::endl;}
    return V[label];
}

Edge* Graph::getEdge(std::string label1, std::string label2) {
    try {if (E.find(label1+label2) == E.end()) throw GraphExceptions("Edge Not Found");}
    catch(GraphExceptions &e) {std::cout<<e.what()<<std::endl;}
    return E[label1+label2];
}

void Graph::addVertex(std::string label) {
/* Initializes and adds a vertex to the graph. */
    //Check if it exists already, if it doesn't add it to V
    if (V.find(label) == V.end()) V[label] = new Vertex(label);
 }

void Graph::addEdge(std::string label1, std::string label2, unsigned long weight) {
/* Initializes and add a vertex to the graph. */
    // Create an edge iff the endpoints v1, v2 are members of V
    std::map<std::string, Vertex*>::iterator v1 = V.find(label1);
    std::map<std::string, Vertex*>::iterator v2 = V.find(label2);
    if (v1 == V.end() || v2 == V.end()) return; // cannot make edge, exit
    // at this point, can create bidirectional edge
    Edge* newEdge = new Edge(label1, label2, weight);
    Edge* converse = new Edge(label2, label1, weight);
    E[label1+label2] = newEdge;
    E[label2+label1] = converse;
    // Update V1 and V2 incidence list respectively
    incidenceList[label1].push_back(converse);
    incidenceList[label2].push_back(newEdge);
    V[label1]->increaseDegree(); V[label2]->increaseDegree(); // increase degree of vertices v1 and v2
}

void Graph::removeVertex(std::string label) {
/* Removes the vertex wiht an id of "label" from the graph */
    std::map<std::string, Vertex*>::iterator vIt = V.find(label);
    // check if it exists
    if (vIt == V.end()) return; // it doesn't exist, exit
    // remove v's incident edges
    std::map<std::string, std::list<Edge*> >::iterator v = incidenceList.find(label); // position of v in the map
    for (std::list<Edge*>::iterator it = incidenceList[label].begin();it != incidenceList[label].end();it++)
        removeInnerEdge((*it)->getOpposite(label)->getId(), label);
    incidenceList.erase(v); // update incidenceList  
    V.erase(vIt);
}

void Graph::removeEdge(std::string label1, std::string label2) {
/* Removes edge with edpoints with id's of "label1" and "label2", respectively */
    std::map<std::string, Edge*>::iterator e = E.find(label1+label2); // position of edge
    // check for existance
    if (e == E.end()) return; //not found cannot delete
    std::map<std::string, Edge*>::iterator converse = E.find(label2+label1); // position of converse of e
    E.erase(e); E.erase(converse); // delete from E
    // Remove from v1, v2 icidenceList respectively
    removeInnerEdge(label1, label2);
    removeInnerEdge(label2, label1);
    // decrease the degree of each endpoint
    V[label1]->decreaseDegree(); V[label2]->decreaseDegree(); 
}

void Graph::removeInnerEdge(std::string label1, std::string label2) {
    std::map<std::string, Edge*>::iterator e = E.find(label1+label2);
    if (e == E.end()) return; //not found cannot delete
    for (std::list<Edge*>::iterator it = incidenceList[label1].begin(); it != incidenceList[label1].end();it++)
        if ((*it)->getOpposite(label1)->getId() == label2) {incidenceList[label1].erase(it); break;}
}

unsigned long Graph::shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path) {
/* Finds shortest path from node A to node B using Dijkstra's Algorithm */
    std::map<std::string, Vertex*> pi; // previous vertex
    std::map<std::string, bool> visitedVertices; // maps already visited vertices
    std::map<std::string, unsigned long> distance; // maps vertices to distance from the source
    std::priority_queue<std::pair<Vertex*, unsigned long> > priorityQ; // compare functionality is applied at the top of this file
    std::pair<Vertex*, unsigned long> source(V[startLabel], 0); 
    
    for (std::map<std::string, Vertex*>::iterator it = V.begin(); it!=V.end();it++) {
        distance[it->first] = 1000000; // initialize distance map
        visitedVertices[it->first] = false; // no visited neighbors yet
    }
    priorityQ.push(source); // source is now in the queue
    distance[startLabel] = 0;
    while (!priorityQ.empty() && visitedVertices[endLabel] != true) {
        Vertex* u = priorityQ.top().first; priorityQ.pop(); // extract-min from queue       
        visitedVertices[u->getId()] = true; // mark it as visted
        for (Vertex* v : computeAdjacencyList(u->getId())) {
            if (visitedVertices[v->getId()] == true) continue; // if visted already, back to loop
            unsigned long uDist = distance[u->getId()] + E[u->getId()+v->getId()]->getWeigth(); // d[u]
            if (distance[v->getId()] > uDist) {
                distance[v->getId()] = uDist;
                std::pair<Vertex*, unsigned long> decreasedKey(v, uDist); // put the new min back in the queue
                priorityQ.push(decreasedKey);
                pi[v->getId()] = u; // now the prior vertex to v is u
            }
        }
    }
    
    path.insert(path.begin(), endLabel);
    for (std::string v = pi[endLabel]->getId();v != startLabel; v = pi[v]->getId()) 
        path.insert(path.begin(), v);
    path.insert(path.begin(), startLabel);

    return distance[endLabel];
}  

Graph::~Graph() {
    for (std::map<std::string, Vertex*>::iterator it = V.begin();it != V.end();it++)
        removeVertex(it->second->getId());
}