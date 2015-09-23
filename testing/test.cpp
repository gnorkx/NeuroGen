#include<cstdlib>
#include<cmath>
#include<iostream>

#include"../net/neuronet.h"

using namespace std;

int main() {

    neuronet net(2,2);

    for(int i = 0; i<5; i++)
        net.addRandNeuron();

    for(int i = 0; i<20; i++)
        net.addRandConnection();

    net.initWeights();

    net.print();

    double in[]={0.5,1};
    double* out;

   for(int i = 0; i<4; i++)
    {
        out = net.run(in);
        cout<<"result "<<out[0]<<" "<<out[1]<<endl;
    }

return 0;
}
