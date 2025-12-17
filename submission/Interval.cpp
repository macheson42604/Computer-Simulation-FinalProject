#include "Interval.hpp"

// Constructor
Interval::Interval(double tL, double tR, double bigLambL, double lamb, int j) {
    tLeft = tL;
    tRight = tR;
    bigLambdaLeft = bigLambL;
    lambda = lamb;
    id = j;

    bigLambdaRight = bigLambdaLeft + (lambda * (tRight - tLeft));
}

// Getters
double Interval::get_tLeft() const {
    return tLeft;
}

double Interval::get_tRight() const {
    return tRight;
}

double Interval::get_bigLambdaLeft() const {
    return bigLambdaLeft;
}

double Interval::get_bigLambdaRight() const {
    return bigLambdaRight;
}

double Interval::get_lambda() const {
    return lambda;
} 

int Interval:: get_id() const {
    return id;
}