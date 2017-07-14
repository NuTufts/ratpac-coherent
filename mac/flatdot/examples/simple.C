/*
// to run
// root
// [0] .L simple.C
// [1] simple("output.root")

int simple( std::string input_file )
{

  TH1* xPos = new TH1F("xPos", "xPos", 100, -30.00, 30.00);
  TH1* yPos = new TH1F("yPos", "yPos", 100, -30.00, 30.00);
  TH1* tSE = new TH1F("totalScintEdep", "totalScintEdep", 100, -30.00, 30.00); 
  TH1* tSEQ = new TH1F("totalScintEdepQuenched", "totalScintEdepQuenched", 100, -30.00, 30.00);
  TH1* ECX = new TH1F("EnergyCentroidX", "EnergyCentroidX", 100, -20.00, 20.00);
  TH1* ECY = new TH1F("EnergyCentroidY", "EnergyCentroidY", 100, -20.00, 20.00);
  TH1* ERMSX = new TH1F("EnergyRMSX", "EnergyRMSX", 100, 0.00, 15.00);
  TH1* ERMSY = new TH1F("EnergyRMSY", "EnergyRMSY", 100, 0.00, 15.00);
  // THIS JUST DUMPS THE NUMBER OF PMT hits
  
  RAT::DSReader reader(input_file.c_str());
  
  int nevents = reader.GetTotal();

  for (int iev=0; iev<nevents; iev++) {
    RAT::DS::Root* dsroot = reader.NextEvent();
    RAT::DS::MC* mc = dsroot->GetMC();
    RAT::DS::MCSummary* mcsummary = mc->GetMCSummary();
    tSE->Fill(mcsummary->totalScintEdep);
    tSEQ->Fill(mcsummary->totalScintEdepQuenched);  
    TVector3 energycentroid = mcsummary->GetEnergyCentroid(); 
    TVector3 energyRMS = mcsummary->GetEnergyRMS(); 
    ECX->Fill(energycentroid.X()); 
    ECY->Fill(energycentroid.Y()); 
    ERMSX->Fill(energyRMS.X()); 
    ERMSY->Fill(energyRMS.Y()); 
    int npes = mc->GetNumPE();
    int npmts = mc->GetMCPMTCount();

    //std::cout <<  "Event " << iev << std::endl;
    //std::cout << "  nPEs=" << npes << " npmts=" << npmts << std::endl;
    int mc_pmt_count = mc->GetMCPMTCount(); 
    int num_evt_photons = 0; 
    for (int index=0; index < mc_pmt_count; index++) {
      RAT::DS::MCPMT* mcpmt = mc->GetMCPMT(index);
      int mc_photon_count = mcpmt->GetMCPhotonCount(); 
      for (int photon=0; photon < mc_photon_count; photon++) { 
        RAT::DS::MCPhoton* mcphoton = mcpmt->GetMCPhoton(photon);
      	TVector3 position = mcphoton->GetPosition(); 
        xPos->Fill(position.X());
	yPos->Fill(position.Y());
        num_evt_photons ++ ;
//        std::cout<<position.X()<< "," << position.Y() << std::endl; 
     }
    }  
}
  //xPos->Draw(); 
  //xPos->GetXaxis()->SetTitle("xHit Position (mm)"); 
  //yPos->Draw(); 
  //yPos->GetXaxis()->SetTitle("yHit Position(mm)"); 
  //tSE->Draw(); 
  //tSE->GetXaxis()->SetTitle("Total Scintillation Edep (MeV)"); 
  //tSEQ->Draw(); 
  //tSEQ->GetXaxis()->SetTitle("Total Scintillation Edep - Quenched (MeV)"); 
  ECX->Draw();
  ECX->GetXaxis()->SetTitle("Energy Centroid - X (mm)");
  //ECY->Draw();
  //ECY->GetXaxis()->SetTitle("Energy Centroid - Y (mm)");
  //ERMSX->Draw();
  //ERMSX->GetXaxis()->SetTitle("Energy RMS - X (mm)");
  //ERMSY->Draw();
  //ERMSY->GetXaxis()->SetTitle("Energy RMS - Y (mm)");
  return 0;
};
*/
