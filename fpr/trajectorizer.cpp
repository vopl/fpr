#include "fpr/trajectorizer.hpp"

#include <algorithm>

namespace fpr
{
    Trajectorizer::Trajectorizer(const Config *config)
        : _config(config)
    {
        {
            std::size_t needHist = _config->_trajectorizerSmoothFilter.size();
            _source.reserve(needHist);
            for(std::size_t i(0); i<needHist; ++i)
            {
                _source.emplace_back(new real[_config->_frequencyGrid.size()]);
                std::fill(_source.back().get(), _source.back().get()+_config->_frequencyGrid.size(), real(0));
            }
            _sourceEndIndex = needHist;
        }

        _smoothed.resize(_config->_frequencyGrid.size());
        //_maxes.resize(_config->_frequencyGrid.size());

        _linesActiveByFreqGrid.resize(_config->_frequencyGrid.size());
    }

    Trajectorizer::~Trajectorizer()
    {
    }

    void Trajectorizer::pushSource(const real *values)
    {
        std::copy(values, values+_config->_frequencyGrid.size(), getSource(_sourceEndIndex));
        _sourceEndIndex++;

        //smooth
        {
            std::size_t w = _config->_trajectorizerSmoothFilter.size();
            std::size_t wDown = w/2;
            std::size_t wUp = w - wDown;

            std::size_t h = _config->_trajectorizerSmoothFilter[0].size();
            std::size_t hDown = h/2;
            std::size_t hUp = h - hDown;

            for(std::size_t y(hDown); y<_config->_frequencyGrid.size()-hUp; ++y)
            {
                real v = real();
                for(std::size_t dx(0); dx<w; ++dx)
                {
                    for(std::size_t dy(0); dy<h; ++dy)
                    {
                        v += getSource(_sourceEndIndex-w+dx)[y-hDown+dy] * _config->_trajectorizerSmoothFilter[dx][dy];
                    }
                }

                _smoothed[y] = v;
            }
        }


        //find maxes
        {
            std::size_t h = _config->_trajectorizerFindMaxHeight;
            std::size_t hDown = h/2;
            std::size_t hUp = h - hDown;

            for(std::size_t y(hDown); y<_config->_frequencyGrid.size()-hUp; ++y)
            {
                real v = _smoothed[y];
                bool isMax = true;
                for(std::size_t y1(y-hDown); y1<y+hUp; ++y1)
                {
                    if(y1!=y && _smoothed[y1] >= v)
                    {
                        isMax = false;
                        break;
                    }
                }

                if(isMax)
                {
                    trackLine(y, _smoothed[y]);
                }
            }
        }

        //cleanup terminated lines
        _linesActiveAll.clear();
        for(std::size_t y(0); y<_linesActiveByFreqGrid.size(); ++y)
        {
            LinePtr &line = _linesActiveByFreqGrid[y];

            if(line)
            {
                if(line->endX() != _sourceEndIndex)
                {
                   line.reset();
                }
                else
                {
                    _linesActiveAll.push_back(line.get());
                }
            }
        }


    }

    void Trajectorizer::fillResult(real *values)
    {
        std::fill(values, values+_linesActiveByFreqGrid.size(), real(0));

//        for(std::size_t y(0); y<_linesActiveByFreqGrid.size(); ++y)
//        {
//            LinePtr &line = _linesActiveByFreqGrid[y];
//            if(line)
//            {
//               values[y] = line->lastValue();
//            }
//        }

        for(std::size_t i(0); i<_linesActiveAll.size(); ++i)
        {
            Line *line = _linesActiveAll[i];
            //if(line->size()>=_config->_trajectorizerLineSmoothWidth)
            {
               values[std::size_t(line->smoothedY())] = line->lastValue();
            }
        }


    }


    std::size_t Trajectorizer::peculiarsAmount() const
    {
        return _peculiars.size();
    }

    void Trajectorizer::flushPeculiars(PeculiarPoint *buffer)
    {
        std::copy(_peculiars.begin(), _peculiars.end(), buffer);
        _peculiars.clear();
    }

    real *Trajectorizer::getSource(std::size_t index)
    {
        return _source[index % _source.size()].get();
    }

    void Trajectorizer::trackLine(std::size_t y, real v)
    {
        //find existing line or make new
        {
            std::size_t h = _config->_trajectorizerFindMaxHeight;
            std::size_t hDown = h/2;
            std::size_t hUp = h - hDown;

            std::size_t yBeg = y - std::min(y, hDown);
            std::size_t yEnd = std::min(y+hUp, _linesActiveByFreqGrid.size());
            real minDiff = std::numeric_limits<real>::max();
            std::size_t yCandidate = _linesActiveByFreqGrid.size();

            for(std::size_t yi(yBeg); yi<yEnd; ++yi)
            {
                LinePtr &candidate = _linesActiveByFreqGrid[yi];

                if(candidate)
                {
                    real diff = fpr::fabs(candidate->lastValue()-v);

                    if(diff < minDiff)
                    {
                        minDiff = diff;
                        yCandidate = yi;
                    }
                }
            }

            if(yCandidate < _linesActiveByFreqGrid.size())
            {
                std::swap(_linesActiveByFreqGrid[y], _linesActiveByFreqGrid[yCandidate]);
            }
            else
            {
                _linesActiveByFreqGrid[y].reset(new trajectorizer::Line(_config, _sourceEndIndex-1, _peculiars));
            }
        }

        //do step in line
        _linesActiveByFreqGrid[y]->step(y, v);
    }

}
