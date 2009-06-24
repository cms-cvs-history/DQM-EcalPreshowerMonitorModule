#ifndef ESOccupancyTask_H
#define ESOccupancyTask_H


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class decleration
//
class MonitorElement;
class DQMStore;

class ESOccupancyTask : public edm::EDAnalyzer {
   public:
      ESOccupancyTask(const edm::ParameterSet& ps);
      virtual ~ESOccupancyTask();


   private:
      virtual void beginJob(const edm::EventSetup & c) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob(void) ;

      // ----------member data ---------------------------
      edm::InputTag rechitlabel_;
      edm::InputTag digilabel_;
      std::string prefixME_;


      DQMStore* dqmStore_;
      MonitorElement* hRecOCC_[4];
      MonitorElement* hRecNHit_[4];
      MonitorElement* hDigiOCC_[4];
      MonitorElement* hDigiNHit_[4];
      MonitorElement* hEng_[4];
      MonitorElement* hEvEng_[4];
      MonitorElement* hE1E2_[2];


      int runNum_, eCount_; 

};

#endif
