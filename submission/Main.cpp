#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "Trace.h"
#include "Interval.hpp"

using namespace std;


void output(double t_K, double nextAlpha) {
    // TODO: check if this is producing 6 sig figs
    // showpoint shows tailing zeros
    double eps = 0.0000005; // printing threshold

    double wrappedTime = nextAlpha - t_K * floor(nextAlpha/t_K);

    if (wrappedTime >= t_K) {
        cerr << "Error: wrappedTIme >= t_K: " << wrappedTime << endl;
        exit(1);
    }
    
    // edge cases if there is a floating point error
    if (wrappedTime >= t_K - eps) {
        wrappedTime = 0.0;
    }
    if (fabs(wrappedTime) < eps) {
        wrappedTime = 0.0;
    }


    cout << "OUTPUT " << setprecision(6) << showpoint << nextAlpha;
    // TODO: check the wrapped arrival time
    cout << " " << wrappedTime << endl;
    // Previous fmod() function: cout << " " << fmod(nextAlpha, t_K) << endl; 
   
}



int main (int argc, char* argv[]) {
    // -----------------------------------------------------------------------------
    // INITIALIZE ALL IMPORTANT VARIABLES
    // -----------------------------------------------------------------------------
    int Q = -1; // initialize to -1 for error detection
    // will contain all of the arrival times (by the end, the size of the vector should be the same as Q)
    vector<double> alphaTimes; 
    // vector of all the interval times
    vector<Interval> intervals;
    double t_K = -1.0;
    double bigLambda_t_K = 0.0;


    // -----------------------------------------------------------------------------
    // READ IN COMMAND LINE ARGUMENT INFO
    // -----------------------------------------------------------------------------
    // File inputs
    if (argc != 4) {
        cerr << "Error: incorrect number of command line arguments. There are only " << argc << " arguments passed" << endl;
        exit(1);
    }
    string traceFileName = argv[1];
    string pieceFileName = argv[2];
    // Open input files
    open_trace_file(traceFileName); // all errors and calls handled in Trace.cpp

    ifstream piecewiseFile;
    piecewiseFile.open(pieceFileName);
    if (!piecewiseFile.is_open()) {
        cerr << "Error: could not open trace file: " << pieceFileName << endl;
        exit(1);
    }
    
    // Set Q value
    try {
        Q = stoi(argv[3]);
        // Check that input Q is within range
        if (Q <= 0) {
            cerr << "Error: Q must be larger than 0, you input: " << Q << endl;
            exit(1);
        }
    } catch (const invalid_argument& e) {
        // NaN
        cerr << "Error: Invalid argument error caught: " << e.what() << endl;
        exit(1);
    } catch (const out_of_range& e) {
        // Number is too big/small for 'int'
        cerr << "Error: Out of range error caught: " << e.what() << endl;
        exit(1);
    }
    

    // -----------------------------------------------------------------------------
    // READ IN FILE TO CREATE INTERVAL OBJECTS
    // -----------------------------------------------------------------------------
    int i = 0; // counter to include in each of the interval objects
    double t_i = 0.0;
    double bigLambda_i = 0.0;
    double t_i1, lambda;
    while (piecewiseFile >> t_i1 >> lambda) {
        Interval interval(t_i, t_i1, bigLambda_i, lambda, i);
        intervals.push_back(interval);


        if (t_i1 < t_i) {
            cerr << "Error: t_i+1 is less than t_i" << endl;
            exit(1);
        }
        
        bigLambda_i += lambda * (t_i1 - t_i); // _____________ THIS LINE _______________
        t_i = t_i1;
        i ++;
    }
    
    // Set t_K to the last read in value;
    if (intervals.size() == 0) {
        cerr << "Error: there are no intervals provided in file" << endl;
        exit(1);
    }

    t_K = intervals[intervals.size() - 1].get_tRight();
    bigLambda_t_K = intervals[intervals.size() - 1].get_bigLambdaRight();


    // -----------------------------------------------------------------------------
    // LOOP UNTIL ALL Q ARRIVALS HAVE BEEN CREATED
    // -----------------------------------------------------------------------------
    // additional variables that will be used and changed within the while loop
    double a_i = 0.0; // a_i
    double a_i1 = 0.0; // a_{i+1}
    double alpha_i = 0.0; // alpha_i (we can still use this variable to represent alpha_{i+1})
    int j_i = 0;
    int j_i1 = -1;
    double currBigLambda = 0.0;
    double nextBigLambda = 0.0;
    int w = 0; 
    
    while ((int)alphaTimes.size() < Q) {
        // implement the algorithm provided in page 7
        a_i = alpha_i - (floor(alpha_i / t_K)* t_K);
        
        // set correct interval
        j_i = 0;
        while (j_i < (int)intervals.size() && a_i > intervals[j_i].get_tRight()) {
            j_i ++;
        }

        if (j_i > (int)intervals.size() - 1) {
            cerr << "j_i out of range: " << j_i << endl;
            exit(1);
        }
    
        // calculate the correct total arrivals for a_i and a_i1
        currBigLambda = intervals[j_i].get_bigLambdaLeft() + (intervals[j_i].get_lambda() * (a_i - intervals[j_i].get_tLeft()));
        nextBigLambda = currBigLambda + exponential();
    
        // calculate the wrap around
        w = floor(nextBigLambda / bigLambda_t_K);
        // decremenent nextBigLambda by w * bigLambda_t_K in order to get correct adjested total number of arrivals for given next arrival time
        nextBigLambda -= w * bigLambda_t_K;

        // fix the correct interval if needed
        if (nextBigLambda < intervals[j_i].get_bigLambdaLeft()) {
            j_i = 0;
        }

        // find the next interval
        j_i1 = j_i; // start off in current interval
        while (j_i1 < (int)intervals.size() && nextBigLambda > intervals[j_i1].get_bigLambdaRight()) {
            j_i1 ++;
        }

        if (j_i1 > (int)intervals.size() - 1) {
            cerr << "j_i1 out of range: " << j_i1 << endl;
            exit(1);
        }
        
        // set to previous time if lambda was 0
        a_i1 = a_i;
        if (intervals[j_i1].get_lambda() != 0.0) {
            a_i1 = ((nextBigLambda - intervals[j_i1].get_bigLambdaLeft()) / intervals[j_i1].get_lambda()) + intervals[j_i1].get_tLeft();
        }        

        double epsilon = 1e-9;
        if (a_i1 >= t_K - epsilon) {
            a_i1 = 0.0;  // Wrap to start of cycle
        }

        // get alpha_i1
        alpha_i = alpha_i + (a_i1 - a_i) + (t_K * w);
        // edge case for the first time alpha_i is reaching t_K
        if (alpha_i - t_K < epsilon) {
            alpha_i = t_K;
        }


        alphaTimes.push_back(alpha_i);
        // output
        output(t_K, alpha_i);
    }

    // check the the lists are the size of Q
    if ((int)alphaTimes.size() != Q) {
        cerr << "Error: Q number of actual arrival times not calculated. " << alphaTimes.size() << " number of actual arrival times calculated" << endl;
        exit(1);
    }

    return 0;
}

