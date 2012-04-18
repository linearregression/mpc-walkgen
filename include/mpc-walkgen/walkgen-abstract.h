////////////////////////////////////////////////////////////////////////////////
///
///\file	walkgen-abstract.h
///\brief	Abstract class to instanciate Walkgen algorithm
///\author	Herdt Andrei
///\author	Lafaye Jory
///\author	Keith François
///\version	1.0
///\date	05/01/12
///
////////////////////////////////////////////////////////////////////////////////


#ifndef _MPC_WALKGEN_ABSTRACT_H_
#define _MPC_WALKGEN_ABSTRACT_H_

#include <mpc-walkgen/sharedpgtypes.h>

#include <Eigen/Core>
#include <cstring>
#include <deque>


namespace MPCWalkgen
{
	class MPC_WALKGEN_API WalkgenAbstract
	{
	    //
	    // Public methods:
	    //
	  public:

	  WalkgenAbstract();

	  virtual ~WalkgenAbstract() =0;

	    /// \brief Initialize the system
	    /// \param[in] robotData: data relative to the robot
	    /// \param[in] mpcData: data relative to the qp solver
	    virtual void init(const RobotData &robotData, const MPCData &mpcData) = 0;


	    /// \brief Call method to handle on-line generation of ZMP reference trajectory.
	    /// \param[in] time : Current time.
	    /// \param[in] previewBodiesNextState

	    /// \return The associated solution
	    ///   If solution.newTraj is true, the method has succeeded.
	    virtual const MPCSolution & online(double time, bool previewBodiesNextState = true) = 0;

	    /// \name Accessors and mutators
	    /// \{
	    /// \brief Set the reference (velocity only as for now)
	    virtual void reference(double dx, double dy, double dyaw) = 0;
	    virtual void reference(Eigen::VectorXd dx, Eigen::VectorXd dy, Eigen::VectorXd dyaw) = 0;
	    /// \}

	    /// \name accessors relative to the state of the robot.
	    /// \{
		virtual const BodyState & bodyState(BodyType body)const=0;
		virtual void bodyState(BodyType body, const BodyState & state)=0;
	    /// \}


	  public:
		/// \name accessors relative to the solver, costly when modified on line
	    /// \{
		virtual double QPSamplingPeriod()const=0;
		virtual void QPSamplingPeriod(double d)=0;

		virtual double mpcSamplingPeriod()const=0;
		virtual void mpcSamplingPeriod(double d)=0;

		virtual double actuationSamplingPeriod()const=0;
		virtual void actuationSamplingPeriod(double d)=0;

		virtual int QPNbSamplings()const=0;
		virtual void QPNbSamplings(int d)=0;

		virtual void ApplyPerturbationForce(Axis axis, BodyType body, double f)=0;
		virtual void ApplyPerturbationAcc(Axis axis, BodyType body, double acc)=0;
		virtual void ApplyPerturbationVel(Axis axis, BodyType body, double vel)=0;
	    /// \}

  };


}

#endif /* _MPC_WALKGEN_ABSTRACT_H_ */
