#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Run.h"

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace edm;

// Proto-type
  class GEMSimCollectionReader: public edm::EDAnalyzer {

  public:
	explicit GEMSimCollectionReader( const ParameterSet& );
	~GEMSimCollectionReader();

  private:
	virtual void analyze(const Event&, const EventSetup& );

  // ========================== mermber data ==========================
    edm::ParameterSet pset;
    edm::Service<TFileService> fs;
    edm::EDGetToken CombinedGEMSimHitsToken;
    std::vector<GEMDetId> PSimHitVect_;
    int nME0SimHits, nGEMSimHits;
  // ==================================================================
};

// Constructor
GEMSimCollectionReader::GEMSimCollectionReader( const ParameterSet& ps ) { 

  CombinedGEMSimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("CombinedGEMSimHitInputLabel") );
} 

// Destructor
GEMSimCollectionReader::~GEMSimCollectionReader() = default;

void GEMSimCollectionReader::analyze( const Event& iEvent, const EventSetup& iSetup ) {

  edm::Handle< std::vector<PSimHit> > CombinedGEMSimHits;
  iEvent.getByToken(CombinedGEMSimHitsToken, CombinedGEMSimHits);
  const std::vector<PSimHit>& gem_simhits = *CombinedGEMSimHits.product();

  // ===================================================================================

  if (CombinedGEMSimHits.product()) {

  int g = 0;
  for (auto& gmsh : gem_simhits) {
    GEMDetId gem_detId = gmsh.detUnitId();
    PSimHitVect_.push_back(gem_detId);
    int gm_station = gem_detId.station();
    std::cout << PSimHitVect_.at(g) << std::endl; g++;
    }std::cout << "nSimHit of GEM : " << g << std::endl;

  }
  else { std::cout << "NOT VALID GEM SIMHIT INPUT!!!" << std::endl; }

  // ===================================================================================
}

// define this as a plugin
DEFINE_FWK_MODULE(GEMSimCollectionReader);
