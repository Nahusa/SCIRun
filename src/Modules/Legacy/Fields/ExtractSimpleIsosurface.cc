/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2009 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include <Modules/Legacy/Fields/ExtractSimpleIsosurface.h>
#include <Core/Algorithms/Legacy/Fields/MeshDerivatives/ExtractSimpleIsosurfaceAlgo.h>
#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/Legacy/Field/Field.h>

using namespace SCIRun;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Modules::Fields;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Algorithms::Fields;

ExtractSimpleIsosurfaceModule::ExtractSimpleIsosurfaceModule()
  : Module(ModuleLookupInfo("ExtractSimpleIsosurface", "NewField", "SCIRun"))
{
  INITIALIZE_PORT(InputField);
  INITIALIZE_PORT(Isovalue);
  INITIALIZE_PORT(OutputField);
}

void ExtractSimpleIsosurfaceModule::setStateDefaults()
{
  setStateDoubleFromAlgo(Parameters::SingleIsoValue);
  setStateStringFromAlgo(Parameters::ListOfIsovalues);
  setStateIntFromAlgo(Parameters::QuantityOfIsovalues);
}

void ExtractSimpleIsosurfaceModule::execute()
{
  FieldHandle field = getRequiredInput(InputField);
  auto isovalueOption = getOptionalInput(Isovalue);

  if (needToExecute())
  {
    update_state(Executing);
    auto state = get_state();

    if (isovalueOption && *isovalueOption && !(*isovalueOption)->empty())
    {
      //TODO: pass entire first column for multiple isovalues
      double iso = (*isovalueOption)->get(0,0);
      state->setValue(Parameters::SingleIsoValue, iso);
    }

    VariableList isos;

    if (state->getValue(Parameters::IsovalueChoice).toString() == "Single")
    {
      auto singleIso = state->getValue(Parameters::SingleIsoValue).toDouble();
      isos.push_back(makeVariable("", singleIso));
    }
    else if (state->getValue(Parameters::IsovalueChoice).toString() == "List")
    {
      auto singleIso = state->getValue(Parameters::SingleIsoValue).toDouble();
      isos.push_back(makeVariable("", singleIso));
      std::cout << "added single isoval" << std::endl;
    }
    if (state->getValue(Parameters::IsovalueChoice).toString() == "Quantity")
    {
      auto singleIso = state->getValue(Parameters::SingleIsoValue).toDouble();
      isos.push_back(makeVariable("", singleIso));
      std::cout << "added single isoval" << std::endl;
    }

    algo().set(Parameters::Isovalues, isos);

    auto output = algo().run_generic(withInputData((InputField, field)));
    sendOutputFromAlgorithm(OutputField, output);
  }
}
