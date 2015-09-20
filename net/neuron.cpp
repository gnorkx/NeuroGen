#include "neuron.h"
#include<iostream>

neuron::neuron(neuron_type type, std::vector<neuron*>* neurons)
{
    //ctor
    //std::cout<<"create neuton"<<std::endl;
    sumOfInput_ = 0;
    nInputs = 0;
    type_ = type;
    neurons_ = neurons;
}

neuron::~neuron()
{
    //dtor
}


void neuron::update()
{

    activation_ = /*activation_fnc(*/ sumOfInput_;// );
    std::cout<<activation_<<std::endl;

    if(type_ != neuron_type::output)
    {
        for( auto &connec : connections_)
        {
            (*neurons_)[connec->to()]->addInput(connec->getWeight()*activation_);
        }
    }
    sumOfInput_ = 0;
    nInputs = 0;

}

double neuron::activation_fnc( double x ) const
{
    return 1./(1. + exp(-x));
}


bool neuron::hasConnectionTo(uint to) const {
    return std::any_of(connections_.begin(), connections_.end(),
        [=](connection* c){ return c->to() == to; });
}
