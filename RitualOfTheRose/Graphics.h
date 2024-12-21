#pragma once
#include "Explorer.h"


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
        colourTagLookupTable[wchar_t(10112)] = "BLUE"; // ➀
        shadowColourTagLookupTable[wchar_t(10112)] = "BLUE"; // ②
        colourTagLookupTable[wchar_t(9313)] = "SKILLTEXTBLUE"; // ②
        shadowColourTagLookupTable[wchar_t(9313)] = "SKILLTEXTBLUEBACKDROP"; // ③
        colourTagLookupTable[wchar_t(9314)] = "EQUIPMENTBLUE"; // ③
        shadowColourTagLookupTable[wchar_t(9314)] = "DARKBROWN"; // ③
        colourTagLookupTable[wchar_t(9315)] = "ELITESKILLYELLOW"; // ④
        shadowColourTagLookupTable[wchar_t(9315)] = "DARKBROWN"; // ④
        colourTagLookupTable[wchar_t(9316)] = "INVISIBLE"; // ⑤
        shadowColourTagLookupTable[wchar_t(9316)] = "INVISIBLE"; // ⑤
        colourTagLookupTable[wchar_t(9317)] = "DAMAGERED"; // ⑥
        shadowColourTagLookupTable[wchar_t(9317)] = "DAMAGERED"; // ⑥
        colourTagLookupTable[wchar_t(9318)] = "HEALINGGREEN"; // ⑦
        shadowColourTagLookupTable[wchar_t(9318)] = "HEALINGGREEN"; // ⑦
        colourTagLookupTable[wchar_t(9319)] = "HEALINGBLUE"; // ⑧
        shadowColourTagLookupTable[wchar_t(9319)] = "HEALINGBLUE"; // ⑧
        colourTagLookupTable[wchar_t(9320)] = "DAMAGEPURPLE"; // ⑨
        shadowColourTagLookupTable[wchar_t(9320)] = "DAMAGEPURPLE"; // ⑨
        customFonts = { 
            pair<string, int>({"Centaur", IDF_CENTAUR}), 
            pair<string,int>({ "GoudyMedieval", IDF_GOUDYMEDIEVAL }),
            pair<string,int>({ "LightText", IDF_LIGHT }),
            pair<string,int>({ "Morris Roman", IDF_MORRIS }),
            pair<string, int>({"HighTowerText", IDF_HIGHTOWER}),
        };
        customFontSizes = {1,2,3,4,5,10,12,13,14,15,16,17,18,19,20,25,30,35,37,40,50};
        Colours["BLACK"] = { 0.0,0.0,0.0,1.0 };
        Colours["WHITE"] = { 1.0,1.0,1.0,1.0 };
        Colours["BLUE"] = { 0.0,0.0,1.0,1.0 };
        Colours["GREEN"] = { 0.0,1.0,0.0,1.0 };
        Colours["RED"] = { 1.0,0.0,0.0,1.0 };
        Colours["DAMAGERED"] = convertIntColour({ 213,0,0,255 }); // life damage
        Colours["INVISIBLE"] = { 0.0,0.0,0.0,0.0 };
        Colours["HEALINGGREEN"] = convertIntColour({ 16,223,0,255 }); // life heal
        Colours["HEALINGBLUE"] = convertIntColour({ 71,205,255,255 }); // energy heal
        Colours["DAMAGEPURPLE"] = convertIntColour({ 166,10,255,255 }); // energy damage
        Colours["DARKBROWN"] = convertIntColour({100,35,0,255});
        Colours["SKILLTEXTBLUE"] = convertIntColour({ 0,246,255,255 });
        Colours["SKILLTEXTBLUEBACKDROP"] = convertIntColour({ 0,6,255,255 });
        Colours["ELITESKILLYELLOW"] = convertIntColour({ 255,234,52,255 });
        Colours["OBVIOUSPINK"] = convertIntColour({ 255,155,251,255 });
        Colours["EQUIPMENTBLUE"] = convertIntColour({ 135,217,255,255 });
    }
    ~Graphics() {
        teardownAllImages();
        teardownAllTextFormats();
        tearDownAllLoadedFonts();
        SafeRelease("Releasing D2D Factory.", & D2DFactory);
        SafeRelease("Releasing IWICFactory", & IWICFactory);
        for (auto ex : DWriteFactories.getKeys().internalList) {
            SafeRelease("", &DWriteFactories[ex]);
            DWriteFactories.internalMap.erase(ex);
        }
        for (auto ex : fontSetBuilders.getKeys().internalList) {
            SafeRelease("", &fontSetBuilders[ex]);
            fontSetBuilders.internalMap.erase(ex);
        }
        for (auto ex : customFontSets.getKeys().internalList) {
            SafeRelease("", &customFontSets[ex]);
            customFontSets.internalMap.erase(ex);
        }
        for (auto ex : fontCollections.getKeys().internalList) {
            SafeRelease("", &fontCollections[ex]);
            fontCollections.internalMap.erase(ex);
        }
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
            if (anchorStyle == "BOTTOMCENTRE") { // stretchable bars that start at bottom
                float normalY = position.second - size.height;
                float stretchedY = ((position.second - size.height) * yStretch) + ((position.second) * (1 - yStretch));
                result = D2D1::RectF(
                    position.first - (size.width / 2),
                    stretchedY,
                    position.first + (size.width / 2),
                    position.second);
            }
            if (anchorStyle == "BOTTOMMIDDLE") { // slightly offset from bottom to look like origin is at player's feet
                float smallD = size.height / 4;
                result = D2D1::RectF(
                    position.first - (size.width / 2),
                    position.second - size.height + smallD,
                    position.first + (size.width / 2),
                    position.second + smallD);
            }
            if (anchorStyle == "ANIMATEONTOP") { // much higher and slightly to the right
                float smallD = size.height / 6.5;
                float smallD2 = size.width / 7.5;
                result = D2D1::RectF(
                    position.first - (size.width / 2) + smallD2,
                    position.second - size.height + smallD,
                    position.first + (size.width / 2) + smallD2,
                    position.second + smallD);
            }
            if (anchorStyle == "CENTRE") {
                result = D2D1::RectF(
                    position.first - (size.width / 2.0f),
                    position.second - (size.height / 2.0f),
                    position.first + (size.width / 2.0f),
                    position.second + (size.height / 2.0f));
            }
            if (anchorStyle == "CENTREXSTRETCH") {
                float normalX1 = position.first - (size.width / 2.0f) + (48.5 * (1- xStretch));
                float normalX2 = position.first + (size.width / 2.0f);
                float stretchedX2 = normalX1 + (size.width * xStretch);
                result = D2D1::RectF(
                    normalX1,
                    position.second - (size.height / 2.0f),
                    stretchedX2,
                    position.second + (size.height / 2.0f));
                int e = 1;
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
        float xStretch = 1.0f;
        float yStretch = 1.0f;
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
            if (hr == E_FAIL) {
                throw exception("Failed to load this texture!");
            }
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
            if (texture == NULL) {
                loadTexture(*&graphics);
                texture = getWhichTexture();
            }
            D2D1_SIZE_F size = texture->GetSize();
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<float, float> position = getAbsolutePosition(renderTargetSize);
            D2D1_RECT_F rect = getRect(position, size);
            return rect;
        }
        bool hasThisFinishedAnimating() {
            if (!animationStyles.contains("SINGLE")) {
                return false;
            }
            if (animationStyles.contains("SINGLE") and animationStyles.contains("FADEOUT")) {
                return opacity == 0.0;
            }
            if (animationStyles.contains("SINGLE") and animationStyles.contains("FADEIN")) {
                return opacity == 1.0;
            }
            return frame == textures.size() - 1;
        }
        void animate() {
            if (!animated) { return; }
            if (animationStyles.contains("SINGLE") and frame<textures.size()-1) {
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_SINGLE", animationSpeed)) {
                    frame++;
                }
            }
            if (animationStyles.contains("LOOP")) { 
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_LOOP", animationSpeed)) {
                    frame++;
                }
            }
            if (animationStyles.contains("FADEOUT")) {
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_FADEOUT", animationSpeed)) {
                    opacity = TChange(opacity, -0.07f, 0.0f, 1.0f);
                }
            }
            if (animationStyles.contains("FADEOUTWHENFINISHED") and frame == textures.size() - 1) {
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_FADE", animationSpeed)) {
                    opacity = TChange(opacity, -0.07f, 0.0f, 1.0f);
                }
            }
            if (animationStyles.contains("FADEIN")) {
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_FADEIN", animationSpeed)) {
                    opacity = TChange(opacity, 0.07f, 0.0f, 1.0f);
                }
            }
        };
        bool hasThisBeenClickedOn(Graphics & graphics, pair<float, float> click) {
            D2D1_RECT_F position = getPosition(graphics);
            bool insideX = position.left <= click.first and position.right >= click.first;
            bool insideY = position.top <= click.second and position.bottom >= click.second;
            return insideX and insideY;
        }
        bool isTheCursorCloseToThis(Graphics& graphics, pair<float, float> click) {
            if (getWhichTexture() == NULL) { return false; }
            D2D1_SIZE_F size = getWhichTexture()->GetSize();
            size.height *= 5;
            size.width *= 2;
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<float, float> position = getAbsolutePosition(renderTargetSize);
            D2D1_RECT_F rect = getRect(position, size);
            bool insideX = rect.left <= click.first and rect.right >= click.first;
            bool insideY = rect.top <= click.second and rect.bottom >= click.second;
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
        void forceThisImageToGoToLastFrameAndStayThere() {
            animated = false;
            frame = sources.size() - 1;
        }
        pair<float, float> getSizeAsPercentage(Graphics& graphics) {
            D2D1_RECT_F position = getPosition(*&graphics);
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            float width = position.right - position.left;
            float height = position.bottom - position.top;
            pair<float, float> result = convertActualToPercent(renderTargetSize, {width, height});
            return result;
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
        Text() {
            tearDownSelf();
        }
        Text(Graphics & graphics, wstring _message, string _format, pair<float, float> _positionAsPercentage, string _anchorStyle, pair<float, float> _size, vector<float> _colour, vector<float> _shadowColour, string _unique_ID, bool _animated, List<string> _animationStyles, int _animationSpeed) {
            message = _message;
            format = _format;
            font = split(format, "_").at(0);
            positionAsPercentage = _positionAsPercentage;
            anchorStyle = _anchorStyle;
            size = _size;
            colour = _colour;
            unique_ID = _unique_ID;
            shadowColour = _shadowColour;
            resetText(*&graphics);
            animated = _animated;
            animationStyles = _animationStyles;
            if (animated and animationStyles.contains("TYPEWRITER")) {
                startTypewriter(*&graphics);
            }
            animationSpeed = _animationSpeed;
        }
        void tearDownSelf() {
            SafeRelease("Releasing the brush", &theBrush);
            SafeRelease("Releasing the shadow brush", &shadowBrush);
            for (string x : extraBrushes.getKeys().internalList) {
                SafeRelease("Releasing extra brush " + x, &extraBrushes[x]);
            }
            for (string x : extraShadowBrushes.getKeys().internalList) {
                SafeRelease("Releasing extra brush " + x, &extraShadowBrushes[x]);
            }
            SafeRelease("Releasing text layout", &textLayout);
            SafeRelease("Releasing shadow text layout", &shadowTextLayout);
        }
        void resetText(Graphics & graphics) {
            tearDownSelf();
            subcolours = interpret_subcolours(graphics, graphics.colourTagLookupTable);
            shadowSubColours = interpret_subcolours(graphics, graphics.shadowColourTagLookupTable);
            HRESULT hr = S_OK;
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(colour[0], colour[1], colour[2], colour[3])),
                &theBrush);
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(shadowColour[0], shadowColour[1], shadowColour[2], shadowColour[3])),
                &shadowBrush);
            for (auto const& [key, value] : subcolours.internalMap) {
                extraBrushes[key] = NULL;
                graphics.hwndRenderTarget->CreateSolidColorBrush(
                    D2D1::ColorF(D2D1::ColorF(graphics.Colours[key][0], graphics.Colours[key][1], graphics.Colours[key][2], graphics.Colours[key][3])),
                    &extraBrushes[key]);
            }
            for (auto const& [key, value] : shadowSubColours.internalMap) {
                extraShadowBrushes[key] = NULL;
                graphics.hwndRenderTarget->CreateSolidColorBrush(
                    D2D1::ColorF(D2D1::ColorF(graphics.Colours[key][0], graphics.Colours[key][1], graphics.Colours[key][2], graphics.Colours[key][3])),
                    &extraShadowBrushes[key]);
            }
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<float, float> position = getAbsolutePosition(renderTargetSize);
            pair<float, float> trueSize = convertPercentToActual(renderTargetSize, size);
            D2D_SIZE_F size_as_d2d = D2D_SIZE_F();
            size_as_d2d.width = trueSize.first;
            size_as_d2d.height = trueSize.second;
            D2D1_RECT_F rect = getRect(position, size_as_d2d);
            D2D1_RECT_F shadowRect = getRect({ position.first - 1, position.second + 1 }, size_as_d2d);
            P.x = rect.left; P.y = rect.top;
            shadowP = P;
            shadowP.x -= 1;
            shadowP.y += 1;

            hr = graphics.DWriteFactories[font]->CreateTextLayout(removeTagsBeforePrinting(graphics, message).c_str(), message.size(), graphics.WriteTextFormats[format], size_as_d2d.width, size_as_d2d.height, &textLayout);
            hr = graphics.DWriteFactories[font]->CreateTextLayout(removeTagsBeforePrinting(graphics, message).c_str(), message.size(), graphics.WriteTextFormats[format], size_as_d2d.width, size_as_d2d.height, &shadowTextLayout);

            for (auto const& [key, value] : subcolours.internalMap) {
                for (auto const& range : subcolours[key].internalList) {
                    textLayout->SetDrawingEffect(extraBrushes[key], range);
                }
            }
            for (auto const& [key, value] : shadowSubColours.internalMap) {
                for (auto const& range : shadowSubColours[key].internalList) {
                    shadowTextLayout->SetDrawingEffect(extraShadowBrushes[key], range);
                }
            }

            if (anchorStyle == "CENTRE") {
                hr = textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                hr = textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                hr = shadowTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                hr = shadowTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            }
        }
        void resetMessage(Graphics& graphics, wstring _message) {
            message = _message;
            resetText(*&graphics);
        }
        const wstring& getMessage() {
            return message;
        }
        void draw(Graphics& graphics) {
            animate(*&graphics);
            graphics.hwndRenderTarget->DrawTextLayout(shadowP, shadowTextLayout, shadowBrush);
            graphics.hwndRenderTarget->DrawTextLayout(P, textLayout, theBrush);
        }
        wstring removeTagsBeforePrinting(Graphics & graphics, wstring input_string) {
            for (int x = 0; x < input_string.size(); x++) {
                if (graphics.colourTagLookupTable.getKeys().contains(input_string[x])) {
                    input_string[x] = wchar_t(32);
                }
            }
            return input_string;
        }
        Map<string, List<DWRITE_TEXT_RANGE>> interpret_subcolours(Graphics & graphics, Map<wchar_t, string> tableToUse) {
            Map<string, List<DWRITE_TEXT_RANGE>> results;
            wstring toAnalyse = message;
            wchar_t invisible = wchar_t(9316);

            // colour in subsections of text
            for (auto const& colour : graphics.colourTagLookupTable.getKeys().internalList) {
                int current_start = -1;
                bool seekingEnd = false;
                for (int x = 0; x < toAnalyse.size(); x++) {
                    if (seekingEnd and current_start != -1 and List<wchar_t>({ wchar_t(46), wchar_t(32), wchar_t(33), wchar_t(63), colour, invisible }).contains(toAnalyse[x])) {
                        int current_end = x;
                        results[tableToUse[colour]].push_back({unsigned(current_start), unsigned(current_end - current_start)});
                        current_start = -1;
                        seekingEnd = false;
                    }
                    if (toAnalyse[x] == colour) {
                        current_start = x;
                        seekingEnd = true;
                    }
                }
                }
            for (int x = 0; x < toAnalyse.size(); x++) {
                if (toAnalyse[x] == invisible) {
                    // make all text from ⑤ onwards invisible regardless
                    results[tableToUse[invisible]].push_back({ unsigned(x), unsigned(toAnalyse.size() - x) });
                }
            }
            return results;
        }
        void startTypewriter(Graphics & graphics) {
            fullMessage = message;
            message = L"⑤" + message;
            resetText(*&graphics);
            animated = true;
        }
        bool animate(Graphics & graphics) { // return true if finished
            bool finished = getMessage().find(L"⑤") == -1;
            if (CLOCK.hasEnoughTimePassed("TYPEWRITER_" + unique_ID, animationSpeed)) {
                if (finished) {
                    return true;
                }
                if (getMessage() == L"") { return false; } // not ready to be animated yet
                wstring message = getMessage();
                int textLength = message.size();
                int howFar = message.find(L"⑤");
                int howFarAsPercentage = howFarAlong();
                int nextLoc = howFar;
                if (howFar < textLength - 1) {
                    nextLoc = howFar + 1;
                    wchar_t nextChar = message.at(nextLoc);
                    List<wchar_t> skipThese = graphics.colourTagLookupTable.getKeys();
                    for (auto x : { 33,63,46 }) {
                        skipThese.push_back(wchar_t(x));
                    }
                    while (nextLoc < message.size() - 1 and skipThese.contains(nextChar)) {
                        nextLoc += 1;
                        nextChar = message.at(nextLoc);
                    }
                }
                else {
                    nextLoc += 1;
                }
                wstring current = fullMessage;
                if (howFar < current.size()) {
                    current.replace(nextLoc, 1, L"⑤");
                }
                resetMessage(*&graphics, current);
                if (animationStyles.contains("PARCHMENT") and howFarAsPercentage < 50 and CLOCK.hasEnoughTimePassed("ParchmentSoundWait", 15)) {
                    audio.playRandomSFXFromThisCollection("PARCHMENT", audio.volumes["SFXVolume"] / 2);
                    CLOCK.startClock("ParchmentSoundWait");
                }
            }
            if (getMessage().find(L"⑤") == -1) {
                CLOCK.startClock("TypewriterFinished");
            }
            return false;
        }
        int howFarAlong() {
            int index = message.find(L"⑤");
            if (index == -1) { return 100; }

            return index * 100 / message.size();
        }
        static wstring commonTextReplacements(string language, wstring input) {
            Map<wstring, wstring> replacements;
            if (!saveContainer.current.loaded) { return input; }
            replacements[L"$DEFAULTBUYBACK$"] = strings[language]["Default Merchant Dialogue"]["Buy Back"];
            replacements[L"$DEFAULTBUY$"] = strings[language]["Default Merchant Dialogue"]["Buy"];
            replacements[L"$SELL$"] = strings[language]["Default Merchant Dialogue"]["Sell"];
            replacements[L"$KIND$"] = strings[language]["Terms Per Character"][ saveContainer.getCurrentMainCharacter() + "_Kind"];
            replacements[L"$POLITE$"] = strings[language]["Terms Per Character"][saveContainer.getCurrentMainCharacter() + "_Polite"];
            replacements[L"$GENDER$"] = strings[language]["Terms Per Character"][saveContainer.getCurrentMainCharacter() + "_Gender"];
            replacements[L"$GENDERUPPER$"] = strings[language]["Terms Per Character"][saveContainer.getCurrentMainCharacter() + "_GenderUpper"];
            replacements[L"$POLITETITLE$"] = strings[language]["Terms Per Character"][saveContainer.getCurrentMainCharacter() + "_PoliteTitle"];

            for (auto const & [key, val] : replacements.internalMap) {
                input = WSReplace(input, key, val);
            }
            return input;
        }

        string format;
        string font;
        pair<float, float> size;
        vector<float> colour = {0.0,0.0,0.0,1.0};
        vector<float> shadowColour = { 0.0,0.0,0.0,1.0 };
        int tagLimit = 10;
        bool animated = false;
        List<string> animationStyles;
        int animationSpeed;
        Map<string, List<DWRITE_TEXT_RANGE>> subcolours;
        Map<string, List<DWRITE_TEXT_RANGE>> shadowSubColours;
        ID2D1SolidColorBrush* theBrush = NULL;
        ID2D1SolidColorBrush* shadowBrush = NULL;
        Map<string, ID2D1SolidColorBrush*> extraBrushes;
        Map<string, ID2D1SolidColorBrush*> extraShadowBrushes;
        IDWriteTextLayout* textLayout = NULL;
        IDWriteTextLayout* shadowTextLayout = NULL;
        D2D1_POINT_2F P;
        D2D1_POINT_2F shadowP;
        wstring fullMessage;

        private:
            wstring message;
    };
    void setup(HWND * hwnd, HINSTANCE _hInstance) {
        hwnd = hwnd;
        hinstance = _hInstance;
        CreateDeviceIndependentResources();
        CreateDeviceResources();
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

        for (auto const & font : customFonts.internalList) {
            IDWriteFactory5* DWriteFactory = NULL;
            IDWriteFontSet* customFontSet = NULL;
            IDWriteFontCollection1* fontCollection = NULL;
            IDWriteFontSetBuilder1* fontSetBuilder = NULL;

            hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(DWriteFactory),reinterpret_cast<IUnknown**>(&DWriteFactory));

            idwriteinmemoryfontfileloaders[font.first] = NULL;
            hr = DWriteFactory->CreateInMemoryFontFileLoader(&idwriteinmemoryfontfileloaders[font.first]);
            hr = DWriteFactory->RegisterFontFileLoader(idwriteinmemoryfontfileloaders[font.first]);
            hr = DWriteFactory->CreateFontSetBuilder(&fontSetBuilder);
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
            hr = fontSetBuilder->CreateFontSet(&customFontSet);
            hr = DWriteFactory->CreateFontCollectionFromFontSet(customFontSet, &fontCollection);

            DWriteFactories[font.first] = DWriteFactory;
            customFontSets[font.first] = customFontSet;
            fontCollections[font.first] = fontCollection;
            fontSetBuilders[font.first] = fontSetBuilder;

            for (auto const& size : customFontSizes.internalList) {
                IDWriteTextFormat* textFormat = NULL;
                hr = DWriteFactory->CreateTextFormat(wstring(font.first.begin(), font.first.end()).c_str(),
                    fontCollection,
                    DWRITE_FONT_WEIGHT_NORMAL,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    size,
                    L"",
                    &textFormat);
                if (SUCCEEDED(hr)) {
                    WriteTextFormats.add({ font.first + "_" + to_string(size), textFormat });
                }
                else {
                    throw exception("Failed to load this font.");
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
        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        HRESULT hr;
        hr = CreateDeviceResources();
        if (SUCCEEDED(hr) && !(hwndRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
        {
            hwndRenderTarget->BeginDraw();

            hwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

            hwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

            int latest_layer = -999;

            // draw layer x's text after drawing layer x's images
            List<int> layers;
            for (auto x : ImageMap.getKeys().internalList) {
                if (!ImageMap[x].empty()) {
                    layers.addToBackIfNotAlreadyInList(x);
                }
            }
            for (auto x : TextMap.getKeys().internalList) {
                if (!TextMap[x].empty()) {
                    layers.addToBackIfNotAlreadyInList(x);
                }
            }
            layers.internalList.sort();

            for (auto layer : layers.internalList) {
                drawTheseImagesByYOrder(ImageMap[layer]);
                for (Text* text : TextMap[layer].internalList) {
                    text->draw(*this);
                }
            }
        }
        hr = hwndRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET){
            hr = S_OK;
            DiscardDeviceResources();
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        chrono::milliseconds renderTime = chrono::duration_cast<chrono::milliseconds>(end - start);
        RenderSpeedHistory.push_back(renderTime);
        if (RenderSpeedHistory.size() > 100) {
            RenderSpeedHistory.pop_front();
        }
        return hr;
    }

    Image* accessPlayerImage() {
        return accessImageViaUniqueID(saveContainer.getCurrentMainCharacter() + "_Explore");
    }
    Image* accessPlayerShadowImage() {
        return accessImageViaUniqueID(saveContainer.getCurrentMainCharacter() + "_Shadow");
    }
    bool isThisInsideRect(pair<float, float> click, RECT position) {
        bool insideX = position.left <= click.first and position.right >= click.first;
        bool insideY = position.top <= click.second and position.bottom >= click.second;
        return insideX and insideY;
    }
    void drawTheseImagesByYOrder(List<Image *> images) {
        while (!images.empty()) {
            float max = 999;
            Image* drawThisOne = NULL;
            for (auto const& value : images.internalList) {
                D2D1_RECT_F imagePosition = value->getPosition(*this);
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
        Map<string, int> cursorsToLoad = list <pair<string, int>>({ {"DEFAULT", CURSOR_DEFAULT}, {"SELECTED", CURSOR_SELECTED}, {"NONE", CURSOR_NONE}});
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
    Text * addText(Text * text, int layer) {
        if (not TextMap.hasKey(layer)) {
            TextMap[layer] = List<Text*>();
        }
        TextMap[layer].push_back(text);
        return text;
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
    void tearDownAllTextOnThisLayer(int layer) {
        while (not TextMap[layer].empty()) {
            Text* toDelete = TextMap[layer].front();
            delete TextMap[layer].front();
            TextMap[layer].front() = NULL;
            TextMap[layer].pop_front();
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
    Text * accessTextViaUniqueID(string uniqueID) {
        for (auto const& [key, val] : TextMap.internalMap) {
            for (int x = 0; x < TextMap[key].internalList.size(); x++) {
                if (TextMap[key].at(x)->unique_ID == uniqueID) {
                    return TextMap[key].at(x);
                }
            }
        }
        return NULL;
    }
    void bumpLayer(Image * theImage, int amount) {
        int current = whichLayerIsThisImageOn(theImage);
        ImageMap[current].forcibleRemove(theImage);
        ImageMap[current + amount].push_front(theImage);
    }
    void moveLayer(Image* theImage, int target) {
        int current = whichLayerIsThisImageOn(theImage);
        ImageMap[current].forcibleRemove(theImage);
        ImageMap[target].push_front(theImage);
    }
    int whichLayerIsThisImageOn(Image* theImage) {
        for (auto layer : ImageMap.getKeys().internalList) {
            if (ImageMap[layer].contains(theImage)) {
                return layer;
            }
        }
        return 0;
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
        int index = -1;
        int the_key = 0;
        Text* theText = NULL;
        for (auto const& [key, val] : TextMap.internalMap) {
            for (int x = 0; x < TextMap[key].internalList.size(); x++) {
                if (TextMap[key].at(x)->unique_ID == uniqueID) {
                    theText = TextMap[key].at(x);
                    index = x;
                    the_key = key;
                    break;
                }
            }

        }
        if (index != -1) {
            TextMap[the_key].remove_at(index);
            delete theText;
            theText = NULL;
        }
    }
    void tearDownAllText() {
        for (auto const& [key, val] : TextMap.internalMap) {
            tearDownAllTextOnThisLayer(key);
        }
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
        return accessTextViaUniqueID(uniqueID) != NULL;
    }
    bool doesThisImageAlreadyExist(string uniqueID) {
        return accessImageViaUniqueID(uniqueID) != NULL;
    }
    void changeCursor(string name) {
        if (CurrentCursor == name) { return; }
        CurrentCursor = name;
        HCURSOR hCursor = Cursors[CurrentCursor];
        SetCursor(hCursor);
    }
    RECT getRectOfWholeScreen() {
        return makeRect(0, 0, controller.actualRenderSizeAsFloat.first, controller.actualRenderSizeAsFloat.second);
    }
    RECT makeRect(float top, float left, float bottom, float right) {
        RECT result;
        result.left = left;
        result.top = top;
        result.right = right;
        result.bottom = bottom;
        return result;
    }
    int interpolate(int from, int to, float percent) {
        int difference = to - from;
        return from + (difference * percent);
    }
    List<pair<float, float>> plot(List<pair<float, float>> basePoints, float density) {
        List<pair< float, float>> results;

        for (float x = 0.0; x < 1.0; x += density) {
            List<float> Xs;
            List<float> Ys;
            List<float> interpolatedXs;
            List<float> interpolatedYs;
            for (auto point : basePoints.internalList) {
                Xs.push_back(point.first);
                Ys.push_back(point.second);
            }
            for (auto index = 0; index < basePoints.size() - 1; index++) {
                interpolatedXs.push_back(interpolate(Xs.at(index), Xs.at(index + 1), x));
                interpolatedYs.push_back(interpolate(Ys.at(index), Ys.at(index + 1), x));
            }
            float currentX = Xs.front();
            float currentY = Ys.front();
            while (interpolatedXs.size() > 1) {
                currentX = interpolate(interpolatedXs.at(0), interpolatedXs.at(1), x);
                interpolatedXs.pop_front();
            }
            while (interpolatedYs.size() > 1) {
                currentY = interpolate(interpolatedYs.at(0), interpolatedYs.at(1), x);
                interpolatedYs.pop_front();
            }
            results.push_back(pair<float, float>(currentX, currentY));
        }
        results.push_back(basePoints.back());
        return results;
    }
    pair<float, float> getEquidistantPoint(pair<float, float> LHS, pair<float, float> RHS) {
        return { LHS.first + RHS.first / 2, LHS.second + RHS.second / 2 };
    }
    pair<float, float> randomVariation(pair<float, float> LHS, float variation) {
        List<float> XRange;
        List<float> YRange;
        for (float x = LHS.first - variation; x < LHS.first + variation; x += 0.1f) {
            XRange.push_back(x);
        }
        for (float y = LHS.second - variation; y < LHS.second + variation; y += 0.1f) {
            YRange.push_back(y);
        }
        pair<float, float> result = { RANDOM.getRandom(XRange), RANDOM.getRandom(YRange) };
        return result;
    }
    wstring insertNewlines(wstring input) {
        // try to make text box formatted nicely for dialogue
        int maxLineSize = 70000;
        List<wstring> words = WSplit(input, L" ");
        wstring result;
        int currentLineSize = 0;
        for (auto & word : words.internalList) {
            int sizeOfWord = getSizeOfWord(L" " + word);
            if (currentLineSize + sizeOfWord > maxLineSize) {
                result += L"\n" + word;
                currentLineSize = 0;
            }
            else {
                currentLineSize += sizeOfWord;
                if (currentLineSize == 0 or result == L"") {
                    result += word;
                }
                else {
                    result += L" " + word;
                }
            }
        }
        return result;
    }
    int getSizeOfWord(wstring input) {
        Map<wstring, int> sizeLookup(List<pair<wstring, int>>({
                pair<wstring, int>(L"A" , 1280), pair<wstring, int>(L"B" , 1044),pair<wstring, int>(L"C" , 1280),pair<wstring, int>(L"D" , 1473),
                pair<wstring, int>(L"E" , 1153),pair<wstring, int>(L"F" , 1044),pair<wstring, int>(L"G" , 1386),pair<wstring, int>(L"H" , 1579),
                pair<wstring, int>(L"I" , 641),pair<wstring, int>(L"J" , 641),pair<wstring, int>(L"K" , 1280),pair<wstring, int>(L"L" , 1153),
                pair<wstring, int>(L"M" , 1792),pair<wstring, int>(L"N" , 1579),pair<wstring, int>(L"O" , 1516),pair<wstring, int>(L"P" , 1087),
                pair<wstring, int>(L"Q" , 1516),pair<wstring, int>(L"R" , 1366),pair<wstring, int>(L"S" , 961),pair<wstring, int>(L"T" , 1366),
                pair<wstring, int>(L"U" , 1473),pair<wstring, int>(L"V" , 1473),pair<wstring, int>(L"W" , 2005),pair<wstring, int>(L"X" , 1386),
                pair<wstring, int>(L"Y" , 1366),pair<wstring, int>(L"Z" , 1260),
                pair<wstring, int>(L"a" , 768), pair<wstring, int>(L"b" , 961),pair<wstring, int>(L"c" , 768),pair<wstring, int>(L"d" , 981),
                pair<wstring, int>(L"e" , 748), pair<wstring, int>(L"f" , 618),pair<wstring, int>(L"g" , 854),pair<wstring, int>(L"h" , 961),
                pair<wstring, int>(L"i" , 492), pair<wstring, int>(L"j" , 469),pair<wstring, int>(L"k" , 938),pair<wstring, int>(L"l" , 492),
                pair<wstring, int>(L"m" , 1450), pair<wstring, int>(L"n" , 961),pair<wstring, int>(L"o" , 961),pair<wstring, int>(L"p" , 981),
                pair<wstring, int>(L"q" , 961), pair<wstring, int>(L"r" , 641),pair<wstring, int>(L"s" , 662),pair<wstring, int>(L"t" , 598),
                pair<wstring, int>(L"u" , 938), pair<wstring, int>(L"v" , 881),pair<wstring, int>(L"w" , 1260),pair<wstring, int>(L"x" , 831),
                pair<wstring, int>(L"y" , 831), pair<wstring, int>(L"z" , 831),
                pair<wstring, int>(L" " , 502), pair<wstring, int>(L"." , 406), pair<wstring, int>(L"!" , 426), pair<wstring, int>(L"?" , 575),
                pair<wstring, int>(L"é" , 748), pair<wstring, int>(L"í" , 492), pair<wstring, int>(L"á" , 768), pair<wstring, int>(L"É" , 1153),
                pair<wstring, int>(L"é" , 748), pair<wstring, int>(L"Í" , 641), pair<wstring, int>(L"Á" , 1280),
                pair<wstring, int>(L"➀", 502), pair<wstring, int>(L"②", 502), pair<wstring, int>(L"③", 502),pair<wstring, int>(L"④", 502),
                pair<wstring, int>(L"⑤", 502),
                pair<wstring, int>(L",", 406),pair<wstring, int>(L"'", 385),
            }));
        int result = 0;
        for (int x = 0; x < input.size(); x++) {
            wstring letter = input.substr(x, 1);
            result += sizeLookup[letter];
        }
        return result;
    }
    List<Image*> allImages() {
        List<Image*> result;
        for (auto layer : ImageMap.getKeys().internalList) {
            for (auto x : ImageMap[layer].internalList) {
                result.push_back(x);
            }
        }
        return result;
    }

    List<chrono::milliseconds> RenderSpeedHistory;

    HWND * hwnd;
    HINSTANCE hinstance;
	ID2D1Factory * D2DFactory;
    IWICImagingFactory * IWICFactory;
    List<pair<string, int>> customFonts;
    List<int> customFontSizes;
    Map<string, IDWriteTextFormat *> WriteTextFormats;
    Map<string, IDWriteInMemoryFontFileLoader*> idwriteinmemoryfontfileloaders;
    Map<string, IDWriteFontFile*> idwritefontfiles;
    Map<string, HANDLE> addfontmemresourcefonts;
    Map<string, IDWriteFontFaceReference*> idwritefontfacereferences;
    Map<string, IDWriteFactory5*> DWriteFactories;
    Map<string, IDWriteFontSetBuilder1*> fontSetBuilders;
    Map<string, IDWriteFontSet*> customFontSets;
    Map<string, IDWriteFontCollection1*> fontCollections;

    ID2D1HwndRenderTarget* hwndRenderTarget;
    Map<int, List<Image *>> ImageMap;
    Map<int, List<Text *>> TextMap;
    Map<string, HCURSOR> Cursors;
    Map<string, vector<float>> Colours;
    Map<wchar_t, string> colourTagLookupTable;
    Map<wchar_t, string> shadowColourTagLookupTable;
    List<Image*> beingDragged;
    List<string> recentlyFinishedBeingDragged;
    Map<int, ID2D1Bitmap*> TextureMemory;
    string CurrentCursor = "DEFAULT";
};
Graphics graphics = Graphics();