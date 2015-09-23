#ifndef NEURON_H
#define NEURON_H

#include<vector>
#include<cmath>
#include<algorithm>

#include"connection.h"


class neuronet;

enum neuron_type {inactive,standard,input,output};


class neuron
{
    public:
        neuron(neuron_type, neuronet& );
        virtual ~neuron();

        void update();

        void addInput(double in ) {sumOfInput_ += in; nInputs_++;};

        //Get
        neuron_type getType( void ) const {return type_;};
        double getActivation( void ) const {return activation_;};
        std::vector<connection*>* getConnections(){return &connections_;};
        uint getIncomming() const {return nIncomming_;};
        connection* getConnectionTo(uint);


        bool hasConnectionTo(uint) const;


        //Set
        void setType(neuron_type type){type_=type;};
        void addIncomming(){nIncomming_++; if(connections_.size()&&!type_) type_=standard;};
        void removeIncomming(){nIncomming_--; if(!nIncomming_) type_=inactive;}


    private:

        double activation_fnc(double) const;

        std::vector<connection*> connections_;
        neuronet* net_;

        neuron_type type_;

        double activation_;

        double sumOfInput_;
        uint nInputs_; //number of set inputs
        uint nIncomming_; //number of incomming connections


};

#endif // NEURON_H
