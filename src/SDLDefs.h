#pragma once

using SDL_WindowUPtr = std::unique_ptr<SDL_Window, DeleterFunc<SDL_DestroyWindow>>;
using SDL_RendererUPtr = std::unique_ptr<SDL_Renderer, DeleterFunc<SDL_DestroyRenderer>>;
using SDL_SurfaceUPtr = std::unique_ptr<SDL_Surface, DeleterFunc<SDL_FreeSurface>>;
using SDL_TextureUPtr = std::unique_ptr<SDL_Texture, DeleterFunc<SDL_DestroyTexture>>;