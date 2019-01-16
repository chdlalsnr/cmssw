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

#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include "DataFormats/Math/interface/deltaPhi.h"

#include <memory>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <set>

class ME0GeometryAnalyzer : public edm::one::EDAnalyzer<>
{
public:
  ME0GeometryAnalyzer( const edm::ParameterSet& pset);

  ~ME0GeometryAnalyzer() override;

  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}

  static bool sortCh(const ME0Chamber &a, const ME0Chamber &b){
    if (a.id().region() == b.id().region()){
      if (a.id().station() == b.id().station()){
        //if (a.id().superChamberId() == b.id().superChamberId()){
          if (a.id().chamber() == b.id().chamber()){
            return a.id().layer() < b.id().layer();
          }else{ return a.id().chamber() < b.id().chamber();}
        //}else{ return a.id().superChamberId() < b.id().superChamberId();}
      }else{ return a.id().station() < b.id().station();}
    }else{ return a.id().region() < b.id().region();}
  };

private:
  const std::string& myName() { return myName_;}

  const int dashedLineWidth_;
  const std::string dashedLine_;
  const std::string myName_;
  std::ofstream ofos;
  std::vector<ME0Chamber> ME0Chambers_;
};

using namespace std;

ME0GeometryAnalyzer::ME0GeometryAnalyzer( const edm::ParameterSet& /*iConfig*/ )
  : dashedLineWidth_(104), dashedLine_( string(dashedLineWidth_, '-') ),
    myName_( "ME0GeometryAnalyzer" )
{
  ofos.open("ME0testOutput.out");
  ofos <<"======================== Opening output file"<< endl;
}

ME0GeometryAnalyzer::~ME0GeometryAnalyzer()
{
  ofos.close();
  ofos <<"======================== Closing output file"<< endl;
}

void
ME0GeometryAnalyzer::analyze( const edm::Event& /*iEvent*/, const edm::EventSetup& iSetup )
{
  edm::ESHandle<ME0Geometry> pDD;
  iSetup.get<MuonGeometryRecord>().get(pDD);

  ofos << myName() << ": Analyzer..." << endl;
  ofos << "start " << dashedLine_ << endl;

  ofos << " Geometry node for ME0Geom is  " << &(*pDD) << endl;
  ofos << " detTypes       \t"              <<pDD->detTypes().size() << endl;
  ofos << " GeomDetUnit       \t"           <<pDD->detUnits().size() << endl;
  ofos << " GeomDet           \t"           <<pDD->dets().size() << endl;
  ofos << " GeomDetUnit DetIds\t"           <<pDD->detUnitIds().size() << endl;
  ofos << " eta partitions \t"              <<pDD->etaPartitions().size() << endl;
  ofos << " layers         \t"              <<pDD->layers().size() << endl;
  ofos << " chambers       \t"              <<pDD->chambers().size() << endl;
  // ofos << " regions        \t"              <<pDD->regions().size() << endl;

  // checking uniqueness of roll detIds
  bool flagNonUniqueRollID = false;
  bool flagNonUniqueRollRawID = false;
  int nstrips = 0;
  int npads = 0;
  for (auto roll1 : pDD->etaPartitions()){
    nstrips += roll1->nstrips();
    npads += roll1->npads();
    for (auto roll2 : pDD->etaPartitions()){
      if (roll1 != roll2){
	if (roll1->id() == roll2->id()) flagNonUniqueRollID = true;
	if (roll1->id().rawId() == roll2->id().rawId()) flagNonUniqueRollRawID = true;
      }
    }
  }
  if (flagNonUniqueRollID or flagNonUniqueRollRawID)
    ofos << " -- WARNING: non unique roll Ids!!!" << endl;

  // checking uniqueness of layer detIds
  bool flagNonUniqueLaID = false;
  bool flagNonUniqueLaRawID = false;
  for (auto la1 : pDD->layers()){
    for (auto la2 : pDD->layers()){
      if (la1 != la2){
	if (la1->id() == la2->id()) flagNonUniqueLaID = true;
	if (la1->id().rawId() == la2->id().rawId()) flagNonUniqueLaRawID = true;
      }
    }
  }
  if (flagNonUniqueLaID or flagNonUniqueLaRawID)
    ofos << " -- WARNING: non unique layer Ids!!!" << endl;

  // checking uniqueness of chamber detIds
  bool flagNonUniqueChID = false;
  bool flagNonUniqueChRawID = false;
  for (auto ch1 : pDD->chambers()){
    ME0Chambers_.push_back(*ch1);
    for (auto ch2 : pDD->chambers()){
      if (ch1 != ch2){
	if (ch1->id() == ch2->id()) flagNonUniqueChID = true;
	if (ch1->id().rawId() == ch2->id().rawId()) flagNonUniqueChRawID = true;
      }
    }
  }
  sort(ME0Chambers_.begin(),ME0Chambers_.end(), sortCh);

  if (flagNonUniqueChID or flagNonUniqueChRawID)
    ofos << " -- WARNING: non unique chamber Ids!!!" << endl;

  // print out number of strips and pads
  ofos << " total number of strips\t"<<nstrips << endl;
  ofos << " total number of pads  \t"<<npads << endl;

  ofos << myName() << ": Begin iteration over geometry..." << endl;
  ofos << "iter " << dashedLine_ << endl;

  //------------------------Global GEMGeometry TEST --------------------------------------------------------
  ofos << myName() << "Begin ME0Geometry TEST" << endl;

  /*
   * possible checklist for an eta partition:
   *   base_bottom, base_top, height, strips, pads
   *   cx, cy, cz, ceta, cphi
   *   tx, ty, tz, teta, tphi
   *   bx, by, bz, beta, bphi
   *   pitch center, pitch bottom, pitch top
   *   deta, dphi
   *   gap thicess
   *   sum of all dx + gap = chamber height
   */

  for (auto ch : ME0Chambers_){

    const BoundPlane& bSurface(ch.surface());
    LocalPoint lCentre( 0., 0., 0. );
    GlobalPoint gCentre(bSurface.toGlobal(lCentre));

    ofos << " " << std::endl;
    ofos << ch.id() << ", center x: " << gCentre.x() << " cm, center y: " << gCentre.y() << " cm, center z: " << gCentre.z() << " cm." << std::endl;

    for (auto la : ch.layers()){

      const BoundPlane& bSurface(la->surface());
      LocalPoint lCentre( 0., 0., 0. );
      GlobalPoint gCentre(bSurface.toGlobal(lCentre));

      ofos << " " << std::endl;
      ofos << la->id() << ", center x: " << gCentre.x() << " cm, center y: " << gCentre.y() << " cm, center z: " << gCentre.z() << " cm." << std::endl;
      ofos << " " << std::endl;

      for (auto roll : la->etaPartitions()){

        const BoundPlane& bSurface(roll->surface());
        auto& parameters(roll->specs()->parameters());
        float bottomEdge(parameters[0]);
        float topEdge(parameters[1]);
        float height(parameters[2]);

        LocalPoint lCentre( 0., 0., 0. );
        GlobalPoint gCentre(bSurface.toGlobal(lCentre));
        LocalPoint lTop( 0., height, 0.);
        GlobalPoint gTop(bSurface.toGlobal(lTop));
        LocalPoint lBottom( 0., -height, 0. );
        GlobalPoint gBottom(bSurface.toGlobal(lBottom));

        ofos << roll->id() << ", x: " << gCentre.x() << " cm, y: " << gCentre.y() << " cm, z: " << gCentre.z() << " cm, 1stStrip: " << roll->toGlobal(roll->centreOfStrip(1)).phi().degrees() << " deg, lastStrip: " << roll->toGlobal(roll->centreOfStrip(roll->nstrips())).phi().degrees() << " deg, ap: " << height << " cm , te: " << topEdge << " cm , be: " << bottomEdge << " cm." << std::endl;
      }
    } 
  
  }
  ofos << dashedLine_ << " end" << endl;
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ME0GeometryAnalyzer);
