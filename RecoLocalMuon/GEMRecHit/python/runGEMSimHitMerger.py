import FWCore.ParameterSet.Config as cms
import os

from Configuration.StandardSequences.Eras import eras
process = cms.Process("GEMSimCollectionMerger", eras.Phase2) 

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D24Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D24_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
#from CondCore.ESSources.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(allowUnscheduled = cms.untracked.bool(True))
run2 = False

process.source = cms.Source ("PoolSource",
	fileNames = cms.untracked.vstring('file:/cms/ldap_home/chdlalsnr/CMSSW_11_0_0_pre2/src/22411/step1.root'),
	skipBadFiles = cms.untracked.bool(True)
)

process.out = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string('step1.root'),
	outputCommands = cms.untracked.vstring("keep *_*_*_*")#'drop *',
	  #"keep *_g4SimHits_*_*",
	  #"keep *_*_CombinedGEMSimHits_*")
)

#process.TFileService = cms.Service("TFileService", fileName = cms.string("outputFile.root"))
#GEMSimHitInputLabel = "MuonGEMHits"
#ME0SimHitInputLabel = "MuonME0Hits"

process.g4SimHits = cms.EDProducer("GEMSimCollectionMerger",
	GEMSimHitInputLabel = cms.InputTag("g4SimHits","MuonGEMHits","SIM"),
	ME0SimHitInputLabel = cms.InputTag("g4SimHits","MuonME0Hits","SIM")
)

process.p = cms.Path(process.g4SimHits)
process.e = cms.EndPath(process.out)
#process.schedule = cms.Schedule (process.p)
