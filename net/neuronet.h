#ifndef NEURONET_H
#define NEURONET_H

#include<vector>
#include<iostream>
#include<random>

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

    neuron* getNeuron(uint idx ) {return getNeuronFix( (*neuronDict_)[idx]);};
    neuron* getNeuronFix(uint idx ){return (*neurons_)[idx];};


    void initWeights();
    void mutateWeights();
    void addRandNeuron();
    int addRandConnection();

    void print();
    protected:
    private:

    //move to out of class!
    static std::default_random_engine randGen_;

    static std::normal_distribution<double> gausInternal_;
    static double gaus_(){return gausInternal_(randGen_);};
    static double gaus_(double x, double s){return s*(gaus_()+x);};
    static double gaus_(double x, double s1, double s2){double g=gaus_(); return (g>0?s2:s1)*(g+x); };

    static std::uniform_real_distribution<double> uniformInternal_;
    static double uniform_(){return uniformInternal_(randGen_);};
    static double uniform_(double a, double b){return (b-a)*uniform_() + a;};


    uint nInputs_;
    uint nOutputs_;
    uint nNeurons_;

    double* outputs_;

    std::vector<connection*>*   connections_;
    std::vector<neuron*>*       neurons_;
    std::vector<uint>*          neuronDict_;
};

#endif // NEURONET_H
