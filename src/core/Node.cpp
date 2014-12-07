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

void Node::debug(const string& message){
    cout<<"DEBUG: "<<name<<": "<<message<<endl;
    //BOOST_LOG_TRIVIAL(loggging::trivial::debug)<<name<<": "<<message;
}

void Node::error(const string& message){
    //BOOST_LOG_TRIVIAL(loggging::trivial::error)<<name<<": "<<message;
}

void Node::warning(const string& message){
    //BOOST_LOG_TRIVIAL(loggging::trivial::warning)<<name<<": "<<message;
}

void Node::info(const string& message){
    //BOOST_LOG_TRIVIAL(loggging::trivial::info)<<name<<": "<<message;
}

}
