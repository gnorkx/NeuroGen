#ifndef NEURON_H
#define NEURON_H

#include<vector>
#include<cmath>
#include<algorithm>

#include"connection.h"

enum neuron_type {inactive,standard,input,output};


class neuron
{
    public:
        neuron(neuron_type, std::vector<neuron*>* );
        virtual ~neuron();

        void update();

        void addInput(double in ) {sumOfInput_ += in; nInputs++;};

        //Get
        neuron_type getType( void ) const {return type_;};
        double getActivation( void ) const {return activation_;};
        std::vector<connection*>* getConnections(){return &connections_;};

        bool hasConnectionTo(uint) const;


        //Set
        void setType(neuron_type type){type_=type;};
        void setNeurons(std::vector<neuron*>* neurons){neurons_ = neurons;}


    private:

        double activation_fnc(double) const;

        std::vector<connection*> connections_;
        std::vector<neuron*>* neurons_;

        neuron_type type_;

        double activation_;

        double sumOfInput_;
        unsigned nInputs;


};

#endif // NEURON_H
