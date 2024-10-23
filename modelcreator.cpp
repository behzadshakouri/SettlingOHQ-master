#include "modelcreator.h"
#include "System.h"
#include "QString"

ModelCreator::ModelCreator()
{

}


bool ModelCreator::Create(model_parameters, System *system)
{
    system->GetQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/main_components.json");
    //system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/unsaturated_soil.json");
    //system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/Well.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/wastewater.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/mass_transfer.json");
    system->ReadSystemSettingsTemplate("/home/behzad/Projects/OpenHydroQual/resources/settings.json");

    Constituent Solids;
    Solids.SetQuantities(system, "Constituent");
    Solids.SetName("Solids");
    Solids.SetType("Constituent");
    system->AddConstituent(Solids,false);

    Constituent Coagulant;
    Coagulant.SetQuantities(system, "Constituent");
    Coagulant.SetName("Coagulant");
    Coagulant.SetType("Constituent");
    system->AddConstituent(Coagulant,false);

    Constituent Settled_Particles;
    Settled_Particles.SetQuantities(system, "Immobile_Constituent");
    Settled_Particles.SetName("Settled_Particles");
    Settled_Particles.SetType("Immobile_Constituent");
    system->AddConstituent(Settled_Particles,false);

    //RxnParameter Rxparam_K;
    //Rxparam_K.SetQuantities(system,"ReactionParameter");
    //Rxparam_K.SetName("K");
    //Rxparam_K.SetVal("base_value",0.1);
    //system->AddReactionParameter(Rxparam_K, false);

    RxnParameter Rxparam_SRC;
    Rxparam_SRC.SetQuantities(system,"ReactionParameter");
    Rxparam_SRC.SetName("Settling_rate_coeff");
    Rxparam_SRC.SetVal("base_value",10);
    system->AddReactionParameter(Rxparam_SRC, false);

    RxnParameter Rxparam_Ks;
    Rxparam_Ks.SetQuantities(system,"ReactionParameter");
    Rxparam_Ks.SetName("Ks");
    Rxparam_Ks.SetVal("base_value",0.5);
    system->AddReactionParameter(Rxparam_Ks, false);

    Reaction Settling;
    Settling.SetQuantities(system,"Reaction");
    Settling.SetName("Settling");
    Settling.SetProperty("Coagulant:stoichiometric_constant","0");
    Settling.SetProperty("Settled_Particles:stoichiometric_constant","1");
    Settling.SetProperty("Solids:stoichiometric_constant","-1");
    Settling.SetProperty("rate_expression","Settling_rate_coeff*Solids*Coagulant/(Coagulant+Ks)");
    system->AddReaction(Settling,false);


    Block Stl_element;
    Stl_element.SetQuantities(system, "Settling element");
    Stl_element.SetName("Settling element (1)");
    Stl_element.SetType("Settling element");
    Stl_element.SetVal("Coagulant:concentration",0);

    CTimeSeries<double> CoagNS;
    CoagNS.CreateOUProcess(0,100,0.05,1.5);
    vector<double> c_params; c_params.push_back(2.5); c_params.push_back(0.6);
    CTimeSeries<double> Coag = CoagNS.MapfromNormalScoreToDistribution("lognormal", c_params);
    //Reactor.Variable("Coagulant:external_mass_flow_timeseries")->SetTimeSeries(Coag);
    Coag.writefile("/home/behzad/Projects/SettlingOHQ-master/Settling/coagulant_mfr.csv");
    Stl_element.SetProperty("Coagulant:external_mass_flow_timeseries","/home/behzad/Projects/SettlingOHQ-master/Settling/coagulant_mfr.csv");

    //Stl_element.SetProperty("Coagulant:external_mass_flow_timeseries","/home/behzad/Projects/SettlingOHQ-master/Settling/coagulant_mfr.txt");

    Stl_element.SetVal("Settled_Particles:concentration",0);
    Stl_element.SetVal("Solids:concentration",0);
    Stl_element.SetVal("Storage",20);
    Stl_element.SetVal("bottom_elevation",0);
    Stl_element.SetVal("x",800);
    Stl_element.SetVal("y",800);
    system->AddBlock(Stl_element,false);

    Block f_head;
    f_head.SetQuantities(system, "fixed_head");
    f_head.SetName("fixed_head (1)");
    f_head.SetType("fixed_head");
    f_head.SetVal("Coagulant:concentration",0);
    f_head.SetVal("Settled_Particles:concentration",0);
    f_head.SetVal("Solids:concentration",0);
    f_head.SetVal("Storage",100000);
    f_head.SetVal("x",1000);
    f_head.SetVal("y",400);
    system->AddBlock(f_head,false);

    Block Reactor;
    Reactor.SetQuantities(system, "Reactor");
    Reactor.SetName("Reactor (1)");
    Reactor.SetType("Reactor");
    Reactor.SetVal("Coagulant:concentration",0);
    Reactor.SetVal("Settled_Particles:concentration",0);
    Reactor.SetVal("Solids:concentration",0);
    Reactor.SetVal("Storage",0.1);
    Reactor.SetVal("x",600);
    Reactor.SetVal("y",400);

    CTimeSeries<double> SolidConcentrationNS;
    SolidConcentrationNS.CreateOUProcess(0,100,0.05,1);
    vector<double> params; params.push_back(3); params.push_back(1);
    CTimeSeries<double> SolidConcentration = SolidConcentrationNS.MapfromNormalScoreToDistribution("lognormal", params);
    //Reactor.Variable("Solids:inflow_concentration")->SetTimeSeries(SolidConcentration);
    SolidConcentration.writefile("/home/behzad/Projects/SettlingOHQ-master/Settling/inflow_concentration.csv");
    Reactor.SetProperty("Solids:inflow_concentration","/home/behzad/Projects/SettlingOHQ-master/Settling/inflow_concentration.csv");

    //Reactor.SetProperty("Solids:inflow_concentration","/home/behzad/Projects/SettlingOHQ-master/Settling/inflow_concentration.txt");

    /*CTimeSeries<double> InflowNS;
    InflowNS.CreateOUProcess(0,100,0.05,0.5);
    vector<double> i_params; i_params.push_back(1.5); i_params.push_back(0.7);
    CTimeSeries<double> Inflow = InflowNS.MapfromNormalScoreToDistribution("lognormal", i_params);
    //Reactor.Variable("inflow")->SetTimeSeries(Inflow);
    Inflow.writefile("/home/behzad/Projects/SettlingOHQ-master/Settling/inflow.csv");
    Reactor.SetProperty("inflow","/home/behzad/Projects/SettlingOHQ-master/Settling/inflow.csv");
    */

    Reactor.SetProperty("inflow","/home/behzad/Projects/SettlingOHQ-master/Settling/inflow.txt");

    system->AddBlock(Reactor,false);
    //system->block("Reactor (1)")->SetProperty("Solids:inflow_concentration","/home/behzad/Projects/SettlingOHQ-master/Settling/inflow_concentration.txt");
    //system->block("Reactor (1)")->SetProperty("inflow","/home/behzad/Projects/SettlingOHQ-master/Settling/inflow.txt");

    Link link1;
    link1.SetQuantities(system, "Fixed flow");
    link1.SetName("Reactor (1) - Settling element (1)");
    link1.SetType("Fixed flow");
    link1.SetVal("flow", 10);
    system->AddLink(link1, "Reactor (1)", "Settling element (1)", false);

    Link link2;
    link2.SetQuantities(system, "Fixed flow");
    link2.SetName("Settling element (1) - fixed_head (1)");
    link2.SetType("Fixed flow");
    link2.SetVal("flow", 10);
    system->AddLink(link2, "Settling element (1)", "fixed_head (1)", false);

    cout<<"Populate functions"<<endl;
    system->PopulateOperatorsFunctions();
    cout<<"Variable parents"<<endl;
    system->SetVariableParents();
    return true;
}
