//////////////////////////////////////////////////////////////////
// (c) Copyright 2010-  by Ken Esler and Jeongnim Kim
//////////////////////////////////////////////////////////////////
//   National Center for Supercomputing Applications &
//   Materials Computation Center
//   University of Illinois, Urbana-Champaign
//   Urbana, IL 61801
//   e-mail: esler@uiuc.edu
//
// Supported by 
//   National Center for Supercomputing Applications, UIUC
//   Materials Computation Center, UIUC
//////////////////////////////////////////////////////////////////

#ifndef AFM_SPO_SET_H
#define AFM_SPO_SET_H

#include "QMCWaveFunctions/SPOSetBase.h"

namespace qmcplusplus 
{

  class AFMSPOSet : public SPOSetBase
  {
  protected:
    // Number of occupied states
    int N;
    
    int pm;
    RealType theta, costheta, sintheta;
    
    //GS is Gamma point, Basis is AFM vector orbitals
    SPOSetBase *GSOrbitals, *BasisOrbitals;

//     void addParameter (string id, int iorb, int basis);

    ValueVector_t GSVal, BasisVal, GSLapl, BasisLapl;
    GradVector_t  GSGrad, BasisGrad;

    ValueMatrix_t GSValMatrix, BasisValMatrix, GSLaplMatrix, BasisLaplMatrix, GradTmpSrc, GradTmpDest;
    GradMatrix_t  GSGradMatrix, BasisGradMatrix;

  public:
    
    AFMSPOSet() : N(0), GSOrbitals(0), BasisOrbitals(0)
    {
    }

    AFMSPOSet(int num_orbs, SPOSetBase *gsOrbs, SPOSetBase* basisOrbs):
      GSOrbitals(gsOrbs), BasisOrbitals(basisOrbs)
    {
      N = num_orbs;
      setOrbitalSetSize(N);
      resize(N);
    }
    
    inline void setpm(int x){pm=x;}
    
    inline void resetTheta(RealType x)
    {
      theta = x;
      costheta = std::cos(theta);
      sintheta = std::sin(theta);
    }

    //    bool put(xmlNodePtr cur, SPOPool_t &spo_pool);
    void resetTargetParticleSet(ParticleSet& P);
    void setOrbitalSetSize(int norbs);
  
    // Read coefficients, or create the XML element.
    bool put (xmlNodePtr node, SPOPool_t &spo_pool);

    // This stores new orbital coefficients int C
    void resetParameters(const opt_variables_type& optvars);
  
    // Evaluate the derivative of the optimized orbitals with
    // respect to the parameters
    void evaluateDerivatives(ParticleSet& P, int iat,
			     const opt_variables_type& active,
			     ValueMatrix_t& d_phi,
			     ValueMatrix_t& d_lapl_phi);
  
    void checkInVariables(opt_variables_type& active);
    void checkOutVariables(const opt_variables_type& active);


    // Evaluate routines.  These call GSOrbitals->evaluate and possibly
    // BasisOrbitals->evaluate, then does the matrix product with
    // C.
    void evaluate(const ParticleSet& P, int iat, ValueVector_t& psi);
    void evaluate(const ParticleSet& P, const PosType& r, vector<RealType> &psi);
    void evaluate(const ParticleSet& P, int iat, ValueVector_t& psi, 
		  GradVector_t& dpsi, ValueVector_t& d2psi);
    void evaluate(const ParticleSet& P, int iat, ValueVector_t& psi, 
		  GradVector_t& dpsi, HessVector_t& d2psi);
    void evaluate_notranspose(const ParticleSet& P, int first, int last,
			      ValueMatrix_t& logdet, GradMatrix_t& dlogdet, 
			      ValueMatrix_t& d2logdet);
    void evaluate_notranspose(const ParticleSet& P, int first, int last
        , ValueMatrix_t& logdet, GradMatrix_t& dlogdet, HessMatrix_t& grad_grad_logdet)
    {
      APP_ABORT("Need specialization of OptimizableOrbitalSet::evaluate_notranspose() for grad_grad_logdet. \n");
    }

    void evaluate_notranspose(const ParticleSet& P, int first, int last
        , ValueMatrix_t& logdet, GradMatrix_t& dlogdet, HessMatrix_t& grad_grad_logdet, GGGMatrix_t& grad_grad_grad_logdet)
    {
      APP_ABORT("Need specialization of OptimazableOrbitalSet::evaluate_notranspose() for grad_grad_grad_logdet. \n");
    }

    void evaluateForDeriv (const ParticleSet &P, int first, int last,
			ValueMatrix_t &val, GradMatrix_t &grad,
			ValueMatrix_t &lapl);
         
             
  void resize(int n) {
    N=n;
    GSVal.resize(N);    GSGrad.resize(N);    GSLapl.resize(N);
    BasisVal.resize(N); BasisGrad.resize(N); BasisLapl.resize(N);
    GSValMatrix.resize (N,N);
    GSGradMatrix.resize(N,N);
    GSLaplMatrix.resize(N,N);
    BasisValMatrix.resize (N,N);
    BasisGradMatrix.resize(N,N);
    BasisLaplMatrix.resize(N,N);
    GradTmpSrc.resize(N,N);
    GradTmpDest.resize(N,N);
    
    BasisSetSize = N;
    
    ///from inherited class
    t_logpsi.resize(N,N);
  }
  
    // Make a copy of myself
    SPOSetBase* makeClone() const;
  };
}

#endif
