# ifndef EDGE_HPP
# define EDGE_HPP
# include <string>
# include "Vertex.hpp"

class Edge {
public:
    //constructors
    Edge(std::string id1, std::string id2, unsigned long w) : weight(w), v1(new Vertex(id1)), v2(new Vertex(id2)) {}
    //getters
    unsigned long getWeigth() {return weight;}
    Vertex* getV1() {return v1;}
    Vertex* getV2() {return v2;}
    //services
    Vertex* getOpposite(std::string vId);
    friend class Graph;
private:
    unsigned long weight;
    Vertex* v1;
    Vertex* v2;
};
# endif