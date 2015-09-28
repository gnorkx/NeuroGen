#include "neuron.h"
#include"neuronet.h"
#include<iostream>

neuron::neuron(neuron_type type, neuronet& net)
{
    //ctor
    //std::cout<<"create neuton"<<std::endl;
    type_       = type;
    net_        = &net;

    sumOfInput_ = 0;
    nInputs_    = 0;
    nIncomming_ = 0;
}

neuron::~neuron()
{
    //dtor
}


void neuron::update()
{

    if(type_ == inactive) return;
    if(!nInputs_) return;
   // std::cout<<"sum: "<< sumOfInput_<<std::endl;
    activation_ = activation_fnc( sumOfInput_/nInputs_ );


    if(type_ != neuron_type::output)
    {
        for( auto &connec : connections_)
        {
            net_->getNeuronFix(connec->to())->addInput(connec->getWeight()*activation_);
   //         std::cout<<type_<< ": "<<connec->getWeight()*activation_<<" pushed to "<<connec->to()<<std::endl;
        }
    }
    sumOfInput_ = 0;
    nInputs_ = 0;

}

double neuron::activation_fnc( double x ) const
{
    return 2./(1. + exp(-x))-1;
}


bool neuron::hasConnectionTo(uint to) const {
    return std::any_of(connections_.begin(), connections_.end(),
        [=](connection* c){ return c->to() == to; });
}

connection* neuron::getConnectionTo(uint idx)
{
    for(auto &c : connections_)
    {
        if(c->to() == idx)
            return c;
    }
    return nullptr;
}


neuron* neuron::clone() const
{
    neuron* n       = new neuron(type_, *net_);
    //connections dont belong to the neuron but to the net!
    //for(auto &c : connections_)
    //   n->connections_.push_back(c);
    n->activation_  = activation_;
    n->sumOfInput_  = sumOfInput_;
    n->nInputs_     = nInputs_;
    n->nIncomming_  = nIncomming_;
    return n;
}
