/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2008 J. Erik Radmall

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

/*! \file petroleumunitsofmeasure.hpp
    \brief Petroleum units of measure
*/

#ifndef quantlib_petroleum_units_of_measure_hpp
#define quantlib_petroleum_units_of_measure_hpp

#include <ql/experimental/commodities/unitofmeasure.hpp>

namespace QuantLib {

    class BarrelUnitOfMeasure : public UnitOfMeasure {
      public:
        BarrelUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                           std::make_shared<Data>("Barrels", "BBL", UnitOfMeasure::Volume);
            data_ = data;
        }
    };

    class MTUnitOfMeasure : public UnitOfMeasure {
      public:
        MTUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                        std::make_shared<Data>("Metric Tonnes", "MT", UnitOfMeasure::Mass);
            data_ = data;
        }
    };

    class MBUnitOfMeasure : public UnitOfMeasure {
      public:
        MBUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                         std::make_shared<Data>("1000 Barrels", "MB", UnitOfMeasure::Volume,
                                  BarrelUnitOfMeasure());
            data_ = data;
        }
    };

    class GallonUnitOfMeasure : public UnitOfMeasure {
      public:
        GallonUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                          std::make_shared<Data>("US Gallons", "GAL", UnitOfMeasure::Volume,
                                   BarrelUnitOfMeasure());
            data_ = data;
        }
    };

    class LitreUnitOfMeasure : public UnitOfMeasure {
      public:
        LitreUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                                std::make_shared<Data>("Litres", "l", UnitOfMeasure::Volume,
                                         BarrelUnitOfMeasure());
            data_ = data;
        }
    };

    class KilolitreUnitOfMeasure : public UnitOfMeasure {
      public:
        KilolitreUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                           std::make_shared<Data>("Kilolitres", "kl", UnitOfMeasure::Volume,
                                    BarrelUnitOfMeasure());
            data_ = data;
        }
    };

    class TokyoKilolitreUnitOfMeasure : public UnitOfMeasure {
      public:
        TokyoKilolitreUnitOfMeasure() {
            static std::shared_ptr<Data> data =
                std::make_shared<Data>("Tokyo Kilolitres", "KL_tk", UnitOfMeasure::Volume,
                         BarrelUnitOfMeasure());
            data_ = data;
        }
    };

}


#endif
