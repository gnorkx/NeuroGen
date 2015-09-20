#include<cstdlib>
#include<cmath>
#include<iostream>

#include"../net/neuronet.h"

using namespace std;

int main() {

    neuronet net(2,1);

    net.addNeuron(2);
    cout<<net.addConnection(0,2,1)<<endl;
    net.addConnection(2,3,1);

    double in[]={1,1};
    double* out;
    out = net.run(in);
    cout<<"result "<<out[0]<<endl;
    out = net.run(in);
    cout<<"result "<<out[0]<<endl;

return 0;
}
