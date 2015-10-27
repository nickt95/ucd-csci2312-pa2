//
// Created by nickt_000 on 9/18/2015.
//

#include <iostream>

using namespace std;

#ifndef PA2_CLUSTERING_H
#define PA2_CLUSTERING_H

namespace clustering {

    class point {
    private:
        int dim;
        double* pointDim;
    public:
        static const char POINT_VALUE_DELIM;

        point();
        point(const int);
        point(const int, double[]);
        point(const point&);
        ~point();

        int getDim() const{
            return dim;
        }
        double* getPointDim() const{
            return pointDim;
        }

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

        friend istream& operator>> (istream&, point&);
        friend ostream& operator<< (ostream&, point&);
    };

    typedef point * pointPtr;

    struct node{
        point payload;
        node* next;
    };

    class cluster {
    private:
        int size;
        node* pointList;
        unsigned int id;
        point centroid;
        bool validCen;
    public:
        static const char POINT_CLUSTER_ID_DELIM;
        static unsigned int __idGenerator;

        cluster();
        cluster(const cluster&);
        ~cluster();

        int getSize() const{
            return size;
        }
        node* getPointList() const{
            return pointList;
        }
        unsigned int getId() const{
            return id;
        }
        void setId(){
            id = __idGenerator;
            __idGenerator++;
        }

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

        friend istream& operator>> (istream&, cluster&);
        friend ostream& operator<< (ostream&, cluster&);

        void setCentroid(const point& p){
            centroid = p;
            validCen = false;
        }
        const point getCentroid(){
            return centroid;
        }
        void setValid(bool b){
            validCen = b;
        }
        bool getValid(){
            return validCen;
        }
        void calculateCen(){
            node* curr = getPointList();

            //if no points
            if(curr == nullptr){
                centroid = point(2);
                setValid(true);
                return;
            }

            //set initial
            point p = curr->payload;
            curr = curr->next;

            //sum of points
            while(curr != nullptr){
                p += curr->payload;
                curr = curr->next;
            }

            //find average
            centroid = p/getSize();
            validCen = true;
        }


        void pickPoints(int, pointPtr*);

        double intraClusterDistance();
        int intraClusterEdges();
    };

    class Move{
    private:
        pointPtr p;
        cluster* to;
        cluster* from;
    public:
        Move(const pointPtr &ptr, cluster* ifrom, cluster* ito){
            p = ptr;
            from = ifrom;
            to = ito;
        }

        void preform(){
            to->add(from->remove(p));
        }
    };


    class kMeans{
    private:
        string in;
        string out;
    public:
        kMeans(string, string);

        const double SCORE_DIFF_THRESHOLD = 10.0;

        void run(int);
    };
}

#endif //PA2_CLUSTERING_H
