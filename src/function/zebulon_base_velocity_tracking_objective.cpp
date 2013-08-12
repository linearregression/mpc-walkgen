#include "zebulon_base_velocity_tracking_objective.h"

using namespace MPCWalkgen;

BaseVelocityTrackingObjective::BaseVelocityTrackingObjective(const BaseModel& baseModel)
:baseModel_(baseModel)
,function_(1)
{
  velRefInWorldFrame_.setZero(2*baseModel_.getNbSamples());

  function_.fill(0);
  gradient_.setZero(1, 1);
  hessian_.setZero(1, 1);

  computeConstantPart();
}


BaseVelocityTrackingObjective::~BaseVelocityTrackingObjective(){}

const MatrixX& BaseVelocityTrackingObjective::getGradient(const VectorX& x0)
{
  assert(velRefInWorldFrame_.size()==x0.size());
  assert(velRefInWorldFrame_.size()==baseModel_.getNbSamples()*2);

  const LinearDynamic& dyn = baseModel_.getBaseVelLinearDynamic();

  int N = baseModel_.getNbSamples();

  gradient_.noalias() = getHessian()*x0;


  tmp_.noalias() = dyn.S * baseModel_.getStateX();
  tmp_.noalias() -= velRefInWorldFrame_.segment(0, N);
  gradient_.block(0, 0, N, 1).noalias() += dyn.UT*tmp_;

  tmp_.noalias() = dyn.S * baseModel_.getStateY();
  tmp_.noalias()-= velRefInWorldFrame_.segment(N, N);
  gradient_.block(N, 0, N, 1).noalias() += dyn.UT*tmp_;


  return gradient_;
}

const MatrixX& BaseVelocityTrackingObjective::getHessian()
{
  return hessian_;
}

void BaseVelocityTrackingObjective::setVelRefInWorldFrame(const VectorX& velRefInWorldFrame)
{
  assert(velRefInWorldFrame.size()==baseModel_.getNbSamples()*2);
  assert(velRefInWorldFrame==velRefInWorldFrame);

  velRefInWorldFrame_ = velRefInWorldFrame;
}

void BaseVelocityTrackingObjective::computeConstantPart()
{
  const LinearDynamic& dyn = baseModel_.getBaseVelLinearDynamic();

  int N = baseModel_.getNbSamples();

  hessian_.resize(2*N, 2*N);
  hessian_.block(0, 0, N, N) = dyn.UT*dyn.U;
  hessian_.block(N, N, N, N) = dyn.UT*dyn.U;
  hessian_.block(0, N, N, N).fill(0.0);
  hessian_.block(N, 0, N, N).fill(0.0);
  tmp_.resize(N);
}
