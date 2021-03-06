/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
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

#include <Modules/Legacy/Bundle/GetMatricesFromBundle.h>
#include <Interface/Modules/Bundle/GetMatricesFromBundleDialog.h>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
#include <Dataflow/Network/ModuleStateInterface.h>  //TODO: extract into intermediate

using namespace SCIRun::Gui;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Modules::Bundles;

GetMatricesFromBundleDialog::GetMatricesFromBundleDialog(const std::string& name, ModuleStateHandle state,
  QWidget* parent /* = 0 */)
  : ModuleDialogGeneric(state, parent)
{
  setupUi(this);
  setWindowTitle(QString::fromStdString(name));
  fixSize();
  addComboBoxManager(fieldComboBox1_, GetMatricesFromBundle::MatrixNames[0]);
  addComboBoxManager(fieldComboBox2_, GetMatricesFromBundle::MatrixNames[1]);
  addComboBoxManager(fieldComboBox3_, GetMatricesFromBundle::MatrixNames[2]);
  addComboBoxManager(fieldComboBox4_, GetMatricesFromBundle::MatrixNames[3]);
  addComboBoxManager(fieldComboBox5_, GetMatricesFromBundle::MatrixNames[4]);
  addComboBoxManager(fieldComboBox6_, GetMatricesFromBundle::MatrixNames[5]);
}

void GetMatricesFromBundleDialog::pullSpecial()
{
  auto names = transient_value_cast<std::vector<std::string>>(state_->getTransientValue(GetMatricesFromBundle::MatrixNameList.name()));
  if (matrixNames_ != names)
  {
    matrixNames_ = names;
    //TODO: put into an array, call functions.
    fieldComboBox1_->clear();
    fieldComboBox2_->clear();
    fieldComboBox3_->clear();
    fieldComboBox4_->clear();
    fieldComboBox5_->clear();
    fieldComboBox6_->clear();
    fieldObjectListWidget_->clear();

    for (const auto& name : names)
    {
      auto qname = QString::fromStdString(name);
      fieldObjectListWidget_->addItem(qname);
      fieldComboBox1_->addItem(qname);
      fieldComboBox2_->addItem(qname);
      fieldComboBox3_->addItem(qname);
      fieldComboBox4_->addItem(qname);
      fieldComboBox5_->addItem(qname);
      fieldComboBox6_->addItem(qname);
    }
    fieldComboBox1_->setCurrentIndex(fieldComboBox1_->findText(QString::fromStdString(state_->getValue(GetMatricesFromBundle::MatrixNames[0]).toString())));
    fieldComboBox2_->setCurrentIndex(fieldComboBox2_->findText(QString::fromStdString(state_->getValue(GetMatricesFromBundle::MatrixNames[1]).toString())));
    fieldComboBox3_->setCurrentIndex(fieldComboBox3_->findText(QString::fromStdString(state_->getValue(GetMatricesFromBundle::MatrixNames[2]).toString())));
    fieldComboBox4_->setCurrentIndex(fieldComboBox4_->findText(QString::fromStdString(state_->getValue(GetMatricesFromBundle::MatrixNames[3]).toString())));
    fieldComboBox5_->setCurrentIndex(fieldComboBox5_->findText(QString::fromStdString(state_->getValue(GetMatricesFromBundle::MatrixNames[4]).toString())));
    fieldComboBox6_->setCurrentIndex(fieldComboBox6_->findText(QString::fromStdString(state_->getValue(GetMatricesFromBundle::MatrixNames[5]).toString())));
  }
}
