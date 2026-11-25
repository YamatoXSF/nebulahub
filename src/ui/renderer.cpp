// NEBULA HUB — Renderer (SDL2 wrapper)

#include "renderer.h"
#include <cstring>
#include <cmath>
#include <algorithm>

Renderer::Renderer(SDL_Renderer* r) : sdl_(r) {}

Renderer::~Renderer() {
    for (auto& [k, t] : texCache_) SDL_DestroyTexture(t);
}

// getFont() removed — SDL2_ttf isn't bundled in this toolchain image.
// text()/measureText() below use geometry-only placeholders instead.

void Renderer::clear(Color bg) {
    SDL_SetRenderDrawColor(sdl_, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderClear(sdl_);
}

void Renderer::present() { SDL_RenderPresent(sdl_); }

void Renderer::fillRect(int x, int y, int w, int h, Color c, float alpha) {
    SDL_SetRenderDrawBlendMode(sdl_, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdl_, c.r, c.g, c.b, (uint8_t)(c.a * alpha));
    SDL_Rect r{x, y, w, h};
    SDL_RenderFillRect(sdl_, &r);
}

void Renderer::drawRect(int x, int y, int w, int h, Color c, int thick) {
    SDL_SetRenderDrawColor(sdl_, c.r, c.g, c.b, c.a);
    for (int t = 0; t < thick; ++t) {
        SDL_Rect r{x+t, y+t, w-2*t, h-2*t};
        SDL_RenderDrawRect(sdl_, &r);
    }
}

void Renderer::fillRoundRect(int x, int y, int w, int h, int radius, Color c, float alpha) {
    // Simple approximation: filled rect + four corner fills
    uint8_t a = (uint8_t)(c.a * alpha);
    SDL_SetRenderDrawBlendMode(sdl_, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdl_, c.r, c.g, c.b, a);
    // Center rect (no corners)
    SDL_Rect center{x + radius, y, w - 2*radius, h};
    SDL_RenderFillRect(sdl_, &center);
    SDL_Rect left{x, y + radius, radius, h - 2*radius};
    SDL_RenderFillRect(sdl_, &left);
    SDL_Rect right{x + w - radius, y + radius, radius, h - 2*radius};
    SDL_RenderFillRect(sdl_, &right);
    // Fill corners with circles (pixel-level, small radius)
    for (int dy = 0; dy < radius; ++dy) {
        for (int dx = 0; dx < radius; ++dx) {
            if (dx*dx + dy*dy <= radius*radius) {
                // Top-left
                SDL_RenderDrawPoint(sdl_, x + radius - 1 - dx, y + radius - 1 - dy);
                // Top-right
                SDL_RenderDrawPoint(sdl_, x + w - radius + dx, y + radius - 1 - dy);
                // Bottom-left
                SDL_RenderDrawPoint(sdl_, x + radius - 1 - dx, y + h - radius + dy);
                // Bottom-right
                SDL_RenderDrawPoint(sdl_, x + w - radius + dx, y + h - radius + dy);
            }
        }
    }
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, Color c) {
    SDL_SetRenderDrawColor(sdl_, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(sdl_, x1, y1, x2, y2);
}

void Renderer::gradientH(int x, int y, int w, int h, Color left, Color right) {
    for (int i = 0; i < w; ++i) {
        float t = (float)i / (float)(w - 1);
        Color c {
            (uint8_t)(left.r + t*(right.r - left.r)),
            (uint8_t)(left.g + t*(right.g - left.g)),
            (uint8_t)(left.b + t*(right.b - left.b)),
            (uint8_t)(left.a + t*(right.a - left.a))
        };
        SDL_SetRenderDrawColor(sdl_, c.r, c.g, c.b, c.a);
        SDL_RenderDrawLine(sdl_, x+i, y, x+i, y+h);
    }
}

// TEXT RENDERING IS TEMPORARILY DISABLED.
// SDL2_ttf is not bundled in this toolchain's Docker image, and no
// replacement (e.g. direct FreeType glyph rendering) has been written
// and verified yet. The functions below do NOT draw any glyphs — they
// keep the same signatures and return layout-safe estimated sizes so
// every other screen's positioning math (centering, wrapping, button
// sizing) still works correctly once real text rendering is added back.
// Next step to restore real text: either locate SDL2_ttf under a
// different name in the toolchain, or implement glyph rendering
// directly against libfreetype.a (confirmed present in this toolchain).

void Renderer::text(const std::string& str, int x, int y, Color c,
                    int size, bool bold, bool shadow) {
    (void)str; (void)x; (void)y; (void)c; (void)size; (void)bold; (void)shadow;
    // Intentionally draws nothing — see note above.
}

SDL_Point Renderer::measureText(const std::string& str, int size, bool bold) {
    // Rough proportional-font estimate: ~0.55 * size per character.
    // Real metrics will differ once actual glyph rendering is in place.
    int w = (int)(str.size() * size * 0.55f);
    int h = (int)(size * 1.2f);
    (void)bold;
    return {w, h};
}

void Renderer::textCentered(const std::string& str, int rx, int ry, int rw, int rh,
                             Color c, int size, bool bold) {
    auto dim = measureText(str, size, bold);
    int x = rx + (rw - dim.x) / 2;
    int y = ry + (rh - dim.y) / 2;
    text(str, x, y, c, size, bold);
}

std::string Renderer::truncate(const std::string& str, int maxWidth, int size, bool bold) {
    auto dim = measureText(str, size, bold);
    if (dim.x <= maxWidth) return str;
    std::string s = str;
    while (!s.empty()) {
        s.pop_back();
        auto d2 = measureText(s + "…", size, bold);
        if (d2.x <= maxWidth) return s + "…";
    }
    return "…";
}

SDL_Texture* Renderer::loadTexture(const std::string& path) {
    auto it = texCache_.find(path);
    if (it != texCache_.end()) return it->second;
    SDL_Texture* t = IMG_LoadTexture(sdl_, path.c_str());
    if (t) texCache_[path] = t;
    return t;
}

void Renderer::freeTexture(const std::string& path) {
    auto it = texCache_.find(path);
    if (it == texCache_.end()) return;
    SDL_DestroyTexture(it->second);
    texCache_.erase(it);
}

void Renderer::drawTexture(SDL_Texture* tex, int x, int y, int w, int h, float alpha) {
    if (!tex) return;
    SDL_SetTextureAlphaMod(tex, (uint8_t)(255.f * alpha));
    SDL_Rect r{x, y, w, h};
    SDL_RenderCopy(sdl_, tex, nullptr, &r);
}

void Renderer::drawTextureRounded(SDL_Texture* tex, int x, int y, int w, int h,
                                   int radius, float alpha) {
    // Simple: just draw the texture, rounded corners are hard without stencils
    // For PS4 SDL2 this is the pragmatic path
    drawTexture(tex, x, y, w, h, alpha);
}

void Renderer::progressBar(int x, int y, int w, int h, float progress,
                            Color bg, Color fg, int radius) {
    fillRoundRect(x, y, w, h, radius, bg);
    int fw = (int)(w * std::max(0.f, std::min(1.f, progress)));
    if (fw > 0) fillRoundRect(x, y, fw, h, radius, fg);
}

void Renderer::scrollBar(int x, int y, int h, float offset, float viewFraction) {
    Color track = fromHex(0x2A2A3AFF);
    Color thumb  = fromHex(COL_ACCENT);
    fillRoundRect(x, y, 6, h, 3, track);
    int th = (int)(h * viewFraction);
    int ty = y + (int)((h - th) * offset);
    fillRoundRect(x, ty, 6, th, 3, thumb);
}
