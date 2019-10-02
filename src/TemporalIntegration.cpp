#include "TemporalIntegration.hpp"

double 
TemporalIntegrator::calculatePsafe(const float & ztSafe, const float & ztNotSafe) 
{
    double dtime = now - lastObservation;
	double time_decay = std::pow(2.0, -dtime / K_DROP);
    double ksafe, knotsafe;
    ksafe = ztSafe;
    knotsafe = ztNotSafe;
    // knotsafe = 1 - ksafe;
    pSafe = ksafe + time_decay * pSafe;
    pNotSafe = knotsafe + time_decay * pNotSafe;
    lastKsafe = ksafe;
    return updatePSafe();
}

double 
TemporalIntegrator::updatePSafe() 
{
  double dtime = now - lastObservation;
	double time_decay = std::pow(2.0, -dtime / K_DROP);
	return time_decay * pSafe / (pSafe + pNotSafe);
}
