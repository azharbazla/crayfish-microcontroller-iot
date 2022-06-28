double furendah()
{
  if (pH < urendah[1])
  {
    return 1;
  }
  else if (pH >= urendah[1] && pH <= urendah[2])
  {
    return (urendah[2] - pH) / (urendah[2] - urendah[1]);
  }
  else if (pH > urendah[2])
  {
    return 0;
  }
}

double fubaik()
{
  if (pH < ubaik[0])
  {
    return 0;
  }
  else if (pH >= ubaik[0] && pH <= ubaik[1])
  {
    return (pH - ubaik[0]) / (ubaik[1] - ubaik[0]);
  }
  else if (pH >= ubaik[1] && pH < ubaik[2])
  {
    return (ubaik[2] - pH) / (ubaik[2] - ubaik[1]);
  }
  else if (pH > ubaik[2])
  {
    return 0;
  }
}

double futinggi()
{
  if (pH < utinggi[0])
  {
    return 0;
  }
  else if (pH >= utinggi[0] && pH <= utinggi[1])
  {
    return (pH - utinggi[0]) / (utinggi[1] - utinggi[0]);
  }
  else if (pH > utinggi[1])
  {
    return 1;
  }
}

double fxbaik()
{
  if (amonia < xbaik[1])
  {
    return 1;
  }
  else if (amonia >= xbaik[1] && amonia <= xbaik[2])
  {
    return (xbaik[2] - amonia) / (xbaik[2] - xbaik[1]);
  }
  else if (amonia > xbaik[2])
  {
    return 0;
  }
}

double fxsedang()
{
  if (amonia < xsedang[0])
  {
    return 0;
  }
  else if (amonia >= xsedang[0] && amonia <= xsedang[1])
  {
    return (amonia - xsedang[0]) / (xsedang[1] - xsedang[0]);
  }
  else if (amonia >= xsedang[1] && amonia <= xsedang[2])
  {
    return (xsedang[2] - amonia) / (xsedang[2] - xsedang[1]);
  }
  else if (amonia > xsedang[2])
  {
    return 0;
  }
}

double fxburuk()
{
  if (amonia <= xburuk[0])
  {
    return 0;
  }
  else if (amonia > xburuk[0] && amonia < xburuk[1])
  {
    return (amonia - xburuk[0]) / (xburuk[1] - xburuk[0]);
  }
  else if (amonia >= xburuk[1])
  {
    return 1;
  }
}

double Min(float a, float b)
{
  if (a < b)
  {
    return a;
  }
  else if (b < a)
  {
    return b;
  }
  else
  {
    return a;
  }
}

void rule()
{
  // if pH rendah and amonia baik then 30%
  minr[1] = Min(furendah(), fxbaik());
  Rule[1] = tp;
  // if pH rendah and amonia sedang then 30%
  minr[2] = Min(furendah(), fxsedang());
  Rule[2] = tp;
  // if pH rendah and amonia buruk then 50%
  minr[3] = Min(furendah(), fxburuk());
  Rule[3] = lp;
  // if pH baik and amonia baik then 0
  minr[4] = Min(fubaik(), fxbaik());
  Rule[4] = nol;
  // if pH baik and amonia sedang then 30%
  minr[5] = Min(fubaik(), fxsedang());
  Rule[5] = tp;
  // if pH baik and amonia buruk then 50%
  minr[6] = Min(fubaik(), fxburuk());
  Rule[6] = lp;
  // if pH tinggi and amonia baik then 30%
  minr[7] = Min(futinggi(), fxbaik());
  Rule[7] = tp;
  // if pH tinggi and amonia sedang then 30%
  minr[8] = Min(futinggi(), fxsedang());
  Rule[8] = tp;
  // if pH tinggi and amonia buruk then 50%
  minr[9] = Min(futinggi(), fxburuk());
  Rule[9] = lp;
}

double defuzzyfikasi()
{
  rule();
  A = 0;
  B = 0;
  for (int i = 1; i <= 9; i++)
  {
    A += Rule[i] * minr[i];
    B += minr[i];
  }
  return A / B;
}
