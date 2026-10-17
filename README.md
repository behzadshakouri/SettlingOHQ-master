# SettlingOHQ

## About

SettlingOHQ is a C++/Qt research utility for creating and evaluating particle-
or solids-settling models with OpenHydroQual. It includes model-generation
code, result-grid handling, and VTK export support.

Generated projects and their settling assumptions, units, boundary conditions,
and numerical settings should be reviewed in OpenHydroQual before use.

## OpenHydroQual resources

| Resource | Link |
|---|---|
| Project website | [openhydroqual.com](https://openhydroqual.com/) |
| Documentation and tutorials | [OpenHydroQual knowledge base](https://openhydroqual.com/knowledge-base/) |
| Searchable examples | [examples.openhydroqual.com](https://examples.openhydroqual.com/) |
| Example source repository | [ArashMassoudieh/OpenHydroQual-Examples](https://github.com/ArashMassoudieh/OpenHydroQual-Examples) |
| Source code and releases | [ArashMassoudieh/OpenHydroQual](https://github.com/ArashMassoudieh/OpenHydroQual) |
| Video tutorials | [OpenHydroQual on YouTube](https://www.youtube.com/@openhydroqual218) |

## Repository contents

- `modelcreator.cpp/.h`: OpenHydroQual model construction;
- `resultgrid.cpp/.h`: simulation-result organization;
- `vtk.h`: visualization/export support;
- `main.cpp`: application entry point; and
- `SettlingOHQ.pro`: Qt/qmake project definition.

## Building

Open `SettlingOHQ.pro` in a compatible Qt environment, configure the required
local OpenHydroQual resources, and build the application.

## Status

This is research software. Validate settling formulations and parameters
against the intended system before quantitative use.
