# ifndef GRAPH_HPP
# define GRAPH_HPP
# include "GraphBase.hpp"
# include "Edge.hpp"
# include <map>

class Graph : public GraphBase{
/* Undirected Graph. Asjacency list implemented*/
public:
    // constructor
    Graph(){};
    // destructor
    ~Graph();
    // getters
    Vertex* getVertex(std::string label);
    Edge* getEdge(std::string label1, std::string label2);
    // sevices
    std::list<Vertex*> computeAdjacencyList(std::string);
    void addVertex(std::string label);
    void removeVertex(std::string label);
    void addEdge(std::string label1, std::string label2, unsigned long weight);
    void removeEdge(std::string label1, std::string label2);
    void removeInnerEdge(std::string label1, std::string label2);
    unsigned long shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path);
private:
    std::map<std::string, Edge*> E; 
    std::map<std::string, Vertex*> V;
    std::map<std::string, std::list<Edge*> > incidenceList;
};
# endif