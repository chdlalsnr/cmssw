#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ModuleContextSentry.h"
#include "FWCore/ServiceRegistry/interface/InternalContext.h"
#include "FWCore/ServiceRegistry/interface/ModuleCallingContext.h"
#include "FWCore/ServiceRegistry/interface/ParentContext.h"

#include "DataFormats/Provenance/interface/ProductID.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include "SimDataFormats/GEMDigiSimLink/interface/GEMDigiSimLink.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoNew.h"

#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <map>

using namespace std;
using namespace edm;

// Proto-type
  class GEMSimCollectionMerger: public edm::EDProducer {

  public:
	explicit GEMSimCollectionMerger( const ParameterSet& );
	~GEMSimCollectionMerger();

  private:

	virtual void produce( Event&, EventSetup const& ) override;
    	virtual void beginJob() override;
    	virtual void endJob() override;

    	virtual void beginRun(Run const& /* iR */, EventSetup const& /* iE */) override;
    	virtual void endRun(Run const& /* iR */, EventSetup const& /* iE */) override;
    	virtual void beginLuminosityBlock(LuminosityBlock const& /* iL */, EventSetup const& /* iE */) override;
    	virtual void endLuminosityBlock(LuminosityBlock const& /* iL */, EventSetup const& /* iE */) override;
    	virtual void respondToOpenInputFile(FileBlock const&) override;
	virtual void respondToCloseInputFile(FileBlock const&) override;

  // ========================== mermber data ==========================
    edm::ParameterSet pset;
    edm::Service<TFileService> fs;
    edm::EDGetToken GEMSimHitsToken;
    edm::EDGetToken ME0SimHitsToken;
    std::vector<PSimHit>* PSimHitVect_; 
  // ==================================================================
};

// Constructor
GEMSimCollectionMerger::GEMSimCollectionMerger( const ParameterSet& ps ) { 

  GEMSimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("GEMSimHitInputLabel") );
  ME0SimHitsToken = consumes< std::vector<PSimHit> >( ps.getParameter<edm::InputTag>("ME0SimHitInputLabel") );
  PSimHitVect_ = new std::vector<PSimHit>;

  //produces<std::vector<PSimHit>>();
}

// Destructor
GEMSimCollectionMerger::~GEMSimCollectionMerger() {

  if ( PSimHitVect_ ) delete PSimHitVect_;
}

void GEMSimCollectionMerger::produce() ( Event& iEvent, EventSetup const& iSetup ) {

  edm::Handle< std::vector<PSimHit> > GEMSimHitsHandle;
  edm::Handle< std::vector<PSimHit> > ME0SimHitsHandle;
  iEvent.getByToken(GEMSimHitsToken, GEMSimHitsHandle);
  iEvent.getByToken(ME0SimHitsToken, ME0SimHitsHandle);

  if ( GEMSimHitHandle.isValid() && ME0SimHitHandle.isValid() ) std::cout << "working" << std::endl;

  /*std::auto_ptr< std::vector<PSimHit> > GEMSimHitCollection( new GEMSimHitCollection );
  iEvent.put( GEMSimHitCollection );*/
}

void GEMSimCollectionMerger::beginJob() {}
void GEMSimCollectionMerger::endJob(){}
void GEMSimCollectionMerger::beginRun(Run const& run, EventSetup const& iSetup){}
void GEMSimCollectionMerger::endRun(Run const& run, EventSetup const& iSetup){}
void GEMSimCollectionMerger::beginLuminosityBlock(LuminosityBlock const& lumi, EventSetup const& iSetup){}
void GEMSimCollectionMerger::endLuminosityBlock(LuminosityBlock const& lumi, EventSetup const& iSetup){}
void GEMSimCollectionMerger::respondToOpenInputFile(FileBlock const& block) {}
void GEMSimCollectionMerger::respondToCloseInputFile(FileBlock const& block) {}

// define this as a plugin
DEFINE_FWK_MODULE(GEMSimCollectionMerger);
