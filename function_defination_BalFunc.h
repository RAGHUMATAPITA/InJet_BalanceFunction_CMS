#include "input_variables.h"
#include "histogram_definition_BalFunc.h"

void signal_corr(std::vector<float> evtw_vec, std::vector<int> hiBin_vect, std::vector<std::vector<TVector3>> jet_vec, std::vector<std::vector<int>> refpartonB_vec, std::vector<std::vector<double>> jetw_vec, std::vector<std::vector<std::vector<TVector3>>> jet_oldtrk_vec, std::vector<std::vector<std::vector<TVector3>>> jet_newtrk_vec, std::vector<std::vector<std::vector<TVector3>>> jet_oldchtrk_vec, std::vector<std::vector<std::vector<TVector3>>> jet_newchtrk_vec, std::vector<std::vector<std::vector<int>>> jet_trk_charge, bool isrc)
{
  if(jet_newtrk_vec.size() != jet_vec.size() || jet_newtrk_vec.size() != evtw_vec.size())
    {
      std::cout<<"event numbers are not same, pleaes check"<<std::endl;
    }
  
  else std::cout<<"Total events is: "<<jet_newtrk_vec.size()<<"  "<<jet_vec.size()<<"  "<<evtw_vec.size()<<std::endl;
  
  std::cout<<endl;
  std::cout<<"~~~~~start signal correlation~~~~~~~~"<<std::endl;
  
  for(int ievt = 0; ievt < (int)jet_newtrk_vec.size(); ievt++) // event loop
    {
      if(ievt%10000 == 0) std::cout<<ievt<<"  events running for signal correlation of total events "<< jet_newtrk_vec.size() <<std::endl;
      
      double evtw = evtw_vec[ievt];
      int ctbin = hiBin_vect[ievt];
      
      int njets = 0;
      
      for(int ijet = 0; ijet < (int)jet_newtrk_vec[ievt].size(); ijet++) // jet loop
	{
	  TVector3 jetVec = jet_vec[ievt][ijet];
	  double jet_pt = jetVec.Pt();
	  double jet_eta = jetVec.Eta();
	  double jet_phi = jetVec.Phi();
	  int ntrk_jet = jet_newtrk_vec[ievt][ijet].size();
	  int nchtrk_jet = jet_newchtrk_vec[ievt][ijet].size();
	  int refpartonB = refpartonB_vec[ievt][ijet]; 
	  double jetw = jetw_vec[ievt][ijet];
	  
	  int nchbin  = hnch_bin_hist->FindBin(nchtrk_jet) - 1;
	  
	  //extract the jet pt bin                                                                                                        
	  int ptbin = hpt_bin_hist->FindBin(jet_pt) - 1;
	  
	  if(ptbin >= pt_bin)
	    {
	      std::cout<<"pT bin is exceeding: "<<jet_pt<<std::endl;
	      continue;
	    }

	  if(isrc)
	    {
	      hJet_pt->Fill(jet_pt, jetw);
	      hJet_ntrk->Fill(ntrk_jet, jetw);
	      hJet_nchtrk->Fill(nchtrk_jet, jetw);
	      hJet_nchtrk_pT->Fill(nchtrk_jet, jet_pt, jetw);
	      hJet_nchtrk_[nchbin]->Fill(nchtrk_jet, jetw);
	    }
	  else
	    {
	      hJet_Gen_pt->Fill(jet_pt, jetw);
              hJet_Gen_ntrk->Fill(ntrk_jet, jetw);
              hJet_Gen_nchtrk->Fill(nchtrk_jet, jetw);
              hJet_Gen_nchtrk_pT->Fill(nchtrk_jet, jet_pt, jetw);
              hJet_Gen_nchtrk_[nchbin]->Fill(nchtrk_jet, jetw);
	    }
	  for(int itrk = 0; itrk < (int)jet_newtrk_vec[ievt][ijet].size(); itrk++) // trigger trk loop
	    {
	      TVector3 old_trgtrk_vec = jet_oldtrk_vec[ievt][ijet][itrk];
	      double old_trgtrk_pt = old_trgtrk_vec.Pt();
	      double old_trgtrk_eta = old_trgtrk_vec.Eta();
	      double old_trgtrk_phi = old_trgtrk_vec.Phi();
	      TVector3 new_trgtrk_vec = jet_newtrk_vec[ievt][ijet][itrk];
	      double new_trgtrk_pt = new_trgtrk_vec.Pt();
	      double new_trgtrk_eta = new_trgtrk_vec.Eta();
	      double new_trgtrk_phi = new_trgtrk_vec.Phi();
	      int trgtrk_charge = jet_trk_charge[ievt][ijet][itrk];
	      if(isrc)
		{
		  halltrk_pt->Fill(old_trgtrk_pt);
		  halltrk_eta->Fill(old_trgtrk_eta);
		  halltrk_phi->Fill(old_trgtrk_phi);
		}
	      else
		{
		  halltrk_Gen_pt->Fill(old_trgtrk_pt);
		  halltrk_Gen_eta->Fill(old_trgtrk_eta);
		  halltrk_Gen_phi->Fill(old_trgtrk_phi);
		}
	      
	      if(TMath::Abs(trgtrk_charge) > 0 )
		{
		  //if(TMath::Abs(new_trgtrk_eta) <= newtrk_eta_max)
		    {
		      //if(new_trgtrk_pt >= newtrk_jt03_min && new_trgtrk_pt <= newtrk_jt03_max)
			{
			  double newchtrk_Ntrig[4] = {1, (double)ptbin, (double)refpartonB, (double)ctbin};
			  
			  if(isrc)
			    {
			      hchtrk_pt->Fill(old_trgtrk_pt, jetw);
			      hchtrk_eta->Fill(old_trgtrk_eta, jetw);
			      hchtrk_phi->Fill(old_trgtrk_phi, jetw);
			      hchtrk_pt_jetaxis->Fill(new_trgtrk_pt, jetw);
			      hchtrk_eta_jetaxis->Fill(new_trgtrk_eta, jetw);
			      hchtrk_phi_jetaxis->Fill(new_trgtrk_phi, jetw);
			      
			      hnewchtrk_Ntrig->Fill(newchtrk_Ntrig, jetw);
			      
			      if(trgtrk_charge > 0)
				{
				  hchtrk_pt_p->Fill(old_trgtrk_pt, jetw);
				  hchtrk_eta_p->Fill(old_trgtrk_eta, jetw);
				  hchtrk_phi_p->Fill(old_trgtrk_phi, jetw);
				  hchtrk_pt_jetaxis_p->Fill(new_trgtrk_pt, jetw);
				  hchtrk_eta_jetaxis_p->Fill(new_trgtrk_eta, jetw);
				  hchtrk_phi_jetaxis_p->Fill(new_trgtrk_phi, jetw);
				  
				  hnewchtrk_Ntrig_p->Fill(newchtrk_Ntrig, jetw);
				}
			      else if(trgtrk_charge < 0)
				{
				  hchtrk_pt_m->Fill(old_trgtrk_pt, jetw);
				  hchtrk_eta_m->Fill(old_trgtrk_eta, jetw);
				  hchtrk_phi_m->Fill(old_trgtrk_phi, jetw);
				  hchtrk_pt_jetaxis_m->Fill(new_trgtrk_pt, jetw);
				  hchtrk_eta_jetaxis_m->Fill(new_trgtrk_eta, jetw);
				  hchtrk_phi_jetaxis_m->Fill(new_trgtrk_phi, jetw);
				  
				  hnewchtrk_Ntrig_m->Fill(newchtrk_Ntrig, jetw);
				}
			    }
			  else
			    {
			      hchtrk_Gen_pt->Fill(old_trgtrk_pt, jetw);
			      hchtrk_Gen_eta->Fill(old_trgtrk_eta, jetw);
			      hchtrk_Gen_phi->Fill(old_trgtrk_phi, jetw);
			      hchtrk_Gen_pt_jetaxis->Fill(new_trgtrk_pt, jetw);
			      hchtrk_Gen_eta_jetaxis->Fill(new_trgtrk_eta, jetw);
			      hchtrk_Gen_phi_jetaxis->Fill(new_trgtrk_phi, jetw);
			      
			      hnewchtrk_Gen_Ntrig->Fill(newchtrk_Ntrig, jetw);
			      
			      if(trgtrk_charge > 0)
				{
				  hchtrk_Gen_pt_p->Fill(old_trgtrk_pt, jetw);
				  hchtrk_Gen_eta_p->Fill(old_trgtrk_eta, jetw);
				  hchtrk_Gen_phi_p->Fill(old_trgtrk_phi, jetw);
				  hchtrk_Gen_pt_jetaxis_p->Fill(new_trgtrk_pt, jetw);
				  hchtrk_Gen_eta_jetaxis_p->Fill(new_trgtrk_eta, jetw);
				  hchtrk_Gen_phi_jetaxis_p->Fill(new_trgtrk_phi, jetw);
				  
				  hnewchtrk_Gen_Ntrig_p->Fill(newchtrk_Ntrig, jetw);
				}
			      else if(trgtrk_charge < 0)
				{
				  hchtrk_Gen_pt_m->Fill(old_trgtrk_pt, jetw);
				  hchtrk_Gen_eta_m->Fill(old_trgtrk_eta, jetw);
				  hchtrk_Gen_phi_m->Fill(old_trgtrk_phi, jetw);
				  hchtrk_Gen_pt_jetaxis_m->Fill(new_trgtrk_pt, jetw);
				  hchtrk_Gen_eta_jetaxis_m->Fill(new_trgtrk_eta, jetw);
				  hchtrk_Gen_phi_jetaxis_m->Fill(new_trgtrk_phi, jetw);
				  
				  hnewchtrk_Gen_Ntrig_m->Fill(newchtrk_Ntrig, jetw);
				}
			    }
			} // trk pt condition
		    } // trk eta condition
		} // charge condition
	      
	      for(int jtrk = 0; jtrk < (int)jet_newtrk_vec[ievt][ijet].size(); jtrk++) // associate trk loop
		{
		  if(itrk == jtrk) continue; // to avoid auto correlation
		  
		  TVector3 old_assotrk_vec = jet_oldtrk_vec[ievt][ijet][jtrk];
		  double old_assotrk_pt = old_assotrk_vec.Pt();
		  double old_assotrk_eta = old_assotrk_vec.Eta();
		  double old_assotrk_phi = old_assotrk_vec.Phi();
		  TVector3 new_assotrk_vec = jet_newtrk_vec[ievt][ijet][jtrk];
		  double new_assotrk_pt = new_assotrk_vec.Pt();
		  double new_assotrk_eta = new_assotrk_vec.Eta();
		  double new_assotrk_phi = new_assotrk_vec.Phi();
		  int assotrk_charge = jet_trk_charge[ievt][ijet][jtrk];
		  
		  double deta_jetaxis = new_trgtrk_eta - new_assotrk_eta;
     		  double dphi_jetaxis = new_trgtrk_phi - new_assotrk_phi;
   		  double dphi2_jetaxis = new_assotrk_phi - new_trgtrk_phi;
                 		  

		  if(dphi_jetaxis > 1.5*TMath::Pi())
		    {
		      dphi_jetaxis = dphi_jetaxis - 2.0*TMath::Pi();
		    }
		  else if(dphi_jetaxis < -0.5*TMath::Pi())
		    {
		      dphi_jetaxis = dphi_jetaxis + 2.0*TMath::Pi();
		    }
		  
		  if(dphi2_jetaxis > 1.5*TMath::Pi())
		    {
		      dphi2_jetaxis = dphi2_jetaxis - 2.0*TMath::Pi();
		    }
		  else if(dphi2_jetaxis < -0.5*TMath::Pi())
		    {
		      dphi2_jetaxis = dphi2_jetaxis + 2.0*TMath::Pi();
		    }

		  if(TMath::Abs(trgtrk_charge) > 0 && TMath::Abs(assotrk_charge) > 0)
		    {
		      if(TMath::Abs(new_trgtrk_eta) <= newtrk_eta_max && TMath::Abs(new_assotrk_eta) <= newtrk_eta_max)
			{
			  if((new_trgtrk_pt >= newtrk_jt03_min && new_trgtrk_pt <= newtrk_jt03_max) && (new_assotrk_pt >= newtrk_jt03_min && new_assotrk_pt <= newtrk_jt03_max))
			    {
			      double signal_axis_newchtrk_jt03_1[5] = {TMath::Abs(deta_jetaxis), dphi_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
			      double signal_axis_newchtrk_jt03_2[5] = {-TMath::Abs(deta_jetaxis), dphi_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
			      double signal_axis_newchtrk_jt03_3[5] = {TMath::Abs(deta_jetaxis), dphi2_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
			      double signal_axis_newchtrk_jt03_4[5] = {-TMath::Abs(deta_jetaxis), dphi2_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
			      if(isrc)
				{
				  hsignal_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);		     
				  hsignal_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				  hsignal_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				  hsignal_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				  
				  if(trgtrk_charge > 0 && assotrk_charge > 0)
				    {
				      hsignal_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				      
				    }
				  if(trgtrk_charge < 0 && assotrk_charge < 0)
				    {
				      
				      hsignal_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				    }
				  if(trgtrk_charge > 0 && assotrk_charge < 0)
				    {
				      hsignal_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				    }
				  if(trgtrk_charge < 0 && assotrk_charge > 0)
				    {
				      hsignal_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				    }
				}
			      else
				{
				  hsignal_Gen_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);		     
				  hsignal_Gen_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				  hsignal_Gen_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				  hsignal_Gen_newchtrk_jt03->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				  
				  if(trgtrk_charge > 0 && assotrk_charge > 0)
				    {
				      hsignal_Gen_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_pp->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				      
				    }
				  if(trgtrk_charge < 0 && assotrk_charge < 0)
				    {
				      
				      hsignal_Gen_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_mm->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				    }
				  if(trgtrk_charge > 0 && assotrk_charge < 0)
				    {
				      hsignal_Gen_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_pm->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				    }
				  if(trgtrk_charge < 0 && assotrk_charge > 0)
				    {
				      hsignal_Gen_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_1, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_2, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_3, jetw/4.);
				      hsignal_Gen_newchtrk_jt03_mp->Fill(signal_axis_newchtrk_jt03_4, jetw/4.);
				    }
				}
			    } // trk pt condition
			} // trk eta condition
		    } // trk charge condition
		} // asso trk loop
	    } // trg trk loop end
	  njets++;
	} // jet loop end
      if(njets > 0)
	{
	  if(isrc) hnjets_afterCut->Fill(njets);
	  else hnjets_Gen_afterCut->Fill(njets);
	}
    } // event loop end
  std::cout<<endl;
  std::cout<<"~~~~~end signal correlation~~~~~~~~"<<std::endl;
} // function loop

void mixing_corr(std::vector<float> evtw_vec, std::vector<int> hiBin_vect, std::vector<std::vector<TVector3>> jet_vec, std::vector<std::vector<int>> refpartonB_vec, std::vector<std::vector<double>> jetw_vec, std::vector<std::vector<std::vector<TVector3>>> jet_oldtrk_vec, std::vector<std::vector<std::vector<TVector3>>> jet_newtrk_vec, std::vector<std::vector<std::vector<TVector3>>> jet_oldchtrk_vec, std::vector<std::vector<std::vector<TVector3>>> jet_newchtrk_vec, std::vector<std::vector<std::vector<int>>> jet_trk_charge, std::vector<float> vertexz, bool isrc)
{
  std::cout<<endl;
  std::cout<<"~~~~~start mixed event correlation~~~~~~~~"<<std::endl;
  
  for(int ievt = 0; ievt < (int)jet_newtrk_vec.size(); ievt++) // 1st event loop
    {
      if(ievt%10000 == 0) std::cout<<ievt<<"  events running for mixing correlation of total events "<< jet_newtrk_vec.size() <<std::endl;
      
      double evtw = evtw_vec[ievt];
      int ctbin = hiBin_vect[ievt];
      float ivertexz = vertexz[ievt];
      
      // mixing algorithm
      int mixstart = ievt+1;
      int mixend   = (int)jet_newtrk_vec.size();
      
      if(mixstart > (0.7*(jet_newtrk_vec.size())))
	{
	  int mixstart = (0.5*(jet_newtrk_vec.size()));
	  int mixend   = (int)jet_newtrk_vec.size();
	}
      
       int nmix = 0;
       /*
       int mixstart = ievt - bkgFactor/2;
       int mixend   = ievt + bkgFactor/2 + 1;

      if(ievt < bkgFactor/2)
	{
	  mixstart = 0;
	  mixend   = bkgFactor + 1;
	}
      else if(ievt > (int)jet_newtrk_vec.size() - bkgFactor/2 - 1)
	{
	  mixstart = (int)jet_newtrk_vec.size() - bkgFactor - 1;
	  mixend   = (int)jet_newtrk_vec.size();
	}
      if( mixend > (int)jet_newtrk_vec.size() )
	mixend = (int)jet_newtrk_vec.size();
       */
       
      for(int jevt = mixstart; jevt < mixend; jevt++) // 2nd event loop
	{
	  if(ievt == jevt) continue;
	  float jvertexz = vertexz[jevt];
	  
	  if (fabs(jvertexz - ivertexz) > 2.0) continue;
	  
	  nmix++;
	  if(nmix > bkgFactor) break;
	  
	  for(int ijet = 0; ijet < (int)jet_newtrk_vec[ievt].size(); ijet++) // 1st jet loop
	    {
	      TVector3 jetVec = jet_vec[ievt][ijet];
	      double jet_pt = jetVec.Pt();
	      double jet_eta = jetVec.Eta();
	      double jet_phi = jetVec.Phi();
	      int nchtrk_jet = jet_newchtrk_vec[ievt][ijet].size();
	      int ntrk_jet = jet_newtrk_vec[ievt][ijet].size();
	      int refpartonB = refpartonB_vec[ievt][ijet]; 
	      double jetw = jetw_vec[ievt][ijet];
	      
	      int nchbin  = hnch_bin_hist->FindBin(nchtrk_jet) - 1;
	      
	      //extract the jet pt bin
	      int ptbin = hpt_bin_hist->FindBin(jet_pt) - 1;
	      
	      if(ptbin >= pt_bin)
		{
		  std::cout<<"pT bin is exceeding in Mixing: "<<jet_pt<<std::endl;
		  continue;
		}
	      
	      for(int jjet = 0; jjet < (int)jet_newtrk_vec[jevt].size(); jjet++) // 2nd jet loop
		{
		  TVector3 jetVec2 = jet_vec[jevt][jjet];
		  double jet_pt2 = jetVec.Pt();
		  double jet_eta2 = jetVec.Eta();
		  double jet_phi2 = jetVec.Phi();
		  int nchtrk_jet2 = jet_newchtrk_vec[jevt][jjet].size();
		  int ntrk_jet2 = jet_newtrk_vec[jevt][jjet].size();
		  int refpartonB2 = refpartonB_vec[jevt][jjet];
		  double jetw2 = jetw_vec[jevt][jjet];
	
		  for(int itrk = 0; itrk < (int)jet_newtrk_vec[ievt][ijet].size(); itrk++) // trigger trk loop
		    {
		      TVector3 old_trgtrk_vec = jet_oldtrk_vec[ievt][ijet][itrk];
		      double old_trgtrk_pt = old_trgtrk_vec.Pt();
		      double old_trgtrk_eta = old_trgtrk_vec.Eta();
		      double old_trgtrk_phi = old_trgtrk_vec.Phi();
		      TVector3 new_trgtrk_vec = jet_newtrk_vec[ievt][ijet][itrk];
		      double new_trgtrk_pt = new_trgtrk_vec.Pt();
		      double new_trgtrk_eta = new_trgtrk_vec.Eta();
		      double new_trgtrk_phi = new_trgtrk_vec.Phi();
		      int trgtrk_charge = jet_trk_charge[ievt][ijet][itrk];
		      		      
		      for(int jtrk = 0; jtrk < (int)jet_newtrk_vec[jevt][jjet].size(); jtrk++) // associate trk loop
			{
			  TVector3 old_assotrk_vec = jet_oldtrk_vec[jevt][jjet][jtrk];
			  double old_assotrk_pt = old_assotrk_vec.Pt();
			  double old_assotrk_eta = old_assotrk_vec.Eta();
			  double old_assotrk_phi = old_assotrk_vec.Phi();
			  TVector3 new_assotrk_vec = jet_newtrk_vec[jevt][jjet][jtrk];
			  double new_assotrk_pt = new_assotrk_vec.Pt();
			  double new_assotrk_eta = new_assotrk_vec.Eta();
			  double new_assotrk_phi = new_assotrk_vec.Phi();
			  int assotrk_charge = jet_trk_charge[jevt][jjet][jtrk];
		
			  double deta_jetaxis = new_trgtrk_eta - new_assotrk_eta;
			  double dphi_jetaxis = new_trgtrk_phi - new_assotrk_phi;
			  double dphi2_jetaxis = new_assotrk_phi - new_trgtrk_phi;
			  
			  if(dphi_jetaxis > 1.5*TMath::Pi())
			    {
			      dphi_jetaxis = dphi_jetaxis - 2.0*TMath::Pi();
			    }
			  else if(dphi_jetaxis < -0.5*TMath::Pi())
			    {
			      dphi_jetaxis = dphi_jetaxis + 2.0*TMath::Pi();
			    }
			  
			  if(dphi2_jetaxis > 1.5*TMath::Pi())
			    {
			      dphi2_jetaxis = dphi2_jetaxis - 2.0*TMath::Pi();
			    }
			  else if(dphi2_jetaxis < -0.5*TMath::Pi())
			    {
			      dphi2_jetaxis = dphi2_jetaxis + 2.0*TMath::Pi();
			    }
			  
			  if(TMath::Abs(trgtrk_charge) > 0 && TMath::Abs(assotrk_charge) > 0)
			    {
			      if(TMath::Abs(new_trgtrk_eta) <= newtrk_eta_max && TMath::Abs(new_assotrk_eta) <= newtrk_eta_max)
				{
				  if((new_trgtrk_pt >= newtrk_jt03_min && new_trgtrk_pt <= newtrk_jt03_max) && (new_assotrk_pt >= newtrk_jt03_min && new_assotrk_pt <= newtrk_jt03_max))
				    {
				      
                                      double mixing_axis_newchtrk_jt03_1[5] = {TMath::Abs(deta_jetaxis), dphi_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
                                      double mixing_axis_newchtrk_jt03_2[5] = {-TMath::Abs(deta_jetaxis), dphi_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
				      double mixing_axis_newchtrk_jt03_3[5] = {TMath::Abs(deta_jetaxis), dphi2_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
				      double mixing_axis_newchtrk_jt03_4[5] = {-TMath::Abs(deta_jetaxis), dphi2_jetaxis, (double)ptbin, (double)refpartonB, (double)ctbin};
				      if(isrc)
					{
					  hmixing_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					  hmixing_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					  hmixing_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					  hmixing_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					  
					  if(trgtrk_charge > 0 && assotrk_charge > 0)
					    {
					      hmixing_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					  if(trgtrk_charge < 0 && assotrk_charge < 0)
					    {
					      hmixing_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					  if(trgtrk_charge > 0 && assotrk_charge < 0)
					    {
					      hmixing_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					  if(trgtrk_charge < 0 && assotrk_charge > 0)
					    {
					      hmixing_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					}
				      else
					{
					  hmixing_Gen_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					  hmixing_Gen_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					  hmixing_Gen_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					  hmixing_Gen_newchtrk_jt03->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					  
					  if(trgtrk_charge > 0 && assotrk_charge > 0)
					    {
					      hmixing_Gen_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_pp->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					  if(trgtrk_charge < 0 && assotrk_charge < 0)
					    {
					      hmixing_Gen_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_mm->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					  if(trgtrk_charge > 0 && assotrk_charge < 0)
					    {
					      hmixing_Gen_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_pm->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					  if(trgtrk_charge < 0 && assotrk_charge > 0)
					    {
					      hmixing_Gen_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_1, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_2, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_3, jetw/4.);
					      hmixing_Gen_newchtrk_jt03_mp->Fill(mixing_axis_newchtrk_jt03_4, jetw/4.);
					    }
					}
				    } // trk pt condition
				} // trk eta condition
			    } // trk charge condition
			} // asso trk loop
		    } // trg trk loop end
		} // 1st jet loop end
	    } // 2nd jet loop end
	} // 2nd event loop end
    } // 1st event loop end
  std::cout<<endl;
  std::cout<<"~~~~~end mixed event correlation~~~~~~~~"<<std::endl;
} // function loop
      
void print_start()
{
  cout << endl;
  time_t init = time(0);
  char* init_time = ctime(&init); // convert now to string form                                        
  cout << "Starting at : " << init_time << endl;
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " << endl;
  cout << endl;
}

void print_stop()
{
  time_t end = time(0);
  char* end_time = ctime(&end); // convert now to string form                                                   
  cout << endl;
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << endl;
  cout << "Stopping at : " << end_time << endl;
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << endl;
}
