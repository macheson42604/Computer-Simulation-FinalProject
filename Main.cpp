#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stdexcept>

using namespace std;

void output(double& trueArrival);

// TODO: we need a value to store t_K

int main (int argc, char* argv[]) {
    /// READ IN COMMAND LINE ARGUMENTS ///
    // File inputs
    string traceFileName = argv[1];
    string pieceFileName = argv[2];

    // Open input files
    openTraceFile(traceFileName); // all errors and calls handled in Trace.cpp

    ifstream pieceFile;
    pieceFile.open(pieceFileName);
    if (!pieceFile.is_open()) {
        cerr << "Error: could not open trace file: " << pieceFile << endl;
        exit(1);
    }
    
    // Q input
    try {
        int Q = stoi(argv[3]);

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

    /// INITIALIZATIONS ///
    double x, y;
    double area = 0;
    int i = 0;
    vector<double> xList, yList, yInput, cdf;

    /// RUN PROGRAM ///

    // Read in first set of values from piecewise file
    if (pieceFile.eof()) {
        cerr << "There are no values within piecewise input file: " << pieceFile << endl;
        exit();
    }

    pieceFile >> x >> y;
    xList.push_back(x);
    yInput.push_back(y);
    i ++;

    // Read in rest of values from piecewise file
    while (pieceFile >> x >> y) {
        area += ((yInput[i - 1] + y) / 2) * (x - xList[i - 1]);
        
        xList.push_back(x);
        yInput.push_back(y);
        i++;
    }


    // Normalize data to create pdf
    for (int j = 0; j < (int)yInput.size(); j ++) {
        yList.push_back(yInput[j] / area);
    }

    // Convert pdf to cdf
    double cdfArea = 0;
    cdf.push_back(0.0);
    for (int j = 1; j < (int)yList.size(); j ++) {
        cdfArea += ((yList[j] + yList[j - 1]) / 2) * (xList[j] - xList[j - 1]); // area of a trapazoid = ((a + b) / 2) * height, where a and b are the lengths of the parallel sides
        cdf.push_back(cdfArea);
    }

    // Find quadratic coefficiencts
    vector<double> a, b, c;
    for (int j = 1; j < xList.size(); j++) {
        double slope = (yList[j] - yList[j - 1]) / (xList[j] - xList[j - 1]);

        double Ai = slope / 2;
        double Bi = yList[j - 1] - slope * xList[j - 1];
        double Ci = -(Ai * xList[j - 1] * xList[j - 1] + Bi * xList[j - 1]);

        a.push_back(Ai);
        b.push_back(Bi);
        c.push_back(Ci);
    }

    // Generate arrivals based on cdf and coefficient equations
    for (int n = 0; n < Q; n ++) {
        double u = get_traceValue();
        double uPrime = -1.0;
        int index = -1;

        for (int j = 1; j < (int)cdf.size(); j ++) {
            if (u < cdf[j]) {
                index = j;
                break;
            }
        }

        // Error check that ish - ensure an index was found
        if (index == -1) {
            cerr << "Error: no index was found where u < cdf_i" << endl;
            exit(1);
        }

        uPrime = u - cdf[index - 1];

        // Error check that ish - ensure a uPrime was found
        if (uPrime < 0) {
            cerr << "Error: u' was incorrectly calculated to: " << uPrime << endl;
            exit(1);
        }

        // TODO: create function for these calucations
        double arrival1 = ( (-1) * b[index - 1] + pow(pow(b[index - 1], 2) - (4 * a[index - 1] * (c[index - 1] - uPrime)), 0.5) ) / (2 * a[index - 1]);
        double arrival2 = ( (-1) * b[index - 1] - pow(pow(b[index - 1], 2) - (4 * a[index - 1] * (c[index - 1] - uPrime)), 0.5) ) / (2 * a[index - 1]);

        // Choose which value to keep
        if (arrival1 >= xList[index - 1] && arrival1 <= xList[index]) {
            output(arrival1);
        } else {
            output(arrival2);
        }
    }
    
    return 0;
}

// TODO: print outputs
void output(double& trueArrival) {

}