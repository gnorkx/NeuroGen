#ifndef NEURON_H
#define NEURON_H

#include<vector>
#include<cmath>

#include"connection.h"

enum neuron_type {inactive,standard,input,output};


class neuron
{
    public:
        neuron();
        virtual ~neuron();

        //Get
        double getActivation() const;
        neuron_type getType( void ) const {return type_;};

        //Set
        void setType(neuron_type type){type_=type;};
        void setNeutons(std::vector<neuron*>* neurons){neurons_ = neurons;}


    private:
        double activation( void ) const;
        double activation_fnc(double) const;

        std::vector<connection*> connections_;
        std::vector<neuron*> *neurons_;

        neuron_type type_;


};

#endif // NEURON_H
