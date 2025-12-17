#include <iostream>

#include "Interval.hpp"

using namespace std;


// Constructor
Interval::Interval(double tL, double tR, double bigLambL, double lamb, int j) {
    tLeft = tL;
    tRight = tR;
    bigLambdaLeft = bigLambL;
    lambda = lamb;
    id = j;

    bigLambdaRight = bigLambdaLeft + (lambda * (tRight - tLeft)); // _____________ THIS LINE _______________

    // Error checking
    if (tLeft < 0) {
        cerr << "Error: tLeft is negative: " << tLeft << endl;
        exit(1);
    }
    if (tRight < 0) {
        cerr << "Error: tRight is negative: " << tRight << endl;
        exit(1);
    }
    if (tLeft > tRight) {
        cerr << "Error: tLeft (" << tLeft << ") greater than tRight (" << tRight << ")" << endl;
        exit(1);
    }

    if (bigLambdaLeft > bigLambdaRight) {
        cerr << "Error: bigLambdaLeft (" << bigLambdaLeft << ") greater than bigLambdaRight (" << bigLambdaRight << ")" << endl;
        exit(1);
    }
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