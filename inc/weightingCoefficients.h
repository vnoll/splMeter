// A weighting filter consists of 3 cascaded IIR filters

// first IIR:
#define A_WEIGHT_1_A0 0.23442154
#define A_WEIGHT_1_A1 0.46884308
#define A_WEIGHT_1_A2 0.23442154
#define A_WEIGHT_1_B0 1
#define A_WEIGHT_1_B1 -0.22408852
#define A_WEIGHT_1_B2 0.01255391

// second IIR:
#define A_WEIGHT_2_A0 1
#define A_WEIGHT_2_A1 -2.00016423
#define A_WEIGHT_2_A2 1.00016424
#define A_WEIGHT_2_B0 1
#define A_WEIGHT_2_B1 -1.89386673
#define A_WEIGHT_2_B2 0.89515653

// third IIR:
#define A_WEIGHT_3_A0  1
#define A_WEIGHT_3_A1  -1.99983577
#define A_WEIGHT_3_A2 0.99983578
#define A_WEIGHT_3_B0 1
#define A_WEIGHT_3_B1 -1.99461557 
#define A_WEIGHT_3_B2 0.99462281

// C weighting filter consists of 2 cascaded IIR filters

// first IIR:
#define C_WEIGHT_1_A0 0.19799054
#define C_WEIGHT_1_A1 0.39598108
#define C_WEIGHT_1_A2 0.19799054
#define C_WEIGHT_1_B0 1
#define C_WEIGHT_1_B1 -0.2240885  
#define C_WEIGHT_1_B2 0.01255391

// second IIR:
#define C_WEIGHT_2_A0 1
#define C_WEIGHT_2_A1 -2
#define C_WEIGHT_2_A2 1
#define C_WEIGHT_2_B0 1
#define C_WEIGHT_2_B1 -1.99461559 
#define C_WEIGHT_2_B2 0.99462284