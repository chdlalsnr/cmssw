#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
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
    std::vector<GEMDetId> PSimHitVect_;
  // ==================================================================
};

// Constructor
GEMSimCollectionMerger::GEMSimCollectionMerger( const ParameterSet& ps ) { 

  produces< std::vector<PSimHit> >("CombinedMuonGEMHits");
  GEMSimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("GEMSimHitInputLabel") );
  ME0SimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("ME0SimHitInputLabel") );
}

// Destructor
GEMSimCollectionMerger::~GEMSimCollectionMerger() = default;

void GEMSimCollectionMerger::produce( Event& iEvent, EventSetup const& iSetup ) {

  edm::Handle< std::vector<PSimHit> > GEMSimHits;
  edm::Handle< std::vector<PSimHit> > ME0SimHits;
  iEvent.getByToken(GEMSimHitsToken, GEMSimHits);
  iEvent.getByToken(ME0SimHitsToken, ME0SimHits);

  // ===================================================================================

  const std::vector<PSimHit>& gem_simhits = *GEMSimHits.product();
  const std::vector<PSimHit>& me0_simhits = *ME0SimHits.product();

  std::vector<PSimHit> gem_vect;
  for (int i = 0; i < int(gem_simhits.size()); i++) { gem_vect.push_back(gem_simhits.at(i)); }
  for (int i = 0; i < int(me0_simhits.size()); i++) { gem_vect.push_back(me0_simhits.at(i)); }

  /*if ( (GEMSimHits.isValid()) && (ME0SimHits.isValid()) ) {

    int g = 0;
    for (auto& gmsh : gem_simhits) {
      GEMDetId gem_detId = gmsh.detUnitId();
      PSimHitVect_.push_back(gem_detId);
      int gm_station = gem_detId.station();
      std::cout << PSimHitVect_.at(g) << std::endl; g++;
    }std::cout << "nSimHit of GEM : " << g << std::endl;

    int m = 0;
    for (auto& m0sh : me0_simhits) {
      GEMDetId me0_detId = m0sh.detUnitId();
      PSimHitVect_.push_back(me0_detId);
      std::cout << PSimHitVect_.at(g+m) << std::endl; m++;
    }std::cout << "nSimHit of ME0 : " << m << std::endl;
  }

  else { std::cout << "NOT VALID GEM SIMHIT INPUT!!!" << std::endl; }*/

  // ===================================================================================
  
  iEvent.put( std::move(std::make_unique<std::vector<PSimHit>>(gem_vect)),"CombinedMuonGEMHits" );
  //iEvent.put( std::move(std::make_unique<std::vector<PSimHit>>(*ME0SimHits)),"CombinedGEMSimHits" );
  //iEvent.put( std::move(std::make_unique<std::vector<PSimHit>>(*ME0SimHits)),"MuonGEMHits" );
}

// define this as a plugin
DEFINE_FWK_MODULE(GEMSimCollectionMerger);
