#ifndef TRACE_H
#define TRACE_H

#include <fstream>
#include <string>
#include <fstream>

using namespace std;

/*
TRACE: NOT A CLASS
    Purpose: Retrieve random variates either from a trace file or endless stream

    Public: 
        - open_trace_file(string) (void): method
            opens trace file passed as the parameter
        - get_traceValue() (double): method
            returns random variate (between 0 and 1)

*/

extern ifstream traceStream;

void open_trace_file(string&);
double get_traceValue();
double exponential();

#endif
