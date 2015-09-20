#include "neuronet.h"

neuronet::neuronet(int nIn, int nOut)
{
    //ctor

    nInputs_  = nIn;
    nOutputs_ = nOut;
    nNeurons_ = nInputs_ + nOutputs_;

    outputs_ = new double[nOutputs_];

    connections_= new std::vector<connection*>;
    neurons_= new std::vector<neuron*>;


    //create input
    for(uint i = 0; i < nInputs_; i++)
        neurons_->push_back(new neuron(neuron_type::input, neurons_));

    //create output
    for(uint i = 0; i < nOutputs_; i++)
        neurons_->push_back(new neuron(neuron_type::output, neurons_));

    //connect in and outputs

    for(uint i = 0; i < nInputs_; i++)
    {
        for(uint j = nNeurons_-nOutputs_; j<nNeurons_; j++)
            addConnection(i,j,1);
    }
}

neuronet::~neuronet()
{
    //dtor

    for( auto &c : *connections_)
    { delete c; c = nullptr; }
    delete connections_;

    for( auto &n : *neurons_)
    { delete n; n = nullptr; }
    delete neurons_;

    delete outputs_;

}

double* neuronet::run(double *input)
{
    for(uint i = 0; i<nInputs_ ;i++)
        (*neurons_)[i]->addInput( input[i] );

    for(auto &n : *neurons_)
        n->update();

    for(uint i = 0; i< nOutputs_; i++ )
        outputs_[i] = (*neurons_)[i + nNeurons_ - nOutputs_]->getActivation();

    return outputs_;
}


int neuronet::addConnection(uint from, uint to, double weight)
{
    if(from >= nNeurons_ || to >= nNeurons_) return -1;
    if(from < nInputs_ && to < nInputs_ ) return -1; //no connection between inputs
    if(from >= nNeurons_ - nOutputs_ && to >= nNeurons_ - nOutputs_ ) return -1; //no connection between outputs


    //has connection like c (only weight differs)?
    if( (*neurons_)[from]->hasConnectionTo( to ) ) return 0;

    connection *c = new connection(from,to,weight);
    connections_->push_back( c );
    (*neurons_)[from]->getConnections()->push_back( c );
    std::cout<<"added Connection: "<<from<<"->"<<to<<": "<<weight<<std::endl;
    return 1;

}

int neuronet::addNeuron( uint idx )
{
    if(idx < nInputs_ || idx > nNeurons_ -nOutputs_ ) return -1;

    neurons_->push_back(new neuron(neuron_type::standard, neurons_));
    neuronDict_->insert(neuronDict_->begin()+idx, neurons_->size() -1 );
    std::cout<<"neuron added: "<<idx<<std::endl;
    nNeurons_++;
    return idx;
}
