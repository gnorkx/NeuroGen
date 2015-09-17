#include "neuron.h"

neuron::neuron()
{
    //ctor
}

neuron::~neuron()
{
    //dtor
}


double neuron::getActivation() const
{
    return 1;
}



double neuron::activation( void ) const
{
    double sum = 0;
    for(auto &con : connections_)
    {
        sum += con->getWeight()*(*neurons_)[con->from()]->getActivation();
    }

    return activation_fnc(sum);

}

double neuron::activation_fnc( double x ) const
{
    return 1./(1. + exp(-x));
}

