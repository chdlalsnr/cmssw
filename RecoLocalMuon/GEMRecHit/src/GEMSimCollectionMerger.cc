#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include "SimDataFormats/GEMDigiSimLink/interface/GEMDigiSimLink.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <string>

using namespace std;
using namespace edm;

// Proto-type
  class GEMSimCollectionMerger: public edm::EDProducer {

  public:
	explicit GEMSimCollectionMerger( const ParameterSet& );
	~GEMSimCollectionMerger();

  private:

	virtual void produce( Event&, EventSetup const& ) override;

  // ========================== mermber data ==========================
    edm::ParameterSet pset;
    edm::Service<TFileService> fs;
    edm::EDGetToken GEMSimHitsToken;
    edm::EDGetToken ME0SimHitsToken;
    std::vector<PSimHit>* PSimHitVect_;
    //std::ofstream ofos;
  // ==================================================================
};

// Constructor
GEMSimCollectionMerger::GEMSimCollectionMerger( const ParameterSet& ps ) { 

  produces< std::vector<PSimHit> >("CombinedGEMSimHits");
  GEMSimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("GEMSimHitInputLabel") );
  ME0SimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("ME0SimHitInputLabel") );

  //std::string mix_(ps.getParameter<std::string>("GEMSimHits", "ME0SimHits"));
  //std::string collection_(ps.getParameter<std::string>("inputCollection"));
  //cf_token = consumes<CrossingFrame<PSimHit> >(edm::InputTag(mix_));

  //ofos.open("testGEMSimHit.out");
  //ofos << "============================= Start Record SimHit DetId =============================");
}

// Destructor
GEMSimCollectionMerger::~GEMSimCollectionMerger() {

  //ofos << "============================== End Record SimHit DetId ==============================");
  //ofos.close();
}

void GEMSimCollectionMerger::produce( Event& iEvent, EventSetup const& iSetup ) {

  edm::Handle< std::vector<PSimHit> > GEMSimHits;
  edm::Handle< std::vector<PSimHit> > ME0SimHits;
  iEvent.getByToken(GEMSimHitsToken, GEMSimHits);
  iEvent.getByToken(ME0SimHitsToken, ME0SimHits);

  /*edm::Handle< CrossingFrame<PSimHit> > cf;
  iEvent.getByToken (cf_token, cf);
  MixCollection<PSimHit> hits{cf.product()};
  for (const auto& hit: hits) {

    DetId detId = hit.detUnitId();
    ofos << "SimHit DetId : " << detId << endl;
  }*/

  // ===================================================================================
  /*const std::vector<PSimHit>& gem_simhits = *CombinedGEMSimHits.product();
  int gh_station = 0;
  int ng = 0;
  for (auto& gmsh : gem_simhits) { 
    GEMDetId gem_detId = gmsh.detUnitId();
    gh_station = gem_detId.layer();
    std::cout << "SimHit station : " << gh_station << std::endl; ng++;
  }std::cout << "nSimHit of GEM : " << ng << std::endl;

  ofos << "( " << gem_detId.region() << ", " << gem_detId.ring() << ", " << gem_detId.station() << ", " << gem_detId.layer() << ", " << gem_detId.chamber() << ", " << gem_detId.roll() << " )" << endl;
  // ===================================================================================*/
  
  iEvent.put( std::make_unique< std::vector<PSimHit> > (*GEMSimHits) );
  iEvent.put( std::make_unique< std::vector<PSimHit> > (*ME0SimHits) );
}

// define this as a plugin
DEFINE_FWK_MODULE(GEMSimCollectionMerger);
