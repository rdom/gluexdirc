#include "GlxPrimaryGeneratorAction.h"
#include "GlxPrimaryGeneratorMessenger.h"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "GlxManager.h"

GlxPrimaryGeneratorAction::GlxPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0){
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  //create a messenger for this class
  fGunMessenger = new GlxPrimaryGeneratorMessenger(this);

  //default kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 // G4ParticleDefinition* particle = particleTable->FindParticle("e+");
  G4ParticleDefinition* particle = particleTable->FindParticle("pi+");	  

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  //fParticleGun->SetParticleEnergy(500.0*keV);
  fParticleGun->SetParticleEnergy(3.5*GeV);	  
}

GlxPrimaryGeneratorAction::~GlxPrimaryGeneratorAction(){
  delete fParticleGun;
  delete fGunMessenger;
}

void GlxPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){ 
  G4double x,y,z;
  G4double radiatorL = GlxManager::Instance()->GetRadiatorL();
  G4double radiatorW = GlxManager::Instance()->GetRadiatorW();
  G4double radiatorH = GlxManager::Instance()->GetRadiatorH();
  
  GlxManager::Instance()->AddEvent(GlxEvent());

  if(GlxManager::Instance()->GetBeamDimension() > 0){ // smearing and divergence
    G4double sigma = GlxManager::Instance()->GetBeamDimension()*mm;
    z = fParticleGun->GetParticlePosition().z();
    x = G4RandGauss::shoot(0,sigma);
    y = G4RandGauss::shoot(0,sigma);
    
    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
    GlxManager::Instance()->Event()->SetPosition(TVector3(x,y,z));
  }
  if(GlxManager::Instance()->GetRunType() == 1){ // LUT generation

    G4int radid = 47-GlxManager::Instance()->GetRadiator();
    //G4int radid = 48.*G4UniformRand();
    G4int arr[]={-795,-365,365,795};
    G4double angle = -G4UniformRand()*M_PI;
    G4ThreeVector vec(0,0,1);
    vec.setTheta(acos(G4UniformRand()));
    vec.setPhi(2*M_PI*G4UniformRand());
    vec.rotateY(M_PI/2.);
    fParticleGun->SetParticleMomentumDirection(vec);
    fParticleGun->SetParticlePosition(G4ThreeVector(radiatorL/2.-0.1,arr[radid/12]+0.5*425-0.5*35-(11-radid%12)*35,0));
  }
  if(GlxManager::Instance()->GetRunType() == 3){ // laser calibration
    G4int radid = 47-GlxManager::Instance()->GetRadiator();
    G4int arr[]={-795,-365,365,795};
    //G4double angle = -G4UniformRand()*M_PI;
    G4ThreeVector vec(0,0,1);
    //vec.rotateY((90.+42.13)/180.*3.1415);
    vec.rotateY(125.*deg);
    //  vec.rotateZ(5./180.*3.1415);
    fParticleGun->SetParticleMomentumDirection(vec);
    //this position is in the middle of the op box:
    //  fParticleGun->SetParticlePosition(G4ThreeVector(2650.,arr[radid/12]+0.5*425-0.5*35-(11-radid%12)*35,0.));
    //this position is at the bar end:
    std::cout<<"position of the primary generator = "<<radiatorL/2.-0.001-91./2.+0.5<<std::endl;
    fParticleGun->SetParticlePosition(G4ThreeVector(radiatorL/2.-0.001-91./2.+0.5,arr[radid/12]+0.5*425-0.5*35-(11-radid%12)*35,0.));
    //fParticleGun->SetParticlePosition(G4ThreeVector(radiatorL/2.+200.,arr[radid/12]+0.5*425-0.5*35-(11-radid%12)*35,-502.));  
  }
	
  if(GlxManager::Instance()->GetRunType() == 0){

    // G4ThreeVector vec(0,0,-1);
    // //  vec.rotateY(M_PI/10.);
	 
    // //added to compare with John:
    // vec.rotateY(4./180.*3.1415);// John's theta
    // vec.rotateZ(40./180.*3.1415);// John's phi
	
    // fParticleGun->SetParticleMomentumDirection(vec);
    // //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,4000));//5600));// changed the sign according to mechanical design
    // fParticleGun->SetParticlePosition(G4ThreeVector(-140.,175.,8.7)); // John's location  
    // //fParticleGun->SetParticlePosition(G4ThreeVector(1000.,175.,-8.7));

    auto theta = GlxManager::Instance()->GetBeamTheta();
    auto phi = GlxManager::Instance()->GetBeamPhi();
    G4ThreeVector vec(0,0,-1);
    vec.rotateY(theta/180.*M_PI);
    vec.rotateZ(phi/180.*M_PI);
	
    fParticleGun->SetParticleMomentumDirection(vec);
    fParticleGun->SetParticlePosition(G4ThreeVector(0,0,4000));
  }

  if(GlxManager::Instance()->GetBeamDimension() < 0){ // random momentum
    //    fParticleGun->SetParticleMomentum(G4ThreeVector(0, 0, 4.0*GeV*G4UniformRand()));

    Double_t xpos = -50 + 1220*2*(1-2*G4UniformRand()); // [-2490,2390]
    Double_t ypos = 5 + (365 - 420/2.) + 420*2*G4UniformRand(); //[160,1000]
    Double_t vertex = 4000;//5600;

    if(GlxManager::Instance()->GetBeamDimension() == -2){
      xpos = GlxManager::Instance()->GetBeamX();
      ypos = GlxManager::Instance()->GetBeamZ();
    }
    
    G4ThreeVector vec(xpos,ypos,-vertex);
    Double_t phi = atan(ypos/xpos);
    Double_t theta =  atan(sqrt(xpos*xpos+ypos*ypos)/vertex);
    
    fParticleGun->SetParticleMomentumDirection(vec);
    fParticleGun->SetParticlePosition(G4ThreeVector(0,0,vertex));//chaged sign according to mechanical design
  }
  
  fParticleGun->GeneratePrimaryVertex(anEvent);
 
  G4ThreeVector dir = fParticleGun->GetParticleMomentumDirection();
  dir *= fParticleGun->GetParticleMomentum();
  GlxManager::Instance()->SetMomentum(TVector3(dir.x(),dir.y(),dir.z()));
}

void GlxPrimaryGeneratorAction::SetOptPhotonPolar(){
  G4double angle = G4UniformRand() * 360.0*deg;
  SetOptPhotonPolar(angle);
}

void GlxPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle){
  if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton"){
    G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
      "the particleGun is not an opticalphoton " << 
      fParticleGun->GetParticleDefinition()->GetParticleName()<< G4endl;
    return;
  }

  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2       = product*product;
 
  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
 
  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
  fParticleGun->SetParticlePolarization(polar);
}
