#pragma once

#include "fpr/config.hpp"
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

    private:
        real *getSource(std::size_t index);

    private:
        const Config *_config = nullptr;

        std::size_t _sourceEndIndex = 0;
        std::vector<std::unique_ptr<real[]>> _source;

        TVReal _work;
    };
}
