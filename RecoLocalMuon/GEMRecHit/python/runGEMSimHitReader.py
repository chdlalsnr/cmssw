import FWCore.ParameterSet.Config as cms
import os

from Configuration.StandardSequences.Eras import eras
process = cms.Process("GEMSimHitCollectionMerger", eras.Phase2) 

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D24Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D24_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(allowUnscheduled = cms.untracked.bool(True))
run2 = False

process.source = cms.Source ("PoolSource",
	fileNames = cms.untracked.vstring('file:/cms/ldap_home/chdlalsnr/CMSSW_11_0_0_pre2/src/RecoLocalMuon/GEMRecHit/python/step1.root'),
	#fileNames = cms.untracked.vstring('file:/cms/ldap_home/chdlalsnr/CMSSW_11_0_0_pre2/src/22411/step1.root'),
	skipBadFiles = cms.untracked.bool(True)
)

#process.TFileService = cms.Service("TFileService", fileName = cms.string("out.root"))

process.test = cms.EDAnalyzer("GEMSimCollectionReader",
	CombinedGEMSimHitInputLabel = cms.InputTag("g4SimHits","CombinedMuonGEMHits","GEMSimCollectionMerger"),
	#ME0SimHitInputLabel = cms.InputTag("g4SimHits","MuonME0Hits","SIM")
	#CombinedGEMSimHitInputLabel = cms.InputTag("g4SimHits","MuonGEMHits","SIM")
)

process.p = cms.Path(process.test)
process.schedule = cms.Schedule (process.p)
