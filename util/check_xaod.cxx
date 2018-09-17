// EDM
#include "xAODJet/JetContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODMuon/MuonContainer.h"
#include "AsgTools/ToolHandle.h"
#include "EgammaAnalysisInterfaces/IAsgElectronLikelihoodTool.h"
#include "MuonAnalysisInterfaces/IMuonSelectionTool.h"

// AnalysisBase
#include "xAODRootAccess/Init.h" 
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/tools/ReturnCheck.h"
#include "AsgTools/AnaToolHandle.h"

// std/stl
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
using namespace std;

// ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"

enum DileptonFlavor {
    EE=0,
    MM,
    EM,
    ME,
    SF,
    DF,
    Invalid
};

int main(int argc, char * argv[])
{
    const char* ALG = argv[0];
    if(argc < 2) {
        cout << "ERROR expect input file" << endl;
        cout << "Usage: " << ALG << " <input-file> [n-events]" << endl;
        return 1;
    }
    string filename = "";
    int nentries = -1;
    try {
        filename = argv[1];
    }
    catch(std::exception& e) {
        cout << "ERROR expect an input file as an argument" << endl;
        return 1;
    }
    if(argc > 2) {
        try {
            nentries = atoi(argv[2]);
        }
        catch(...) {

        }
    }

    RETURN_CHECK(ALG, xAOD::Init());
    xAOD::TEvent event(xAOD::TEvent::kClassAccess);

    std::unique_ptr<TFile> ifile(TFile::Open(filename.c_str(), "READ"));
    if( ! ifile.get() || ifile->IsZombie() ) {
        throw std::logic_error("ERROR Could not open input file: " + filename);
        return 1;
    }

    asg::AnaToolHandle<CP::IMuonSelectionTool> muon_sel_tool;
    string toolname = "CP::MuonSelectionTool/MediumMuonTool";
    muon_sel_tool.setTypeAndName(toolname);
    RETURN_CHECK(ALG, muon_sel_tool.setProperty("MaxEta", 2.7));
    RETURN_CHECK(ALG, muon_sel_tool.setProperty("MuQuality", int(xAOD::Muon::Medium)));
    RETURN_CHECK(ALG, muon_sel_tool.retrieve());

    asg::AnaToolHandle<IAsgElectronLikelihoodTool> ele_sel_tool;
    toolname = "AsgElectronLikelihoodTool/LooseEleTool";
    ele_sel_tool.setTypeAndName(toolname);
    RETURN_CHECK(ALG, ele_sel_tool.setProperty("WorkingPoint", "LooseBLLHElectron"));
    RETURN_CHECK(ALG, ele_sel_tool.retrieve());
    

    cout << "Opened file : " << filename << endl;

    RETURN_CHECK(ALG, event.readFrom(ifile.get()));

    // count histograms
    int n_total_el = 0;
    int n_total_mu = 0;

    // begin event loop
    const unsigned long long entries = event.getEntries();
    if(nentries < 0) nentries = entries;
    cout << "Will read " << nentries << " events from file" << endl;
    for(unsigned long long entry = 0; entry < nentries; ++entry) {
        if( entry % 500 == 0) {
            cout << "Processing entry " << entry << " / " << nentries << endl;
        }
        bool ok = event.getEntry(entry) >= 0;
        if(!ok) throw std::logic_error("ERROR getEntry failed");

        // lets get the containers
        const xAOD::JetContainer* jets = 0;
        RETURN_CHECK(ALG, event.retrieve(jets, "AntiKt4EMTopoJets"));

        const xAOD::ElectronContainer* electrons = 0;
        RETURN_CHECK(ALG, event.retrieve(electrons, "Electrons"));

        const xAOD::MuonContainer* muons = 0;
        RETURN_CHECK(ALG, event.retrieve(muons, "Muons"));

        int n_elec = 0;
        int n_mu = 0;

        for(const auto & ele : *electrons) {
            if(ele_sel_tool->accept(ele)) {
                if(ele->pt() > 10. * 1000. && std::abs(ele->eta()) < 2.47) {
                    n_elec++;
                } // kinmeatic
            } // LLH
        } // ele
        n_total_el += n_elec;

        for(const auto & mu : *muons) {
            if(muon_sel_tool->accept(*mu)) {
                if(mu->pt() > 10. * 1000 && std::abs(mu->eta()) < 2.5) {
                    n_mu++;
                }
            }
        } // mu
        n_total_mu += n_mu;

    } // entry
    
    cout << "total electrons seen: " << n_total_el << endl;
    cout << "total muons     seen: " << n_total_mu << endl;

    return 0;
}


