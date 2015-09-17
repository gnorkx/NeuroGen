#ifndef CONNECTION_H
#define CONNECTION_H


class connection
{
    public:
        connection();
        connection(int from, int to, float weight):from_(from), to_(to), weight_(weight){};

        //Get

        int from(){return from_;};
        int to(){return to_;};
        float getWeight(){return weight_;};
    private:

        int from_;
        int to_;
        float weight_;
};

#endif // CONNECTION_H
