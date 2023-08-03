#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer, SDL_Window* window) 
  : renderer(renderer) {
	texture = NULL;
	width = 0;
	height = 0;
	pixels = NULL;
	pitch = 0;

  format = SDL_GetWindowPixelFormat(window);
  mappingFormat = SDL_AllocFormat(format);
}

Texture::Texture(SDL_Renderer* renderer) 
  : renderer(renderer) {
	texture = nullptr;
	width = 0;
	height = 0;
	pixels = nullptr;
	pitch = 0;

  format = SDL_PIXELFORMAT_RGBA8888;
  mappingFormat = SDL_AllocFormat(format);
}

Texture::~Texture() {
  SDL_FreeFormat(mappingFormat);
  if (texture) {
    SDL_DestroyTexture(texture);
  }

  texture = nullptr;
  width = 0;
  height = 0;
  pixels = nullptr;
  pitch = 0;
}

void Texture::executeShader(std::function<Uint32(Uint32)> func) {
	lockTexture();
  Uint32* pixels = getPixels();
  
  for(int i = 0; i < width * height; ++i) {
    pixels[i] = func(pixels[i]);
  }
  unlockTexture();
}

void Texture::load(std::string path) {
  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }

	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, format, 0);
  newTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, loadedSurface->w, loadedSurface->h);

  SDL_LockTexture(newTexture, NULL, &pixels, &pitch);
  memcpy(pixels, formattedSurface->pixels, pitch * formattedSurface->h);
  SDL_UnlockTexture(newTexture);

  pixels = nullptr;
  width = loadedSurface->w;
  height = loadedSurface->h;
  
  SDL_FreeSurface(formattedSurface);
  SDL_FreeSurface(loadedSurface);
  
	texture = newTexture;
}

Uint32 Texture::color(Uint8 red, Uint8 green, Uint8 blue) {
  return SDL_MapRGB(mappingFormat, red, green, blue);
}

void Texture::render(int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
  int rWidth = width;
  int rHeight = height;

  if (w != 0) {
    rWidth = w;
  }

  if (h != 0) {
    rHeight = h;
  }

	SDL_Rect renderQuad = { x, y, rWidth, rHeight };

	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer, texture, clip, &renderQuad);
}

bool Texture::lockTexture() {
	if(pixels != nullptr) {
    // texture is already locked
    return false;
	}

  SDL_LockTexture(texture, NULL, &pixels, &pitch);
	return true;
}

bool Texture::unlockTexture() {
	if(pixels == nullptr) {
    // texture is not locked
    return false;
	}

  SDL_UnlockTexture(texture);
  pixels = nullptr;
  pitch = 0;

  return true;
}

Uint32 Texture::getPixel(int x, int y) {
  if(pixels == nullptr) {
    return 0;
  }

  Uint32* pixels = getPixels();
  Uint32 pixel = pixels[(y * pitch / 4) + x];
  return pixel;
}

Uint32 Texture::getPixel(int xy) {
  if(pixels == nullptr) {
    return 0;
  }

  lockTexture();
  Uint32* pixels = getPixels();
  Uint32 pixel = pixels[xy];
  unlockTexture();

  return pixel;
}

Uint32* Texture::getPixels() {
	return (Uint32*) pixels;
}
