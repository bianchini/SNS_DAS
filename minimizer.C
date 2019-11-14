#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TString.h"

//TMatrixD B = THilbertMatrixD(4,4);
const double elements[16] = 
  {1.0, 0.1, -0.1, 0.2, 
   0.1, 1., 0., 0.0,
   -0.1, 0., 1., 0.4,
   0.2, 0., 0.4, 1.
  };
TMatrixDSym B(4, elements);

const double grad[4] = {-0.3, -2., -0.5, 2.0};
TVectorD g(4, grad);
const double f0 = 1.0;

double Quadratic(const double *xx )
{
  TVectorD x(4, xx);
  return f0 + g*x + 0.5*(x*(B*x));
}

void minimizer(const char * minName = "Minuit", const char *algoName = "Migrad"){

  ROOT::Math::Minimizer* minimum = ROOT::Math::Factory::CreateMinimizer(minName, algoName);
  minimum->SetMaxFunctionCalls(1000000); 
  minimum->SetTolerance(0.001);
  minimum->SetPrintLevel(1);

  ROOT::Math::Functor f( &Quadratic, 4);
  double step[4] = {0.01, 0.01, 0.01, 0.01};
  double start[4] = { 0., 0., 0., 0.};
  
  minimum->SetFunction(f);

  for(unsigned int i=0 ; i <4; ++i){
    minimum->SetVariable(i, Form("x%d",i), start[i], step[i]);
  }

  // do the minimization
  std::cout << "Numerical:" << std::endl;
  minimum->Minimize();
  const double *xs = minimum->X();
  std::cout << "Minimum: f(" << xs[0] << "," << xs[1] <<  "," << xs[2] << "," << xs[3] << "): "
	    << minimum->MinValue()  << std::endl;

  std::cout << "Analytical:" << std::endl;
  TVectorD x0(4, start);
  TVectorD xs_ana = x0 - B.InvertFast()*(B*x0 + g);
  std::cout << "Minimum: f(" << xs_ana[0] << "," << xs_ana[1] <<  "," << xs_ana[2] << "," << xs_ana[3] << "): "
            << Quadratic(xs_ana.GetMatrixArray())  << std::endl;
  return;
}
