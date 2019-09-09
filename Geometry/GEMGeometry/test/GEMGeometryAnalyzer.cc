
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

#include <math.h>

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

  ofos << myName() << ": Analyzer..." << std::endl;
  ofos << "start " << dashedLine_ << std::endl;

  ofos << " Geometry node for GEMGeom is  " << &(*pDD) << endl;
  ofos << " detTypes       \t" << pDD->detTypes().size() << endl;
  ofos << " GeomDetUnit       \t" << pDD->detUnits().size() << endl;
  ofos << " GeomDet           \t" << pDD->dets().size() << endl;
  ofos << " GeomDetUnit DetIds\t" << pDD->detUnitIds().size() << endl;
  ofos << " eta partitions \t" << pDD->etaPartitions().size() << endl;
  ofos << " chambers       \t" << pDD->chambers().size() << endl;
  ofos << " super chambers  \t" << pDD->superChambers().size() << endl;
  ofos << " rings  \t\t" << pDD->rings().size() << endl;
  ofos << " stations  \t\t" << pDD->stations().size() << endl;
  ofos << " regions  \t\t" << pDD->regions().size() << endl;

  // checking uniqueness of roll detIds
  bool flagNonUniqueRollID = false;
  bool flagNonUniqueRollRawID = false;
  int nstrips = 0;
  int npads = 0;
  for (auto roll1 : pDD->etaPartitions()) {
    nstrips += roll1->nstrips();
    npads += roll1->npads();
    for (auto roll2 : pDD->etaPartitions()) {
      if (roll1 != roll2) {
        if (roll1->id() == roll2->id())
          flagNonUniqueRollID = true;
        if (roll1->id().rawId() == roll2->id().rawId())
          flagNonUniqueRollRawID = true;
      }
    }
  }
  // checking the number of strips and pads
  ofos << " total number of strips\t" << nstrips << endl;
  ofos << " total number of pads  \t" << npads << endl;
  if (flagNonUniqueRollID or flagNonUniqueRollRawID)
    ofos << " -- WARNING: non unique roll Ids!!!" << endl;

  // checking uniqueness of chamber detIds
  bool flagNonUniqueChID = false;
  bool flagNonUniqueChRawID = false;
  for (auto sch1 : pDD->superChambers()) {
    gemChambers_.push_back(*sch1);
    for (auto sch2 : pDD->superChambers()) {
      if (sch1 != sch2) {
        if (sch1->id() == sch2->id()) flagNonUniqueChID = true;
        if (sch1->id().rawId() == sch2->id().rawId()) flagNonUniqueChRawID = true;
      }
    }
  }
  sort(gemChambers_.begin(), gemChambers_.end(), sortCh);
  if (flagNonUniqueChID or flagNonUniqueChRawID)
    ofos << " -- WARNING: non unique chamber Ids!!!" << endl;

  ofos << myName() << ": Begin iteration over geometry..." << endl;
  ofos << "iter " << dashedLine_ << endl;

  //----------------------- Global GEMGeometry TEST -------------------------------------------------------
  ofos << myName() << "Begin GEMGeometry structure TEST" << endl;

  for (auto sch : gemChambers_) {

    const BoundPlane& bSurface(sch.surface());
    LocalPoint lCentre(0.,0.,0.);
    GlobalPoint gCentre(bSurface.toGlobal(lCentre));

    ofos << " " << std::endl;
    ofos << sch.id() << ", center x: " << gCentre.x() << " cm, center y: " <<  gCentre.y() << " cm, center z: " << gCentre.z() << " cm." << std::endl;

    // checking the dimensions of each partition & chamber
    for (auto ch : sch.chambers()) {

      int nRolls(ch->nEtaPartitions());
      const BoundPlane& bSurface(ch->surface());
      LocalPoint lCentre(0.,0.,0.);
      GlobalPoint gCentre(bSurface.toGlobal(lCentre));

      ofos << " " << std::endl;
      ofos << ch->id() << ", center x: " << gCentre.x() << " cm, center y: " <<  gCentre.y() << " cm, center z: " << gCentre.z() << " cm." << std::endl; 
      ofos << " " << std::endl;

      /*
      * possible checklist for an eta partition:
      *   base_bottom, base_top, height, strips, pads
      *   cx, cy, cz, ceta, cphi
      *   tx, ty, tz, teta, tphi
      *   bx, by, bz, beta, bphi
      *   pitch center, pitch bottom, pitch top
      *   deta, dphi
      *   gap thickness
      *   sum of all dx + gap = chamber height
      */

      for (auto roll : ch->etaPartitions()) {

        GEMDetId rId(roll->id());
        const BoundPlane& bSurface(roll->surface());
        const StripTopology* topology(&(roll->specificTopology()));

        // base_bottom, base_top, height, strips, pads (all half length)
        auto& parameters(roll->specs()->parameters());
        float bottomEdge(parameters[0]);
        float topEdge(parameters[1]);
        float height(parameters[2]);
        float nStrips(parameters[3]);
        float nPads(parameters[4]);

        LocalPoint lCentre(0., 0., 0.);
        GlobalPoint gCentre(bSurface.toGlobal(lCentre));
        LocalPoint lTop(0., height, 0.);
        GlobalPoint gTop(bSurface.toGlobal(lTop));
        LocalPoint lBottom(0., -height, 0.);
        GlobalPoint gBottom(bSurface.toGlobal(lBottom));

        // gx, gy, gz, geta, gphi (center)
        double cx(gCentre.x());
        double cy(gCentre.y());
        double cz(gCentre.z());
        double ceta(gCentre.eta());
        double cphi(gCentre.phi().degrees());
        if (cphi < 0.) cphi += 360.;

        double tx(gTop.x());
        double ty(gTop.y());
        double tz(gTop.z());
        double teta(gTop.eta());
        double tphi(gTop.phi().degrees());
        if (tphi < 0.) tphi += 360.;

        double bx(gBottom.x());
        double by(gBottom.y());
        double bz(gBottom.z());
        double beta(gBottom.eta());
        double bphi(gBottom.phi().degrees());
        if (bphi < 0.) bphi += 360.;

        // pitch bottom, pitch top, pitch centre
        float pitch(roll->pitch());
        float topPitch(roll->localPitch(lTop));
        float bottomPitch(roll->localPitch(lBottom));

        // Type - should be GHA[1-nRolls]
        string type(roll->type().name());
        // print info about edges
        LocalPoint lEdge1(topology->localPosition(0.));
        LocalPoint lEdgeN(topology->localPosition((float)nStrips));

        double cstrip1(roll->toGlobal(lEdge1).phi().degrees());
        double cstripN(roll->toGlobal(lEdgeN).phi().degrees());
        double dphi(cstripN - cstrip1);
        if (dphi < 0.) dphi += 360.;
        double deta(abs(beta - teta));
        const bool printDetails(true);

        if (printDetails) {

	  ofos << roll->id() << ", x: "<< gCentre.x() << " cm, y: " << gCentre.y() << " cm, z: " << gCentre.z()
	  << " cm, 1stStrip: " << roll->toGlobal(roll->centreOfStrip(0)).phi().degrees() << " deg, lastStrip: "
	  << roll->toGlobal(roll->centreOfStrip((roll->nstrips())-1)).phi().degrees()
	  << " deg, rBottom: " << pow((pow(gBottom.x(),2.0)+pow(gBottom.y(),2.0)),0.5)
	  << ", rCentre: " << pow((pow(gCentre.x(),2.0)+pow(gCentre.y(),2.0)),0.5)
	  << ", rTop: " << pow((pow(gTop.x(),2.0)+pow(gTop.y(),2.0)),0.5) << endl;
	  //" deg, ap: " << height << " cm , te: " << topEdge << " cm , be: " << bottomEdge << " cm." << std::endl;
        }

	ofos << endl;
        for (int i = 0; i < roll->nstrips(); i++) {
		ofos << " the " << i+1 << "-th strip R: "<< pow(pow((roll->centreOfStrip(i).x()),2.0) + pow((roll->centreOfStrip(i).y()),2.0), 0.5) << "cm, x: "
		<< roll->centreOfStrip(i).x() << "cm. y: " << roll->centreOfStrip(i).y() << "cm. z: " << roll->centreOfStrip(i).z() << "cm." << endl;
	}
        ofos << endl;
      }
    }
  }
  ofos << dashedLine_ << " end" << std::endl;
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GEMGeometryAnalyzer);
