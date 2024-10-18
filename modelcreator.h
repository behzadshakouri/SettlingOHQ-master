#ifndef MODELCREATOR_H
#define MODELCREATOR_H

class System;

struct model_parameters
{

    double concentration;
    double diffusion_coefficient;
    double stoichiometric_constant;

    double DepthtoGroundWater = 2;
    double DepthofWell = 1;
    double RadiousOfInfluence = 1.5;
    int nr = 10;
    int nz = 10;
    double K_sat;
    double alpha;
    double n;
    double rw;
    double theta_sat;
    double theta_r;
    double initial_theta;
};

class ModelCreator
{
public:
    ModelCreator();
    bool Create(model_parameters mp, System *system);

private:
    const double pi = 3.141521;
};

#endif // MODELCREATOR_H
