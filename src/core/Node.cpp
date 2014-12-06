#include "Node.h"

namespace robot{

string Node::getName() const{
    return name;
}

void Node::setName(const string& _name){
    name=_name;
}

void Node::init(){

}

void Node::start(){
    thread=boost::thread(&Node::main,this);
}

void Node::stop(){

}

void Node::join(){
    thread.join();
}

}
