#ifndef ESTimingTask_H
#define ESTimingTask_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TF1.h"

class MonitorElement;
class DQMStore;

class ESTimingTask : public edm::EDAnalyzer {

 public:
  
  ESTimingTask(const edm::ParameterSet& ps);
  virtual ~ESTimingTask();
  
 private:
  
  virtual void beginJob(void);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(void) ;
  
  // ----------member data ---------------------------
  edm::InputTag rechitlabel_;
  edm::InputTag digilabel_;
  std::string prefixME_;
  
  DQMStore* dqmStore_;
  MonitorElement* hTiming_[2][2];

  TF1 *fit_;

  int runNum_, eCount_; 
  
};

#endif