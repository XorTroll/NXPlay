#include "Includes.h"

namespace UI
{
    static SDL_Window *sdl_wnd;
    static SDL_Surface *sdl_surf;
    static SDL_Renderer *sdl_render;
    static TTF_Font *fntLarge;
    static TTF_Font *fntMedium;
    static TTF_Font *fntSmall;
    static Mix_Music *audio;

    static string Back;
    static SDL_Color txtcolor;
    static SDL_Surface *sdls_Back;
    static SDL_Texture *sdlt_Back;
    static int TitleX = 60;
    static int TitleY = 30;
    static int Opt1X = 55;
    static int Opt1Y = 115;

    static vector<string> options;
    static int selected = 0;
    static vector<string> foptions;
    static int fselected = 0;
    static string FooterText;

    static int vol = 64;
    static string curfilename = ":nofile:";
    static string curfile = ":nofile:";

    SDL_Surface *InitSurface(string Path)
    {
        SDL_Surface *srf = IMG_Load(Path.c_str());
        if(srf)
        {
            Uint32 colorkey = SDL_MapRGB(srf->format, 0, 0, 0);
            SDL_SetColorKey(srf, SDL_TRUE, colorkey);
        }
        return srf;
    }

    SDL_Texture *InitTexture(SDL_Surface *surf)
    {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(sdl_render, surf);
        return tex;
    }

    void DrawText(TTF_Font *font, int x, int y, SDL_Color colour, const char *text)
    {
        SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, colour, 1280);
        SDL_SetSurfaceAlphaMod(surface, 255);
        SDL_Rect position = { x, y, surface->w, surface->h };
        SDL_BlitSurface(surface, NULL, sdl_surf, &position);
        SDL_FreeSurface(surface);
    }

    void DrawRect(int x, int y, int w, int h, SDL_Color colour)
    {
        SDL_Rect rect;
        rect.x = x; rect.y = y; rect.w = w; rect.h = h;
        SDL_SetRenderDrawColor(sdl_render, colour.r, colour.g, colour.b, colour.a);
        SDL_RenderFillRect(sdl_render, &rect);
    }

    void DrawBackXY(SDL_Surface *surf, SDL_Texture *tex, int x, int y)
    {
        SDL_Rect position;
        position.x = x;
        position.y = y;
        position.w = surf->w;
        position.h = surf->h;
        SDL_RenderCopy(sdl_render, tex, NULL, &position);
    }

    void DrawBack(SDL_Surface *surf, SDL_Texture *tex)
    {
        DrawBackXY(surf, tex, 0, 0);	
    }

    void Exit()
    {
        TTF_Quit();
        IMG_Quit();
        Mix_CloseAudio();
        Mix_Quit();
        SDL_DestroyRenderer(sdl_render);
        SDL_FreeSurface(sdl_surf);
        SDL_DestroyWindow(sdl_wnd);
        SDL_Quit();
        romfsExit();
        plExit();
        exit(0);
    }

    void Draw()
    {
        SDL_RenderClear(sdl_render);
        DrawBack(sdls_Back, sdlt_Back);
        DrawText(fntLarge, TitleX, TitleY, txtcolor, "NXPlay - Multimedia player [v0.1]");
        int ox = Opt1X;
        int oy = Opt1Y;
        for(int i = 0; i < options.size(); i++)
        {
            if(i == selected)
            {
                DrawText(fntMedium, ox, oy, { 140, 140, 140, 255 }, options[i].c_str());
                if(i == 0)
                {
                    int fx = 450;
                    int fy = 125;
                    for(int j = 0; j < foptions.size(); j++)
                    {
                        if(j == fselected)
                        {
                            DrawText(fntMedium, fx, fy, { 140, 140, 140, 255 }, foptions[j].c_str());
                        }
                        else DrawText(fntMedium, fx, fy, txtcolor, foptions[j].c_str());
                        fy += 45;
                    }
                }
                else if(i == 1)
                {
                    DrawText(fntMedium, 475, 200, txtcolor, "NXPlay - Multimedia player, by XorTroll");
                    DrawText(fntMedium, 500, 250, txtcolor, "Move between pages using L-stick.");
                    DrawText(fntMedium, 500, 275, txtcolor, "Move between audio files using R-stick on the player page.");
                    DrawText(fntMedium, 500, 300, txtcolor, "Start playing an audio or play another one pressing A.");
                    DrawText(fntMedium, 500, 325, txtcolor, "Change volume up/down using D-pad.");
                    DrawText(fntMedium, 500, 350, txtcolor, "Pause/resume playing audio pressing Y.");
                    DrawText(fntMedium, 500, 375, txtcolor, "Restart audio playback pressing X.");
                    DrawText(fntMedium, 500, 400, txtcolor, "Stop audio playback pressing B.");
                    DrawText(fntMedium, 500, 425, txtcolor, "Exit app pressing Plus or Minus.");
                    DrawText(fntMedium, 475, 475, txtcolor, "Supported types: MP3, WAV, OGG, FLAC, MOD audios");
                    DrawText(fntMedium, 475, 500, txtcolor, "Video support coming soon!");
                }
            }
            else DrawText(fntMedium, ox, oy, txtcolor, options[i].c_str());
            oy += 50;
        }
        DrawText(fntLarge, TitleX, 672, txtcolor, FooterText.c_str());
        SDL_RenderPresent(sdl_render);
    }
    
    void Loop()
    {
        hidScanInput();
        int k = hidKeysDown(CONTROLLER_P1_AUTO);
        int h = hidKeysHeld(CONTROLLER_P1_AUTO);
        if(h & KEY_DUP)
        {
            if(vol < 128)
            {
                vol++;
                Mix_VolumeMusic(vol);
                float percentage = ((float)vol / 128.0) * 100.0;
                float pvol;
                modf(percentage, &pvol);
                FooterText = "Volume: " + to_string(vol) + " / " + "128 (" + to_string((int)pvol) + "%)";
            }
            Draw();
        }
        else if(h & KEY_DDOWN)
        {
            if(vol > 0)
            {
                vol--;
                Mix_VolumeMusic(vol);
                float percentage = ((float)vol / 128.0) * 100.0;
                float pvol;
                modf(percentage, &pvol);
                FooterText = "Volume: " + to_string(vol) + " / " + "128 (" + to_string((int)pvol) + "%)";
            }
            Draw();
        }
        if(k & KEY_LSTICK_UP)
        {
            if(selected > 0) selected -= 1;
            else selected = options.size() - 1;
            Draw();
        }
        else if(k & KEY_LSTICK_DOWN)
        {
            if(selected < options.size() - 1) selected += 1;
            else selected = 0;
            Draw();
        }
        else if(k & KEY_RSTICK_UP)
        {
            if(selected == 0)
            {
                if(fselected > 0) fselected -= 1;
                else fselected = foptions.size() - 1;
                Draw();
            }
        }
        else if(k & KEY_RSTICK_DOWN)
        {
            if(selected == 0)
            {
                if(fselected < foptions.size() - 1) fselected += 1;
                else fselected = 0;
                Draw();
            }
        }
        else if(k & KEY_X)
        {
            Mix_RewindMusic();
            FooterText = "Rewinded audio: " + curfilename;
            Draw();
        }
        else if(k & KEY_Y)
        {
            if(Mix_PausedMusic() == 1)
            {
                Mix_ResumeMusic();
                FooterText = "Resumed audio: " + curfilename;
            }
            else
            {
                Mix_PauseMusic();
                FooterText = "Paused audio: " + curfilename;
            }
            Draw();
        }
        else if(k & KEY_A)
        {
            
            string file = "sdmc:/switch/nxplay/" + foptions[fselected];
            if(curfile != file)
            {
                curfile = file;
                curfilename = foptions[fselected];
                Mix_CloseAudio();
                Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
                audio = Mix_LoadMUS(curfile.c_str());
                Mix_PlayMusic(audio, 1);
                FooterText = "Loading audio: " + curfilename;
            }
            else if(Mix_PlayingMusic() != 1)
            {
                Mix_PlayMusic(audio, 1);
                FooterText = "Playing audio: " + curfilename;
            }
            Draw();
        }
        else if(k & KEY_B)
        {
            Mix_HaltMusic();
            FooterText = "Stopped audio: " + curfilename;
            Draw();
        }
        else if(k & KEY_PLUS || k & KEY_MINUS) Exit();
    }

    void Init()
    {
        romfsInit();
        ColorSetId id;
        setsysInitialize();
        setsysGetColorSetId(&id);
        if(id == ColorSetId_Dark)
        {
            Back = "romfs:/Graphics/Background.Dark.png";
            txtcolor = { 255, 255, 255, 255 };
        }
        else
        {
            Back = "romfs:/Graphics/Background.Light.png";
            txtcolor = { 0, 0, 0, 255 };
        }
        setsysExit();
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(1280, 720, 0, &sdl_wnd, &sdl_render);
        sdl_surf = SDL_GetWindowSurface(sdl_wnd);
        SDL_SetRenderDrawBlendMode(sdl_render, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        TTF_Init();
        SDL_SetRenderDrawColor(sdl_render, 255, 255, 255, 255);
        Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
        Mix_VolumeMusic(vol);
        plInitialize();
        PlFontData data;
        plGetSharedFontByType(&data, PlSharedFontType_Standard);
        SDL_RWops *ops = SDL_RWFromMem(data.address, data.size);
        fntLarge = TTF_OpenFontRW(ops, 1, 25); // TTF_OpenFont("romfs:/Fonts/NintendoStandard.ttf", 25);
        fntMedium = TTF_OpenFontRW(ops, 1, 20); // TTF_OpenFont("romfs:/Fonts/NintendoStandard.ttf", 20);
        fntSmall = TTF_OpenFontRW(ops, 1, 10); // TTF_OpenFont("romfs:/Fonts/NintendoStandard.ttf", 10);
        sdls_Back = InitSurface(Back);
        sdlt_Back = InitTexture(sdls_Back);
        options.push_back("Playable file(s)");
        options.push_back("About NXPlay");
        FooterText = "Ready to play audio!";
        dirent *dent;
        DIR *dp;
        struct stat st;
        dp = opendir("sdmc:/switch/nxplay/");
        if(dp)
        {
            while(true)
            {
                dent = readdir(dp);
                if(dent == NULL) break;
                string path = string(dent->d_name);
                string fullp = "sdmc:/switch/nxplay/" + path;
                string ext = fullp.substr(fullp.find_last_of(".") + 1);
                transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if(stat(fullp.c_str(), &st) == 0)
                {
                    if(st.st_mode & S_IFREG)
                    {
                        if(ext == "mp3" || ext == "wav" || ext == "ogg" || ext == "flac" || ext == "mod") foptions.push_back(path);
                    }
                }
            }
        }
        else mkdir("sdmc:/switch/nxplay", 0777);
        if(foptions.empty()) FooterText = "No audio files were found at \"sdmc:/switch/nxplay\".";
        Draw();
    }
}