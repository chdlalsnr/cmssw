
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"

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
#include "math.h"

class GEMGeometryAnalyzer : public edm::one::EDAnalyzer<> {

public:
  GEMGeometryAnalyzer(const edm::ParameterSet& pset);
  ~GEMGeometryAnalyzer() override;

  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}

private:
  std::ofstream ofos;
  edm::EDGetToken GEMRecHitToken;
};

using namespace std;
GEMGeometryAnalyzer::GEMGeometryAnalyzer(const edm::ParameterSet& pset) {
  GEMRecHitToken = consumes<GEMRecHitCollection>( pset.getParameter<edm::InputTag>("GEMRecHitInputLabel") );
  ofos.open("GEMtestOutput.out");
}

GEMGeometryAnalyzer::~GEMGeometryAnalyzer() {
  ofos.close();
}

void GEMGeometryAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& iSetup) {
  edm::ESHandle<GEMGeometry> pDD;
  iSetup.get<MuonGeometryRecord>().get(pDD);

  edm::Handle<GEMRecHitCollection> GEMRecHits;
  e.getByToken(GEMRecHitToken, GEMRecHits);

  for (GEMRecHitCollection::const_iterator recHit = GEMRecHits->begin(); recHit != GEMRecHits->end(); ++recHit) {

    GEMDetId id((*recHit).gemId());

    int region = (int)id.region();
    int station = (int)id.station();
    //int layer = (int)id.layer();
    //int chamber = (int)id.chamber();
    //int roll = (int)id.roll();

    LocalPoint rhLP = recHit->localPosition();
    GlobalPoint rhGP = pDD->idToDet((*recHit).gemId())->surface().toGlobal(rhLP);

    Float_t globalX = rhGP.x();
    Float_t globalY = rhGP.y();
    double deg = atan2(globalY,globalX)*180./3.1415926535897;

    if ((deg > -180.) && (deg < -5.)) deg += 360.;

    ofos << region << " " << station << " " << globalX << " " << globalY << " " << deg << " " << endl;
  }
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GEMGeometryAnalyzer);
