#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDProducer.h"
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
	virtual void beginJob() override;
	virtual void endJob() override;

	virtual void beginRun( const Run&, const EventSetup&) override;
	virtual void endRun( const Run&, const EventSetup&) override;
	void initValue();

  // ========================== mermber data ==========================
    edm::ParameterSet pset;
    //edm::EDGetToken ME0SimHitsToken_;
    //edm::EDGetToken GEMSimHitsToken_;
    //edm::Handle<edm::PSimHitContainer> GEMSimHitContainer;
    //edm::Handle<edm::PSimHitContainer> ME0SimHitContainer;
    //edm::EDGetTokenT<edm::PSimHitContainer> gemSimHits_;
    //edm::EDGetTokenT<edm::PSimHitContainer> me0SimHits_;
    edm::Service<TFileService> fs;
    //edm::InputTag gemSimHitLabel;
    //edm::InputTag me0SimHitLabel;
    edm::EDGetToken GEMSimHitsToken;
    edm::EDGetToken ME0SimHitsToken;
    int nME0SimHits, nGEMSimHits;
  // ==================================================================
};

// Constructor
GEMSimCollectionReader::GEMSimCollectionReader( const ParameterSet& ps ) { 

  //cfg = iConfig;
  //ME0SimHitsToken_ = consume<edm::PSimHitContainer>(iConfig.getParameter<edm::InputTag>("ME0SimInputLabel"));
  //GEMSimHitsToken_ = consume<edm::PsimHitContainer>(iConfig.getParameter<dem::InputTag>("GEMSimInputLabel"));
  std::cout << "==================== Construction of GEMSimHitCollectionReader ====================" << std::endl;

  //pset = ps;
  //ParameterSet serviceParameters = pset.getParameter<ParameterSet>("ServiceParameters");
  //theService = new MuonServiceProxy(serviceParameters);
  //edm::EDGetTokenT< std::vector<PSimHit> > me0SimHits_;
  //edm::EDGetTokenT< std::vector<PSimHit> > gemSimHits_;
  //me0SimHits_ = consumes< std::vector<PSimHit> >("MuonME0Hits");
  //gemSimHits_ = consumes< std::vector<PSimHit> >("MuonGEMHits");

  //gemSimHitLabel = ps.getParameter<InputTag>("MuonGEMHits");
  //me0SimHitLabel = ps.getParameter<InputTag>("MuonME0Hits");
  //gemSimHitToken = consumes<std::vector<PSimHit>>("gemSimHitLabel");
  //me0SimHitToken = consumes<std::vector<PSimHit>>("me0SimHitLabel");

  GEMSimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("GEMSimHitInputLabel") );
  ME0SimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("ME0SimHitInputLabel") );

  //Tokens    
  //gemSimHits_ = consumes<edm::PSimHitContainer>(iPSet.getParameter<edm::InputTag>("MuonGEMHits"));
  //me0SimHits_ = consumes<edm::PSimHitContainer>(iPSet.getParameter<edm::InputTag>("MuonME0Hits"));

  //edm::Handle<edm::PSimHitContainer> GEMSimHitContainer;
  //edm::Handle<edm::PSimHitContainer> ME0SimHitContainer;
  //iEvent.getByToken( gemSimHits_, GEMSimHitContainer );
  //iEvent.getByToken( me0SimHits_, ME0SimHitContainer );
  //std::cout << me0SimHits_ << std::endl;
  //std::cout << gemSimHits_ << std::endl;
} 

// Destructor
GEMSimCollectionReader::~GEMSimCollectionReader() {

  std::cout << "===================== Destruction of GEMSimHitCollectionReader ====================" << std::endl;
}

void GEMSimCollectionReader::analyze( const Event& iEvent, const EventSetup& iSetup ) {

  edm::Handle< std::vector<PSimHit> > GEMSimHits;
  edm::Handle< std::vector<PSimHit> > ME0SimHits;
  iEvent.getByToken(GEMSimHitsToken, GEMSimHits);
  iEvent.getByToken(ME0SimHitsToken, ME0SimHits);

  //std::vector<EncodedEventId> gemColVect;
  //std::vector<EncodedEventId> me0ColVect;

  //initValue();
  
  if (GEMSimHits.isValid()) {

    const std::vector<PSimHit>& gem_simhits = *GEMSimHits.product();
    int gm_layer = 0;

    int g = 0;
    for (auto& gmsh : gem_simhits) { 
      GEMDetId gem_detId = gmsh.detUnitId();
      gm_layer = gem_detId.layer();
      std::cout << gm_layer << std::endl; g++;
     }std::cout << "nSimHit of GEM : " << g << std::endl;
  }
  else { std::cout << "NOT VALID GEM SIMHIT INPUT!!!" << std::endl; }

  if (ME0SimHits.isValid()) {

    const std::vector<PSimHit>& me0_simhits = *ME0SimHits.product();
    int m0_layer = 0;

    int m = 0;
    for (auto& m0sh : me0_simhits) {
      GEMDetId me0_detId = m0sh.detUnitId();
      m0_layer = me0_detId.layer();
      std::cout << m0_layer << std::endl; m++;
    }std::cout << "nSimHit of ME0 : " << m << std::endl;
  }
  else { std::cout << "NOT VALID ME0 SIMHIT INPUT!!!" << std::endl; }

}

void GEMSimCollectionReader::beginJob() {}
void GEMSimCollectionReader::endJob() {}

void GEMSimCollectionReader::beginRun( const Run& run, const EventSetup& iSetup ) {

}

void GEMSimCollectionReader::endRun( const Run&, const EventSetup& ) {

  std::cout << "==================== End Run of GEMSimHitCollectionReader ====================" << std::endl;
}

void initValue() {}

// define this as a plugin
DEFINE_FWK_MODULE(GEMSimCollectionReader);
