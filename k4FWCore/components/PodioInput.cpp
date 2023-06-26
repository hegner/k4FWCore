#include "PodioInput.h"

#include "TFile.h"
#include "TROOT.h"

#include "k4FWCore/DataWrapper.h"
#include "k4FWCore/PodioDataSvc.h"

DECLARE_COMPONENT(PodioInput)

PodioInput::PodioInput(const std::string& name, ISvcLocator* svcLoc) : GaudiAlgorithm(name, svcLoc) {}

StatusCode PodioInput::initialize() {
  if (GaudiAlgorithm::initialize().isFailure())
    return StatusCode::FAILURE;

  // check whether we have the PodioEvtSvc active
  m_podioDataSvc = dynamic_cast<PodioDataSvc*>(evtSvc().get());
  if (nullptr == m_podioDataSvc)
    return StatusCode::FAILURE;

  // TODO: add an upfront check for existence of data products

  return StatusCode::SUCCESS;
}

StatusCode PodioInput::execute() {
  size_t cntr = 0;
  // Re-create the collections from ROOT file

  for (auto& collName : m_collectionNames) {
    debug() << "Registering collection to read " << collName << endmsg;
    if (m_podioDataSvc->readCollection(collName).isFailure()) {
      return StatusCode::FAILURE;
    }
  }

  // Tell data service that we are done with requested collections
  m_podioDataSvc->endOfRead();
  return StatusCode::SUCCESS;
}

StatusCode PodioInput::finalize() {
  if (GaudiAlgorithm::finalize().isFailure())
    return StatusCode::FAILURE;
  return StatusCode::SUCCESS;
}
