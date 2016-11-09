#pragma once

#include "peer.hpp"
#include <cstdint>
#include <deque>
#include <soci.h>

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
        };

        struct PeerFpValue
                : FpValue
        {
            std::uint64_t _trackId;
        };


        struct Point
        {
            FpValue _fpValue;

            //initial peer points
            std::vector<PeerFpValue> _initialPeerFpValues;

            //candidate in point
        };

        std::deque<Point> _points;

        //candidates


    };

}}
