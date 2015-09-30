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
            addConnectionFix(i,j,1);
    }
}

neuronet::neuronet(const neuronet& rhs)
{
    nInputs_    = rhs.nInputs_;
    nOutputs_   = rhs.nOutputs_;
    nNeurons_   = rhs.nNeurons_;

    outputs_    = new double[nOutputs_];

    neurons_    = new std::vector<neuron*>;
    for(auto &n : *rhs.neurons_)
        neurons_->push_back(n->clone());
    for( auto &n : *neurons_)
        n->setNet(this);

    connections_= new std::vector<connection*>;
    for(auto &c : *rhs.connections_)
    {
        connection* cNew = c->clone();
        connections_->push_back(cNew);
        //the neurons need their connections:
        getNeuronFix(cNew->from())->getConnections()->push_back(cNew);
    }
    neuronDict_ = new std::vector<uint>;
    for(auto &i : *rhs.neuronDict_)
        neuronDict_->push_back( i );
}

neuronet::neuronet(neuronet&& rhs)
{
    nInputs_    = rhs.nInputs_;
    nOutputs_   = rhs.nOutputs_;
    nNeurons_   = rhs.nNeurons_;

    outputs_    = rhs.outputs_;
    rhs.outputs_ = nullptr;

    connections_= rhs.connections_;
    rhs.connections_ = nullptr;

    neurons_= rhs.neurons_;
    rhs.neurons_ = nullptr;
    for( auto &n : *neurons_)
        n->setNet(this);

    neuronDict_= rhs.neuronDict_;
    rhs.neuronDict_= nullptr;

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
        getNeuronFix(i)->addInput( input[i] );

    for(auto &n : *neuronDict_)
        getNeuronFix(n)->update();

    for(uint i = 0; i< nOutputs_; i++ )
        outputs_[i] = getNeuronFix(i + nInputs_)->getActivation();

    return outputs_;
}


int neuronet::addConnectionFix(uint from, uint to, double weight, bool force)
{
    if(from == to) return -1;
    if(from >= nNeurons_ || to >= nNeurons_) return -1;
    if(from < nInputs_ && to < nInputs_ ) return -1; //no connection between inputs
    if(to == 0) return -1; //bias neuron
    if(from >= nInputs_ && to >= nInputs_ && from < nInputs_+nOutputs_ && to < nInputs_+nOutputs_) return -1; //no connection between outputs


    //has connection like c (only weight differs)?
    if( getNeuronFix(from)->hasConnectionTo( to ) )
    {
        if(!force) return 0;
        else getNeuronFix(from)->getConnectionTo(to)->setWeight(weight);

    }

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
        c->setWeight( gaus_(w,0.1) );
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
    if(maxCon<=connections_->size()) return 0;
    if(maxCon<0.5*connections_->size())
    {
        uint from, to;
        uint cnt = 0;
        while(1){
            from = uniform_(0,nNeurons_-1);
            to = uniform_(1,nNeurons_-1);
            if( addConnectionFix(from,to,gaus_()) > 0 )
                break;
            if(cnt++ > 100)
                {std::cout<<" inf loop\n "; break;}
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
        uint cnt = 0;
        while(1)
        {
            uint idx = uniform_(0,candidates.size());
            if( addConnectionFix(candidates[idx]->from(), candidates[idx]->to(),gaus_()) > 0)
                break;
            if(cnt++ > 100)
                {std::cout<<" inf loop2\n "; break;}
        }
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


double neuronet::getSumOfWeights() const
{
    double sum = 0;
    for(auto &c : *connections_)
        sum += c->getWeight();
    return sum;
}

double neuronet::getSqSumOfWeights() const
{
    double sum = 0;
    for(auto &c : *connections_)
        sum += pow(c->getWeight(),2);
    return sum;
}
