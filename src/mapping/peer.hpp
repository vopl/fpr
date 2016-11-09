#pragma once

#include <cstdint>

namespace aid { namespace mapping
{
    struct Peer
    {
        std::uint64_t _trackId;

        double _timeStart;
        double _timeStop;


        double _similarity;
    };

}}
