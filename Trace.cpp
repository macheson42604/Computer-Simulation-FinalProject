#include <iostream>
#include <fstream>
#include <string>

#include "Trace.h"

using namespace std;

ifstream traceFileStream;

void open_trace_file(string& traceFileName) {
    traceFileStream.open(traceFileName);
    if (!traceFileStream.is_open()) {
        cerr << "Error: could not open trace file: " << traceFileName << endl;
        exit(1);
    }
}

double get_traceValue() {
    // get value
    double traceValue = -1;
    if (!(traceFileStream >> traceValue)) {
        cerr << "Error: get_traceValue() executed incorrectly" << endl;
        exit(1);
    }

    // First check that we have a value to give
    if (traceFileStream.eof()) {
        cerr << "Error: not enough values in trace file" << endl;
        exit(1);
    }

    return traceValue;
 }