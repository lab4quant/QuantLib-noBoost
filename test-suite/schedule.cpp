/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2012 StatPro Italia srl

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

#include "utilities.hpp"
#include <ql/time/schedule.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/calendars/japan.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/calendars/weekendsonly.hpp>

using namespace QuantLib;


namespace {

    void check_dates(const Schedule& s,
                     const std::vector<Date>& expected) {
        if (s.size() != expected.size()) {
            FAIL("expected " << expected.size() << " dates, "
                       << "found " << s.size());
        }
        for (Size i=0; i<expected.size(); ++i) {
            if (s[i] != expected[i]) {
                FAIL_CHECK("expected " << expected[i]
                            << " at index " << i << ", "
                            "found " << s[i]);
            }
        }
    }

}


TEST_CASE( "Schedule_DailySchedule", "[Schedule]" ) {
    INFO("Testing schedule with daily frequency...");

    Date startDate = Date(17,January,2012);

    Schedule s =
        MakeSchedule().from(startDate).to(startDate+7)
                      .withCalendar(TARGET())
                      .withFrequency(Daily)
                      .withConvention(Preceding);

    std::vector<Date> expected(6);
    // The schedule should skip Saturday 21st and Sunday 22rd.
    // Previously, it would adjust them to Friday 20th, resulting
    // in three copies of the same date.
    expected[0] = Date(17,January,2012);
    expected[1] = Date(18,January,2012);
    expected[2] = Date(19,January,2012);
    expected[3] = Date(20,January,2012);
    expected[4] = Date(23,January,2012);
    expected[5] = Date(24,January,2012);

    check_dates(s, expected);
}

TEST_CASE( "Schedule_EndDateWithEomAdjustment", "[Schedule]" ) {
    INFO(
        "Testing end date for schedule with end-of-month adjustment...");

    Schedule s =
        MakeSchedule().from(Date(30,September,2009))
                      .to(Date(15,June,2012))
                      .withCalendar(Japan())
                      .withTenor(6*Months)
                      .withConvention(Following)
                      .withTerminationDateConvention(Following)
                      .forwards()
                      .endOfMonth();

    std::vector<Date> expected(7);
    // The end date is adjusted, so it should also be moved to the end
    // of the month.
    expected[0] = Date(30,September,2009);
    expected[1] = Date(31,March,2010);
    expected[2] = Date(30,September,2010);
    expected[3] = Date(31,March,2011);
    expected[4] = Date(30,September,2011);
    expected[5] = Date(30,March,2012);
    expected[6] = Date(29,June,2012);

    check_dates(s, expected);

    // now with unadjusted termination date...
    s = MakeSchedule().from(Date(30,September,2009))
                      .to(Date(15,June,2012))
                      .withCalendar(Japan())
                      .withTenor(6*Months)
                      .withConvention(Following)
                      .withTerminationDateConvention(Unadjusted)
                      .forwards()
                      .endOfMonth();
    // ...which should leave it alone.
    expected[6] = Date(15,June,2012);

    check_dates(s, expected);
}


TEST_CASE( "Schedule_DatesPastEndDateWithEomAdjustment", "[Schedule]" ) {
    INFO(
        "Testing that no dates are past the end date with EOM adjustment...");

    Schedule s =
        MakeSchedule().from(Date(28,March,2013))
                      .to(Date(30,March,2015))
                      .withCalendar(TARGET())
                      .withTenor(1*Years)
                      .withConvention(Unadjusted)
                      .withTerminationDateConvention(Unadjusted)
                      .forwards()
                      .endOfMonth();

    std::vector<Date> expected(3);
    expected[0] = Date(31,March,2013);
    expected[1] = Date(31,March,2014);
    // March 31st 2015, coming from the EOM adjustment of March 28th,
    // should be discarded as past the end date.
    expected[2] = Date(30,March,2015);

    check_dates(s, expected);

    // also, the last period should not be regular.
    if (s.isRegular(2))
        FAIL_CHECK("last period should not be regular");
}

TEST_CASE( "Schedule_DatesSameAsEndDateWithEomAdjustment", "[Schedule]" ) {
    INFO(
        "Testing that next-to-last date same as end date is removed...");

    Schedule s =
        MakeSchedule().from(Date(28,March,2013))
                      .to(Date(31,March,2015))
                      .withCalendar(TARGET())
                      .withTenor(1*Years)
                      .withConvention(Unadjusted)
                      .withTerminationDateConvention(Unadjusted)
                      .forwards()
                      .endOfMonth();

    std::vector<Date> expected(3);
    expected[0] = Date(31,March,2013);
    expected[1] = Date(31,March,2014);
    // March 31st 2015, coming from the EOM adjustment of March 28th,
    // should be discarded as the same as the end date.
    expected[2] = Date(31,March,2015);

    check_dates(s, expected);

    // also, the last period should be regular.
    if (!s.isRegular(2))
        FAIL_CHECK("last period should be regular");
}

TEST_CASE( "Schedule_ForwardDatesWithEomAdjustment", "[Schedule]" ) {
    INFO(
        "Testing that the last date is not adjusted for EOM when "
        "termination date convention is unadjusted...");

    Schedule s =
        MakeSchedule().from(Date(31,August,1996))
                      .to(Date(15,September,1997))
                      .withCalendar(UnitedStates(UnitedStates::GovernmentBond))
                      .withTenor(6*Months)
                      .withConvention(Unadjusted)
                      .withTerminationDateConvention(Unadjusted)
                      .forwards()
                      .endOfMonth();

    std::vector<Date> expected(4);
    expected[0] = Date(31,August,1996);
    expected[1] = Date(28,February,1997);
    expected[2] = Date(31,August,1997);
    expected[3] = Date(15,September,1997);

    check_dates(s, expected);
}

TEST_CASE( "Schedule_BackwardDatesWithEomAdjustment", "[Schedule]" ) {
    INFO(
        "Testing that the first date is not adjusted for EOM "
        "going backward when termination date convention is unadjusted...");

    Schedule s =
        MakeSchedule().from(Date(22,August,1996))
                      .to(Date(31,August,1997))
                      .withCalendar(UnitedStates(UnitedStates::GovernmentBond))
                      .withTenor(6*Months)
                      .withConvention(Unadjusted)
                      .withTerminationDateConvention(Unadjusted)
                      .backwards()
                      .endOfMonth();

    std::vector<Date> expected(4);
    expected[0] = Date(22,August,1996);
    expected[1] = Date(31,August,1996);
    expected[2] = Date(28,February,1997);
    expected[3] = Date(31,August,1997);

    check_dates(s, expected);
}

TEST_CASE( "Schedule_DoubleFirstDateWithEomAdjustment", "[Schedule]" ) {
    INFO(
        "Testing that the first date is not duplicated due to "
        "EOM convention when going backwards...");

    Schedule s =
        MakeSchedule().from(Date(22,August,1996))
                      .to(Date(31,August,1997))
                      .withCalendar(UnitedStates(UnitedStates::GovernmentBond))
                      .withTenor(6*Months)
                      .withConvention(Following)
                      .withTerminationDateConvention(Following)
                      .backwards()
                      .endOfMonth();

    std::vector<Date> expected(3);
    expected[0] = Date(30,August,1996);
    expected[1] = Date(28,February,1997);
    expected[2] = Date(29,August,1997);

    check_dates(s, expected);
}

TEST_CASE( "Schedule_CDS2015Convention", "[Schedule]" ) {
    INFO("Testing CDS2015 semi-annual rolling convention...");
    //From September 20th 2016 to March 19th 2017 of the next Year,
    //end date is December 20th 2021 for a 5 year Swap
    Schedule s1 =
        MakeSchedule().from(Date(12, December, 2016))
                      .to(Date(12, December, 2016) + Period(5, Years))
                      .withCalendar(WeekendsOnly())
                      .withTenor(3*Months)
                      .withConvention(ModifiedFollowing)
                      .withTerminationDateConvention(Unadjusted)
                      .withRule(DateGeneration::CDS2015);
    CHECK(s1.startDate() == Date(20, September, 2016));
    CHECK(s1.endDate() == Date(20, December, 2021));
    Schedule s2 =
        MakeSchedule().from(Date(1, March, 2017))
                      .to(Date(1, March, 2017) + Period(5, Years))
                      .withCalendar(WeekendsOnly())
                      .withTenor(3*Months)
                      .withConvention(ModifiedFollowing)
                      .withTerminationDateConvention(Unadjusted)
                      .withRule(DateGeneration::CDS2015);
    CHECK(s2.startDate() == Date(20, December, 2016));
    CHECK(s2.endDate() == Date(20, December, 2021));
    //From March 20th 2017 to September 19th 2017
    //end date is June 20th 2022 for a 5 year Swap
    Schedule s3 =
        MakeSchedule().from(Date(20, March, 2017))
                      .to(Date(20, March, 2017) + Period(5, Years))
                      .withCalendar(WeekendsOnly())
                      .withTenor(3*Months)
                      .withConvention(ModifiedFollowing)
                      .withTerminationDateConvention(Unadjusted)
                      .withRule(DateGeneration::CDS2015);
    CHECK(s3.startDate() == Date(20, March, 2017));
    CHECK(s3.endDate() == Date(20, June, 2022));

}

TEST_CASE( "Schedule_DateConstructor", "[Schedule]" ) {
    INFO("Testing the constructor taking a vector of dates and "
                       "possibly additional meta information...");

    std::vector<Date> dates;
    dates.emplace_back(Date(16, May, 2015));
    dates.emplace_back(Date(18, May, 2015));
    dates.emplace_back(Date(18, May, 2016));
    dates.emplace_back(Date(31, December, 2017));

    // schedule without any additional information
    Schedule schedule1(dates);
    if (schedule1.size() != dates.size())
        FAIL_CHECK("schedule1 has size " << schedule1.size() << ", expected "
                                          << dates.size());
    for (Size i = 0; i < dates.size(); ++i)
        if (schedule1[i] != dates[i])
            FAIL_CHECK("schedule1 has " << schedule1[i] << " at position " << i
                                         << ", expected " << dates[i]);
    if (schedule1.calendar() != NullCalendar())
        FAIL_CHECK("schedule1 has calendar " << schedule1.calendar().name()
                                              << ", expected null calendar");
    if (schedule1.businessDayConvention() != Unadjusted)
        FAIL_CHECK("schedule1 has convention "
                    << schedule1.businessDayConvention()
                    << ", expected unadjusted");

    // schedule with metadata
    std::vector<bool> regular;
    regular.emplace_back(false);
    regular.emplace_back(true);
    regular.emplace_back(false);
    Schedule schedule2(dates, TARGET(), Following, ModifiedPreceding, 1 * Years,
                       DateGeneration::Backward, true, regular);
    for (Size i = 1; i < dates.size(); ++i)
        if (schedule2.isRegular(i) != regular[i - 1])
            FAIL_CHECK("schedule2 has a "
                        << (schedule2.isRegular(i) ? "regular" : "irregular")
                        << " period at position " << i << ", expected "
                        << (regular[i - 1] ? "regular" : "irregular"));
    if (schedule2.calendar() != TARGET())
        FAIL_CHECK("schedule1 has calendar " << schedule2.calendar().name()
                                              << ", expected TARGET");
    if (schedule2.businessDayConvention() != Following)
        FAIL_CHECK("schedule2 has convention "
                    << schedule2.businessDayConvention()
                    << ", expected Following");
    if (schedule2.terminationDateBusinessDayConvention() != ModifiedPreceding)
        FAIL_CHECK("schedule2 has convention "
                    << schedule2.terminationDateBusinessDayConvention()
                    << ", expected Modified Preceding");
    if (schedule2.tenor() != 1 * Years)
        FAIL_CHECK("schedule2 has tenor " << schedule2.tenor()
                                           << ", expected 1Y");
    if (schedule2.rule() != DateGeneration::Backward)
        FAIL_CHECK("schedule2 has rule " << schedule2.rule()
                                          << ", expected Backward");
    if (schedule2.endOfMonth() != true)
        FAIL_CHECK("schedule2 has end of month flag false, expected true");
}

TEST_CASE( "Schedule_FourWeeksTenor", "[Schedule]" ) {
    INFO(
        "Testing that a four-weeks tenor works...");

    try {
        Schedule s =
            MakeSchedule().from(Date(13,January,2016))
                          .to(Date(4,May,2016))
                          .withCalendar(TARGET())
                          .withTenor(4*Weeks)
                          .withConvention(Following)
                          .forwards();
    } catch (Error& e) {
        FAIL_CHECK("A four-weeks tenor caused an exception: " << e.what());
    }
}
