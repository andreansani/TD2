#include <ResponsiveAnalogRead.h>

ResponsiveAnalogRead sensorPolegar1(15, true);
ResponsiveAnalogRead sensorPolegar2(14, true);
ResponsiveAnalogRead sensorIndicador1(13, true);
ResponsiveAnalogRead sensorIndicador2(12, true);
ResponsiveAnalogRead sensorMedio1(11, true);
ResponsiveAnalogRead sensorMedio2(10, true);
ResponsiveAnalogRead sensorAnelar1(9, true);
ResponsiveAnalogRead sensorAnelar2 (8, true);
ResponsiveAnalogRead sensorMindinho1(7, true);
ResponsiveAnalogRead sensorMindinho2(6, true);
ResponsiveAnalogRead sensorAbPolInd(5, false);
ResponsiveAnalogRead sensorAbIndMed(4, false);
ResponsiveAnalogRead sensorAbMedAne(3, false);
ResponsiveAnalogRead sensorAbAneMin(2, false);

String valores,milsec;
int PolegarGraus1, IndicadorGraus1, MedioGraus1, AnelarGraus1, MindinhoGraus1; 
int PolegarGraus2, IndicadorGraus2, MedioGraus2, AnelarGraus2, MindinhoGraus2; 
int grausAbPolInd, grausAbIndMed, grausAbMedAne, grausAbAneMin; 

void setup() {
  Serial.begin(115200);
  //Serial.print("R:\tRPol1\tRPol2\tRInd1\tRInd2\tRMed1\tRMed2\tRAne1\tRAne2\tRMin1\tRMin2\tRPolInd\tRIndMed\tRMedAne\tRAneMin\t");
  Serial.print("T:\tFPol1\tFPol2\tFInd1\tFInd2\tFMed1\tFMed2\tFAne1\tFAne2\tFMin1\tFMin2\tFPolInd\tFIndMed\tFMedAne\tFAneMin\t");
  Serial.println("T:\tGPol1\tGPol2\tGInd1\tGInd2\tGMed1\tGMed2\tGAne1\tGAne2\tGMin1\tGMin2\tGPolInd\tGIndMed\tGMedAne\tGAneMin");
}

void loop() {

  sensorPolegar1.update();
  sensorPolegar2.update();
  sensorIndicador1.update();
  sensorIndicador2.update();
  sensorMedio1.update();
  sensorMedio2.update();
  sensorAnelar1.update();
  sensorAnelar2.update();
  sensorMindinho1.update();
  sensorMindinho2.update();
  sensorAbPolInd.update();
  sensorAbIndMed.update();
  sensorAbMedAne.update();
  sensorAbAneMin.update();

  PolegarGraus1   = map(sensorPolegar1.getValue(),   789, 860, 0, 70);
  PolegarGraus2   = map(sensorPolegar2.getValue(),   720, 830, 0, 100);
  IndicadorGraus1 = map(sensorIndicador1.getValue(), 761, 884, 0, 80);
  IndicadorGraus2 = map(sensorIndicador2.getValue(), 702, 753, 0, 100);
  MedioGraus1     = map(sensorMedio1.getValue(),     742, 899, 0, 80);
  MedioGraus2     = map(sensorMedio2.getValue(),     747, 900, 0, 100);
  AnelarGraus1    = map(sensorAnelar1.getValue(),    740, 849, 0, 80);
  AnelarGraus2    = map(sensorAnelar2.getValue(),    713, 767, 0, 100);
  MindinhoGraus1  = map(sensorMindinho1.getValue(),  721, 871, 0, 80);
  MindinhoGraus2  = map(sensorMindinho2.getValue(),  731, 827, 0, 100);
  grausAbPolInd   = map(sensorAbPolInd.getValue(),   784, 875, 0, -45);
  grausAbIndMed   = map(sensorAbIndMed.getValue(),   820, 858, 20, 0);
  grausAbMedAne   = map(sensorAbMedAne.getValue(),   875, 910, 20, 0);
  grausAbAneMin   = map(sensorAbAneMin.getValue(),   828, 890, 35, 0);
//  
//  valores = "R;\t";
//  valores += String(sensorPolegar1.getRawValue()) + "\t";
//  valores += String(sensorPolegar2.getRawValue()) + "\t";
//  valores += String(sensorIndicador1.getRawValue()) + "\t";
//  valores += String(sensorIndicador2.getRawValue()) + "\t";
//  valores += String(sensorMedio1.getRawValue()) + "\t";
//  valores += String(sensorMedio2.getRawValue()) + "\t";
//  valores += String(sensorAnelar1.getRawValue()) + "\t";
//  valores += String(sensorAnelar2.getRawValue()) + "\t";
//  valores += String(sensorMindinho1.getRawValue()) + "\t";
//  valores += String(sensorMindinho2.getRawValue()) + "\t";
//  valores += String(sensorAbPolInd.getRawValue()) + "\t";
//  valores += String(sensorAbIndMed.getRawValue()) + "\t";
//  valores += String(sensorAbMedAne.getRawValue()) + "\t";
//  valores += String(sensorAbAneMin.getRawValue()) + "\t";
  milsec = String(millis()) + "\t";
  valores = milsec;
  valores += String(sensorPolegar1.getValue()) + "\t";
  valores += String(sensorPolegar2.getValue()) + "\t";
  valores += String(sensorIndicador1.getValue()) + "\t";
  valores += String(sensorIndicador2.getValue()) + "\t";
  valores += String(sensorMedio1.getValue()) + "\t";
  valores += String(sensorMedio2.getValue()) + "\t";
  valores += String(sensorAnelar1.getValue()) + "\t";
  valores += String(sensorAnelar2.getValue()) + "\t";
  valores += String(sensorMindinho1.getValue()) + "\t";
  valores += String(sensorMindinho2.getValue()) + "\t";
  valores += String(sensorAbPolInd.getValue()) + "\t";
  valores += String(sensorAbIndMed.getValue()) + "\t";
  valores += String(sensorAbMedAne.getValue()) + "\t";
  valores += String(sensorAbAneMin.getValue()) + "\t";
  valores += milsec + "\t";
  valores += String(PolegarGraus1) + "\t";
  valores += String(PolegarGraus2) + "\t";
  valores += String(IndicadorGraus1) + "\t";
  valores += String(IndicadorGraus2) + "\t";
  valores += String(MedioGraus1) + "\t";
  valores += String(MedioGraus2) + "\t";
  valores += String(AnelarGraus1) + "\t";
  valores += String(AnelarGraus2) + "\t";
  valores += String(MindinhoGraus1) + "\t";
  valores += String(MindinhoGraus2) + "\t";
  valores += String(grausAbPolInd) + "\t";
  valores += String(grausAbIndMed) + "\t";
  valores += String(grausAbMedAne) + "\t";
  valores += String(grausAbAneMin);
  
  Serial.println(valores);
  
  delay(100);
}






