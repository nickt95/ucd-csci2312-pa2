//
// Created by nickt_000 on 9/18/2015.
//

#include "clustering.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>

using namespace clustering;

const char POINT_VALUE_DELIM = ',';
const char POINT_CLUSTER_ID_DELIM = ':';
unsigned int cluster::__idGenerator = 1;

point::point(const int i) {
    assert(i > 0);
    dim = i;
    pointDim = new double[i];
}

point::point(const int i, double *pDouble) {
    assert(i > 0);
    dim = i;
    pointDim = pDouble;
}

point::~point() {
    delete [] pointDim;
}

double point::distanceTo(const point point1, const point point2) {
    assert(point1.dim == point2.dim);

    double sum = 0;
    for(int i = 0; i < point1.dim; i++){
        sum += pow(point1.pointDim[i]+point2.pointDim[i],2);
    }

    return sqrt(sum);
}

point::point(const point &newPoint) {
    dim = newPoint.dim;
    pointDim = newPoint.pointDim;
}

point &point::operator=(const point &rhs) {
    dim = rhs.dim;
    pointDim = rhs.pointDim;
    return *this;
}

bool point::operator==(const point &rhs) {
     if(dim != rhs.dim){
         return false;
     }
     else{
         for(int i = 0; i < rhs.dim; i++){
             if(pointDim[i] != rhs.pointDim[i]){
                 return false;
             }
         }
         return true;
     }
}

bool point::operator!=(const point &rhs) {
    return !(*this == rhs);
}

bool point::operator<(const point &rhs) {
    assert(dim == rhs.dim);

    for(int i = 0; i < dim; i++){
        if(pointDim[i] != rhs.pointDim[i]){
            return pointDim[i] < rhs.pointDim[i];
        }
    }
    return false;
}

bool point::operator>(const point &rhs) {
    assert(dim == rhs.dim);

    for(int i = 0; i < dim; i++){
        if(pointDim[i] != rhs.pointDim[i]){
            return pointDim[i] > rhs.pointDim[i];
        }
    }
    return false;
}

bool point::operator<=(const point &rhs) {
    return !(*this > rhs);
}

bool point::operator>=(const point &rhs) {
    return !(*this < rhs);
}

point point::operator+(const point &rhs) {
    assert(dim == rhs.dim);

    point temp(*this);

    for(int i = 0; i < dim; i++){
        temp.pointDim[i] += rhs.pointDim[i];
    }

    return temp;
}

point point::operator-(const point &rhs) {
    assert(dim == rhs.dim);

    point temp(*this);

    for(int i = 0; i < dim; i++){
        temp.pointDim[i] -= rhs.pointDim[i];
    }

    return temp;
}

point point::operator*(const double rhs) {
    point temp(*this);

    for(int i = 0; i < dim; i++){
        temp.pointDim[i] *= rhs;
    }

    return temp;
}

point point::operator/(const double rhs) {
    assert(rhs != 0);
    point temp(*this);

    for(int i = 0; i < dim; i++){
        temp.pointDim[i] /= rhs;
    }

    return temp;
}

point &point::operator+=(const point &rhs) {
    *this = *this + rhs;
    return *this;
}

point &point::operator-=(const point &rhs) {
    *this = *this - rhs;
    return *this;
}

point &point::operator*=(const double rhs) {
    *this = *this * rhs;
    return *this;
}

point &point::operator/=(const double rhs) {
    *this = *this / rhs;
    return *this;
}

cluster::cluster() {
    size = 0;
    pointList = nullptr;
    setId();
    setValid(false);
}

void cluster::add(pointPtr const ptr) {
    if(pointList == nullptr){
        node n;
        n.payload = *ptr;
        pointList = &n;
        size++;
        setValid(false);
        return;
    }

    node* curr = pointList;
    node* next = curr->next;

    while(*ptr < curr->payload){
        curr = next;
        next = curr->next;

        if(next == nullptr){
            node n;
            n.payload = *ptr;
            curr->next = &n;
            setValid(false);
            return;
        }
    }

    node n;
    n.payload = *ptr;
    n.next = next;
    (*curr).next = &n;
    size++;
    setValid(false);
}

cluster::cluster(const cluster &newCluster) {
    size = 0;
    pointList = nullptr;

    node* nextPoint = newCluster.pointList;
    while(nextPoint != nullptr){
        add(&(nextPoint->payload));
        nextPoint = nextPoint->next;
    }
    setId();
    setValid(false);
}

cluster::~cluster() {
    node* curr = getPointList();
    node* next;

    while(curr != nullptr){
        next = curr->next;
        delete curr;
        curr = next;
    }
}


pointPtr cluster::remove(pointPtr const ptr) {
    if(*ptr == pointList->payload){
        node* del = pointList;
        pointList = del->next;
        delete del;
        setValid(false);
        return ptr;
    }

    node* prev = pointList;
    node* curr = prev->next;

    while(*ptr != curr->payload){
        prev = curr;
        assert(curr->next != nullptr);
        curr = curr->next;
    }

    (*prev).next = curr->next;
    delete curr;
    size--;
    setValid(false);
    return ptr;
}

point::point() {
    dim = 2;
    pointDim = new double[2];
}

cluster &cluster::operator=(const cluster &rhs) {
    size = 0;
    pointList = nullptr;

    node* nextPoint = rhs.pointList;
    while(nextPoint != nullptr){
        add(&(nextPoint->payload));
        nextPoint = nextPoint->next;
    }

    setValid(false);
    return *this;
}

cluster cluster::operator+(cluster &rhs) {
    cluster temp;
    cluster newRhs(rhs-*this);

    node* curr;
    node* next;

    if(pointList != nullptr){
        curr = pointList;
        next = curr->next;

        temp.add(&(curr->payload));
        while(next != nullptr){
            curr = next;
            next = curr->next;
            temp.add(&(curr->payload));
        }
    }

    if(newRhs.pointList != nullptr){
        curr = newRhs.pointList;
        next = curr->next;

        temp.add(&(curr->payload));
        while(next != nullptr){
            curr = next;
            next = curr->next;
            temp.add(&(curr->payload));
        }
    }

    return temp;
}

cluster cluster::operator-(const cluster &rhs) {
    cluster temp;

    node* lCurr = pointList;
    node* lNext = lCurr->next;
    node* rCurr;
    node* rNext;
    bool isIn;

    while(lNext != nullptr){
        rCurr = rhs.pointList;
        rNext = rCurr->next;
        isIn = false;

        while(rNext != nullptr && !isIn){
            isIn = lCurr == rCurr;
            rCurr = rNext;
            rNext = rCurr->next;
        }

        if(!isIn){
            isIn = lCurr == rCurr;
        }

        if(!isIn){
            temp.add(&(lCurr->payload));
        }

        lCurr = lNext;
        lNext = lCurr->next;
    }

    rCurr = rhs.pointList;
    rNext = rCurr->next;
    isIn = false;

    while(rNext != nullptr && !isIn){
        isIn = lCurr == rCurr;
        rCurr = rNext;
        rNext = rCurr->next;
    }

    if(!isIn){
        isIn = lCurr == rCurr;
    }

    if(!isIn){
        temp.add(&(lCurr->payload));
    }

    return temp;
}

bool cluster::operator==(const cluster &rhs) {
    if(pointList == nullptr && rhs.pointList == nullptr){
        return true;
    }

    if(pointList == nullptr || rhs.pointList == nullptr){
        return false;
    }

    node* lCurr = pointList;
    node* lNext = lCurr->next;
    node* rCurr = rhs.pointList;
    node* rNext = rCurr->next;

    while(lNext != nullptr && rNext != nullptr){
        if(lCurr->payload != rCurr->payload){
            return false;
        }

        lCurr = lNext;
        lNext = lCurr->next;
        rCurr = lNext;
        rNext = lCurr->next;
    }

    return (lCurr->payload == rCurr->payload) && (lNext == nullptr && rNext == nullptr);
}

cluster &cluster::operator+=(cluster &rhs) {
    *this = *this + rhs;
    return *this;
}

cluster &cluster::operator-=(cluster &rhs) {
    *this = *this - rhs;
    return *this;
}

cluster &cluster::operator+(point &rhs) {
    this->add(&rhs);
    return *this;
}

cluster &cluster::operator-(point &rhs) {
    this->remove(&rhs);
    return *this;
}

cluster &cluster::operator+=(point &rhs) {
    *this = *this + rhs;
    return *this;
}

cluster &cluster::operator-=(point &rhs) {
    *this = *this - rhs;
    return *this;
}

istream &operator>>(istream &lhs, point &rhs) {
    string temp;
    char c;
    double *newPoint;
    int newDim = 0;

    while(lhs.get(c)){
        if(c == ','){
            newPoint[newDim] = atof(temp.c_str());
            ++newDim;
            temp = "";
        }
        else{
            temp += c;
        }
    }

    newPoint[newDim] = atof(temp.c_str());
    ++newDim;
    rhs = point(newDim, newPoint);

    return lhs;
}

ostream &operator<<(ostream &lhs, point &rhs) {
    int curr = 0;

    if(rhs.getDim() == 0)
        return lhs;

    while(true){
        lhs << rhs.getPointDim()[curr];

        if(curr == rhs.getDim()-1)
            break;

        lhs << POINT_VALUE_DELIM;
        curr++;
    }

    return lhs;
}

istream &operator>>(istream &lhs, cluster &rhs) {
    stringstream ss;
    point tempPoint;
    string tempStr;
    rhs = cluster();
    string temp;
    char c;
    double *newPoint;
    int newDim;

    lhs >> tempStr;
    while(tempStr != ""){
        ss << tempStr;

        newDim = 0;
        temp = "";
        newPoint = nullptr;
        while(ss.get(c)){
            if(c == ','){
                newPoint[newDim] = atof(temp.c_str());
                ++newDim;
                temp = "";
            }
            else{
                temp += c;
            }
        }

        newPoint[newDim] = atof(temp.c_str());
        ++newDim;
        tempPoint = point(newDim, newPoint);

        rhs.add(&tempPoint);
        lhs >> tempStr;
    }

    return lhs;
}

ostream &operator<<(ostream &lhs, cluster &rhs) {
    node* curr = rhs.getPointList();
    point temp;
    int currN;

    while(curr != nullptr){
        temp = curr->payload;

        currN = 0;
        if(temp.getDim() != 0){
            while(true){
                lhs << temp.getPointDim()[currN];

                if(currN == temp.getDim()-1)
                    break;

                lhs << POINT_VALUE_DELIM;
                currN++;
            }
        }

        lhs << POINT_CLUSTER_ID_DELIM << rhs.getId() << endl;

        curr = curr->next;
    }

    return lhs;
}

void cluster::pickPoints(int k, pointPtr *pointArray) {
    //get size of steps to take
    int stepSize = getSize()/k;

    //point to start
    node* curr = getPointList();

    //get points
}
