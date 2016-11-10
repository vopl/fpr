#pragma once

#include <cstdint>
#include <string>

namespace aid { namespace mapping
{
    struct Peer
    {
        std::uint64_t _trackId;
        std::string _trackName;

        double _timeStart;
        double _timeStop;


        double _similarity;
    };

}}
