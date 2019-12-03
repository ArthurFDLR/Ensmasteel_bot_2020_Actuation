#include "Mega.h"
#include "Contacteur.h"



void Mega::startTimer()
{
  tTimerStart = millis() / 1000.0;
}

bool Mega::timerDelay(float delta)
{
  return millis() / 1000.0 - (tTimerStart + delta) > 0;
}



void Mega::actuate()
{
  //Actualisation des parametres
  float m = millis();
  float dt = (m - millisActu) / 1000.0;
  millisActu = m;
  if (dt > 0.5)
    dt = 0.005;
  
  this->sharpPaletD.actuate(dt);
  this->sharpPaletG.actuate(dt);

  pinces.actuate();
  elevator.actuate(dt);
  barillet.actuate(dt);
  communication.update();

  /*
  //Communication
  if (tirette.isJustPressed())
  {
    comm.send(MessageE::Tirette);
  }
  switch (comm.lastMessage)
  {
    case MessageE::Pince_Extended:
      pinces.bothPincesSet(ServoPosition::Extended);
      comm.taken();
      break;
    case MessageE::Pince_Half_Extended:
      pinces.bothPincesSet(ServoPosition::HalfExtended);
      comm.taken();
      break;
    case MessageE::Pince_Half_Retracted:
      pinces.bothPincesSet(ServoPosition::HalfRetracted);
      comm.taken();
      break;
    case MessageE::Pince_Retracted:
      pinces.bothPincesSet(ServoPosition::Retracted);
      comm.taken();
      break;
    case MessageE::Jaune:
      barillet.coteviolet = false;
      comm.taken();
      break;
    case MessageE::Violet:
      barillet.coteviolet = true;
      comm.taken();
      break;
    case MessageE::Start_Chaos:
      Serial.println("START CHAOS");
      actionCourante = Chaos;
      etapeChaos = PrepChaos;
      comm.taken();
      break;
    case MessageE::Start_Goldonium:
      actionCourante = RecupGoldonium;
      etapeRecupGoldonium = Safety;
      comm.taken();
      break;
    case MessageE::IdleM:
      actionCourante = Idle;
      comm.taken();
      break;

    case MessageE::Depose_Goldonium:
      actionCourante = DeposeGoldonium;
      etapeRecupGoldonium = Safety;
      comm.taken();
      break;

    case MessageE::PoussePaletBleuAccel:
      actionCourante = PoussePaletBleu;
      etapePaletBleu = SafetyPal;
      comm.taken();
      break;
    case MessageE::DeposePaletSolM:
      actionCourante = DeposePaletSolA;
      etapeDeposePaletSol = SafetyEtapeDeposePaletSol;
      comm.taken();
      break;
    case MessageE::VideDistributeurM:
      actionCourante = VideDistributeur;
      etapeVideDistributeur = SafetyEtapeVideDistributeur;
      break;
   case MessageE::DeposeAccelerateurM:
   actionCourante = DeposePaletAccelerateur;
   etapeDeposePaletAccelerateur = SafetyDeposePaletAccelerateur;
   break;
   case MessageE::DeposePaletBleuBalance:
   actionCourante = DeposeBleuBalance;
   etapeDeposeBleuBalance=SafetyetapeDeposeBleuBalance;
   break;
  }
      //------------------------------------------------Evitemement--------------------------------
      if (comm.anticol == AnticolE::Front)
      {
        if (sharpAVD.getState() == Alerte || sharpAVG.getState() == Alerte)
        {
          if (!evitting)
            comm.send(Evitemment);
          evitting = true;
#ifdef STATE
          Serial.println("Evitemment avant");
#endif // STATE
        }
        else
        {
          if (evitting)
          {
            comm.send(Evitemment_Clear);
#ifdef STATE
            Serial.println("Fin evitemment");
#endif // STATE
            evitting = false;
          }

        }
      }

      if (comm.anticol == AnticolE::Back)
      {
        if (sharpARD.getState() == Alerte || sharpARG.getState() == Alerte)
        {
          if (!evitting)
            comm.send(Evitemment);
          evitting = true;
#ifdef STATE
          Serial.println("Evitemment Arriere");
#endif // STATE
        }
        else
        {
          if (evitting)
          {
            comm.send(Evitemment_Clear);
#ifdef STATE
            Serial.println("Fin evitemment");
#endif // STATE
            evitting = false;
          }
        }
      }

      if (evitting && comm.anticol == AnticolE::No)
      {
#ifdef STATE
        Serial.print("Fin evitemment");
#endif // STATE
        comm.send(Evitemment_Clear);
        evitting = false;
      }

      //--------------------------------------------------doigt bloc palet-------------------------

      if (sharpPaletD.getState() == Proximity && actionCourante == actionCouranteE::Chaos)
      {
        doigtDroit.set(Retracted);
      }
      else
      {
        doigtDroit.set(Extended);
      }
      if (sharpPaletG.getState() == Proximity && actionCourante == actionCouranteE::Chaos )
      {
        doigtGauche.set(Retracted);
      }
      else
      {
        doigtGauche.set(Extended);
      }


      //--------------------------------------------------Sous actions------------------------
      switch (actionCourante)
      {
        case (Chaos):
          switch (etapeChaos)
          {
            case (PrepChaos):
              pompeD.stop();
              pompeG.stop();
              brasDroit.set(Retracted);
              brasGauche.set(Retracted);
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough() && (sharpPaletD.getState() == Proximity and sharpPaletG.getState() == Proximity))
              {
                etapeChaos = DescentSouffletSol;
              }
              break;
            case (DescentSouffletSol):
              pompeD.suck();
              pompeG.suck();
              elevator.aim = AIMTakeOnFloor;
              if (elevator.goodenough() ) 
              {
                etapeChaos = RemontePalet;
              }
              //contremesure descendre plus bas mais peu se faire indéfiniment, on fait confiance aux pompes
              break;
            case (RemontePalet):
              elevator.aim = AIMAboveFinger;
              if (elevator.goodenough())
              {
                etapeChaos = DeposeOneFloor;
              }
              break;
            case (DeposeOneFloor):
              elevator.aim = AIMDepositOneFloor;
              if (elevator.goodenough())
              {
                etapeChaos = DeposeRemonte;
              }
              break;
            case (DeposeRemonte):
              pompeD.stop();
              pompeG.stop();
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough())
              {
                etapeChaos = TourneBarillet;
              }
              break;
            case (TourneBarillet):
              //barillet.goToDelta(2.0*iPosBarillet*BARILLET_AngleToNext);
              barillet.goTo(2.0 * iPosBarillet * BARILLET_AngleToNext);
              if (barillet.goodenough())
              {
                iPosBarillet++;
                etapeChaos = PrepChaos;
                if (iPosBarillet == 4)
                {
                  actionCourante = Idle;
                  barillet.RedefinitionPosBleuium();
                }
              }
              break;
          }
          break; //Fin Chao
        //---------------------------------------action recup goldo----------------------------
        case (RecupGoldonium):
          switch (etapeRecupGoldonium)
          {
            case (Safety):
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough() )
              {
                etapeRecupGoldonium = Recup;
              }
              break;
            case (Recup):
              pompeD.suck();
              pompeG.suck();
              brasDroit.set(Extended);
              brasGauche.set(Extended);
              startTimer();
              etapeRecupGoldonium = ElevateurniveauGoldo;
              break;
              case(ElevateurniveauGoldo):
              if (timerDelay(0.5))
              elevator.aim = AIMBlueiumAcceleratorLevel;
              break;
          }
         break;//Fin recupGOld
        //---------------------------------------action deposegoldo goldo----------------------------
        case (DeposeGoldonium):
          elevator.aim = AIMBalanceLevel;
          if (elevator.goodenough())
          {
            pompeD.stop();
            pompeG.stop();
          }
          break;// Fin DeposeGoldo
        //-----------------------------------------On pousse le palet bleu--------------------------------
        case (PoussePaletBleu):
          switch (etapePaletBleu)
          {
            case (SafetyPal):
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough())
              {
                etapePaletBleu = Pousse;
              }
              break;
            case (Pousse):
              brasDroit.set(Extended);
              brasGauche.set(Extended);
              elevator.aim = AIMInAccelerator - 0.02;
              break;
          }
          break;
        //------------------------------------Idle-------------------------
        case (Idle):
          //Se toucher la nouille
          break;
             //------------------------------------DeposePaletSolA-------------------------
        case (DeposePaletSolA):
          switch (etapeDeposePaletSol)
          {
            case (SafetyEtapeDeposePaletSol):
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough())
              {
                etapeDeposePaletSol = AttrapePaletBarillet;
                startTimer();
              }
              break;
            case (AttrapePaletBarillet):
              pompeD.suck();
              pompeG.suck();
              brasDroit.set(Retracted);
              brasGauche.set(Retracted);
              if (timerDelay(0.5))
                elevator.aim = AIMTakeOneFloor;
              if (elevator.goodenough() && timerDelay(0.6))
              {
                etapeDeposePaletSol = RemontePaletPourDepose;
              }
              break;
            case (RemontePaletPourDepose):
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough())
              {
                etapeDeposePaletSol = PretADeposeSol;
                startTimer();
              }
              break;
            case (PretADeposeSol):
              brasDroit.set(Extended);
              brasGauche.set(Extended);
              if (elevator.goodenough() && timerDelay(0.5))
              {
                pompeD.stop();
                pompeG.stop();
                etapeDeposePaletSol = TourneBarilletPourDeposeSol;
              }
              break;
            case (TourneBarilletPourDeposeSol):
              barillet.goTo(2.0 * iPosBarillet * BARILLET_AngleToNext);
              if (barillet.goodenough() and iPosBarillet < 6)
              {
                iPosBarillet++;
                etapeDeposePaletSol = (SafetyEtapeDeposePaletSol) ;
              }
              break;
          }
          break;

        //--------------------------------------vide distributeur---------------------

        case (VideDistributeur):
          switch (etapeVideDistributeur)
          {
            case (SafetyEtapeVideDistributeur):
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough())
              {
                etapeVideDistributeur = PlacementBarilletDistributeur;
                brasDroit.set(ServoPosition::HalfExtended);
                brasGauche.set(ServoPosition::HalfExtended);
                startTimer();
              }
              break;
            case (PlacementBarilletDistributeur):
              if (iPosBarilletVideDistributeur == 0)
              {
                barillet.goTo(barillet.Poscellule3);
                if (barillet.goodenough() and iPosBarilletVideDistributeur < 6)
                {
                  iPosBarilletVideDistributeur++;
                  etapeVideDistributeur = (RecupDistributeur) ;
                }
              }

              else if (iPosBarilletVideDistributeur == 1)
              {
                barillet.goTo(barillet.Poscellule1);
                if (barillet.goodenough() and iPosBarilletVideDistributeur < 6)
                {
                  iPosBarilletVideDistributeur++;
                  etapeVideDistributeur = (RecupDistributeur) ;
                }
              }
              else if (iPosBarilletVideDistributeur == 2)
              {
                barillet.goTo(barillet.Poscellule5);
                if (barillet.goodenough() and iPosBarilletVideDistributeur < 6)
                {
                  iPosBarilletVideDistributeur++;
                  etapeVideDistributeur = (RecupDistributeur) ;
                }
              }
              else if (iPosBarilletVideDistributeur == 3)
              {
                barillet.goTo(barillet.Poscellule2);
                if (barillet.goodenough() and iPosBarilletVideDistributeur < 6)
                {
                  iPosBarilletVideDistributeur++;
                  etapeVideDistributeur = (RecupDistributeur) ;
                }
              }
              else if (iPosBarilletVideDistributeur == 4)
              {
                barillet.goTo(barillet.Poscellule5);
                if (barillet.goodenough() and iPosBarilletVideDistributeur < 6)
                {
                  iPosBarilletVideDistributeur++;
                  etapeVideDistributeur = (RecupDistributeur) ;
                }
              }
              break;
            case (RecupDistributeur):
              pompeD.suck();
              pompeG.suck();
              elevator.aim = AIMDistribLevel;


              if (elevator.goodenough() && comm.lastMessage == MessageE::Ok)
              {
                comm.taken();
                etapeVideDistributeur = RemonteVideDistributeur;
              }
              break;
            case (RemonteVideDistributeur):
              elevator.aim = AIMAboveBarel;
              if (elevator.goodenough() )
              {
                etapeVideDistributeur = DescentVideDistributeur ;
                startTimer();
              }
              break;
            case (DescentVideDistributeur):
              brasDroit.set(Retracted);
              brasGauche.set(Retracted);
              if (timerDelay(0.5))
              {
                if (iPosBarilletVideDistributeur < 3)
                {
                  elevator.aim = AIMDepositeTwoFloor;
                }
                else
                {
                  elevator.aim = AIMDepositThreeFloor ;
                }
              }
              if (elevator.goodenough() && timerDelay(0.6))
              {
                pompeG.stop();
                pompeD.stop();
                etapeVideDistributeur = SafetyEtapeVideDistributeur ;
              }
              break;
          }
          break;//Fin Vide distributeur

            //--------------------------------------------------Depose accelero-------------------------------------
            case (DeposePaletAccelerateur):
              switch (etapeDeposePaletAccelerateur)
              {
                case (SafetyDeposePaletAccelerateur):
                  elevator.aim = AIMAboveBarel;
                  if (elevator.goodenough())
                  {
                    etapeDeposePaletAccelerateur = AttrapePaletBarilletAccelerateur;
                    startTimer();
                  }
                  break;
                case (AttrapePaletBarilletAccelerateur):
                  pompeD.suck();
                  pompeG.suck();
                  brasDroit.set(Retracted);
                  brasGauche.set(Retracted);
                  if (timerDelay(0.5))
                    if (iPosBarilletDeposePaletAccelerateur < 3) {
                      elevator.aim = AIMTakeThreeFloor;
                    }
                    else if (iPosBarilletDeposePaletAccelerateur < 6) {
                      elevator.aim = AIMTakeTwoFloor;
                    }
                    else if (iPosBarilletDeposePaletAccelerateur < 9) {
                      elevator.aim = AIMTakeOneFloor;
                    }
                  if (elevator.goodenough() && timerDelay(0.6))
                  {
                    etapeDeposePaletAccelerateur = RemontePaletPourDeposeAccelerateur;
                  }
                  break;
                case (RemontePaletPourDeposeAccelerateur):
                  elevator.aim = AIMAboveBarel;
                  if (elevator.goodenough())
                  {
                    etapeDeposePaletAccelerateur = PretADeposePaletAccelerateur;
                    startTimer();
                  }
                  break;
                case (PretADeposePaletAccelerateur):
                  brasDroit.set(Extended);
                  brasGauche.set(Extended);
                  if (timerDelay(0.5))
                  {
                    etapeDeposePaletAccelerateur = DescentElevateurDeposePaletAccelerateur;
                  }
                  break;
                case (DescentElevateurDeposePaletAccelerateur):

                  elevator.aim = AIMInAccelerator;
                  if (elevator.goodenough())
                  {
                    etapeDeposePaletAccelerateur = PompeOffDeposePaletAccelerateur;
                    startTimer();
                  }
                  break;
                case (PompeOffDeposePaletAccelerateur):
                    pompeD.stop();
                    pompeG.stop();
                    if (timerDelay(1.)){
                    etapeDeposePaletAccelerateur = RemonteElevateurPourBarriletDeposePaletAccelerateur;
                    }
                  break;
                case (RemonteElevateurPourBarriletDeposePaletAccelerateur):
                  elevator.aim = AIMAboveBarel;
                  if (elevator.goodenough())
                  {
                    etapeDeposePaletAccelerateur = TourneBarilletPourDeposeAccelerateur;
                    startTimer();
                  }
                  break;
                case (TourneBarilletPourDeposeAccelerateur):
                  barillet.goTo(2.0 * iPosBarilletDeposePaletAccelerateur * BARILLET_AngleToNext);
                  if (barillet.goodenough() and iPosBarilletDeposePaletAccelerateur < 9)
                  {
                    iPosBarilletDeposePaletAccelerateur++;
                    etapeDeposePaletAccelerateur = (SafetyDeposePaletAccelerateur) ;
                  }
                  break;
              }
           break;//-------------------------fin depose accelerateur


// ------------------------------------------------------deposeBleuBalance-------------------
         case(DeposeBleuBalance):
              switch(etapeDeposeBleuBalance){
                case(SafetyetapeDeposeBleuBalance):
                                  elevator.aim = AIMAboveBarel;
                  if (elevator.goodenough())
                  {
                    etapeDeposeBleuBalance = SetBarilletDeposeBleu;
                    startTimer();
                  }
                  break;
                  case(SetBarilletDeposeBleu):
                    barillet.goTo(barillet.Poscellule1);
                    if (barillet.coteviolet){
                    brasDroit.set(Retracted);
                    brasGauche.set(Extended);
                    }
                    else{
                     brasDroit.set(Retracted);
                    brasGauche.set(Extended);
                    }
                  if (barillet.goodenough() and timerDelay(0.5)){
                     etapeDeposeBleuBalance=DescentElevateurDeposeBleu;
                  }
                  break;
                  case(DescentElevateurDeposeBleu):
                  pompeG.suck();
                  pompeD.suck();
                  if (iPosDeposeBleuBalance=3){
                  elevator.aim = AIMTakeThreeFloor;
                  iPosDeposeBleuBalance--;
              }
                  else if (iPosDeposeBleuBalance=2){
                  elevator.aim = AIMTakeTwoFloor;
                  iPosDeposeBleuBalance--;
              }
                  else if (iPosDeposeBleuBalance<2){
                  elevator.aim = AIMTakeTwoFloor;
                  iPosDeposeBleuBalance--;
                  }
                  if (elevator.goodenough())
                  {
                    etapeDeposeBleuBalance = RemonteElevateurPourDeposeBalance;
                  }
                  break;

                  case(RemonteElevateurPourDeposeBalance):
                                   elevator.aim = AIMAboveBarel;
                  if (elevator.goodenough())
                  {
                    etapeDeposeBleuBalance = JettePaletBalance;
                    startTimer();
                  }
                  break;
                  case(JettePaletBalance):
                  brasDroit.set(Extended);
                  brasGauche.set(Extended);
                  if (timerDelay(0.1)){/////-------------------------------------A regler pour un beau lancé------------
                  pompeG.stop();
                  pompeD.stop();
                  }
                  if (timerDelay(1.) and (iPosDeposeBleuBalance>0)){
                  etapeDeposeBleuBalance = SafetyetapeDeposeBleuBalance;
                  }
                  break;
              }
              break;//fin depose palet Bleu

              }//fin ActionCourante



          //    Serial.print("AVG ");Serial.print(sharpAVG.raw());Serial.print("\t");
          //    Serial.print("AVD ");Serial.print(sharpAVD.raw());Serial.print("\t");
          //    Serial.print("ARG ");Serial.print(sharpARG.raw());Serial.print("\t");
          //    Serial.print("ARD ");Serial.print(sharpARD.raw());Serial.print("\t");
          //    Serial.print("Palet Gauche raw ");Serial.print(sharpPaletG.raw());Serial.print("\t");
          //    Serial.print("Palet Droite raw ");Serial.print(sharpPaletD.raw());Serial.print("\t");
          //    Serial.print("amp pompe Gauche ");pompeG.isSucked();Serial.print("\t");
          //    Serial.print("amp pompe Droite ");pompeD.isSucked();Serial.println("\t");
      */
      }




void Mega::init()
{
  Serial2.begin(250000);
  actionCourante = Distrib;
  evitting = false;
  elevator = Elevator(ELEVATOR_PIN_CONTACTEUR, ELEVATOR_PIN_CODEUSE_A, ELEVATOR_PIN_CODEUSE_B, ELEVATOR_TickToPos, ELEVATOR_PIN_MOTEUR_PWR, ELEVATOR_PIN_MOTEUR_SENS, ELEVATOR_PIN_MOTEUR_BRAKE);
  barillet = Barillet(BARILLET_PIN_CONTACTEUR, BARILLET_PIN_CODEUSE_A, BARILLET_PIN_CODEUSE_B, BARILLET_TickToPos, BARILLET_PIN_MOTEUR_PWR, BARILLET_PIN_MOTEUR_SENS, BARILLET_PIN_MOTEUR_BRAKE, coteviolet, this);
  brasGauche = MegaServo(BRAS_GAUCHE_PIN, BRAS_GAUCHE_RETRACTED, BRAS_GAUCHE_HALF_RETRACTED, BRAS_GAUCHE_HALF_EXTENDED, BRAS_GAUCHE_EXTENDED);
  brasDroit = MegaServo(BRAS_DROIT_PIN, BRAS_DROIT_RETRACTED, BRAS_DROIT_HALF_RETRACTED, BRAS_DROIT_HALF_EXTENDED, BRAS_DROIT_EXTENDED);
  doigtGauche = MegaServo(DOIGT_GAUCHE_PIN, DOIGT_GAUCHE_RETRACTED, DOIGT_GAUCHE_HALF_RETRACTED, DOIGT_GAUCHE_HALF_EXTENDED, DOIGT_GAUCHE_EXTENDED);
  doigtDroit = MegaServo(DOIGT_DROIT_PIN, DOIGT_DROIT_RETRACTED, DOIGT_DROIT_HALF_RETRACTED, DOIGT_DROIT_HALF_EXTENDED, DOIGT_DROIT_EXTENDED);
  pinces = Pinces(true);
  
  sharpPaletD = Sharp(SHARP_PALET_DROITE_PIN, 4242, SHARP_PALET_SEUIL);
  sharpPaletG = Sharp(SHARP_PALET_GAUCHE_PIN, 4242, SHARP_PALET_SEUIL);
  
  pompeG = Pompe(POMPE_GAUCHE_PIN_MOTEUR_PWR, POMPE_GAUCHE_PIN_MOTEUR_SENS, POMPE_GAUCHE_PIN_MOTEUR_BRAKE, POMPE_GAUCHE_PIN_AMP, true);
  pompeD = Pompe(POMPE_DROITE_PIN_MOTEUR_PWR, POMPE_DROITE_PIN_MOTEUR_SENS, POMPE_DROITE_PIN_MOTEUR_BRAKE, POMPE_DROITE_PIN_AMP, false);
  tirette = Contacteur(PIN_TIRETTE);
  communication = Communication();
  elevator.init();
#ifdef STATE
  Serial.println("Fin de la contaction de l'elevator. On remonte");
#endif // STATE
  while (abs(elevator.pos - elevator.aim) > 0.005 || abs(elevator.dPos) > 0.005)
  {
    actuate();
    delay(1);
  }
  elevator.moteurElevator->order = 0;
  elevator.moteurElevator->actuate();
#ifdef STATE
  Serial.println("Fin de la remontee de l'elevator. On fait tourner le barrillet");
#endif // STATE
  barillet.init();
#ifdef STATE
  Serial.println("Fin de la contaction du barillet. On Se place");
#endif // STATE
  while (!barillet.goodenough())
  {
    actuate();
    delay(1);
  }
  barillet.pos = 0;
  barillet.goTo(0.0);

#ifdef STATE
  Serial.println("Fin du palcement, init terminee");
#endif // STATE
  //barillet.RedefinitionPosBleuium();
  millisInit = millis();
  millisActu = millis();



}
