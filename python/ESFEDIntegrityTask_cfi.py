import FWCore.ParameterSet.Config as cms
    
ecalPreshowerFEDIntegrityTask = cms.EDAnalyzer('ESFEDIntegrityTask',
                                               prefixME = cms.untracked.string('EcalPreshower'),
                                               ESDCCCollections = cms.InputTag("ecalPreshowerDigis"),
                                               ESKChipCollections = cms.InputTag("ecalPreshowerDigis"),
                                               FEDRawDataCollection = cms.InputTag("source"),
                                               OutputFile = cms.untracked.string("")
                                               )

