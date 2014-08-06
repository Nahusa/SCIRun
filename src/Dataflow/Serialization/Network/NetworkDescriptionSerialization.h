/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
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

/// @todo Documentation Dataflow/Serialization/Network/NetworkDescriptionSerialization.h


#ifndef CORE_SERIALIZATION_NETWORK_NETWORK_DESCRIPTION_SERIALIZATION_H
#define CORE_SERIALIZATION_NETWORK_NETWORK_DESCRIPTION_SERIALIZATION_H 

#include <Dataflow/Serialization/Network/ModuleDescriptionSerialization.h>
#include <Dataflow/Serialization/Network/StateSerialization.h>
#include <Dataflow/Serialization/Network/ModulePositionGetter.h>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/version.hpp>
#include <Dataflow/Serialization/Network/share.h>

namespace SCIRun {
namespace Dataflow {
namespace Networks {

  typedef std::vector<ConnectionDescriptionXML> ConnectionsXML;

  struct SCISHARE ModuleWithState 
  {
    ModuleLookupInfoXML module;
    State::SimpleMapModuleStateXML state;
    ModuleWithState(const ModuleLookupInfoXML& m = ModuleLookupInfoXML(), const State::SimpleMapModuleStateXML& s = State::SimpleMapModuleStateXML()) : module(m), state(s) {}
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(module);
      ar & BOOST_SERIALIZATION_NVP(state);
    } 
  };

  struct SCISHARE ModuleNoteXML
  {
    std::string noteHTML, noteText;
    int position, fontSize;
    //, colorR, colorG, colorB;
    ModuleNoteXML(const std::string& html = "", int p = 0, const std::string& text = "", int f = 12) :
      noteHTML(html), noteText(text), position(p), fontSize(f)/*, colorR(r), colorG(g), colorB(b)*/ {}
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(noteHTML);
      ar & BOOST_SERIALIZATION_NVP(noteText);
      ar & BOOST_SERIALIZATION_NVP(position);
      ar & BOOST_SERIALIZATION_NVP(fontSize);
//       ar & BOOST_SERIALIZATION_NVP(colorR);
//       ar & BOOST_SERIALIZATION_NVP(colorG);
//       ar & BOOST_SERIALIZATION_NVP(colorB);
    } 
  };

  typedef std::map<std::string, ModuleWithState> ModuleMapXML;
  typedef std::map<std::string, ModuleNoteXML> ModuleNotesMapXML;

  struct SCISHARE ModuleNotes
  {
    ModuleNotesMapXML notes;
  };

  class SCISHARE NetworkXML
  {
  public:
    ModuleMapXML modules;
    ConnectionsXML connections;
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(modules);
      ar & BOOST_SERIALIZATION_NVP(connections);
    } 
  };

  struct SCISHARE NetworkFile 
  {
    NetworkXML network;
    ModulePositions modulePositions;
    ModuleNotes moduleNotes;
    //ConnectionNotesXML connectionNotes;
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & boost::serialization::make_nvp("networkInfo", network);
      ar & boost::serialization::make_nvp("modulePositions", modulePositions.modulePositions);
      if (version > 0)
        ar & boost::serialization::make_nvp("moduleNotes", moduleNotes.notes);
    }
  };

}}}

BOOST_CLASS_VERSION(SCIRun::Dataflow::Networks::NetworkFile, 1)

#endif
