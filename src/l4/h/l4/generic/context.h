#pragma once

#include <l4/object/context.h>

void restore_context(const struct context *ctx);
void save_context(struct context *ctx);
void context_init(struct context *ctx);
