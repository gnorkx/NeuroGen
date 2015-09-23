#include "neuronet.h"

#include<iomanip>
#include<iostream>

neuronet::neuronet(int nIn, int nOut)
{
    //ctor

    nInputs_    = nIn;
    nOutputs_   = nOut;
    nNeurons_   = nInputs_ + nOutputs_;

    outputs_    = new double[nOutputs_];

    connections_= new std::vector<connection*>;
    neurons_    = new std::vector<neuron*>;
    neuronDict_ = new std::vector<uint>;


    //create input
    for(uint i = 0; i < nInputs_; i++)
    {
        neurons_->push_back(new neuron(neuron_type::input, *this));
        neuronDict_->push_back(i);
    }
    //create output
    for(uint i = 0; i < nOutputs_; i++)
    {
        neurons_->push_back(new neuron(neuron_type::output, *this));
        neuronDict_->push_back(neurons_->size()-1);
    }

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

    delete neuronDict_;

    delete outputs_;

}

double* neuronet::run(double *input)
{
    for(uint i = 0; i<nInputs_ ;i++)
        getNeuron(i)->addInput( input[i] );

    for(auto &n : *neuronDict_)
        getNeuronFix(n)->update();

    for(uint i = 0; i< nOutputs_; i++ )
        outputs_[i] = getNeuron(i + nNeurons_ - nOutputs_)->getActivation();

    return outputs_;
}


int neuronet::addConnection(uint from, uint to, double weight)
{
    if(from >= nNeurons_ || to >= nNeurons_) return -1;
    if(from < nInputs_ && to < nInputs_ ) return -1; //no connection between inputs
    if(from >= nNeurons_ - nOutputs_ && to >= nNeurons_ - nOutputs_ ) return -1; //no connection between outputs


    //has connection like c (only weight differs)?
    if( getNeuronFix(from)->hasConnectionTo( to ) ) return 0;

    connection *c = new connection(from,to,weight);
    connections_->push_back( c );
    getNeuronFix(from)->getConnections()->push_back( c );
    getNeuronFix(to)->addIncomming();
    std::cout<<"added Connection: "<<from<<"->"<<to<<": "<<weight<<std::endl;
    return 1;

}

int neuronet::addNeuron( uint idx )
{
    if(idx < nInputs_ || idx > nNeurons_ -nOutputs_ ) return -1;

    neurons_->push_back(new neuron(neuron_type::inactive, *this));
    neuronDict_->insert(neuronDict_->begin()+idx, neurons_->size() -1 );
    nNeurons_++;
    std::cout<<"neuron added: "<<idx<<" / "<<nNeurons_<<std::endl;
    return idx;
}

std::default_random_engine neuronet::randGen_;
std::uniform_real_distribution<double> neuronet::uniformInternal_(0.,1.);
std::normal_distribution<double> neuronet::gausInternal_(0.,1.);

void neuronet::initWeights()
{
    uint nIncomming;
    double width = 2.;
    for(auto &c : *connections_)
    {
        nIncomming = getNeuronFix(c->to())->getIncomming();
        c->setWeight( gaus_(0,width/nIncomming) );
    }
}

void neuronet::mutateWeights()
{
    //add asymmetric gaus
    for(auto &c : *connections_)
    {
        double w = c->getWeight();
        c->setWeight( gaus_(w,0.1*w) );
    }
}

void neuronet::addRandNeuron()
{
    uint minIdx = nInputs_;
    uint maxIdx = nNeurons_-nOutputs_;
    uint idx = (maxIdx-minIdx)*uniform_()+minIdx; //uniform ==1 ?!

    addNeuron(idx);
}

int neuronet::addRandConnection()
{
    uint maxCon = nNeurons_*(nNeurons_-1) - nInputs_*(nInputs_-1) - nOutputs_*(nOutputs_-1);
        std::cout<<maxCon<<"\n";
    if(maxCon<=connections_->size()) return 0;
    if(maxCon<0.5*connections_->size())
    {
        uint from, to;
        while(1){
            from = uniform_(0,nNeurons_-1);
            if(from<nInputs_) to = uniform_(nInputs_,nNeurons_);
            else if(from>nNeurons_-nOutputs_) to = uniform_(0,nNeurons_-nInputs_);
            else to = uniform_(0,nNeurons_);
            if(from==to) continue;

            if(!getNeuronFix(from)->hasConnectionTo(to))
            {
                std::cout<<addConnection(from,to,gaus_())<<std::endl;
                break;
            }
        }
        return 1;
    }else{
        std::vector<connection*> candidates;
        for(uint i = 0; i<nNeurons_; i++)
        {
            uint j = i<nInputs_?nInputs_:0;
            for(; j<nNeurons_; j++)
            {
                if(i==j) continue;
                if(!getNeuronFix(i)->hasConnectionTo(j))
                    candidates.push_back(new connection(i,j,0));
            }
        }
        uint idx = uniform_(0,candidates.size());
        std::cout<<addConnection(candidates[idx]->from(), candidates[idx]->to(),gaus_())<<std::endl;
        for(auto &c: candidates)
            delete c;
        return 1;
    }
}


void neuronet::print()
{

     std::cout<<"fr\\to:\t";
    for(uint i = 0; i<nNeurons_;i++)
    {
        if(getNeuron(i)->getType() == neuron_type::input)
            std::cout<<"i";
        else if(getNeuron(i)->getType() == neuron_type::output)
            std::cout<<"o";

        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;


    for(uint i = 0; i<nNeurons_;i++)
    {
        if(getNeuron(i)->getType() == neuron_type::input)
            std::cout<<"i";
        else if(getNeuron(i)->getType() == neuron_type::output)
            std::cout<<"o";

        std::cout<<i<<"\t";
        for(uint j = 0; j<nNeurons_; j++)
        {
            connection* c = getNeuron(i)->getConnectionTo(getFixNeuronId(j));
            if(!c) std::cout << " - \t";
            else std::cout<<std::setprecision(2)<<c->getWeight()<<"\t";
        }
        std::cout<<"\n";
    }
    std::cout<<std::setprecision(6);
}
