/// \file  EventAction.cc
/// \brief Implementation of the EventAction class

#include "TTree.h"

#include "RunAction.hh"
#include "EventAction.hh"
#include "ScintHit.hh"
#include "SiPMHit.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"

/**
 	TODO: 	Ntuple
**/

EventAction::EventAction(RunAction* runAction) : 
	G4UserEventAction(), fRunAction(runAction), fCollIDScint_gate(-1), fCollIDScint_telescope(-1), fCollIDSiPM_telescope(-1)
	, fEvID(-1){
		tmp_scint_telescope = new ScintSD("Scint_telescope",1);
		tmp_sipm_telescope = new SiPMSD("SiPM_telescope",2);
		// tmp_scint_gate = new ScintSD("Scint_gate",3);
	}

EventAction::~EventAction(){}

void EventAction::BeginOfEventAction(const G4Event*){}

void EventAction::EndOfEventAction(const G4Event* event){
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	if(event->GetEventID()==0){
	}

	// Hits collections
	G4HCofThisEvent*HCE = event->GetHCofThisEvent();
	if(!HCE) return;

	//###################################
	ScintHit* scintHit;
	G4int N;
	G4bool gate = false;
	G4bool telescope = true;
	if(gate){
		if(fCollIDScint_gate < 0){
			G4SDManager* SDMan = G4SDManager::GetSDMpointer();
			fCollIDScint_gate = SDMan->GetCollectionID("Scint_gate/scintCollection");
		}

		if(fCollIDScint_gate){
		ScintHitsCollection* ScintHitCollection_gate = (ScintHitsCollection*) (HCE->GetHC(fCollIDScint_gate));

		N = ScintHitCollection_gate->entries();
		for(int i = 0; i < N; i++){
			scintHit = (*ScintHitCollection_gate)[i];

			fEvID = event->GetEventID();

			tmp_scint_gate->FillNtupla(man, scintHit, 3);

			scintHit->Clear();
		}
		}
	}
	//###################################
	if(telescope){
	if(fCollIDScint_telescope < 0){
		G4SDManager* SDMan = G4SDManager::GetSDMpointer();
		fCollIDScint_telescope = SDMan->GetCollectionID("Scint_telescope/scintCollection");
	}
	
	ScintHitsCollection* ScintHitCollection_telescope = (ScintHitsCollection*) (HCE->GetHC(fCollIDScint_telescope));

	N = ScintHitCollection_telescope->entries();
	for(int i = 0; i < N; i++){
		scintHit = (*ScintHitCollection_telescope)[i];

		fEvID = event->GetEventID();

		tmp_scint_telescope->FillNtupla(man, scintHit, 1);

		scintHit->Clear();
	}

	if(fEvID % 100 == 0 || (fEvID & (fEvID - 1)) == 0 ) 
	std::cout << "Filled first ntupla" << std::endl;
	}

	//###################################
	if(fCollIDSiPM_telescope < 0){
		G4SDManager* SDMan = G4SDManager::GetSDMpointer();
		fCollIDSiPM_telescope = SDMan->GetCollectionID("SiPM_telescope/sipmCollection");
	}
	
	SiPMHitsCollection* SiPMHitCollection_telescope = (SiPMHitsCollection*) (HCE->GetHC(fCollIDSiPM_telescope));

	SiPMHit* sipmHit;
	N = SiPMHitCollection_telescope->entries();
	for(int i = 0; i < N; i++){
		sipmHit = (*SiPMHitCollection_telescope)[i];

		fEvID = event->GetEventID();

		tmp_sipm_telescope->FillNtupla(man, sipmHit, 2);

		sipmHit->Clear();
	}

	if(fEvID % 100 == 0 || (fEvID & (fEvID - 1)) == 0 ) 
	{std::cout << "Filled second ntupla" << std::endl;
	std::cout << "Event n. " << fEvID << std::endl;}
}
