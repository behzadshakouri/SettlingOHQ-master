#include "modelcreator.h"
#include "System.h"
#include "QString"

ModelCreator::ModelCreator()
{

}


bool ModelCreator::Create(model_parameters mp, System *system)
{
    system->GetQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/main_components.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/unsaturated_soil.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/Well.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/wastewater.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/mass_transfer.json");
    system->ReadSystemSettingsTemplate("/home/behzad/Projects/OpenHydroQual/resources/settings.json");

    //cout<<"Constituent"<<endl;
    Constituent ct;
    ct.SetQuantities(system->GetMetaModel(), "Constituent");
    //ct.SetVal("concentration",mp.concentration);
    //ct.SetVal("diffusion_coefficient",mp.diffusion_coefficient);
    //ct.SetVal("stoichiometric_constant",mp.stoichiometric_constant);
    ct.SetName("Constituent_1");
    ct.SetType("Constituent");
    system->AddConstituent(ct,false);


    //cout<<"Reaction_parameter"<<endl;
    RxnParameter Rxparam;
    Rxparam.SetQuantities(system,"ReactionParameter");
    Rxparam.SetName("K");
    Rxparam.SetVal("base_value",0.1);
    system->AddReactionParameter(Rxparam, false);

    cout<<"Reaction"<<endl;
    Reaction R1;
    R1.SetQuantities(system,"Reaction");
    R1.SetName("R1");
    R1.SetProperty("Constituent_1:stoichiometric_constant","-1");
    R1.SetProperty("rate_expression","K*Constituent_1");
    system->AddReaction(R1,false);



    //cout<<"Well"<<endl;
    Block well;
    well.SetQuantities(system->GetMetaModel(), "Well_aggregate");
    well.SetName("Well");
    well.SetType("Well_aggregate");
    well.SetVal("_height",mp.DepthofWell*4000);
    well.SetVal("_width",mp.rw*4000);
    well.SetVal("bottom_elevation",-mp.DepthofWell);
    well.SetVal("diameter",mp.rw*2);
    well.SetVal("depth",0.5);
    well.SetVal("porosity",1);
    well.SetVal("Constituent_1:concentration",1);
    well.SetVal("x",-mp.rw*4000);
    well.SetVal("y",0);
    system->AddBlock(well,false);


    cout<<"Populate functions"<<endl;
    system->PopulateOperatorsFunctions();
    cout<<"Variable parents"<<endl;
    system->SetVariableParents();
    return true;
}
