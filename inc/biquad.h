#ifndef SPLMETER_BIQUAD_H
#define SPLMETER_BIQUAD_H

class Biquad {

    public:
        Biquad(double a0, double a1,  double a2, double b0, double b1, double b2) {
            this->a0 = a0;
            this->a1 = a1;
            this->a2 = a2;
            this->b0 = b0;
            this->b1 = b1;
            this->b2 = b2;
            z1 = z2 = 0.0;
     }

    double filter(double in) ;
    void resetZ();
    private:
        double a0, a1, a2, b0, b1, b2;
        double z1, z2;

};

#endif //SPLMETER_BIQUAD_H