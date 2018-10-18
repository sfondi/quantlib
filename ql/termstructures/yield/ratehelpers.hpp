/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 StatPro Italia srl
 Copyright (C) 2007, 2008, 2009, 2015 Ferdinando Ametrano
 Copyright (C) 2007, 2009 Roland Lichters
 Copyright (C) 2015 Maddalena Zanzi
 Copyright (C) 2015 Paolo Mazzocchi

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

/*! \file ratehelpers.hpp
    \brief deposit, FRA, futures, swap and basis swap rate helpers
*/

#ifndef quantlib_ratehelpers_hpp
#define quantlib_ratehelpers_hpp

#include <ql/termstructures/bootstraphelper.hpp>
#include <ql/instruments/vanillaswap.hpp>
#include <ql/instruments/floatfloatswap.hpp>
#include <ql/instruments/bmaswap.hpp>
#include <ql/instruments/futures.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/calendars/unitedstates.hpp>

namespace QuantLib {

    class SwapIndex;
    class Quote;

    typedef BootstrapHelper<YieldTermStructure> RateHelper;
    typedef RelativeDateBootstrapHelper<YieldTermStructure>
                                                        RelativeDateRateHelper;

    //! Rate helper for bootstrapping over IborIndex futures prices
    class FuturesRateHelper : public RateHelper {
      public:
        FuturesRateHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          Natural lengthInMonths,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesRateHelper(Real price,
                          const Date& iborStartDate,
                          Natural lengthInMonths,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        FuturesRateHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          const Date& iborEndDate,
                          const DayCounter& dayCounter,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesRateHelper(Real price,
                          const Date& iborStartDate,
                          const Date& endDate,
                          const DayCounter& dayCounter,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        FuturesRateHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          const boost::shared_ptr<IborIndex>& iborIndex,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesRateHelper(Real price,
                          const Date& iborStartDate,
                          const boost::shared_ptr<IborIndex>& iborIndex,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        //@}
        //! \name FuturesRateHelper inspectors
        //@{
        Real convexityAdjustment() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        Time yearFraction_;
        Handle<Quote> convAdj_;
    };


    //! Rate helper for bootstrapping over deposit rates
    class DepositRateHelper : public RelativeDateRateHelper {
      public:
        DepositRateHelper(const Handle<Quote>& rate,
                          const Period& tenor,
                          Natural fixingDays,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter);
        DepositRateHelper(Rate rate,
                          const Period& tenor,
                          Natural fixingDays,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter);
        DepositRateHelper(const Handle<Quote>& rate,
                          const boost::shared_ptr<IborIndex>& iborIndex);
        DepositRateHelper(Rate rate,
                          const boost::shared_ptr<IborIndex>& iborIndex);
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(YieldTermStructure*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        void initializeDates();
        Date fixingDate_;
        boost::shared_ptr<IborIndex> iborIndex_;
        RelinkableHandle<YieldTermStructure> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over %FRA rates
    class FraRateHelper : public RelativeDateRateHelper {
      public:
        FraRateHelper(const Handle<Quote>& rate,
                      Natural monthsToStart,
                      Natural monthsToEnd,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(Rate rate,
                      Natural monthsToStart,
                      Natural monthsToEnd,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(const Handle<Quote>& rate,
                      Natural monthsToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(Rate rate,
                      Natural monthsToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(const Handle<Quote>& rate,
                      Period periodToStart,
                      Natural lengthInMonths,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(Rate rate,
                      Period periodToStart,
                      Natural lengthInMonths,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(const Handle<Quote>& rate,
                      Period periodToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraRateHelper(Rate rate,
                      Period periodToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(YieldTermStructure*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        void initializeDates();
        Date fixingDate_;
        Period periodToStart_;
        Pillar::Choice pillarChoice_;
        boost::shared_ptr<IborIndex> iborIndex_;
        RelinkableHandle<YieldTermStructure> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over swap rates
    /*! \todo use input SwapIndex to create the swap */
    class SwapRateHelper : public RelativeDateRateHelper {
      public:
        SwapRateHelper(const Handle<Quote>& rate,
                       const boost::shared_ptr<SwapIndex>& swapIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<YieldTermStructure>& discountingCurve
                                               = Handle<YieldTermStructure>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapRateHelper(const Handle<Quote>& rate,
                       const Period& tenor,
                       const Calendar& calendar,
                       // fixed leg
                       Frequency fixedFrequency,
                       BusinessDayConvention fixedConvention,
                       const DayCounter& fixedDayCount,
                       // floating leg
                       const boost::shared_ptr<IborIndex>& iborIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<YieldTermStructure>& discountingCurve
                                            = Handle<YieldTermStructure>(),
                       Natural settlementDays = Null<Natural>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapRateHelper(Rate rate,
                       const boost::shared_ptr<SwapIndex>& swapIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<YieldTermStructure>& discountingCurve
                                            = Handle<YieldTermStructure>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapRateHelper(Rate rate,
                       const Period& tenor,
                       const Calendar& calendar,
                       // fixed leg
                       Frequency fixedFrequency,
                       BusinessDayConvention fixedConvention,
                       const DayCounter& fixedDayCount,
                       // floating leg
                       const boost::shared_ptr<IborIndex>& iborIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<YieldTermStructure>& discountingCurve
                                            = Handle<YieldTermStructure>(),
                       Natural settlementDays = Null<Natural>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(YieldTermStructure*);
        //@}
        //! \name SwapRateHelper inspectors
        //@{
        Spread spread() const;
        boost::shared_ptr<VanillaSwap> swap() const;
        const Period& forwardStart() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      protected:
        void initializeDates();
        Natural settlementDays_;
        Period tenor_;
        Pillar::Choice pillarChoice_;
        Calendar calendar_;
        BusinessDayConvention fixedConvention_;
        Frequency fixedFrequency_;
        DayCounter fixedDayCount_;
        boost::shared_ptr<IborIndex> iborIndex_;
        boost::shared_ptr<VanillaSwap> swap_;
        RelinkableHandle<YieldTermStructure> termStructureHandle_;
        Handle<Quote> spread_;
        Period fwdStart_;
        Handle<YieldTermStructure> discountHandle_;
        RelinkableHandle<YieldTermStructure> discountRelinkableHandle_;
    };

	//! Rate helper for bootstrapping over basis swap spreads
	// The curve to be bootstrapped is by definition linked on index2
	// The exogenus forwarding curve is by definition linked on index1
	// The basis spread can be added both on leg1 or leg2
	class FloatFloatSwapRateHelper : public RelativeDateRateHelper {
      public:

		FloatFloatSwapRateHelper(const Handle<Quote>& basisSpread,
						   const Date& effectiveDate,
						   const Period& tenor,
						   const Calendar& calendar,
						   const BusinessDayConvention& convention,
						   const BusinessDayConvention& terminationDateConvention,
						   const boost::shared_ptr<IborIndex>& index1,
						   const boost::shared_ptr<IborIndex>& index2,
						   const Size& basisLeg,
						   const Handle<YieldTermStructure>& discountingCurve
								 = Handle<YieldTermStructure>(),
						   const bool& endOfMonth=false,
						   const DayCounter& dayCount1 = DayCounter(),
						   const DayCounter& dayCount2 = DayCounter(),
						   const Pillar::Choice& pillarChoice = Pillar::LastRelevantDate,
						   const Date& customPillarDate = Date());
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(YieldTermStructure*);
        //@}
		////! \name SwapRateHelper inspectors
        //@{
        boost::shared_ptr<FloatFloatSwap> basisSwap() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
	protected:
        void initializeDates();
		boost::shared_ptr<FloatFloatSwap> basisSwap_;
        Period tenor_;
		Handle<YieldTermStructure> discountHandle_;
		boost::shared_ptr<InterestRateIndex> index1_;
		boost::shared_ptr<InterestRateIndex> index2_;  
		Size basisLeg_;
		DayCounter dayCount1_;
		DayCounter dayCount2_;
		Period tenor1_;
		Period tenor2_;
		Schedule schedule1_;
		Schedule schedule2_;
        RelinkableHandle<YieldTermStructure> discountRelinkableHandle_;
        Pillar::Choice pillarChoice_;
		Date customPillarDate_;
        RelinkableHandle<YieldTermStructure> termStructureHandle_;
};

    //! Rate helper for bootstrapping over BMA swap rates
    class BMASwapRateHelper : public RelativeDateRateHelper {
      public:
        BMASwapRateHelper(const Handle<Quote>& liborFraction,
                          const Period& tenor, // swap maturity
                          Natural settlementDays,
                          const Calendar& calendar,
                          // bma leg
                          const Period& bmaPeriod,
                          BusinessDayConvention bmaConvention,
                          const DayCounter& bmaDayCount,
                          const boost::shared_ptr<BMAIndex>& bmaIndex,
                          // ibor leg
                          const boost::shared_ptr<IborIndex>& index);
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(YieldTermStructure*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
    protected:
        void initializeDates();
        Period tenor_;
        Natural settlementDays_;
        Calendar calendar_;
        Period bmaPeriod_;
        BusinessDayConvention bmaConvention_;
        DayCounter bmaDayCount_;
        boost::shared_ptr<BMAIndex> bmaIndex_;
        boost::shared_ptr<IborIndex> iborIndex_;

        boost::shared_ptr<BMASwap> swap_;
        RelinkableHandle<YieldTermStructure> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over Fx Swap rates
    /*! The forward is given by `fwdFx = spotFx + fwdPoint`.

        `isFxBaseCurrencyCollateralCurrency` indicates if the base
        currency of the FX currency pair is the one used as collateral.

        `calendar` is usually the joint calendar of the two currencies
        in the pair.

        `tradingCalendar` can be used when the cross pairs don't
        include the currency of the business center (usually USD; the
        corresponding calendar is `UnitedStates`).  If given, it will
        be used for adjusting the earliest settlement date and for
        setting the latest date. Due to FX spot market conventions, it
        is not sufficient to pass a JointCalendar with UnitedStates
        included as `calendar`; with regard the earliest date, this
        calendar is only used in case the spot date of the two
        currencies is not a US business day.

        \warning The ON fx swaps can be achieved by setting
                 `fixingDays` to 0 and using a tenor of '1d'. The same
                 tenor should be used for TN swaps, with `fixingDays`
                 set to 1.  However, handling ON and TN swaps for
                 cross rates without USD is not trivial and should be
                 treated with caution. If today is a US holiday, ON
                 trade is not possible. If tomorrow is a US Holiday,
                 the ON trade will be at least two business days long
                 in the other countries and the TN trade will not
                 exist. In such cases, if this helper is used for
                 curve construction, probably it is safer not to pass
                 a trading calendar to the ON and TN helpers and
                 provide fwdPoints that will yield proper level of
                 discount factors.
    */
    class FxSwapRateHelper : public RelativeDateRateHelper {
      public:
        FxSwapRateHelper(const Handle<Quote>& fwdPoint,
                         const Handle<Quote>& spotFx,
                         const Period& tenor,
                         Natural fixingDays,
                         const Calendar& calendar,
                         BusinessDayConvention convention,
                         bool endOfMonth,
                         bool isFxBaseCurrencyCollateralCurrency,
                         const Handle<YieldTermStructure>& collateralCurve,
                         const Calendar& tradingCalendar = Calendar());
        //! \name RateHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(YieldTermStructure*);
        //@}
        //! \name FxSwapRateHelper inspectors
        //@{
        Real spot() const { return spot_->value(); }
        Period tenor() const { return tenor_; }
        Natural fixingDays() const { return fixingDays_; }
        Calendar calendar() const { return cal_; }
        BusinessDayConvention businessDayConvention() const { return conv_; }
        bool endOfMonth() const { return eom_; }
        bool isFxBaseCurrencyCollateralCurrency() const {
                                return isFxBaseCurrencyCollateralCurrency_; }
        Calendar tradingCalendar() const { return tradingCalendar_; }
        Calendar adjustmentCalendar() const { return jointCalendar_; }
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
    private:
        void initializeDates();
        Handle<Quote> spot_;
        Period tenor_;
        Natural fixingDays_;
        Calendar cal_;
        BusinessDayConvention conv_;
        bool eom_;
        bool isFxBaseCurrencyCollateralCurrency_;

        RelinkableHandle<YieldTermStructure> termStructureHandle_;

        Handle<YieldTermStructure> collHandle_;
        RelinkableHandle<YieldTermStructure> collRelinkableHandle_;

        Calendar tradingCalendar_;
        Calendar jointCalendar_;
    };

    // inline

    inline Spread SwapRateHelper::spread() const {
        return spread_.empty() ? 0.0 : spread_->value();
    }

	inline boost::shared_ptr<FloatFloatSwap> FloatFloatSwapRateHelper::basisSwap() const {
        return basisSwap_;
	}

    inline boost::shared_ptr<VanillaSwap> SwapRateHelper::swap() const {
        return swap_;
    }

    inline const Period& SwapRateHelper::forwardStart() const {
        return fwdStart_;
    }

}

#endif
