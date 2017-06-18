/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010 Klaus Spanderen
 
 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file extendedornsteinuhlenbeckprocess.hpp
    \brief extended Ornstein-Uhlenbeck process
*/

#ifndef quantlib_extended_ornstein_uhlenbeck_process_hpp
#define quantlib_extended_ornstein_uhlenbeck_process_hpp

#include <ql/stochasticprocess.hpp>

#include <functional>

namespace QuantLib {

    class OrnsteinUhlenbeckProcess;

    //! Extended Ornstein-Uhlenbeck process class
    /*! This class describes the Ornstein-Uhlenbeck process governed by
        \f[
            dx = a (b(t) - x_t) dt + \sigma dW_t.
        \f]

        \ingroup processes
    */
    class ExtendedOrnsteinUhlenbeckProcess : public StochasticProcess1D {
      public:
        enum Discretization { MidPoint, Trapezodial, GaussLobatto };

        ExtendedOrnsteinUhlenbeckProcess(
                                Real speed, Volatility sigma, Real x0,
                                std::function<Real (Real)> b,
                                Discretization discretization = MidPoint,
                                Real intEps = 1e-4);

        //! \name StochasticProcess interface
        //@{
        Real x0() const;
        Real speed() const;
        Real volatility() const;
        Real drift(Time t, Real x) const;
        Real diffusion(Time t, Real x) const;
        Real expectation(Time t0, Real x0, Time dt) const;
        Real stdDeviation(Time t0, Real x0, Time dt) const;
        Real variance(Time t0, Real x0, Time dt) const;
        //@}
      private:
        const Real speed_;
        const Volatility vol_;
        const std::function<Real (Real)> b_;
        const Real intEps_;
        const std::shared_ptr<OrnsteinUhlenbeckProcess> ouProcess_;
        const Discretization discretization_;
    };
}


#endif
