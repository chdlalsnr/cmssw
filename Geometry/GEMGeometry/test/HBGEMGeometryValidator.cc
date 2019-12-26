
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

class HBGEMGeometryValidator : public edm::one::EDAnalyzer<> {

public:
  HBGEMGeometryValidator(const edm::ParameterSet& pset);
  ~HBGEMGeometryValidator() override;

  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}

private:
  std::ofstream ofos;
  edm::EDGetToken GEMRecHitToken;
};

using namespace std;
HBGEMGeometryValidator::HBGEMGeometryValidator(const edm::ParameterSet& pset) {
  GEMRecHitToken = consumes<GEMRecHitCollection>( pset.getParameter<edm::InputTag>("GEMRecHitInputLabel") );
  ofos.open("HBGeometryValidation.out");
}

HBGEMGeometryValidator::~HBGEMGeometryValidator() {
  ofos.close();
}

void HBGEMGeometryValidator::analyze(const edm::Event& e, const edm::EventSetup& iSetup) {

  edm::ESHandle<GEMGeometry> pDD;
  iSetup.get<MuonGeometryRecord>().get(pDD);

  edm::Handle<GEMRecHitCollection> GEMRecHits;
  e.getByToken(GEMRecHitToken, GEMRecHits);

  vector<double> x_r1s1t, y_r1s1t, x_r1s2t, y_r1s2t;
  vector<double> x_r_1s1t, y_r_1s1t, x_r_1s2t, y_r_1s2t;
  vector<double> x_r1s1f, y_r1s1f, x_r1s2f, y_r1s2f;
  vector<double> x_r_1s1f, y_r_1s1f, x_r_1s2f, y_r_1s2f;

  double r,Rs,Rl = 0.;
  double r2,R2 = 0.;

  for (auto region : pDD->regions()) {
  for (auto station : region->stations()) {
  for (auto ring : station->rings()) {
  for (auto sch : ring->superChambers()) {
  for (auto ch : sch->chambers()) {
  for (auto roll : ch->etaPartitions()) {

    GEMDetId rId(roll->id());

    const BoundPlane& bSurface(roll->surface());
    auto& parameters(roll->specs()->parameters());
    float height(parameters[2]);

    LocalPoint  lTop( 0., height, 0.);
    GlobalPoint gTop(bSurface.toGlobal(lTop));
    LocalPoint  lBottom( 0., -height, 0.);
    GlobalPoint gBottom(bSurface.toGlobal(lBottom));

    if ( (rId.station()==1) && ((rId.chamber())%2==1) && (rId.roll()==8)) r = pow((pow(gBottom.x(),2)+pow(gBottom.y(),2)),.5);
    if ( (rId.station()==1) && ((rId.chamber())%2==1) && (rId.roll()==1)) Rs = pow((pow(gTop.x(),2)+pow(gTop.y(),2)),.5);
    if ( (rId.station()==1) && ((rId.chamber())%2==0) && (rId.roll()==1)) Rl = pow((pow(gTop.x(),2)+pow(gTop.y(),2)),.5);
    if (rId.station()==2 && (rId.roll()==8)) r2 = pow((pow(gBottom.x(),2)+pow(gBottom.y(),2)),.5);
    if (rId.station()==2 && (rId.roll()==1)) R2 = pow((pow(gTop.x(),2)+pow(gTop.y(),2)),.5);
  }}}}}} //ofos << r << " " << Rs << " " << Rl << " " << r2 << " " << R2 << "." << endl;

  for (GEMRecHitCollection::const_iterator recHit = GEMRecHits->begin(); recHit != GEMRecHits->end(); ++recHit) {

    //Float_t x = recHit->localPosition().x();
    //Float_t xErr = recHit->localPositionError().xx();
    //Float_t y = recHit->localPosition().y();
    //Float_t yErr = recHit->localPositionError().yy();

    GEMDetId id((*recHit).gemId());

    int region = (int)id.region();
    int station = (int)id.station();
    int layer = (int)id.layer();
    int chamber = (int)id.chamber();
    int roll = (int)id.roll();

    LocalPoint rhLP = recHit->localPosition();
    GlobalPoint rhGP = pDD->idToDet((*recHit).gemId())->surface().toGlobal(rhLP);

    //Float_t globalR = rhGP.perp();
    Float_t globalX = rhGP.x();
    Float_t globalY = rhGP.y();
    //Float_t globalZ = rhGP.z();
    double deg = atan2(globalY,globalX)*180./3.1415926535897;

    if ((deg > -180.) && (deg < -5.)) deg += 360.;

    ofos << "Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << endl;

    int isOdd(chamber%2);
    if (region == -1) {
      if (station == 1) {
	if (isOdd) {
	//if ( (deg > -5. && deg < 5.) || (deg > 15. && deg < 25.) || (deg > 35. && deg < 45.) || (deg > 55. && deg < 65.) || (deg > 75. && deg < 85.) || (deg > 95. && deg < 105.) || (deg > 115. && deg < 125.) || (deg > 135. && deg < 145.) || (deg > 155. && deg < 165.) || (deg > 175. && deg < 185.) || (deg > 195. && deg < 205.) || (deg > 215. && deg < 225.) || (deg > 235. && deg < 245.) || (deg > 255. && deg < 265.) || (deg > 275. && deg < 285.) || (deg > 295. && deg < 305.) || (deg > 315. && deg < 325.) || (deg > 335. && deg < 345.) ) {
	  if ( (pow(pow(globalX,2)+pow(globalY,2),.5)>r) && (pow(pow(globalX,2)+pow(globalY,2),.5)<Rs) ) {
	    x_r_1s1t.push_back(globalX);
	    y_r_1s1t.push_back(globalY);
	  }
	  else {
	    x_r_1s1f.push_back(globalX);
	    y_r_1s1f.push_back(globalY);
	    ofos << "********** RECONSTRUCTION IN UNSENSITIVE REGION!!! Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << " **********" << endl;
	  }
	}
	else {
	  if ( (pow(pow(globalX,2)+pow(globalY,2),.5)>r) && (pow(pow(globalX,2)+pow(globalY,2),.5)<Rl) ) {
            x_r_1s1t.push_back(globalX);
            y_r_1s1t.push_back(globalY);
	  }
          else {
            x_r_1s1f.push_back(globalX);
            y_r_1s1f.push_back(globalY);
	    ofos << "********** RECONSTRUCTION IN UNSENSITIVE REGION!!! Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << " **********" << endl;
	  }
	}
      }
      else if (station == 2) {
	if ( (pow(pow(globalX,2)+pow(globalY,2),.5)>r2) && (pow(pow(globalX,2)+pow(globalY,2),.5)<R2) ) {
          x_r_1s2t.push_back(globalX);
          y_r_1s2t.push_back(globalY);
        }
        else {
          x_r_1s2f.push_back(globalX);
          y_r_1s2f.push_back(globalY);
	  ofos << "********** RECONSTRUCTION IN UNSENSITIVE REGION!!! Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << " **********" << endl;
        }
      }
    }
    else if (region == 1) {
      if (station == 1) {
	if (isOdd) {
	//if ( (deg > -5. && deg < 5.) || (deg > 15. && deg < 25.) || (deg > 35. && deg < 45.) || (deg > 55. && deg < 65.) || (deg > 75. && deg < 85.) || (deg > 95. && deg < 105.) || (deg > 115. && deg < 125.) || (deg > 135. && deg < 145.) || (deg > 155. && deg < 165.) || (deg > 175. && deg < 185.) || (deg > 195. && deg < 205.) || (deg > 215. && deg < 225.) || (deg > 235. && deg < 245.) || (deg > 255. && deg < 265.) || (deg > 275. && deg < 285.) || (deg > 295. && deg < 305.) || (deg > 315. && deg < 325.) || (deg > 335. && deg < 345.) ) {
          if ( (pow(pow(globalX,2)+pow(globalY,2),.5)>r) && (pow(pow(globalX,2)+pow(globalY,2),.5)<Rs) ) {
            x_r1s1t.push_back(globalX);
            y_r1s1t.push_back(globalY);
          }
          else {
            x_r1s1f.push_back(globalX);
            y_r1s1f.push_back(globalY);
	    ofos << "********** RECONSTRUCTION IN UNSENSITIVE REGION!!! Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << " **********" << endl;
          }
	}
        else {
          if ( (pow(pow(globalX,2)+pow(globalY,2),.5)>r) && (pow(pow(globalX,2)+pow(globalY,2),.5)<Rl) ) {
            x_r1s1t.push_back(globalX);
            y_r1s1t.push_back(globalY);
          }
          else {
            x_r1s1f.push_back(globalX);
            y_r1s1f.push_back(globalY);
	    ofos << "********** RECONSTRUCTION IN UNSENSITIVE REGION!!! Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << " **********" << endl;
          }
	}
      }
      else if (station == 2) {
        if ( (pow(pow(globalX,2)+pow(globalY,2),.5)>r2) && (pow(pow(globalX,2)+pow(globalY,2),.5)<R2) ) {
          x_r1s2t.push_back(globalX);
          y_r1s2t.push_back(globalY);
        }
        else {
          x_r1s2f.push_back(globalX);
          y_r1s2f.push_back(globalY);
	  ofos << "********** RECONSTRUCTION IN UNSENSITIVE REGION!!! Global Location : (" << globalX << " cm, " << globalY << " cm) Detector Id : region " << region << " station " << station << " chamber " << chamber << " layer " << layer << " roll " << roll << " **********" << endl;
        }
      }
    }
  }
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HBGEMGeometryValidator);
