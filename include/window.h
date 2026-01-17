#pragma once

#include "context.h"

class Window {
    Context &ctx;

public:
    explicit Window(Context &ctx) : ctx(ctx) {
    };

    void Clear();

    void Present();
};
