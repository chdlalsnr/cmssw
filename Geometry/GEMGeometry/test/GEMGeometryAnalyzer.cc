
/** Derived from DTGeometryAnalyzer by Nicola Amapane
 *
 *  \author M. Maggi - INFN Bari
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include "DataFormats/Math/interface/deltaPhi.h"

#include <memory>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <set>

class GEMGeometryAnalyzer : public edm::one::EDAnalyzer<> {

public:
  GEMGeometryAnalyzer(const edm::ParameterSet& pset);
  ~GEMGeometryAnalyzer() override;

  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}

  static bool sortCh(const GEMSuperChamber &a, const GEMSuperChamber &b){
    if (a.id().region() == b.id().region()){
      if (a.id().station() == b.id().station()){
        if (a.id().superChamberId() == b.id().superChamberId()){
          if (a.id().chamber() == b.id().chamber()){
            return a.id().layer() < b.id().layer();
          }else{ return a.id().chamber() < b.id().chamber();}
        }else{ return a.id().superChamberId() < b.id().superChamberId();}
      }else{ return a.id().station() < b.id().station();}
    }else{ return a.id().region() < b.id().region();}
  };

private:
  const std::string& myName() { return myName_; }

  const int dashedLineWidth_;
  const std::string dashedLine_;
  const std::string myName_;
  std::ofstream ofos;
  std::vector<GEMSuperChamber> gemChambers_;
};

using namespace std;
GEMGeometryAnalyzer::GEMGeometryAnalyzer(const edm::ParameterSet& /*iConfig*/)
    : dashedLineWidth_(104), dashedLine_(std::string(dashedLineWidth_, '-')), myName_("GEMGeometryAnalyzer") {
  ofos.open("GEMtestOutput.out");
  ofos << "======================== Opening output file" << std::endl;
}

GEMGeometryAnalyzer::~GEMGeometryAnalyzer() {
  ofos.close();
  ofos << "======================== Closing output file" << std::endl;
}

void GEMGeometryAnalyzer::analyze(const edm::Event& /*iEvent*/, const edm::EventSetup& iSetup) {
  edm::ESHandle<GEMGeometry> pDD;
  iSetup.get<MuonGeometryRecord>().get(pDD);

  // checking uniqueness of roll detIds
  int nstrips = 0;
  int npads = 0;
  for (auto roll1 : pDD->etaPartitions()) {
    nstrips += roll1->nstrips();
    npads += roll1->npads();
  }

  for (auto sch : pDD->superChambers()){
    gemChambers_.push_back(*sch);
  }
  sort(gemChambers_.begin(),gemChambers_.end(),sortCh);

  //----------------------- Global GEMGeometry TEST -------------------------------------------------------
  ofos << myName() << "Begin GEMGeometry structure TEST" << endl;
  ofos << " " << std::endl;

  for (auto sch : gemChambers_) {
    GEMDetId schId(sch.id());

    for (auto ch : sch.chambers()) {
      GEMDetId chId(ch->id());

      for (auto roll : ch->etaPartitions()) {
        GEMDetId rId(roll->id());

        const BoundPlane& bSurface(roll->surface());
        const StripTopology* topology(&(roll->specificTopology()));
        auto& parameters(roll->specs()->parameters());
        float bottomEdge(parameters[0]);
        float topEdge(parameters[1]);
        float height(parameters[2]);
        float nStrips(parameters[3]);
        float nPads(parameters[4]);

        LocalPoint lCentre(0., 0., 0.);
        GlobalPoint gCentre(bSurface.toGlobal(lCentre));

              /*LocalPoint lTop(0., height, 0.);
              GlobalPoint gTop(bSurface.toGlobal(lTop));

              LocalPoint lBottom(0., -height, 0.);
              GlobalPoint gBottom(bSurface.toGlobal(lBottom));*/

              ofos << roll->id() << "nStrip: " << nStrips << ", 1stStrip: " << roll->toGlobal(roll->centreOfStrip(0)).phi().degrees() << " deg, lastStrip: " << roll->toGlobal(roll->centreOfStrip(roll->nstrips())).phi().degrees() << " deg, stripJustBeforeTheLast: " << roll->toGlobal(roll->centreOfStrip(roll->nstrips()-1)).phi().degrees() << " deg." << std::endl;
      }
    }
  }

  ofos << dashedLine_ << " end" << std::endl;
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GEMGeometryAnalyzer);
