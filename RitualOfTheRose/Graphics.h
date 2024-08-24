#pragma once
#include "Utils.h"


template<class Interface>
inline void
SafeRelease(string ID, Interface** ppInterfaceToRelease
)
{
    try {
        if (*ppInterfaceToRelease != NULL)
        {
            (*ppInterfaceToRelease)->Release();

            (*ppInterfaceToRelease) = NULL;
        }
    }
    catch (...) {
        return;
    }

}

class Graphics {
public:
    Graphics() {
        colourTagLookupTable[wchar_t(10112)] = "BLUE";
        customFonts = { 
            pair<string, int>({"Centaur", IDF_CENTAUR}), 
            pair<string,int>({ "GoudyMedieval", IDF_GOUDYMEDIEVAL }),
            pair<string,int>({ "LightText", IDF_LIGHT }),
            pair<string,int>({ "Tower", IDF_TOWER }),
        };
        customFontSizes = {5,10,15,20,25,30,35,37,40};
        Colours["BLACK"] = { 0.0,0.0,0.0,1.0 };
        Colours["WHITE"] = { 1.0,1.0,1.0,1.0 };
        Colours["BLUE"] = { 0.0,0.0,1.0,1.0 };
        Colours["DARKBROWN"] = convertIntColour({100,35,0,255});
    }
    ~Graphics() {
        teardownAllImages();
        teardownAllTextFormats();
        tearDownAllLoadedFonts();
        SafeRelease("Releasing D2D Factory.", & D2DFactory);
        SafeRelease("Releasing IWICFactory", & IWICFactory);
        SafeRelease("Releasing DWrite Factory.", & DWriteFactory);
        SafeRelease("Releasing hwndRenderTarget.", & hwndRenderTarget);
    }
    class Drawable {
    public:
        D2D1_RECT_F getRect(pair<float, float> position, D2D1_SIZE_F size) {
            size.height *= scale;
            size.width *= scale;
            D2D1_RECT_F result;
            if (anchorStyle == "TOPLEFT") {
                result = D2D1::RectF(position.first, position.second, position.first + size.width, position.second + size.height);
            }
            if (anchorStyle == "BOTTOMMIDDLE") { // slightly offset from bottom to look like origin is at player's feet
                float smallD = size.height / 4;
                result = D2D1::RectF(
                    position.first - (size.width / 2),
                    position.second - size.height + smallD,
                    position.first + (size.width / 2),
                    position.second + smallD);
            }
            if (anchorStyle == "CENTRE") {
                result = D2D1::RectF(
                    position.first - (size.width / 2.0f),
                    position.second - (size.height / 2.0f),
                    position.first + (size.width / 2.0f),
                    position.second + (size.height / 2.0f));
            }
            return result;
        }
        pair<float, float> getAbsolutePosition(D2D1_SIZE_F renderTargetSize) {
            return { renderTargetSize.width * positionAsPercentage.first / 100.0f, renderTargetSize.height * positionAsPercentage.second / 100.0f };
        }
        pair<float, float> convertPercentToActual(D2D1_SIZE_F renderTargetSize, pair<int, int> RHS) {
            return { renderTargetSize.width * RHS.first / 100.0f, renderTargetSize.height * RHS.second / 100.0f };
        }
        pair<float, float> convertActualToPercent(D2D1_SIZE_F renderTargetSize, pair<int, int> RHS) {
            return { RHS.first * 100.0f / renderTargetSize.width, RHS.second * 100.0f / renderTargetSize.height };
        }
        pair<float, float> positionAsPercentage = { 0.0f,0.0f };
        string anchorStyle;
        string unique_ID;
        float scale = 1.0f;
        bool operator==(const Drawable& rhs) const {
            return unique_ID == rhs.unique_ID;
        }
        bool operator<(const Drawable & RHS) const {
            return positionAsPercentage.second < RHS.positionAsPercentage.second;
        };
        bool operator>(const Drawable& RHS) const {
            return positionAsPercentage.second > RHS.positionAsPercentage.second;
        };
    };
	class Image : public Drawable {
	public:
        Image() {}
        Image(List<int> _sources, pair<float, float > position, string _anchorStyle, float _opacity, string _uniqueID) {
            sources = _sources;
            positionAsPercentage = position;
            anchorStyle = _anchorStyle;
            opacity = _opacity;
            unique_ID = _uniqueID;
            resetTextures();
        }
		~Image() {}
        void resetTextures() {
            for (int x = 0; x < sources.size(); x++) {
                textures.push_back(NULL);
            }
        }
        bool isLoaded() {
			return getWhichTexture() != NULL;
		}
        HRESULT loadTexture(Graphics & graphics) {
            int source = sources.at(frame);
            ID2D1Bitmap* texture = getWhichTexture();
            HRESULT hr = S_OK;
            if (texture == NULL and graphics.TextureMemory.getKeys().contains(source) and graphics.TextureMemory[source] != NULL) {
                texture = graphics.TextureMemory[source];
            }
            if (texture == NULL) {
                IWICBitmapDecoder* pDecoder = NULL;
                IWICBitmapFrameDecode* pSource = NULL;
                IWICStream* pStream = NULL;
                IWICFormatConverter* pConverter = NULL;
                IWICBitmapScaler* pScaler = NULL;

                HRSRC imageResHandle = NULL;
                HGLOBAL imageResDataHandle = NULL;
                void* pImageFile = NULL;
                DWORD imageFileSize = 0;

                // Locate the resource.
                imageResHandle = FindResource(HINST_THISCOMPONENT, MAKEINTRESOURCE(source), L"PNG");

                hr = imageResHandle ? S_OK : E_FAIL;
                if (SUCCEEDED(hr))
                {
                    // Load the resource.
                    imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

                    hr = imageResDataHandle ? S_OK : E_FAIL;
                }
                if (SUCCEEDED(hr))
                {
                    // Lock it to get a system memory pointer.
                    pImageFile = LockResource(imageResDataHandle);

                    hr = pImageFile ? S_OK : E_FAIL;
                }
                if (SUCCEEDED(hr))
                {
                    // Calculate the size.
                    imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

                    hr = imageFileSize ? S_OK : E_FAIL;
                }
                if (SUCCEEDED(hr))
                {
                    // Create a WIC stream to map onto the memory.
                    hr = graphics.IWICFactory->CreateStream(&pStream);
                }
                if (SUCCEEDED(hr))
                {
                    // Initialize the stream with the memory pointer and size.
                    hr = pStream->InitializeFromMemory(
                        reinterpret_cast<BYTE*>(pImageFile),
                        imageFileSize
                    );
                }
                if (SUCCEEDED(hr))
                {
                    // Create a decoder for the stream.
                    hr = graphics.IWICFactory->CreateDecoderFromStream(
                        pStream,
                        NULL,
                        WICDecodeMetadataCacheOnLoad,
                        &pDecoder
                    );
                }
                if (SUCCEEDED(hr))
                {
                    // Create the initial frame.
                    hr = pDecoder->GetFrame(0, &pSource);
                }
                if (SUCCEEDED(hr))
                {
                    // Convert the image format to 32bppPBGRA
                    // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
                    hr = graphics.IWICFactory->CreateFormatConverter(&pConverter);
                }
                if (SUCCEEDED(hr))
                {
                    UINT originalWidth, originalHeight;
                    hr = pSource->GetSize(&originalWidth, &originalHeight);
                    if (SUCCEEDED(hr))
                    {
                        hr = graphics.IWICFactory->CreateBitmapScaler(&pScaler);
                        if (SUCCEEDED(hr))
                        {
                            hr = pScaler->Initialize(
                                pSource,
                                originalWidth,
                                originalHeight,
                                WICBitmapInterpolationModeCubic
                            );
                            if (SUCCEEDED(hr))
                            {
                                hr = pConverter->Initialize(
                                    pScaler,
                                    GUID_WICPixelFormat32bppPBGRA,
                                    WICBitmapDitherTypeNone,
                                    NULL,
                                    0.f,
                                    WICBitmapPaletteTypeMedianCut
                                );
                            }
                        }
                    }
                    else
                    {
                        hr = pConverter->Initialize(
                            pSource,
                            GUID_WICPixelFormat32bppPBGRA,
                            WICBitmapDitherTypeNone,
                            NULL,
                            0.f,
                            WICBitmapPaletteTypeMedianCut
                        );
                    }
                }
                if (SUCCEEDED(hr))
                {
                    //create a Direct2D bitmap from the WIC bitmap.
                    hr = graphics.hwndRenderTarget->CreateBitmapFromWicBitmap(
                        pConverter,
                        NULL,
                        &texture
                    );
                }
                SafeRelease("Releasing decoder." + unique_ID, & pDecoder);
                SafeRelease("Releasing source for " + unique_ID, & pSource);
                SafeRelease("Releasing stream for " + unique_ID, & pStream);
                SafeRelease("Releasing converter for " + unique_ID, & pConverter);
                SafeRelease("Releasing scaler for " + unique_ID, & pScaler);
            }
            textures.at(frame) = texture;
            storeTexturesInMemory(graphics);
            return hr;
        }
        ID2D1Bitmap* getWhichTexture() {
            if (frame >= sources.size()) {
                frame = 0;
            }
            if (frame > textures.size()) {
                frame = 0; // only happens if sources change to a smaller set of sources
            }
            if (textures.empty()) {
                resetTextures();
            }
            return textures.at(frame);
        }
        void draw(Graphics & graphics) {
            animate();
            ID2D1Bitmap* texture = getWhichTexture();
            if (texture == NULL) {
                loadTexture(*&graphics);
            }
            texture = textures.at(frame);
            D2D1_RECT_F rect = getPosition(graphics);
            graphics.hwndRenderTarget->DrawBitmap(
                texture,
                rect, 
                opacity);
        }
        D2D1_RECT_F getPosition(Graphics & graphics) {
            ID2D1Bitmap* texture = getWhichTexture();
            D2D1_SIZE_F size = texture->GetSize();
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<float, float> position = getAbsolutePosition(renderTargetSize);
            D2D1_RECT_F rect = getRect(position, size);
            return rect;
        }
        void animate() {
            if (!animated) { return; }
            if (animationStyles.contains("SINGLE") and frame<textures.size()-1) {
                frame++;
            }
            if (animationStyles.contains("LOOP")) { 
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_FADE", animationSpeed)) {
                    frame++;
                }
            }
            if (animationStyles.contains("FADEOUT")) {
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_FADE", animationSpeed)) {
                    opacity = TChange(opacity, -0.07f, 0.0f, 1.0f);
                }
            }
        };
        bool hasThisBeenClickedOn(Graphics & graphics, pair<float, float> click) {
            D2D1_RECT_F position = getPosition(graphics);
            bool insideX = position.left <= click.first and position.right >= click.first;
            bool insideY = position.top <= click.second and position.bottom >= click.second;
            return insideX and insideY;
        }
        void resetSources(Graphics & graphics, List<int> _sources) {
            storeTexturesInMemory(graphics);
            sources = _sources;
            textures.internalList = { };
            resetTextures();
        }
        void storeTexturesInMemory(Graphics & graphics) {
            for (int x = 0; x < sources.size(); x++) {
                if (textures.at(x) != NULL and graphics.TextureMemory[sources.at(x)] == NULL) {
                    graphics.TextureMemory[sources.at(x)] = textures.at(x);
                }
            }
        }

        List<int> sources;
        int frame = 0;
        bool animated = false;
        List<string> animationStyles;
        float opacity = 1.0;
		List<ID2D1Bitmap  *> textures;
        int animationSpeed;
        string direction = "";
        string action = "STAND";
	};
    class Text : public Drawable {
    public:
        Text() {}
        Text(wstring _message, string _format, pair<int, int> _positionAsPercentage, string _anchorStyle, pair<int, int> _size, vector<float> _colour, vector<float> _shadowColour, string _unique_ID) {
            message = _message;
            format = _format;
            positionAsPercentage = _positionAsPercentage;
            anchorStyle = _anchorStyle;
            size = _size;
            colour = _colour;
            unique_ID = _unique_ID;
            shadowColour = _shadowColour;
        }
        void draw(Graphics& graphics) {
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<int, int> position = getAbsolutePosition(renderTargetSize);
            pair<int, int> trueSize = convertPercentToActual(renderTargetSize, size);
            D2D_SIZE_F size_as_d2d = D2D_SIZE_F();
            size_as_d2d.width = trueSize.first;
            size_as_d2d.height = trueSize.second;
            D2D1_RECT_F rect = getRect(position, size_as_d2d);
            D2D1_RECT_F shadowRect = getRect({ position.first -1, position.second + 1 }, size_as_d2d);
            ID2D1SolidColorBrush* theBrush = NULL;
            ID2D1SolidColorBrush* shadowBrush = NULL;
            HRESULT hr = S_OK;
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(colour[0], colour[1], colour[2], colour[3])),
                &theBrush);
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(shadowColour[0], shadowColour[1], shadowColour[2], shadowColour[3])),
                &shadowBrush);
            IDWriteTextLayout * textLayout = NULL;
            graphics.DWriteFactory->CreateTextLayout(removeTagsBeforePrinting(graphics, message).c_str(), message.size(), graphics.WriteTextFormats[format], size_as_d2d.width, size_as_d2d.height, &textLayout);
            D2D1_POINT_2F P;
            P.x = rect.left; P.y = rect.top;
            if (anchorStyle == "CENTRE") {
                hr = textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                hr = textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            }
            D2D1_POINT_2F shadowP = P;
            shadowP.x -= 1;
            shadowP.y += 1;
            graphics.hwndRenderTarget->DrawTextLayout(shadowP, textLayout, shadowBrush);
            Map<string, List<DWRITE_TEXT_RANGE>> subcolours = interpret_subcolours(graphics);
            Map<string, ID2D1SolidColorBrush*> extraBrushes;
            for (auto const& [key, value] : subcolours.internalMap) {
                extraBrushes[key] = NULL;
                graphics.hwndRenderTarget->CreateSolidColorBrush(
                    D2D1::ColorF(D2D1::ColorF(graphics.Colours[key][0], graphics.Colours[key][1], graphics.Colours[key][2], graphics.Colours[key][3])),
                    &extraBrushes[key]);
                for (auto const& range : subcolours[key].internalList) {
                    textLayout->SetDrawingEffect(extraBrushes[key], range);
                }
            } 
            graphics.hwndRenderTarget->DrawTextLayout(P, textLayout, theBrush);
            SafeRelease("Releasing the brush", & theBrush);
            SafeRelease("Releasing the shadow brush", & shadowBrush);
            SafeRelease("Releasing text layout", & textLayout);
            for (string x : extraBrushes.getKeys().internalList) {
                SafeRelease("Releasing extra brush " + x, & extraBrushes[x]);
            }
        }
        wstring removeTagsBeforePrinting(Graphics & graphics, wstring input_string) {
            for (int x = 0; x < input_string.size(); x++) {
                if (graphics.colourTagLookupTable.getKeys().contains(input_string[x])) {
                    input_string[x] = wchar_t(32);
                }
            }
            return input_string;
        }
        Map<string, List<DWRITE_TEXT_RANGE>> interpret_subcolours(Graphics & graphics) {
            Map<string, List<DWRITE_TEXT_RANGE>> results;
            for (auto const& colour : graphics.colourTagLookupTable.getKeys().internalList) {
                int current_start = -1;
                bool seekingEnd = false;
                for (int x = 0; x < fullMessage.size(); x++) {
                    if (seekingEnd and current_start != -1 and List<wchar_t>({ wchar_t(46), wchar_t(32), wchar_t(33), wchar_t(63), colour }).contains(fullMessage[x])) {
                        int current_end = x;
                        results[graphics.colourTagLookupTable[colour]].push_back({unsigned(current_start), unsigned(current_end - current_start)});
                        current_start = -1;
                        seekingEnd = false;
                    }
                    if (fullMessage[x] == colour) {
                        current_start = x;
                        seekingEnd = true;
                    }
                    
                }
                }
            return results;
        }
        void startTypewriter() {
            fullMessage = message;
            message = L"";
        }
        int howFarAlong() {
            if (fullMessage.size() == 0) { return 100; }
            return message.size() * 100 / fullMessage.size();
        }

        wstring fullMessage;
        wstring message;
        string format;
        pair<int, int> size;
        vector<float> colour = {0.0,0.0,0.0,1.0};
        vector<float> shadowColour = { 0.0,0.0,0.0,1.0 };
        int tagLimit = 10;
    };
    void setup(HWND * hwnd, HINSTANCE _hInstance) {
        hwnd = hwnd;
        hinstance = _hInstance;
        CreateDeviceIndependentResources();
    }
    HRESULT CreateDeviceIndependentResources() {
        HRESULT hr;
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory);
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            reinterpret_cast<void**>(&IWICFactory)
        );
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(DWriteFactory),
            reinterpret_cast<IUnknown**>(&DWriteFactory)
        );
        IDWriteTextFormat* textFormat = NULL;
        hr = DWriteFactory->CreateTextFormat(L"Times New Roman",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"",
            &textFormat);
        if (SUCCEEDED(hr)) {
            WriteTextFormats.add({ "DEFAULT", textFormat });
        }

        hr = DWriteFactory->CreateFontSetBuilder(&fontSetBuilder);

        for (auto const & font : customFonts.internalList) {
            idwriteinmemoryfontfileloaders[font.first] = NULL;
            hr = DWriteFactory->CreateInMemoryFontFileLoader(&idwriteinmemoryfontfileloaders[font.first]);
            hr = DWriteFactory->RegisterFontFileLoader(idwriteinmemoryfontfileloaders[font.first]);
            idwritefontfiles[font.first] = NULL;
            HINSTANCE hInstance = ::GetModuleHandle(nullptr);
            HRSRC  hFntRes = FindResource(hInstance, MAKEINTRESOURCE(font.second), L"BINARY");
            HGLOBAL hFntMem = LoadResource(hInstance, hFntRes);
            void* FntData = LockResource(hFntMem);
            DWORD nFonts = 0, len = SizeofResource(hInstance, hFntRes);
            addfontmemresourcefonts[font.first] = AddFontMemResourceEx(FntData, len, nullptr, &nFonts);
            idwritefontfiles[font.first] = NULL;
            hr = idwriteinmemoryfontfileloaders[font.first]->CreateInMemoryFontFileReference(
                DWriteFactory,
                FntData,
                len,
                NULL,
                &idwritefontfiles[font.first]);
            
            idwritefontfacereferences[font.first] = NULL;
            DWriteFactory->CreateFontFaceReference(idwritefontfiles[font.first], 0, DWRITE_FONT_SIMULATIONS_NONE, &idwritefontfacereferences[font.first]);
            fontSetBuilder->AddFontFaceReference(idwritefontfacereferences[font.first]);
        }

        hr = fontSetBuilder->CreateFontSet(&customFontSet);
        hr = DWriteFactory->CreateFontCollectionFromFontSet(customFontSet, &fontCollection);

        for (auto const& font : customFonts.internalList) {
            for (auto const& size : customFontSizes.internalList) {
                hr = DWriteFactory->CreateTextFormat(wstring(font.first.begin(), font.first.end()).c_str(),
                    fontCollection,
                    DWRITE_FONT_WEIGHT_NORMAL,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    size,
                    L"",
                    &textFormat);
                if (SUCCEEDED(hr)) {
                    WriteTextFormats.add({ font.first + "_" + to_string(size), textFormat});
                    }
                }
            }
        
       loadCursors();

        return hr;
    }
    HRESULT CreateDeviceResources() {
        HRESULT hr = S_OK;
        if (!hwndRenderTarget) {
            RECT rc;
            GetClientRect(*hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(static_cast<UINT>(rc.right - rc.left),static_cast<UINT>(rc.bottom - rc.top));
            
            hr = D2DFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(*hwnd, size),
                &hwndRenderTarget); 
        }
        return hr;
    }
    HRESULT OnRender() {
        HRESULT hr;
        hr = CreateDeviceResources();
        if (SUCCEEDED(hr) && !(hwndRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
        {
            hwndRenderTarget->BeginDraw();

            hwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

            hwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            int latest_layer = -999;

            // draw layer x's text after drawing layer x's images
            for (auto const& [key, val] : ImageMap.internalMap) {
                if (latest_layer < key and not ImageMap[key].empty()) { latest_layer = key; }
                drawTheseImagesByYOrder(ImageMap[key]);
                for (auto & [subkey, subval] : TextMap[key].internalMap) {
                    subval.draw(*this);
                }
            }
            // draw any textures that are on higher layers than any existing images
            for (auto const& [key, val] : TextMap.internalMap) {
                if (key <= latest_layer) { continue; }
                for (auto& [subkey, subval] : TextMap[key].internalMap) {
                    subval.draw(*this);
                }
            }

            // add code here to draw every image 
        }
        hr = hwndRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET){
            hr = S_OK;
            DiscardDeviceResources();
        }
        return hr;
    }

    void drawTheseImagesByYOrder(List<Image *> images) {
        while (!images.empty()) {
            float max = 999;
            Image* drawThisOne = NULL;
            for (auto const& value : images.internalList) {
                if (value->positionAsPercentage.second < max) {
                    max = value->positionAsPercentage.second;
                    drawThisOne = value;
                }
            }
            drawThisOne->draw(*this);
            images.forcibleRemove(drawThisOne);
        }
        
    }
    void loadCursors() {
        Map<string, int> cursorsToLoad = list <pair<string, int>>({ {"DEFAULT", CURSOR_DEFAULT}, {"SELECTED", CURSOR_SELECTED} });
        for (auto const& [key, value] :cursorsToLoad.internalMap) {
            HICON defaultCursor = LoadIcon(hinstance, MAKEINTRESOURCE(value));
            ICONINFO iconinfo;
            GetIconInfo(defaultCursor, &iconinfo);
            iconinfo.xHotspot = 0;
            iconinfo.yHotspot = 0;
            Cursors[key] = CreateIconIndirect(&iconinfo);
            DeleteObject(defaultCursor);
        }
    }
    void DiscardDeviceResources()
    {
        SafeRelease("Releasing hwnd render target ", & hwndRenderTarget);
    }
    Image * addImage(Image * image, int layer) {
        if (not ImageMap.hasKey(layer)) {
            ImageMap[layer] = List<Image *>();
        }
        ImageMap[layer].push_back(image);
        return image;
    }
    void addText(Text text, int layer) {
        if (not TextMap.hasKey(layer)) {
            TextMap[layer] = Map<string, Text>();
        }
        TextMap[layer][text.unique_ID] = text;
    }
    void teardownAllImagesOnThisLayer(int layer) {
        while (not ImageMap[layer].empty()) {
            Image* toDelete = ImageMap[layer].front();
            for (auto source : toDelete->sources.internalList) {
                if (TextureMemory.getKeys().contains(source) and TextureMemory[source] != NULL) {
                    SafeRelease("Releasing this texture in the texture memory: " + to_string(source), &TextureMemory[source]);
                }
            }
            delete ImageMap[layer].front();
            ImageMap[layer].front() = NULL;
            ImageMap[layer].pop_front();
        }
    }
    void teardownAllImages() {
        for (auto const & [key, val] : ImageMap.internalMap) {
            teardownAllImagesOnThisLayer(key);
        }
        for (auto const& [key, val] : TextureMemory.internalMap) {
            SafeRelease("Releasing this texture in the texture memory: " + to_string(key), &TextureMemory[key]);
        }
        TextureMemory.clear();
        ImageMap.clear();
    }
    void teardownAllTextFormats() {
        for (auto const& [key, val] : WriteTextFormats.internalMap) {
            SafeRelease("Releasing this write text format: " + key, & WriteTextFormats[key]);
        }
    }
    Image* accessImageViaUniqueID(string uniqueID) {
        for (auto const& [key, val] : ImageMap.internalMap) {
            for (int x = 0; x < ImageMap[key].internalList.size(); x++) {
                if (ImageMap[key].at(x)->unique_ID == uniqueID) {
                    return ImageMap[key].at(x);
                }
            }
        }
        return NULL;
    }
    void tearDownSpecifiedImage(string uniqueID) {
        int index = -1;
        int the_key = 0;
        Image* theImage = NULL;
        for (auto const& [key, val] : ImageMap.internalMap) {
            for (int x = 0; x < ImageMap[key].internalList.size(); x++) {
                if (ImageMap[key].at(x)->unique_ID == uniqueID) {
                    theImage = ImageMap[key].at(x);
                    index = x;
                    the_key = key;
                    break;
                }
            }
            
        }
        if (index != -1) {
            ImageMap[the_key].remove_at(index);
            delete theImage;
            theImage = NULL;
        }
    }
    void tearDownSpecifiedText(string uniqueID) {
        for (auto const& [key, val] : TextMap.internalMap) {
            if (TextMap[key].getKeys().contains(uniqueID)) {
                TextMap[key].internalMap.erase(uniqueID);
            }
        }
    }
    void tearDownAllText() {
        TextMap.clear();
    }
    void tearDownAllCursors() {
        for (auto const& [key, val]: Cursors.internalMap) {
            DestroyCursor(val);
        }
        Cursors.clear();
    }
    void tearDownAllLoadedFonts() {
        for (auto const& [key, val] : addfontmemresourcefonts.internalMap) {
            RemoveFontMemResourceEx(val);
        }
    }
    bool doesThisTextAlreadyExist(string uniqueID) {
        for (auto const& [key, val] : TextMap.internalMap) {
            if (TextMap[key].getKeys().contains(uniqueID)) {
                return true;
            }
        }
        return false;
    }
    bool doesThisImageAlreadyExist(string uniqueID) {
        return accessImageViaUniqueID(uniqueID) != NULL;
    }
    void changeCursor(string name) {
        CurrentCursor = name;
        HCURSOR hCursor = Cursors[CurrentCursor];
        SetCursor(hCursor);
    }


    HWND * hwnd;
    HINSTANCE hinstance;
	ID2D1Factory * D2DFactory;
    IWICImagingFactory * IWICFactory;
    IDWriteFontSetBuilder1* fontSetBuilder;
    IDWriteFontSet* customFontSet;
    IDWriteFontCollection1* fontCollection;
    List<pair<string, int>> customFonts;
    List<int> customFontSizes;
    Map<string, IDWriteTextFormat *> WriteTextFormats;
    Map<string, IDWriteInMemoryFontFileLoader*> idwriteinmemoryfontfileloaders;
    Map<string, IDWriteFontFile*> idwritefontfiles;
    Map<string, HANDLE> addfontmemresourcefonts;
    Map<string, IDWriteFontSetBuilder1*> idwritefontsetbuilders;
    Map<string, IDWriteFontFaceReference*> idwritefontfacereferences;
    IDWriteFactory5* DWriteFactory;
    ID2D1HwndRenderTarget* hwndRenderTarget;
    Map<int, List<Image *>> ImageMap;
    Map<int, Map<string, Text>> TextMap;
    Map<string, HCURSOR> Cursors;
    Map<string, vector<float>> Colours;
    Map<wchar_t, string> colourTagLookupTable;
    List<Image*> beingDragged;
    Map<int, ID2D1Bitmap*> TextureMemory;
    string CurrentCursor = "DEFAULT";
};
Graphics graphics = Graphics();