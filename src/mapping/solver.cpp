#include "mapping/solver.hpp"
#include <cassert>
#include <soci/postgresql/soci-postgresql.h>
#include <soci/statement.h>

using namespace soci;

namespace aid { namespace mapping
{
    Solver::Solver()
    {
        _sql.open(postgresql, "postgresql://postgres@127.0.0.1:5432/test");
    }

    Solver::~Solver()
    {

    }

    void Solver::pushFingerprintValue(double pos, std::uint64_t value)
    {
        Point p;
        p._fpValue._pos = pos;
        p._fpValue._value = value;

        PeerFpValue peerFpValue;
        statement st = (_sql.prepare << "SELECT track_id, pos, val FROM query1(:value)",
                into(peerFpValue._trackId),
                into(peerFpValue._pos),
                into(peerFpValue._value),
                use(value));

        st.execute();

        while(st.fetch())
        {
            p._initialPeerFpValues.push_back(peerFpValue);
        }

        _points.push_back(p);
    }

    void Solver::update()
    {
        //загрузить всех кандидатов
        //рассовать куски кандидатов по точкам
        //делать протяжку
        //сформировать веса кусков кандидатов
        assert(0);
    }

    std::vector<Peer> Solver::getPeers()
    {
        assert(0);
    }

}}
