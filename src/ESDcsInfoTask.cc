#include <iostream>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Scalers/interface/DcsStatus.h"

#include "DQM/EcalPreshowerMonitorModule/interface/ESDcsInfoTask.h"

using namespace cms;
using namespace edm;
using namespace std;

ESDcsInfoTask::ESDcsInfoTask(const ParameterSet& ps) {

   dqmStore_ = Service<DQMStore>().operator->();

   prefixME_ = ps.getUntrackedParameter<string>("prefixME", "");

   enableCleanup_ = ps.getUntrackedParameter<bool>("enableCleanup", false);

   mergeRuns_ = ps.getUntrackedParameter<bool>("mergeRuns", false);

   dcsStatuslabel_ 	= ps.getParameter<InputTag>("DcsStatusLabel");

   meESDcsFraction_ = 0;
   meESDcsActiveMap_ = 0;

}

ESDcsInfoTask::~ESDcsInfoTask() {

}

void ESDcsInfoTask::beginJob(void){

   char histo[200];

   if ( dqmStore_ ) {

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");

      sprintf(histo, "DCSSummary");
      meESDcsFraction_ = dqmStore_->bookFloat(histo);
      meESDcsFraction_->Fill(-1.0);

      sprintf(histo, "DCSSummaryMap");
      meESDcsActiveMap_ = dqmStore_->book1D(histo,histo, 2, 0., 2.);
      meESDcsActiveMap_->setAxisTitle("(ES+/ES-)", 1);


   }

}

void ESDcsInfoTask::endJob(void) {

   if ( enableCleanup_ ) this->cleanup();

}

void ESDcsInfoTask::beginLuminosityBlock(const edm::LuminosityBlock& lumiBlock, const  edm::EventSetup& iSetup){

   this->reset();

   for( int i = 0; i < 2; i++) {
      meESDcsActiveMap_->setBinContent(i+1 , -1.0);
   }	    

}

void ESDcsInfoTask::endLuminosityBlock(const edm::LuminosityBlock&  lumiBlock, const  edm::EventSetup& iSetup) {

}

void ESDcsInfoTask::reset(void) {

   if ( meESDcsFraction_ ) meESDcsFraction_->Reset();

   if ( meESDcsActiveMap_ ) meESDcsActiveMap_->Reset();

}


void ESDcsInfoTask::cleanup(void){

   if ( dqmStore_ ) {

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");

      if ( meESDcsFraction_ ) dqmStore_->removeElement( meESDcsFraction_->getName() );

      if ( meESDcsActiveMap_ ) dqmStore_->removeElement( meESDcsActiveMap_->getName() );

   }

}

void ESDcsInfoTask::analyze(const Event& e, const EventSetup& c){ 

   ievt_++;

   float ESpDcsStatus = 0;
   float ESmDcsStatus = 0;

   Handle<DcsStatusCollection> dcsStatus;
   try{
      e.getByLabel(dcsStatuslabel_, dcsStatus);

      for(DcsStatusCollection::const_iterator dcsStatusItr = dcsStatus->begin(); dcsStatusItr != dcsStatus->end(); ++dcsStatusItr) 
      {
	 ESpDcsStatus = dcsStatusItr->ready(DcsStatus::ESp);
	 ESmDcsStatus = dcsStatusItr->ready(DcsStatus::ESm);
      }

   } catch ( cms::Exception &e  ) {
      LogDebug("") << "Error! can't get dcsStatus collection !" << std::endl;
   }

   ESpDcsStatus = (ESpDcsStatus + float(ievt_-1)*meESDcsActiveMap_->getBinContent(1))/float(ievt_);	
   ESmDcsStatus = (ESmDcsStatus + float(ievt_-1)*meESDcsActiveMap_->getBinContent(2))/float(ievt_);

   meESDcsActiveMap_->setBinContent(1, ESpDcsStatus);	
   meESDcsActiveMap_->setBinContent(2, ESmDcsStatus);

   meESDcsFraction_->Fill( (ESpDcsStatus + ESmDcsStatus)/2. );

}

//define this as a plug-in
DEFINE_FWK_MODULE(ESDcsInfoTask);
