#pragma once

#include "fpr/config.hpp"
#include "fpr/trajectorizer/line.hpp"
#include <memory>

namespace fpr
{
    class Trajectorizer
    {
    public:
        Trajectorizer(const Config *config);
        ~Trajectorizer();


        void pushSource(const real *values);
        void fillResult(real *values);

        std::size_t peculiarsAmount() const;
        void flushPeculiars(PeculiarPoint *buffer);

    private:
        real *getSource(std::size_t index);

        void trackLine(std::size_t y, real v);

    private:
        const Config *_config = nullptr;

        std::size_t _sourceEndIndex = 0;
        std::vector<std::unique_ptr<real[]>> _source;

        TVReal _smoothed;
        //TVReal _maxes;

    private:
        using Line = trajectorizer::Line;
        using LinePtr = std::unique_ptr<Line>;

        std::vector<LinePtr> _linesActiveByFreqGrid;
        std::vector<Line *> _linesActiveAll;

        std::vector<PeculiarPoint> _peculiars;
    };
}
