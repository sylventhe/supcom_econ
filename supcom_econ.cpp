// supcom_econ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Army.h"
#include "BluePrint.h"
#include "Unit.h"
#include "BuildCommand.h"

#include <conio.h>

// Temporary Unit BluePrint Definitions
BluePrintData ue10001 = { "ue10001", "UEF Armored Command Unit", 10, 6000000.0, 18000.0, 5000000.0, 1.0, 20.0, 650.0, 4000.0, 0.0, 0.0 };
BluePrintData ueb0101 = { "ueb0101", "UEF T1 Land Factory",      20, 300.0, 230.0, 2100.0, 0.0,  0.0, 80.0, 0.0, 0.0, 0.0 };
BluePrintData ueb1101 = { "ueb1101", "UEF T1 Power Generator",    0, 125,   75.0,  750.0,  0.0, 20.0,  0.0, 0.0, 0.0, 0.0 };
BluePrintData ueb1103 = { "ueb1103", "UEF T1 Mass Extractor", 13.01, 60.0,  36.0,  360.0,  2.0,  0.0,  0.0, 0.0, 0.0, 2.0 };
BluePrintData ue10105 = { "ue10105", "UEF T1 Engineer",         5.0, 260.0, 52.0,  260.0,  0.0,  0.0, 10.0, 0.0, 0.0, 0.0 };

int main()
{
  BluePrint bp_t1acu(ue10001);
  BluePrint bp_t1lfac(ueb0101);
  BluePrint bp_t1pgen(ueb1101);
  BluePrint bp_t1mex(ueb1103);
  BluePrint bp_t1eng(ue10105);

  Army army("ARMY_0");
  Unit t1acu(&army, "t1acu", &bp_t1acu);
  
  army.AddUnit(&t1acu);
  BuildCommand *com = new BuildCommand(&army, &t1acu, "t1_lfac1", &bp_t1lfac);
  t1acu.AddCommand(com);
  com = new BuildCommand(&army, &t1acu, "t1_mex1", &bp_t1mex);
  t1acu.AddCommand(com);
  com = new BuildCommand(&army, &t1acu, "t1_pgen1", &bp_t1pgen);
  t1acu.AddCommand(com);
  com = new BuildCommand(&army, &t1acu, "t1_pgen2", &bp_t1pgen);
  t1acu.AddCommand(com);
  com = new BuildCommand(&army, &t1acu, "t1_pgen3", &bp_t1pgen);
  t1acu.AddCommand(com);
  com = new BuildCommand(&army, "t1_lfac1", "t1_eng1", &bp_t1eng);
  army.AddCommand("t1_lfac1", com);

  army.Execute();

  _getch();

    return 0;
}

