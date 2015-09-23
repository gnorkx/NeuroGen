#ifndef CONNECTION_H
#define CONNECTION_H

#include"definitions.h"

class connection
{
    public:
        connection();
        connection(int from, int to, float weight):from_(from), to_(to), weight_(weight){};

        //set
        void setWeight(double weight){weight_=weight;};
        //Get

        uint from(){return from_;};
        uint to(){return to_;};
        double getWeight() const {return weight_;};


        connection* clone() const {return new connection(from_, to_, weight_);};


    private:

        uint from_;
        uint to_;
        double weight_;
};

#endif // CONNECTION_H
