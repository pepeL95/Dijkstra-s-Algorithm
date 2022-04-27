# ifndef VERTEX_HPP
# define VERTEX_HPP
# include <string>
# include <list>

typedef std::string Elem;

class Vertex {
public:
    //constructors
    Vertex(Elem id = "default") : vId(id), degree(0) {}
    //getters
    size_t deg() {return degree;}
    Elem getId() {return vId;}
    // setters
    void setId(std::string id) {vId = id;}
    // services
    void increaseDegree() {degree++;}
    void decreaseDegree() {degree--;}
    friend class Graph;
private:
    Elem vId;
    size_t degree;
};

# endif