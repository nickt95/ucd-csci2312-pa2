//
// Created by nickt_000 on 9/18/2015.
//

#ifndef PA2_CLUSTERING_H
#define PA2_CLUSTERING_H

namespace clustering {

    class point {
    private:
        int dim;
        double* pointDim;
    public:
        point();
        point(const int);
        point(const int, double[]);
        point(const point&);
        ~point();

        double distanceTo(const point, const point);

        point& operator= (const point&);
        bool operator== (const point&);
        bool operator!= (const point&);
        bool operator< (const point&);
        bool operator> (const point&);
        bool operator<= (const point&);
        bool operator>= (const point&);

        point operator+ (const point&);
        point operator- (const point&);
        point operator* (const double);
        point operator/ (const double);
        point& operator+= (const point&);
        point& operator-= (const point&);
        point& operator*= (const double);
        point& operator/= (const double);
    };

    typedef point * pointPtr;

    class cluster {
    private:
        struct node{
            point payload;
            node* next;
        };
        int size;
        node* pointList;
    public:
        cluster();
        cluster(const cluster&);
        ~cluster();

        void add(const pointPtr);
        pointPtr remove(const pointPtr);

        cluster& operator= (const cluster&);
        cluster operator+ (cluster&);
        cluster operator- (const cluster&);
        bool operator== (const cluster&);
        cluster& operator+= (cluster&);
        cluster& operator-= (cluster&);
        cluster& operator+ (point&);
        cluster& operator- (point&);
        cluster& operator+= (point&);
        cluster& operator-= (point&);
    };

}

#endif //PA2_CLUSTERING_H
