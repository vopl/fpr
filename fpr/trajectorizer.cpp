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

        _work.resize(_config->_frequencyGrid.size());
    }

    Trajectorizer::~Trajectorizer()
    {
    }

    void Trajectorizer::pushSource(const real *values)
    {
        std::copy(values, values+_config->_frequencyGrid.size(), getSource(_sourceEndIndex));
        _sourceEndIndex++;
    }

    void Trajectorizer::fillResult(real *values)
    {
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
                real v = 0;
                for(std::size_t dx(0); dx<w; ++dx)
                {
                    for(std::size_t dy(0); dy<h; ++dy)
                    {
                        v += getSource(_sourceEndIndex-w+dx)[y-hDown+dy] * _config->_trajectorizerSmoothFilter[dx][dy];
                    }
                }

                _work[y] = v;
            }
        }


        //find maxes
        {
            std::size_t h = _config->_trajectorizerFindMaxHeight;
            std::size_t hDown = h/2;
            std::size_t hUp = h - hDown;

            for(std::size_t y(0); y<hDown; ++y)
            {
                values[y] = 0;
            }

            for(std::size_t y(0); y<hUp; ++y)
            {
                values[_config->_frequencyGrid.size()-1-y] = 0;
            }

            for(std::size_t y(hDown); y<_config->_frequencyGrid.size()-hUp; ++y)
            {
                real v = _work[y];
                bool isMax = true;
                for(std::size_t y1(y-hDown); y1<y+hUp; ++y1)
                {
                    if(y1!=y && _work[y1] >= v)
                    {
                        isMax = false;
                        break;
                    }
                }

                values[y] = isMax * v;
            }

        }
    }

    real *Trajectorizer::getSource(std::size_t index)
    {
        return _source[index % _source.size()].get();
    }

}
