#include "Pch.h"
#include "Render.h"

#define OPTIMIZE_DENORMALS 0
#define SAVE_BMP           1
#define SAVE_PPM           0
#define RENDER_TO_WINDOW   1

#if OPTIMIZE_DENORMALS
    #include <xmmintrin.h>
#endif

constexpr const char *imagePath = "renders";
constexpr const char *imageName = "render";

#if RENDER_TO_WINDOW
constexpr int WINDOW_SIZE_MULT = 3;

int RenderToWindow(const ImageInfo &imageInfo);
#endif

int main(int, char **)
{
#if defined(OPTIMIZE_DENORMALS)
    _mm_setcsr(_mm_getcsr() | (_MM_FLUSH_ZERO_ON | _MM_DENORMALS_ZERO_ON));
#endif

    ImageInfo imageInfo = Render();

#if SAVE_BMP || SAVE_PPM
    std::filesystem::create_directory(imagePath);
#endif

#if SAVE_BMP
    SaveBMP(std::format("{}/{}.bmp", imagePath, imageName), imageInfo, OverwriteFileOption::AddSuffix);
#endif

#if SAVE_PPM
    SavePPM(std::format("{}/{}.ppm", imagePath, imageName), imageInfo, OverwriteFileOption::AddSuffix);
#endif

#if RENDER_TO_WINDOW
    return RenderToWindow(imageInfo);
#else
    return EXIT_SUCCESS;
#endif
}

#if RENDER_TO_WINDOW
int RenderToWindow(const ImageInfo &imageInfo)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        ERROR("SDL_Init Error: {}", SDL_GetError());
        return EXIT_FAILURE;
    }

    ON_SCOPE_EXIT([]
        { SDL_Quit(); });

    auto window = SDL_WindowUPtr(SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, imageInfo.width * WINDOW_SIZE_MULT, imageInfo.height * WINDOW_SIZE_MULT, SDL_WINDOW_SHOWN));
    if (!window)
    {
        ERROR("SDL_CreateWindow Error: {}", SDL_GetError());
        return EXIT_FAILURE;
    }

    auto renderer = SDL_RendererUPtr(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!renderer)
    {
        ERROR("SDL_CreateRenderer Error: {}", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_TextureUPtr tex = [&imageInfo, &renderer]()
    {
        SDL_SurfaceUPtr surface = ImageInfoToSurface(imageInfo);
        if (!surface)
        {
            ERROR("SDL_LoadBMP Error: {}", SDL_GetError());
            return SDL_TextureUPtr();
        }

        return SDL_TextureUPtr(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
    }();

    if (!tex)
    {
        ERROR("SDL_CreateTextureFromSurface Error: {}", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

    bool quit = false;
    while (!quit)
    {
        SDL_Event e;
        SDL_WaitEvent(&e);
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }

        SDL_RenderCopy(renderer.get(), tex.get(), nullptr, nullptr);
        SDL_RenderPresent(renderer.get());
    }

    return EXIT_SUCCESS;
}
#endif