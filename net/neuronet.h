#ifndef NEURONET_H
#define NEURONET_H

#include<vector>
#include<iostream>

#include"definitions.h"
#include"connection.h"
#include"neuron.h"

class neuronet
{
    public:
        neuronet(int, int);
        virtual ~neuronet();



        double* run(double* );


    uint getFixNeuronId(uint idx) const {return (*neuronDict_)[idx];};

    int addConnection(uint, uint, double);
    int addNeuron(uint);

    protected:
    private:

    uint nInputs_;
    uint nOutputs_;
    uint nNeurons_;

    double* outputs_;

    std::vector<connection*>*   connections_;
    std::vector<neuron*>*       neurons_;
    std::vector<uint>*          neuronDict_;
};

#endif // NEURONET_H
