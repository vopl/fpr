#pragma once

#include "peer.hpp"
#include <cstdint>
#include <deque>
#include <soci.h>
#include <cmath>

namespace aid { namespace mapping
{
    class Solver
    {
    public:
        Solver();
        ~Solver();

        /*
         * setup
         *      максимальная дистанция при извлечении начальных точек, 0-1-2
         *      максимальная дистанция при сопоставлении точек пиров, 0-64
         *      минимальный габарит времени для сопоставления пиров
         */

        void pushFingerprintValue(double pos, std::uint64_t value);

        void update();

        std::vector<Peer> getPeers();

    private:
        soci::session _sql;

    private:

        struct FpValue
        {
            double _pos;
            std::uint64_t _value;

            bool operator==(const FpValue &with) const
            {
                return std::abs(_pos - with._pos)<std::numeric_limits<double>::epsilon()*100 &&
                        _value == with._value;
            }
        };

        struct PeerFpValue
                : FpValue
        {
            std::uint64_t _trackId;

            bool operator==(const PeerFpValue &with) const
            {
                return _trackId == with._trackId &&
                        static_cast<const FpValue&>(*this) == static_cast<const FpValue&>(with);
            }

        };


        struct Point
        {
            FpValue _fpValue;

            //initial peer points
            std::vector<PeerFpValue> _initialPeerFpValues;

            //candidate in point
            std::vector<PeerFpValue> _allPeerFpValues;
        };

        std::deque<Point> _points;

        //candidates
        struct CandidateRange
        {
            std::size_t _trackId = 0;
            double _start = 0;
            double _stop = 0;

            std::vector<FpValue> _fpValues;
        };

        const double _lookupCandidateTime = 100;//sec

        std::map<std::size_t, CandidateRange> _candidateRanges; //trackId -> candidateRange

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::vector<Peer> _peers;


    private:
        bool match(const FpValue &src, const FpValue &peer);

    };

}}
