#ifndef INTERVAL_HPP
#define INTERVAL_HPP

class Interval {
    private:
        double tLeft;
        double tRight;
        double bigLambdaLeft;
        double bigLambdaRight;
        double lambda;
        int id;

    public:
        // Constructor
        Interval(double, double, double, double, int);

        // Getters
        double get_tLeft() const;
        double get_tRight() const;
        double get_bigLambdaLeft() const;
        double get_bigLambdaRight() const;
        double get_lambda() const;
        int get_id() const;
};

#endif 