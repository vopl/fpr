#include "mapping/solver.hpp"
#include <cassert>
#include <soci/postgresql/soci-postgresql.h>
#include <soci/statement.h>

#include <cmath>
#include <cstdint>
#include <algorithm>

using namespace soci;


namespace
{
    template<class T>
    std::size_t popcnt(T v)
    {
        std::size_t res = 0;
        while(v)
        {
            res += v & 1;
            v = v >> 1;
        }

        return res;
    }
}


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
        statement st = (_sql.prepare << "SELECT track_id, pos, val FROM query2(:value)",
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
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //сформировать перечень кандидатов
        _candidateRanges.clear();
        for(const Point &point : _points)
        {
            for(const PeerFpValue &peerFpValue : point._initialPeerFpValues)
            {
                if(_candidateRanges.end() == _candidateRanges.find(peerFpValue._trackId))
                {
                    CandidateRange cr;
                    cr._trackId = peerFpValue._trackId;
                    cr._start = peerFpValue._pos - _lookupCandidateTime;
                    cr._stop = peerFpValue._pos + _lookupCandidateTime;
                    _candidateRanges[peerFpValue._trackId] = cr;

                }
                else
                {
                    CandidateRange &cr = _candidateRanges[peerFpValue._trackId];
                    cr._start = std::min(cr._start, peerFpValue._pos - _lookupCandidateTime);
                    cr._stop = std::max(cr._stop, peerFpValue._pos + _lookupCandidateTime);
                }
            }
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //загрузить всех кандидатов
        {
            FpValue candidateFpValue;
            std::uint64_t track_id;
            double start, stop;
            statement st = (_sql.prepare << "SELECT pos, val FROM fingerprint WHERE track_id=:track_id AND pos>:start AND pos<:stop ORDER BY pos",
                    into(candidateFpValue._pos),
                    into(candidateFpValue._value),
                    use(track_id),
                    use(start),
                    use(stop));

            for(auto &crp : _candidateRanges)
            {
                CandidateRange &cr = crp.second;

                track_id = cr._trackId;
                start = cr._start;
                stop = cr._stop;

                st.execute();

                while(st.fetch())
                {
                    cr._fpValues.push_back(candidateFpValue);
                }
            }
        }


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //рассовать куски кандидатов по точкам
        {
            for(Point &p : _points)
            {
                p._allPeerFpValues.clear();
            }

            for(std::size_t i(0); i<_points.size(); ++i)
            {
                Point &p0 = _points[i];
                for(const PeerFpValue &peerFpValue : p0._initialPeerFpValues)
                {
                    //p0._allPeerFpValues.push_back(peerFpValue);
                    if(p0._allPeerFpValues.end() ==
                       std::find(
                           p0._allPeerFpValues.begin(),
                           p0._allPeerFpValues.end(),
                           peerFpValue))
                    {
                        p0._allPeerFpValues.push_back(peerFpValue);
                    }


                    CandidateRange &cr = _candidateRanges[peerFpValue._trackId];

                    //найти у кандидата позицию которая соответствует начальной точке
                    std::size_t peerIndex0 = 0;
                    for(std::size_t k(0); k<cr._fpValues.size(); ++k)
                    {
                        if(std::abs(cr._fpValues[k]._pos - peerFpValue._pos) < std::numeric_limits<double>::epsilon()*100 &&
                           cr._fpValues[k]._value == peerFpValue._value)
                        {
                            peerIndex0 = k;
                        }

                    }

                    //назад
                    {
                        std::size_t srcIndex = i-1;
                        std::size_t peerIndex = peerIndex0-1;
                        while(srcIndex < _points.size() && peerIndex < cr._fpValues.size())
                        {
                            bool matched = false;
                            for(std::size_t srcStep(1); srcStep<3; ++srcStep)
                            {
                                if(srcIndex >= _points.size() || peerIndex >= cr._fpValues.size()) break;

                                for(std::size_t dstStep(1); dstStep<3; ++dstStep)
                                {
                                    if(srcIndex-srcStep >= _points.size() || peerIndex-dstStep >= cr._fpValues.size())
                                    {
                                        srcIndex += 10000;
                                        peerIndex += 100000;
                                        break;
                                    }

                                    if(match(_points[srcIndex-srcStep]._fpValue, cr._fpValues[peerIndex-dstStep]))
                                    {
                                        srcIndex -= srcStep;
                                        peerIndex -= dstStep;

                                        PeerFpValue peerFpValue;
                                        peerFpValue._trackId = cr._trackId;
                                        peerFpValue._pos = cr._fpValues[peerIndex]._pos;
                                        peerFpValue._value = cr._fpValues[peerIndex]._value;

                                        if(_points[srcIndex]._allPeerFpValues.end() ==
                                           std::find(
                                               _points[srcIndex]._allPeerFpValues.begin(),
                                               _points[srcIndex]._allPeerFpValues.end(),
                                               peerFpValue))
                                        {
                                            _points[srcIndex]._allPeerFpValues.push_back(peerFpValue);
                                        }


                                        srcStep = 10000;
                                        dstStep = 10000;
                                        matched = true;
                                    }
                                }
                            }

                            if(!matched)
                            {
                                break;
                            }

                        }
                    }

                    //вперед
                    {
                        std::size_t srcIndex = i+1;
                        std::size_t peerIndex = peerIndex0+1;
                        while(srcIndex < _points.size() && peerIndex < cr._fpValues.size())
                        {
                            bool matched = false;
                            for(std::size_t srcStep(1); srcStep<3; ++srcStep)
                            {
                                if(srcIndex >= _points.size() || peerIndex >= cr._fpValues.size()) break;

                                for(std::size_t dstStep(1); dstStep<3; ++dstStep)
                                {
                                    if(srcIndex+srcStep >= _points.size() || peerIndex+dstStep >= cr._fpValues.size())
                                    {
                                        srcIndex += 10000;
                                        peerIndex += 100000;
                                        break;
                                    }

                                    if(match(_points[srcIndex+srcStep]._fpValue, cr._fpValues[peerIndex+dstStep]))
                                    {
                                        srcIndex += srcStep;
                                        peerIndex += dstStep;

                                        PeerFpValue peerFpValue;
                                        peerFpValue._trackId = cr._trackId;
                                        peerFpValue._pos = cr._fpValues[peerIndex]._pos;
                                        peerFpValue._value = cr._fpValues[peerIndex]._value;

                                        if(_points[srcIndex]._allPeerFpValues.end() ==
                                           std::find(
                                               _points[srcIndex]._allPeerFpValues.begin(),
                                               _points[srcIndex]._allPeerFpValues.end(),
                                               peerFpValue))
                                        {
                                            _points[srcIndex]._allPeerFpValues.push_back(peerFpValue);
                                        }


                                        srcStep = 10000;
                                        dstStep = 10000;
                                        matched = true;
                                    }
                                }
                            }

                            if(!matched)
                            {
                                break;
                            }

                        }
                    }


                }
            }

        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //делать протяжку
        {
            _peers.clear();
            bool done = false;

            while(!done)
            {
                done = true;
                for(std::size_t i(0); i<_points.size(); ++i)
                {
                    if(_points[i]._allPeerFpValues.empty())
                    {
                        continue;
                    }

                    done = false;

                    PeerFpValue initiator = _points[i]._allPeerFpValues[0];
                    _points[i]._allPeerFpValues.erase(_points[i]._allPeerFpValues.begin());

                    Peer peer;
                    std::vector<std::size_t> peerValueDistances;
                    std::vector<double> peerPosDeltas;
                    peerValueDistances.push_back(popcnt(_points[i]._fpValue._value ^ initiator._value));

                    peer._trackId = initiator._trackId;

                    peer._timeStart = _points[i]._fpValue._pos;
                    peer._timeStop = peer._timeStart;

                    peer._peerTimeStart = initiator._pos;
                    peer._peerTimeStop = peer._peerTimeStart;

                    peer._similarity = 0;


                    double prevPos = _points[i]._fpValue._pos;
                    double prevPeerPos = initiator._pos;

                    for(std::size_t j(i+1); j<_points.size(); ++j)
                    {
                        Point &p = _points[j];

                        double wantPeerPos = (p._fpValue._pos - prevPos) + prevPeerPos;

                        //найти кандидата с таким же треком и ближайшей позицией
                        double minPeerPosDelta = std::numeric_limits<double>::max();
                        std::size_t minPeerIndex = p._allPeerFpValues.size();

                        for(std::size_t k(0); k<p._allPeerFpValues.size(); ++k)
                        {
                            if(p._allPeerFpValues[k]._trackId == initiator._trackId)
                            {
                                double peerPosDelta = std::abs(p._allPeerFpValues[k]._pos - wantPeerPos);
                                if(peerPosDelta < minPeerPosDelta)
                                {
                                    minPeerPosDelta = peerPosDelta;
                                    minPeerIndex = k;
                                }
                            }
                        }

                        if(minPeerIndex<p._allPeerFpValues.size() && minPeerPosDelta < /*MAGIC*/ 0.1)
                        {
                            //found
                            PeerFpValue next = p._allPeerFpValues[minPeerIndex];
                            peerPosDeltas.push_back(minPeerPosDelta);
                            peerValueDistances.push_back(popcnt(p._fpValue._value ^ next._value));

                            peer._timeStop = p._fpValue._pos;
                            peer._peerTimeStop = next._pos;

                            p._allPeerFpValues.erase(p._allPeerFpValues.begin() + minPeerIndex);
                            continue;
                        }
                        else
                        {
                            //not found
                            break;
                        }



                    }

                    //flush peer
                    if(peerValueDistances.size() > 2)
                    {
                        double similarityPos = std::accumulate(peerPosDeltas.begin(), peerPosDeltas.end(), double(0));
                        if(peerPosDeltas.size())
                        {
                            similarityPos /= peerPosDeltas.size();
                        }
                        similarityPos = std::min(double(1), std::max(double(0), (0.01-similarityPos)/0.01));

                        double similarityValue = std::accumulate(peerValueDistances.begin(), peerValueDistances.end(), double(0));
                        if(peerValueDistances.size())
                        {
                            similarityValue /= peerValueDistances.size();
                        }
                        similarityValue = std::min(double(1), std::max(double(0), 32-similarityValue));

                        peer._similarity = (similarityPos + similarityValue)/2;

                        _peers.push_back(peer);
                    }

                    break;
                }
            }

            std::sort(_peers.begin(), _peers.end(), [](const Peer &a, const Peer &b){
                return std::tie(a._trackId, a._timeStart) < std::tie(b._trackId, b._timeStart);
            });


            for(Peer &peer : _peers)
            {
                _sql << "SELECT name FROM track WHERE id=:id",
                        into(peer._trackName),
                        use(peer._trackId);
            }
        }
    }

    std::vector<Peer> Solver::getPeers()
    {
        return _peers;
    }

    bool Solver::match(const FpValue &src, const FpValue &peer)
    {
        return popcnt(src._value ^ peer._value) < 10;
    }

}}
